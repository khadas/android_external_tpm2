#!/usr/bin/python2

# Copyright 2015 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""A code generator for TPM 2.0 structures.

The structure generator takes as input a structures file as emitted by the
extract_structures.sh script. It outputs valid C into tpm_generated.{h,c}.
tpm_generated.{h,c} consist of functions to marshal TPM2.0 structures.

The input grammar is documented in extract_structures.sh. Sample input for
structures looks like this:
_BEGIN_TYPES
_OLD_TYPE UINT32
_NEW_TYPE TPM_HANDLE
_END
_BEGIN_CONSTANTS
_CONSTANTS (UINT32) TPM_SPEC
_TYPE UINT32
_NAME TPM_SPEC_FAMILY
_VALUE 0x322E3000
_NAME TPM_SPEC_LEVEL
_VALUE 00
_END
_BEGIN_STRUCTURES
_STRUCTURE TPMS_TIME_INFO
_TYPE UINT64
_NAME time
_TYPE TPMS_CLOCK_INFO
_NAME clockInfo
_END
"""

from __future__ import print_function

import argparse
import re
import collections
from subprocess import call

import union_selectors

_BASIC_TYPES = ['uint8_t', 'int8_t', 'uint16_t', 'int16_t', 'uint32_t',
                'int32_t', 'uint64_t', 'int64_t']
_OUTPUT_FILE_H = 'tpm_generated.h'
_OUTPUT_FILE_CC = 'tpm_generated.c'
_COPYRIGHT_HEADER = (
    '// Copyright 2015 The Chromium OS Authors. All rights reserved.\n'
    '// Use of this source code is governed by a BSD-style license that can '
    'be\n'
    '// found in the LICENSE file.\n'
    '\n'
    '// THIS CODE IS GENERATED - DO NOT MODIFY!\n')
_HEADER_FILE_GUARD_HEADER = """
#ifndef %(name)s
#define %(name)s
"""
_HEADER_FILE_GUARD_FOOTER = """
#endif  // %(name)s
"""
_HEADER_FILE_INCLUDES = """
#include <endian.h>
#include <string.h>

#include "TPM_Types.h"
#include "Tpm.h"
"""
_IMPLEMENTATION_FILE_INCLUDES = """
#include "tpm_generated.h"
"""
# Function signatures for generated marshaling code are specified in TCG TPM2.0
# Library Specification, Part 4: Supporting Routines, sections 4.2.2 and 4.2.3.
_MARSHAL_BASIC_TYPE = """
UINT16 %(type)s_Marshal(%(type)s *source, BYTE **buffer, INT32 *size) {
  %(type)s value_net = *source;
  if (!size || *size < sizeof(%(type)s)) {
    return sizeof(%(type)s);
  }
  switch (sizeof(%(type)s)) {
    case 2:
      value_net = htobe16(*source);
      break;
    case 4:
      value_net = htobe32(*source);
      break;
    case 8:
      value_net = htobe64(*source);
      break;
    default:
      break;
  }
  memcpy(*buffer, &value_net, sizeof(%(type)s));
  *buffer += sizeof(%(type)s);
  *size -= sizeof(%(type)s);
  return sizeof(%(type)s);
}

TPM_RC %(type)s_Unmarshal(%(type)s *target, BYTE **buffer, INT32 *size) {
  %(type)s value_net = 0;
  if (!size || *size < sizeof(%(type)s)) {
    return TPM_RC_INSUFFICIENT;
  }
  memcpy(&value_net, *buffer, sizeof(%(type)s));
  switch (sizeof(%(type)s)) {
    case 2:
      *target = be16toh(value_net);
      break;
    case 4:
      *target = be32toh(value_net);
      break;
    case 8:
      *target = be64toh(value_net);
      break;
    default:
      *target = value_net;
  }
  *buffer += sizeof(%(type)s);
  *size -= sizeof(%(type)s);
  return TPM_RC_SUCCESS;
}
"""
_STANDARD_MARSHAL_DECLARATION = """
UINT16 %(type)s_Marshal(
    %(type)s *source,
    BYTE **buffer,
    INT32 *size);

TPM_RC %(type)s_Unmarshal(
    %(type)s *target,
    BYTE **buffer,
    INT32 *size);
"""

def IsTPM2B(name):
  return name.startswith('TPM2B_')


class TPMType(object):
  """Base type for all TPMTypes

     Contains functions and string literals common to all TPM types.
  """
  # A function to marshal a TPM typedef.
  _TYPEDEF_MARSHAL_FUNCTION = """
UINT16 %(new_type)s_Marshal(
    %(new_type)s *source,
    BYTE **buffer,
    INT32 *size) {
  return %(old_type)s_Marshal(source, buffer, size);
}
"""
  # The function signature and unmarshaling call to the base type of a TPM
  # typedef. After the value is unmarshaled, additional validation code is
  # generated based on tables in TCG TPM2.0 Library Specification, Part 2:
  # Structures.
  _TYPEDEF_UNMARSHAL_START = """
TPM_RC %(new_type)s_Unmarshal(
    %(new_type)s *target,
    BYTE **buffer,
    INT32 *size) {
  TPM_RC result;
  result = %(old_type)s_Unmarshal(target, buffer, size);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""
  _UNMARSHAL_END = '\n  return TPM_RC_SUCCESS;\n}\n'
  # Snippets of code for value validation.
  _VALUE_START_SWITCH = '\n  switch (%(name)s) {'
  _VALUE_CASE = '\n    case %(value)s:'
  _VALUE_CASE_IFDEF = '\n#ifdef %(value)s\n    case %(value)s:\n#endif'
  _VALUE_END_SWITCH = """
      break;
    default:
      return %(error_code)s;
  }"""
  # A declaration for marshaling and unmarshaling functions for a TPM type.
  _MARSHAL_DECLARATION = _STANDARD_MARSHAL_DECLARATION
  # Snippets of code which make calls to marshaling functions. Marshals a value
  # of type 'type' into a field 'name' within a structure. This is used in
  # generation of structure and command marshaling code.
  _MARSHAL_CALL = """
  total_size += %(type)s_Marshal(
      &source->%(name)s, buffer, size);"""
  _UNMARSHAL_CALL = """
  result = %(type)s_Unmarshal(
      &target->%(name)s, buffer, size);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""

  def HasConditional(self):
    """Returns true if TPMType has a conditional value."""
    return False

  def OutputMarshalCall(self, out_file, field_type, field_name, extra_argument):
    """Write a call to Marshal function for TPMType to |out_file|.

       Accumulates a variable 'total_size' with the result of marshaling
       field |field_name| in structure 'source'.

    Args:
      out_file: The output file.
      field_type: The type of the field in 'source' struct to be marshalled.
      field_name: The name of the field in 'source' struct to be marshalled.
      extra_argument: Value representing a BOOL value, selector value, or array
          length depending on context. Not used here.
    """
    out_file.write(self._MARSHAL_CALL % {'type': field_type,
                                         'name': field_name})

  def OutputUnmarshalCall(
      self, out_file, field_type, field_name, extra_argument):
    """Write a call to Unmarshal function for TPMType to |out_file|.

       Assigns result of unmarshaling field |field_name| in structure 'source'
       to variable 'result'. Returns if the unmarshalling was unsuccessful.

    Args:
      out_file: The output file.
      field_type: The type of the field in 'target' struct to be unmarshalled.
      field_name: The name of the field in 'target' struct to be unmarshalled.
      extra_argument: Value representing a BOOL value or selector value
          depending on context. Not used here.
    """
    out_file.write(self._UNMARSHAL_CALL % {'type': field_type,
                                           'name': field_name})

  def _OutputTypedefMarshalDecl(self, out_file, declared_types, typemap):
    """Write marshal declarations for TPM typedefs to |out_file|.

       Can only be called on Typedef, ConstantType, AttributeStruct, and
       Interface objects.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in declared_types:
      return
    if self.old_type not in declared_types:
      typemap[self.old_type].OutputMarshalDecl(
          out_file, declared_types, typemap)
    out_file.write(self._MARSHAL_DECLARATION % {'type': self.new_type})
    declared_types.add(self.new_type)

  def _OutputStructOrUnionMarshalDecl(self, out_file, declared_types, typemap):
    """Write marshal declarations for a TPM Structure or Union.

       Can only be called on Structure and Union objects.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
        declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    # TPMU_NAME and TPMU_ENCRYPTED_SECRET type are never used across the
    # interface.
    if (self.name in declared_types or
        self.name == 'TPMU_NAME' or
        self.name == 'TPMU_ENCRYPTED_SECRET'):
      return
    out_file.write(self._MARSHAL_DECLARATION % {'type': self.name})
    declared_types.add(self.name)

class Typedef(TPMType):
  """Represents a TPM typedef.

  Attributes:
    old_type: The existing type in a typedef statement.
    new_type: The new type in a typedef statement.
  """
  # A function to unmarshal a TPM typedef with no extra validation.
  _TYPEDEF_UNMARSHAL_FUNCTION = """
TPM_RC %(new_type)s_Unmarshal(
    %(new_type)s *target,
    BYTE **buffer,
    INT32 *size) {
  return %(old_type)s_Unmarshal(target, buffer, size);
}
"""
  def __init__(self, old_type, new_type):
    """Initializes a Typedef instance.

    Args:
      old_type: The base type of the attribute structure.
      new_type: The name of the type.
    """
    self.old_type = old_type
    self.new_type = new_type

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementations for Typedef to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in marshalled_types:
      return
    if self.old_type not in marshalled_types:
      typemap[self.old_type].OutputMarshalImpl(
          out_file, marshalled_types, typemap)
    out_file.write(self._TYPEDEF_MARSHAL_FUNCTION % {'old_type': self.old_type,
                                                     'new_type': self.new_type})
    out_file.write(
        self._TYPEDEF_UNMARSHAL_FUNCTION % {'old_type': self.old_type,
                                            'new_type': self.new_type})
    marshalled_types.add(self.new_type)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for Typedef to |out_file|.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    self._OutputTypedefMarshalDecl(out_file, declared_types, typemap)

class ConstantType(TPMType):
  """Represents a TPM Constant type definition.

  Attributes:
     old_type: The base type of the constant (e.g. 'int').
     new_type: The name of the type (e.g. 'TPM_RC').
     valid_values: The list of valid values this type can take (e.g.
         'TPM_RC_SUCCESS').
     error_code: Error to be returned when unmarshalling is unsuccessful.
  """
  _CHECK_VALUE = """
  if (*target == %(value)s) {
    return TPM_RC_SUCCESS;
  }"""
  _CHECK_VALUE_IFDEF = """
#ifdef %(value)s
  if (*target == %(value)s) {
    return TPM_RC_SUCCESS;
  }
#endif"""
  _UNMARSHAL_END = """
  return %(error_code)s;
}
"""
  _IFDEF_TYPE_RE = re.compile(r'^TPM_(ALG|CC).*')

  def __init__(self, old_type, new_type):
    """Initializes a ConstantType instance.

    Values are added to valid_values attribute during parsing.

    Args:
      old_type: The base type of the constant type.
      new_type: The name of the type.
    """
    self.old_type = old_type
    self.new_type = new_type
    self.valid_values = []
    self.error_code = 'TPM_RC_VALUE'

  def _NeedsIfdef(self):
    """Returns True if new_type is a type which needs ifdef enclosing."""
    return self._IFDEF_TYPE_RE.search(self.new_type)

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementations for ConstantType to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in marshalled_types:
      return
    if self.old_type not in marshalled_types:
      typemap[self.old_type].OutputMarshalImpl(
          out_file, marshalled_types, typemap)
    out_file.write(self._TYPEDEF_MARSHAL_FUNCTION % {'old_type': self.old_type,
                                                     'new_type': self.new_type})
    out_file.write(self._TYPEDEF_UNMARSHAL_START % {'old_type': self.old_type,
                                                    'new_type': self.new_type})
    for value in self.valid_values:
      if self._NeedsIfdef():
        out_file.write(self._CHECK_VALUE_IFDEF % {'value': value})
      else:
        out_file.write(self._CHECK_VALUE % {'value': value})
    out_file.write(self._UNMARSHAL_END % {'error_code': self.error_code})
    marshalled_types.add(self.new_type)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for ConstantType to |out_file|.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    self._OutputTypedefMarshalDecl(out_file, declared_types, typemap)

class AttributeStructure(TPMType):
  """Represents a TPM attribute structure type definition.

  Attributes:
     old_type: The base type of the constant (e.g. 'int').
     new_type: The name of the type (e.g. 'TPMA_OBJECT').
     reserved: The list of bit bounds where bits must be 0 (e.g. ['10_2','3']).
  """
  # Attribute structures need an explicit cast to the base type.
  _ATTRIBUTE_MARSHAL_FUNCTION = """
UINT16 %(new_type)s_Marshal(
    %(new_type)s *source,
    BYTE **buffer,
    INT32 *size) {
  return %(old_type)s_Marshal((%(old_type)s*)source, buffer, size);
}
"""
  _ATTRIBUTE_UNMARSHAL_START = """
TPM_RC %(new_type)s_Unmarshal(
    %(new_type)s *target,
    BYTE **buffer,
    INT32 *size) {
  TPM_RC result;
  result = %(old_type)s_Unmarshal((%(old_type)s*)target, buffer, size);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""
  _CHECK_RESERVED = """
  if (target->reserved%(bits)s != 0) {
    return TPM_RC_RESERVED_BITS;
  }"""

  def __init__(self, old_type, new_type):
    """Initializes an AttributeStructure instance.

    Values may be added to reserved attribute during parsing.

    Args:
      old_type: The base type of the attribute structure.
      new_type: The name of the type.
    """
    self.old_type = old_type
    self.new_type = new_type
    self.reserved = []

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementations for AttributStructure to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in marshalled_types:
      return
    if self.old_type not in marshalled_types:
      typemap[self.old_type].OutputMarshalImpl(
          out_file, marshalled_types, typemap)
    out_file.write(self._ATTRIBUTE_MARSHAL_FUNCTION %
        {'old_type': self.old_type,
         'new_type': self.new_type})
    out_file.write(self._ATTRIBUTE_UNMARSHAL_START %
        {'old_type': self.old_type,
         'new_type': self.new_type})
    for bits in self.reserved:
      out_file.write(self._CHECK_RESERVED % {'bits': bits})
    out_file.write(self._UNMARSHAL_END)
    marshalled_types.add(self.new_type)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for AttributeStructure to |out_file|.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    self._OutputTypedefMarshalDecl(out_file, declared_types, typemap)

class Interface(TPMType):
  """Represents a TPM interface type definition.

  Attributes:
     old_type: The base type of the interface (e.g. 'TPM_HANDLE').
     new_type: The name of the type (e.g. 'TPMI_DH_OBJECT').
     valid_values: List of valid values for new_type. If this is not empty,
         valid values for new_type is explicitly defined in the spec.
     bounds: List of pairs representing bounds. If nonempty, target must fall
         between one of these bounds.
     conditional_value: Name of conditionally allowed value. If there is no
         such value, this variable will be None.
     supported_values: String literal indicating the name of a list of supported
         values to be substituted at compile time (e.g. 'AES_KEY_SIZES_BITS').
         If this is not None, valid values for new_type depends on the
         implementation.
     error_code: Return code when an unmarshalling error occurs.
  """
  _INTERFACE_CONDITIONAL_UNMARSHAL_START = """
TPM_RC %(new_type)s_Unmarshal(
    %(new_type)s *target,
    BYTE **buffer,
    INT32 *size,
    BOOL allow_conditional_value) {
  TPM_RC result;"""
  _INTERFACE_UNMARSHAL_START = """
TPM_RC %(new_type)s_Unmarshal(
    %(new_type)s *target,
    BYTE **buffer,
    INT32 *size) {
  TPM_RC result;"""
  _UNMARSHAL_VALUE = """
  result = %(old_type)s_Unmarshal(target, buffer, size);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""
  _SETUP_CHECK_SUPPORTED_VALUES = """
  uint16_t supported_values[] = %(supported_values)s;
  size_t length = sizeof(supported_values)/sizeof(supported_values[0]);
  size_t i;
  BOOL is_supported_value = FALSE;"""
  _CHECK_SUPPORTED_VALUES = """
  for (i = 0; i < length; ++i) {
    if (*target == supported_values[i]) {
      is_supported_value = TRUE;
      break;
    }
  }
  if (!is_supported_value) {
    return %(error_code)s;
  }"""
  _CHECK_CONDITIONAL = """
  if (*target == %(name)s) {
    return allow_conditional_value ? TPM_RC_SUCCESS : %(error_code)s;
  }"""
  _SETUP_CHECK_VALUES = '\n  BOOL has_valid_value = FALSE;'
  _VALUE_END_SWITCH = """
      has_valid_value = TRUE;
      break;
  }"""
  _CHECK_BOUND = """
  if((*target >= %(lower)s) && (*target <= %(upper)s)) {
    has_valid_value = TRUE;
  }"""
  _CHECK_VALUES_END = """
  if (!has_valid_value) {
    return %(error_code)s;
  }"""
  _CONDITIONAL_MARSHAL_DECLARATION = """
UINT16 %(type)s_Marshal(
    %(type)s *source,
    BYTE **buffer,
    INT32 *size);

TPM_RC %(type)s_Unmarshal(
    %(type)s *target,
    BYTE **buffer,
    INT32 *size,
    BOOL allow_conditioanl_value);
"""
  _CONDITIONAL_UNMARSHAL_CALL = """
  result = %(type)s_Unmarshal(
      &target->%(name)s, buffer, size, %(flag)s);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""
  _IFDEF_TYPE_RE = re.compile(r'^TPMI_(ALG|ECC)_.*')

  def __init__(self, old_type, new_type):
    """Initializes an Interface instance.

    Values may be added/assigned to valid_values, bounds, conditional_value,
    supported_values, and error_code attributes new values during parsing.

    Args:
      old_type: The base type of the interface.
      new_type: The name of the type.
    """
    self.old_type = old_type
    self.new_type = new_type
    self.valid_values = []
    self.bounds = []
    self.conditional_value = None
    self.supported_values = None
    self.error_code = 'TPM_RC_VALUE'

  def HasConditional(self):
    """Returns true if Interface has a valid conditional_value."""
    return self.conditional_value != None

  def _NeedsIfdef(self):
    """Returns True if new_type is a type which needs ifdef enclosing."""
    return self._IFDEF_TYPE_RE.search(self.new_type)

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementation for Interface to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in marshalled_types:
      return
    if self.old_type not in marshalled_types:
      typemap[self.old_type].OutputMarshalImpl(
          out_file, marshalled_types, typemap)
    out_file.write(self._TYPEDEF_MARSHAL_FUNCTION % {'old_type': self.old_type,
                                                     'new_type': self.new_type})
    if self.conditional_value:
      out_file.write(self._INTERFACE_CONDITIONAL_UNMARSHAL_START %
          {'old_type': self.old_type,
           'new_type': self.new_type})
    else:
      out_file.write(
          self._INTERFACE_UNMARSHAL_START % {'old_type': self.old_type,
                                             'new_type': self.new_type})
    # Creating necessary local variables.
    if self.supported_values:
      out_file.write(self._SETUP_CHECK_SUPPORTED_VALUES %
          {'supported_values': self.supported_values})
    if len(self.valid_values)+len(self.bounds) > 0:
      out_file.write(self._SETUP_CHECK_VALUES)

    out_file.write(self._UNMARSHAL_VALUE % {'old_type': self.old_type})

    if self.supported_values:
      out_file.write(self._CHECK_SUPPORTED_VALUES %
          {'supported_values': self.supported_values,
           'error_code': self.error_code})
    if self.conditional_value:
      out_file.write(
          self._CHECK_CONDITIONAL % {'name': self.conditional_value,
                                     'error_code': self.error_code})
    # Checking for valid values.
    if len(self.valid_values)+len(self.bounds) > 0:
      if len(self.valid_values) > 0:
        out_file.write(self._VALUE_START_SWITCH % {'name': '*target'})
        for value in self.valid_values:
          if self._NeedsIfdef():
            out_file.write(self._VALUE_CASE_IFDEF % {'value': value})
          else:
            out_file.write(self._VALUE_CASE % {'value': value})
        out_file.write(self._VALUE_END_SWITCH)
      for (lower, upper) in self.bounds:
        out_file.write(
            self._CHECK_BOUND % {'lower': lower, 'upper': upper})
      out_file.write(self._CHECK_VALUES_END % {'error_code': self.error_code})

    out_file.write(self._UNMARSHAL_END)
    marshalled_types.add(self.new_type)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for Interface to |out_file|.

       Outputted declaration depends on whether Interface type has a
       conditionally valid value.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.new_type in declared_types:
      return
    if self.old_type not in declared_types:
      typemap[self.old_type].OutputMarshalDecl(
          out_file, declared_types, typemap)
    if self.HasConditional():
      out_file.write(
          self._CONDITIONAL_MARSHAL_DECLARATION % {'type': self.new_type})
    else:
      out_file.write(self._MARSHAL_DECLARATION % {'type': self.new_type})
    declared_types.add(self.new_type)

  def OutputUnmarshalCall(
      self, out_file, field_type, field_name, conditional_valid):
    """Write a call to Unmarshal function for Interface type to |out_file|.

       Override TPMType OutputUnmarshalCall because when an Interface type has
       a conditionally valid value, a BOOL value (|conditional_valid|) is passed
       as a parameter.

    Args:
      out_file: The output file.
      field_type: The type of the field in 'target' struct to be unmarshalled.
      field_name: The name of the field in 'target' struct to be unmarshalled.
      conditional_valid: BOOL value passed to function call indicating if
          Interface conditional_value is valid.
    """
    if self.conditional_value:
      out_file.write(
          self._CONDITIONAL_UNMARSHAL_CALL % {'type': field_type,
                                              'name': field_name,
                                              'flag': conditional_valid})
    else:
      out_file.write(self._UNMARSHAL_CALL % {'type': field_type,
                                             'name': field_name})

class Structure(TPMType):
  """Represents a TPM structure.

  Attributes:
    name: The name of the structure.
    fields: A list of Field objects representing struct fields.
    upper_bounds: A dictionary of (name, val) tuples mapping name to max val.
    lower_bounds: A dictionary of (name, val) tuples mapping name to min val.
    size_check: Set if TPM2B structure must be size checked (triggered by size=)
    valid_tag_values: A list of values field tag is allowed to take.
    error_code: The return code to be returned if an error occurs
  """
  _STRUCTURE_MARSHAL_START = """
UINT16 %(name)s_Marshal(
    %(name)s *source,
    BYTE **buffer,
    INT32 *size) {
  UINT16 total_size = 0;"""
  _STRUCTURE_UNMARSHAL_START = """
TPM_RC %(name)s_Unmarshal(
    %(name)s *target,
    BYTE **buffer,
    INT32 *size) {
  TPM_RC result;"""
  _MARSHAL_END = '\n  return total_size;\n}\n'
  _SETUP_ARRAY_FIELD = '\n  INT32 i;'
  _CHECK_SIZE_START = """
  UINT32 start_size = *size;
  UINT32 struct_size;"""
  _CHECK_SIZE_END = """
  struct_size = start_size - *size - sizeof(target->t.size);
  if (struct_size != target->t.size) {
    return TPM_RC_SIZE;
  }"""
  _TPM2B_ZERO_SIZE = """
  if (target->t.size == 0) {
    return %(return_value)s;
  }"""
  _CHECK_BOUND = """
  if (target->%(name)s %(operator)s %(bound_value)s) {
    return %(error_code)s;
  }"""

  def __init__(self, name):
    """Initializes a Structure instance.

    Initially the instance will have no fields, upper_bounds, lower_bounds, or
    valid_tag_values. Those can be added with AddField(), AddUpperBound(),
    AddLowerBound(), and AddTagVal() methods.

    Args:
      name: The name of the structure.
    """
    self.name = name
    self.fields = []
    self.upper_bounds = {}
    self.lower_bounds = {}
    self.size_check = False
    self.valid_tag_values = []
    self.error_code = 'TPM_RC_VALUE'

  class Field(object):
    """Represents a field in TPM structure

    Attributes:
      field_type: The type of field.
      field_name: The name of the field.
      extra_argument: Argument passed to OutputMarshalCall. Value represents a
          BOOL value, selector value, or array length depending on context.
      is_array: Boolean indicating whether field is an array.
    """
    _MARSHAL_FIELD_ARRAY = """
  for (i = 0; i < source->%(array_length)s; ++i) {
    total_size += %(type)s_Marshal(
        &source->%(name)s[i], buffer, size);
  }"""
    _UNMARSHAL_FIELD_ARRAY = """
  for (i = 0; i < target->%(array_length)s; ++i) {
    result = %(type)s_Unmarshal(
        &target->%(name)s[i], buffer, size);
    if (result != TPM_RC_SUCCESS) {
      return result;
    }
  }"""
    def __init__(self, field_type, field_name, extra_argument, is_array):
      """Initializes a Structure.Field instance.

      Initially the instance will have no fields, upper_bounds, lower_bounds, or
      valid_tag_values. Those can be added with AddField(), AddUpperBound(),
      AddLowerBound(), and AddTagVal() methods.

      Args:
        field_type: The type of field.
        field_name: The name of the field.
        extra_argument: Argument passed to OutputMarshalCall. Value represents a
            BOOL value, selector value, or array length depending on context.
        is_array: Boolean indicating whether field is an array.
      """
      self.field_type = field_type
      self.field_name = field_name
      self.extra_argument = extra_argument
      self.is_array = is_array

    def OutputMarshal(self, out_file, typemap):
      """Write a call to marshal the field this instance represents.

      Args:
        out_file: The output file.
        typemap: A dict mapping type names to the corresponding object.
      """
      if self.is_array:
        out_file.write(
            self._MARSHAL_FIELD_ARRAY % {'type': self.field_type,
                                         'name': self.field_name,
                                         'array_length': self.extra_argument})
      else:
        typemap[self.field_type].OutputMarshalCall(
            out_file, self.field_type, self.field_name, self.extra_argument)

    def OutputUnmarshal(self, out_file, typemap):
      """Write a call to unmarshal the field this instance represents.

      Args:
        out_file: The output file.
        typemap: A dict mapping type names to the corresponding object.
      """
      if self.is_array:
        out_file.write(
            self._UNMARSHAL_FIELD_ARRAY % {'type': self.field_type,
                                           'name': self.field_name,
                                           'array_length': self.extra_argument})
      else:
        typemap[self.field_type].OutputUnmarshalCall(
            out_file, self.field_type, self.field_name, self.extra_argument)

  def AddField(self, field_type, field_name, extra_argument, is_array):
    """Adds a field to fields attribute in Structure.

    Args:
      field_name: The name of the field.
      field_type: The type of field.
      extra_argument: Argument passed to OutputMarshalCall. Value represents a
          BOOL value, selector value, or array length depending on context.
      is_array: Boolean indicating whether field is an array.
    """
    # Each TPM2B is a union of two sized buffers, one which is type specific
    # (the 't' element) and the other is a generic value (the 'b' element). For
    # this reason a 't.' is prepended for fields in a TPM2B type. See section
    # 9.11.6 in TCG TPM2.0 Library Specification, Part 2: Structures for more
    # details.
    if IsTPM2B(self.name):
      field_name = 't.' + field_name
      if is_array:
        extra_argument = 't.' + extra_argument
    self.fields.append(
        self.Field(field_type, field_name, extra_argument, is_array))

  def AddUpperBound(self, field_name, value):
    """Adds an upper bound for a field.

    Args:
       field_name: Name of field with bound.
       value: Value of upper bound.
    """
    if IsTPM2B(self.name):
      field_name = 't.' + field_name
    self.upper_bounds[field_name] = value

  def AddLowerBound(self, field_name, value):
    """Adds a lower bound for a field.

    Args:
       field_name: Name of field with bound.
       value: Value of lower bound.
    """
    if IsTPM2B(self.name):
      field_name = 't.' + field_name
    self.lower_bounds[field_name] = value

  def AddTagValue(self, value):
    """Adds a valid value for tag field.

    Args:
       value: Valid value for tag field.
    """
    self.valid_tag_values.append(value)

  def _GetFieldTypes(self):
    """Creates a set which holds all current field types.

    Returns:
      A set of field types.
    """
    return set([field.field_type for field in self.fields])

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementations for Structure to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if self.name in marshalled_types:
      return
    # Make sure any dependencies already have marshal functions defined.
    for field_type in self._GetFieldTypes():
      if field_type not in marshalled_types:
        typemap[field_type].OutputMarshalImpl(
            out_file, marshalled_types, typemap)
        marshalled_types.add(field_type)

    out_file.write(self._STRUCTURE_MARSHAL_START % {'name': self.name})
    # If any field is an array, create local variable INT32 i.
    for field in self.fields:
      if field.is_array:
        out_file.write(self._SETUP_ARRAY_FIELD)
        break
    for field in self.fields:
      field.OutputMarshal(out_file, typemap)
    out_file.write(self._MARSHAL_END)

    out_file.write(self._STRUCTURE_UNMARSHAL_START % {'name': self.name})
    if self.size_check:
      out_file.write(self._CHECK_SIZE_START)
    # If any field is an array, create local variable INT32 i.
    for field in self.fields:
      if field.is_array:
        out_file.write(self._SETUP_ARRAY_FIELD)
        break
    for field in self.fields:
      field.OutputUnmarshal(out_file, typemap)
      return_value = self.error_code
      if field.field_name == 't.size' and self.size_check:
        out_file.write(self._TPM2B_ZERO_SIZE % {'return_value': 'TPM_RC_SIZE'})
      if field.field_name == 't.size' and not self.size_check:
        out_file.write(
            self._TPM2B_ZERO_SIZE % {'return_value': 'TPM_RC_SUCCESS'})
      if field.field_name in self.upper_bounds:
        if (field.field_name == 'count' or
            field.field_name == 't.size' or
            field.field_name == 'size'):
          return_value = 'TPM_RC_SIZE'
        out_file.write(self._CHECK_BOUND %
            {'name': field.field_name,
             'operator': '>',
             'bound_value': self.upper_bounds[field.field_name],
             'error_code': return_value})
      if field.field_name in self.lower_bounds:
        if (field.field_name == 'count' or
            field.field_name == 't.size' or
            field.field_name == 'size'):
          return_value = 'TPM_RC_SIZE'
        out_file.write(self._CHECK_BOUND %
            {'name': field.field_name,
             'operator': '<',
             'bound_value': self.lower_bounds[field.field_name],
             'error_code': return_value})
      if field.field_name == 'tag' and len(self.valid_tag_values) != 0:
        out_file.write(self._VALUE_START_SWITCH % {'name': 'target->tag'})
        for value in self.valid_tag_values:
          out_file.write(self._VALUE_CASE % {'value': value})
        out_file.write(self._VALUE_END_SWITCH % {'error_code': 'TPM_RC_TAG'})
    if self.size_check:
      out_file.write(self._CHECK_SIZE_END)
    out_file.write(self._UNMARSHAL_END)

    marshalled_types.add(self.name)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for Structure to |out_file|.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    self._OutputStructOrUnionMarshalDecl(out_file, declared_types, typemap)

class Union(TPMType):
  """Represents a TPM union.

  Attributes:
    name: The name of the union.
    fields: A list of Field objects representing union fields.
  """

  _UNION_MARSHAL_START = """
UINT16 %(name)s_Marshal(
    %(name)s *source,
    BYTE **buffer,
    INT32 *size,
    UINT32 selector) {
  switch(selector) {"""
  _UNION_UNMARSHAL_START = """
TPM_RC %(name)s_Unmarshal(
    %(name)s *target,
    BYTE **buffer,
    INT32 *size,
    UINT32 selector) {
  switch(selector) {"""
  _MARSHAL_END = '\n  }\n  return 0;\n}\n'
  _UNMARSHAL_END = '\n  }\n  return TPM_RC_SELECTOR;\n}\n'
  _MARSHAL_DECLARATION = """
UINT16 %(type)s_Marshal(
    %(type)s *source,
    BYTE **buffer,
    INT32 *size,
    UINT32 selector);

TPM_RC %(type)s_Unmarshal(
    %(type)s *target,
    BYTE **buffer,
    INT32 *size,
    UINT32 selector);
"""
  _CASE_SELECTOR = """
    case %(selector)s:"""
  _MARSHAL_EMPTY = """
      return 0;"""
  _UNMARSHAL_EMPTY = """
      return TPM_RC_SUCCESS;"""
  _MARSHAL_FIELD = """
      return %(type)s_Marshal(
          (%(type)s*)&source->%(name)s, buffer, size);"""
  _UNMARSHAL_FIELD = """
      return %(type)s_Unmarshal(
          (%(type)s*)&target->%(name)s, buffer, size);"""
  _SETUP_MARSHAL_FIELD_ARRAY = """
    INT32 i;
    UINT16 total_size = 0;"""
  _SETUP_UNMARSHAL_FIELD_ARRAY = """
    INT32 i;
    TPM_RC result = TPM_RC_SUCCESS;"""
  _MARSHAL_FIELD_ARRAY = """
    for (i = 0; i < %(array_length)s; ++i) {
      total_size += %(type)s_Marshal(
          &source->%(name)s[i], buffer, size);
    }
    return total_size;"""
  _UNMARSHAL_FIELD_ARRAY = """
    for (i = 0; i < %(array_length)s; ++i) {
      result = %(type)s_Unmarshal(
          &target->%(name)s[i], buffer, size);
      if (result != TPM_RC_SUCCESS) {
        return result;
      }
    }
    return TPM_RC_SUCCESS;"""
  _UNMARSHAL_FIELD_CONDITIONAL = """
    return %(type)s_Unmarshal(
        &target->%(name)s, buffer, size, FALSE);"""
  _UNION_MARSHAL_CALL = """
  total_size += %(type)s_Marshal(
      &source->%(name)s, buffer, size, source->%(selector)s);"""
  _UNION_UNMARSHAL_CALL = """
  result = %(type)s_Unmarshal(
      &target->%(name)s, buffer, size, target->%(selector)s);
  if (result != TPM_RC_SUCCESS) {
    return result;
  }"""
  _IFDEF = '\n#ifdef %(type)s'
  _ENDIF = '\n#endif'
  _IFDEF_TYPE_RE = re.compile(r'^TPM_(ALG|CC).*')
  # Represents a field in a TPM union.
  Field = collections.namedtuple(
      'Field', ['field_type', 'field_name', 'array_length'])

  def __init__(self, name):
    """Initializes a Union instance.

    Initially the instance will have no fields. Fields are added with the
    AddField() method.

    Args:
      name: The name of the structure.
    """
    self.name = name
    self.fields = []

  def _NeedsIfdef(self, selector):
    """Returns True if selector is a type which needs ifdef enclosing."""
    return self._IFDEF_TYPE_RE.search(selector)

  def AddField(self, field_type, field_name, array_length):
    """Adds a field to fields attribute in Union.

    Args:
      field_name: The name of the field.
      field_type: The type of field.
      array_length: Length of array if field is an array type. None if not an
          array type.
    """
    # xor is a C++ keyword and must be fixed.
    if field_name == 'xor':
      field_name = 'xor_'
    self.fields.append(self.Field(field_type, field_name, array_length))

  def _OutputMarshalField(
      self, out_file, field_type, field_name, array_length, selector, typemap):
    """Write a call to marshal a field in this union.

    Args:
      out_file: The output file.
      field_name: The name of the field.
      field_type: The type of field.
      array_length: Variable indicating length of array, None if field is not
          an array.
      selector: Selector value associated with field.
      typemap: A dict mapping type names to the corresponding object.
    """
    if array_length:
      out_file.write(self._MARSHAL_FIELD_ARRAY % {'type': field_type,
                                                  'name': field_name,
                                                  'array_length': array_length})
    else:
      out_file.write(self._MARSHAL_FIELD % {'type': field_type,
                                            'name': field_name})

  def _OutputUnmarshalField(
      self, out_file, field_type, field_name, array_length, selector, typemap):
    """Write a call to unmarshal a field in this union.

    Args:
      out_file: The output file.
      field_name: The name of the field.
      field_type: The type of field.
      array_length: Variable indicating length of array, None if field is not
          an array.
      selector: Selector value associated with field.
      typemap: A dict mapping type names to the corresponding object.
    """
    if array_length:
      out_file.write(
          self._UNMARSHAL_FIELD_ARRAY % {'type': field_type,
                                         'name': field_name,
                                         'array_length': array_length})
    elif typemap[field_type].HasConditional():
      out_file.write(
          self._UNMARSHAL_FIELD_CONDITIONAL % {'type': field_type,
                                               'name': field_name})
    else:
      out_file.write(self._UNMARSHAL_FIELD % {'type': field_type,
                                              'name': field_name})

  def OutputMarshalImpl(self, out_file, marshalled_types, typemap):
    """Writes marshal implementations for Union to |out_file|.

    Args:
      out_file: The output file.
      marshalled_types: A set of types for which marshal and unmarshal functions
          have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    if (self.name in marshalled_types or
        self.name == 'TPMU_NAME' or
        self.name == 'TPMU_ENCRYPTED_SECRET'):
      return
    selector_values = union_selectors.GetUnionSelectorValues(self.name)
    field_types = {f.field_name: f.field_type for f in self.fields}
    array_lengths = {f.field_name: f.array_length for f in self.fields}
    out_file.write(self._UNION_MARSHAL_START % {'name': self.name})
    # Make sure any dependencies already have marshal functions defined.
    for field_name in field_types:
      field_type = field_types[field_name]
      if field_type not in marshalled_types:
        typemap[field_type].OutputMarshalImpl(
            out_file, marshalled_types, typemap)
        marshalled_types.add(field_type)
    # Set up variables if Union is an array type.
    if self.fields[0].array_length:
      out_file.write(self._SETUP_MARSHAL_FIELD_ARRAY)
    for selector in selector_values:
      field_name = union_selectors.GetUnionSelectorField(self.name, selector)
      if self._NeedsIfdef(selector):
        out_file.write(self._IFDEF % {'type': selector})
      out_file.write(self._CASE_SELECTOR % {'selector':selector})
      # Selector is not associated with a name, so no marshaling occurs.
      if not field_name:
        out_file.write(self._MARSHAL_EMPTY)
        if self._NeedsIfdef(selector):
          out_file.write(self._ENDIF)
        continue
      field_type = field_types[field_name]
      array_length = array_lengths[field_name]
      self._OutputMarshalField(
          out_file, field_type, field_name, array_length, selector, typemap)
      if self._NeedsIfdef(selector):
        out_file.write(self._ENDIF)
    out_file.write(self._MARSHAL_END)
    out_file.write(self._UNION_UNMARSHAL_START % {'name': self.name})
    # Set up variables if Union is an array type.
    if self.fields[0].array_length:
      out_file.write(self._SETUP_UNMARSHAL_FIELD_ARRAY)
    for selector in selector_values:
      field_name = union_selectors.GetUnionSelectorField(self.name, selector)
      if self._NeedsIfdef(selector):
        out_file.write(self._IFDEF % {'type': selector})
      out_file.write(self._CASE_SELECTOR % {'selector': selector})
      # Selector is not associated with a name, so no unmarshaling occurs.
      if not field_name:
        out_file.write(self._UNMARSHAL_EMPTY)
        if self._NeedsIfdef(selector):
          out_file.write(self._ENDIF)
        continue
      field_type = field_types[field_name]
      array_length = array_lengths[field_name]
      self._OutputUnmarshalField(
          out_file, field_type, field_name, array_length, selector, typemap)
      if self._NeedsIfdef(selector):
        out_file.write(self._ENDIF)
    out_file.write(self._UNMARSHAL_END)
    marshalled_types.add(self.name)

  def OutputMarshalDecl(self, out_file, declared_types, typemap):
    """Writes marshal declarations for Union to |out_file|.

    Args:
      out_file: The output file.
      declared_types: A set of types for which marshal and unmarshal function
          declarations have already been generated.
      typemap: A dict mapping type names to the corresponding object.
    """
    self._OutputStructOrUnionMarshalDecl(out_file, declared_types, typemap)

  def OutputMarshalCall(self, out_file, field_type, field_name, selector):
    """Write a call to marshal function for Union type to |out_file|.

       Override TPMType OutputMarshalCall to pass in selector value.

    Args:
      out_file: The output file.
      field_type: The type of the field in 'target' struct to be unmarshalled.
      field_name: The name of the field in 'target' struct to be unmarshalled.
      selector: The name of the selector value.
    """
    out_file.write(self._UNION_MARSHAL_CALL % {'type': field_type,
                                               'name': field_name,
                                               'selector': selector})

  def OutputUnmarshalCall(self, out_file, field_type, field_name, selector):
    """Write a call to unmarshal function for Union type to |out_file|.

       Override TPMType OutputUnmashalCall to pass in selector value.

    Args:
      out_file: The output file.
      field_type: The type of the field in 'target' struct to be unmarshalled.
      field_name: The name of the field in 'target' struct to be unmarshalled.
      selector: The name of the selector value.
    """
    out_file.write(self._UNION_UNMARSHAL_CALL % {'type': field_type,
                                                 'name': field_name,
                                                 'selector': selector})

class StructureParser(object):
  """Structure definition parser.

  The input text file is extracted from the PDF file containing the TPM
  structures specification from the Trusted Computing Group. The syntax
  of the text file is defined by extract_structures.sh.

  - Parses typedefs to a list of Typedef objects.
  - Parses constants to a list of ConstantType objects.
  - Parses attribute structures to a list of AttributeStruct objects.
  - Parses interfaces to a list of Interface objects.
  - Parses structs to a list of Structure objects.
  - Parses unions to a list of Union objects

  The parser also creates 'typemap' dict which maps every type to its generator
  object. This typemap helps manage type dependencies.

  Example usage:
  parser = StructureParser(open('myfile'))
  types, typemap = parser.Parse()
  """

  # Compile regular expressions.
  _BEGIN_TYPES_TOKEN = '_BEGIN_TYPES'
  _BEGIN_CONSTANTS_TOKEN = '_BEGIN_CONSTANTS'
  _BEGIN_ATTRIBUTE_STRUCTS_TOKEN = '_BEGIN_ATTRIBUTE_STRUCTS'
  _BEGIN_INTERFACES_TOKEN = '_BEGIN_INTERFACES'
  _BEGIN_STRUCTURES_TOKEN = '_BEGIN_STRUCTURES'
  _BEGIN_UNIONS_TOKEN = '_BEGIN_UNIONS'
  _BEGIN_DEFINES_TOKEN = '_BEGIN_DEFINES'
  _END_TOKEN = '_END'
  _OLD_TYPE_RE = re.compile(r'^_OLD_TYPE\s+(\w+)$')
  _NEW_TYPE_RE = re.compile(r'^_NEW_TYPE\s+(\w+)$')
  _STRUCTURE_RE = re.compile(r'^_STRUCTURE\s+(\w+)$')
  _UNION_RE = re.compile(r'^_UNION\s+(\w+)$')
  _TYPE_RE = re.compile(r'^_TYPE\s+(\w+)$')
  _NAME = r'[a-zA-Z0-9_()\[\]/\*\+\-]+'
  _NAME_RE = re.compile(r'^_NAME\s+([a-zA-Z0-9_()/]+)$')
  _SUBSTITUTE_RE = re.compile(r'^_SUBSTITUTE\s+([a-zA-Z0-9_()/]+)$')
  _CONDITIONAL_RE = re.compile(r'^_CONDITIONAL\s+([a-zA-Z0-9_()/]+)$')
  _MIN_RE = re.compile(r'^_MIN\s+([a-zA-Z0-9_()/]+)$')
  _MAX_RE = re.compile(r'^_MAX\s+([a-zA-Z0-9_()/]+)$')
  _RETURN_RE = re.compile(r'^_RETURN\s+([a-zA-Z0-9_()/]+)$')
  _RESERVED_RE = re.compile(r'^_RESERVED\s+([a-zA-Z0-9_()/]+)$')

  # Regular expressions for structures.
  _NAME_UNION_RE = re.compile(
      r'^_NAME\s+([a-zA-Z0-9_()/]+)\s+_UNION\s+([a-zA-Z0-9_()/]+)$')
  _NAME_ARRAY_RE = re.compile(
      r'^_NAME\s+([a-zA-Z0-9_()/]+)\s+_ARRAY\s+([a-zA-Z0-9_()/]+)$')
  _NAME_PLUS_RE = re.compile(r'^_NAME\s+([a-zA-Z0-9_()\[\]/\*\+\-]+)\s+_PLUS$')
  _VALID_RE = re.compile(r'^_VALID\s+([a-zA-Z0-9_()\[\]/\*\+\-]+)$')
  _FIELD_MAX_RE = re.compile(
      r'^_MAX\s+([a-zA-Z0-9_()/]+)\s+([a-zA-Z0-9_()/]+)$')
  _FIELD_MIN_RE = re.compile(
      r'^_MIN\s+([a-zA-Z0-9_()/]+)\s+([a-zA-Z0-9_()/]+)$')
  _NAME_SIZE_CHECK_RE = re.compile(r'^_NAME size _CHECK$')

  def __init__(self, in_file):
    """Initializes a StructureParser instance.

    Args:
      in_file: A file as returned by open() which has been opened for reading.
    """
    self._line = None
    self._in_file = in_file

  def _NextLine(self):
    """Gets the next input line.

    Returns:
     # The next input line if another line is available, None otherwise.
    """
    try:
      self._line = self._in_file.next()
    except StopIteration:
      self._line = None

  def Parse(self):
    """Parse everything in a structures file.

    Returns:
      A list of TPMType objects and a type-map as described in the class
      documentation. The list of TPMType objects in order contains objects of
      derived type Typdef, ConstantTypes, AttributeStruct, Interface,
      Structure, and Union.
    """
    self._NextLine()
    types = []
    typemap = {}
    # maps types to valid constants
    while self._line:
      if self._BEGIN_TYPES_TOKEN == self._line.rstrip():
        types += self._ParseTypes(typemap)
      elif self._BEGIN_CONSTANTS_TOKEN == self._line.rstrip():
        types += self._ParseConstants(typemap)
      elif self._BEGIN_ATTRIBUTE_STRUCTS_TOKEN == self._line.rstrip():
        types += self._ParseAttributeStructs(typemap)
      elif self._BEGIN_INTERFACES_TOKEN == self._line.rstrip():
        types += self._ParseInterfaces(typemap)
      elif self._BEGIN_STRUCTURES_TOKEN == self._line.rstrip():
        types += self._ParseStructures(typemap)
      elif self._BEGIN_UNIONS_TOKEN == self._line.rstrip():
        types += self._ParseUnions(typemap)
      else:
        print('Invalid file format: %s' % self._line)
        break
      self._NextLine()
    return types, typemap

  def _ParseTypes(self, typemap):
    """Parses a typedefs section.

    The current line should be _BEGIN_TYPES and the method will stop parsing
    when an _END line is found.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of Typedef objects.
    """
    types = []
    self._NextLine()
    while self._END_TOKEN != self._line.rstrip():
      match = self._OLD_TYPE_RE.search(self._line)
      if not match:
        print('Invalid old type: %s' % self._line)
        return types
      old_type = match.group(1)
      self._NextLine()
      match = self._NEW_TYPE_RE.search(self._line)
      if not match:
        print('Invalid new type: %s' % self._line)
        return types
      new_type = match.group(1)
      self._NextLine()
      # We don't need code for BOOL type to be generated.
      if new_type == 'BOOL':
        continue
      t = Typedef(old_type, new_type)
      types.append(t)
      typemap[new_type] = t
    return types

  def _ParseConstants(self, typemap):
    """Parses a constants section.

    The current line should be _BEGIN_CONSTANTS and the method will stop parsing
    when an _END line is found. Each group of constants has an associated type
    alias.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of Constant objects.
    """
    constants = []
    self._NextLine()
    while self._END_TOKEN != self._line.rstrip():
      match_old = self._OLD_TYPE_RE.search(self._line)
      if not match_old:
        print('Invalid constants section, expected OLD_TYPE: %s' % self._line)
        return constants
      old_type = match_old.group(1)
      self._NextLine()
      match_new = self._NEW_TYPE_RE.search(self._line)
      if not match_new:
        print('Invalid constants section, expected NEW_TYPE: %s' % self._line)
        return constants
      new_type = match_new.group(1)
      self._NextLine()
      current_constant_type = ConstantType(old_type, new_type)
      match_name = self._NAME_RE.search(self._line)
      if not match_name:
        print('Invalid constant name: %s' % self._line)
        return constants
      while match_name:
        name = match_name.group(1)
        current_constant_type.valid_values.append(name)
        self._NextLine()
        match_name = self._NAME_RE.search(self._line)
        match_return = self._RETURN_RE.search(self._line)
      if not match_return:
        print('Invalid constants section, expected RETURN: %s' % self._line)
        return constants
      current_constant_type.error_code = match_return.group(1)
      self._NextLine()
      # We don't need code for TPM_PS type to be generated.
      if new_type == 'TPM_PS':
        continue
      constants.append(current_constant_type)
      typemap[new_type] = current_constant_type
    return constants

  def _ParseAttributeStructs(self, typemap):
    """Parses an attribute structs section.

    The current line should be _BEGIN_ATTRIBUTE_STRUCTS and the method will
    stop parsing when an _END line is found. Each attribute structure has an
    associated type alias.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of AttributeStruct objects.
    """
    attribute_structs = []
    self._NextLine()
    while self._END_TOKEN != self._line.rstrip():
      match_old = self._OLD_TYPE_RE.search(self._line)
      if not match_old:
        print('Invalid attributes section, expected OLD_TYPE: %s' % self._line)
        return attribute_structs
      old_type = match_old.group(1)
      self._NextLine()
      match_new = self._NEW_TYPE_RE.search(self._line)
      if not match_new:
        print('Invalid attributes section, expected NEW_TYPE: %s' % self._line)
        return attribute_structs
      new_type = match_new.group(1)
      self._NextLine()
      current_attribute_struct = AttributeStructure(old_type, new_type)
      match_reserved = self._RESERVED_RE.search(self._line)
      while match_reserved:
        bits = match_reserved.group(1)
        current_attribute_struct.reserved.append(bits)
        self._NextLine()
        match_reserved = self._RESERVED_RE.search(self._line)
      attribute_structs.append(current_attribute_struct)
      typemap[new_type] = current_attribute_struct
    return attribute_structs

  def _ParseInterfaces(self, typemap):
    """Parses an interface types section.

    The current line should be _BEGIN_INTERFACES and the method will stop
    parsing when an _END line is found. Each interface type has an associated
    type alias.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of Interface objects.
    """
    interfaces = []
    self._NextLine()
    while self._END_TOKEN != self._line.rstrip():
      match_old = self._OLD_TYPE_RE.search(self._line)
      if not match_old:
        print('Invalid interfaces section, expected OLD_TYPE: %s' % self._line)
        return interfaces
      old_type = match_old.group(1)
      self._NextLine()
      match_new = self._NEW_TYPE_RE.search(self._line)
      if not match_new:
        print('Invalid interfaces section, expected NEW_TYPE: %s' % self._line)
        return interfaces
      new_type = match_new.group(1)
      self._NextLine()
      current_interface = Interface(old_type, new_type)
      while True:
        match_name = self._NAME_RE.search(self._line)
        match_conditional_value = self._CONDITIONAL_RE.search(self._line)
        match_min = self._MIN_RE.search(self._line)
        match_return = self._RETURN_RE.search(self._line)
        match_supported_values = self._SUBSTITUTE_RE.search(self._line)
        if match_name:
          current_interface.valid_values.append(match_name.group(1))
        elif match_supported_values:
          current_interface.supported_values = match_supported_values.group(1)
        elif match_min:
          lower = match_min.group(1)
          self._NextLine()
          match_max = self._MAX_RE.search(self._line)
          if not match_max:
            print('Invalid interfaces section, expected _MAX: %s' % self._line)
            return interfaces
          upper = match_max.group(1)
          current_interface.bounds.append((lower, upper))
        elif match_conditional_value:
          current_interface.conditional_value = match_conditional_value.group(1)
        elif match_return:
          current_interface.error_code = match_return.group(1)
          self._NextLine()
          break
        else:
          print('Invalid interfaces section: %s' % self._line)
          break
        self._NextLine()
      interfaces.append(current_interface)
      typemap[new_type] = current_interface
    return interfaces

  def _ParseStructures(self, typemap):
    """Parses a structures section.

    The current line should be _BEGIN_STRUCTURES and the method will stop
    parsing when an _END line is found.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of Structure objects.
    """
    structures = []
    self._NextLine()
    while  self._END_TOKEN != self._line.rstrip():
      match_structure = self._STRUCTURE_RE.search(self._line)
      if not match_structure:
        print('Invalid structure section, expected _STRUCTURE: %s' % self._line)
        return structures
      structure_name = match_structure.group(1)
      current_structure = Structure(structure_name)
      self._NextLine()
      match_type = self._TYPE_RE.search(self._line)
      if not match_type:
        print('Invalid field type: %s' % self._line)
        return structures
      while match_type:
        field_type = match_type.group(1)
        self._NextLine()
        match_name = self._NAME_RE.search(self._line)
        match_name_union = self._NAME_UNION_RE.search(self._line)
        match_name_array = self._NAME_ARRAY_RE.search(self._line)
        match_name_plus = self._NAME_PLUS_RE.search(self._line)
        match_name_size_check = self._NAME_SIZE_CHECK_RE.search(self._line)
        if match_name:
          field_name = match_name.group(1)
          current_structure.AddField(
              field_type, field_name, 'FALSE', False)
          self._NextLine()
          # Check for extra lines after tag field.
          if field_name == 'tag':
            match_valid = self._VALID_RE.search(self._line)
            if match_valid:
              while True:
                match_valid = self._VALID_RE.search(self._line)
                if match_valid:
                  current_structure.AddTagValue(match_valid.group(1))
                  self._NextLine()
                else:
                  self._NextLine()
                  break
          # Check for bounds.
          else:
            match_field_max = self._FIELD_MAX_RE.search(self._line)
            match_field_min = self._FIELD_MIN_RE.search(self._line)
            if match_field_max:
              current_structure.AddUpperBound(
                  match_field_max.group(1), match_field_max.group(2))
              self._NextLine()
            elif match_field_min:
              current_structure.AddLowerBound(
                  match_field_min.group(1), match_field_min.group(2))
              self._NextLine()
        elif match_name_union:
          field_name = match_name_union.group(1)
          selector = match_name_union.group(2)
          current_structure.AddField(
              field_type, field_name, selector, False)
          self._NextLine()
        elif match_name_array:
          field_name = match_name_array.group(1)
          array_length = match_name_array.group(2)
          current_structure.AddField(
              field_type, field_name, array_length, True)
          self._NextLine()
          # Check for bounds.
          match_field_max = self._FIELD_MAX_RE.search(self._line)
          match_field_min = self._FIELD_MIN_RE.search(self._line)
          if match_field_max:
            current_structure.AddUpperBound(
                match_field_max.group(1), match_field_max.group(2))
            self._NextLine()
          elif match_field_min:
            current_structure.AddLowerBound(
                match_field_min.group(1), match_field_min.group(2))
            self._NextLine()
        elif match_name_plus:
          field_name = match_name_plus.group(1)
          current_structure.AddField(
              field_type, field_name, 'TRUE', False)
          self._NextLine()
        elif match_name_size_check:
          field_name = 'size'
          current_structure.AddField(
              field_type, field_name, 'FALSE', False)
          current_structure.size_check = True
          self._NextLine()
        else:
          print('Invalid field name: %s' % self._line)
          return structures
        match_type = self._TYPE_RE.search(self._line)
        match_return = self._RETURN_RE.search(self._line)
        if match_return:
          current_structure.error_code = match_return.group(1)
          self._NextLine()
      structures.append(current_structure)
      typemap[structure_name] = current_structure
    return structures

  def _ParseUnions(self, typemap):
    """Parses a unions section.

    The current line should be _BEGIN_UNIONS and the method will stop parsing
    when an _END line is found.

    Args:
      typemap: A dictionary to which parsed types are added.

    Returns:
      A list of Union objects.
    """
    unions = []
    self._NextLine()
    while  self._END_TOKEN != self._line.rstrip():
      match_union = self._UNION_RE.search(self._line)
      if not match_union:
        print('Invalid structure section, expected _STRUCTURE: %s' % self._line)
        return unions
      union_name = match_union.group(1)
      current_union = Union(union_name)
      self._NextLine()
      match_type = self._TYPE_RE.search(self._line)
      if not match_type:
        print('Invalid field type: %s' % self._line)
        return unions
      while match_type:
        field_type = match_type.group(1)
        self._NextLine()
        match_name = self._NAME_RE.search(self._line)
        match_name_array = self._NAME_ARRAY_RE.search(self._line)
        if match_name:
          field_name = match_name.group(1)
          # Field could be interface type with conditional value
          current_union.AddField(field_type, field_name, None)
          self._NextLine()
        elif match_name_array:
          field_name = match_name_array.group(1)
          array_length = match_name_array.group(2)
          current_union.AddField(field_type, field_name, array_length)
          self._NextLine()
        else:
          print('Invalid field name: %s' % self._line)
          return unions
        match_type = self._TYPE_RE.search(self._line)
      unions.append(current_union)
      typemap[union_name] = current_union
    return unions

def GenerateHeader(types, typemap):
  """Generates a header file with declarations for all given generator objects.

  Args:
    types: A list of Typedef objects.
    typemap: A dict mapping type names to the corresponding object.
  """
  out_file = open(_OUTPUT_FILE_H, 'w')
  out_file.write(_COPYRIGHT_HEADER)
  guard_name = 'TPM2_%s_' % _OUTPUT_FILE_H.upper().replace('.', '_')
  out_file.write(_HEADER_FILE_GUARD_HEADER % {'name': guard_name})
  out_file.write(_HEADER_FILE_INCLUDES)
  # These types are built-in or defined by <stdint.h>; they serve as base cases
  # when defining type dependencies.
  declared_types = set(_BASIC_TYPES)
  # Generate serialize / parse function declarations.
  for basic_type in _BASIC_TYPES:
    out_file.write(_STANDARD_MARSHAL_DECLARATION % {'type': basic_type})
  for tpm_type in types:
    tpm_type.OutputMarshalDecl(out_file, declared_types, typemap)
  out_file.write(_HEADER_FILE_GUARD_FOOTER % {'name': guard_name})
  out_file.close()
  call(['clang-format', '-i', '-style=Chromium', 'tpm_generated.h'])


def GenerateImplementation(types, typemap):
  """Generates implementation code for each type.

  Args:
    types: A list of Typedef objects.
    typemap: A dict mapping type names to the corresponding object.
  """
  out_file = open(_OUTPUT_FILE_CC, 'w')
  out_file.write(_COPYRIGHT_HEADER)
  out_file.write(_IMPLEMENTATION_FILE_INCLUDES)
  marshalled_types = set(_BASIC_TYPES)
  for basic_type in _BASIC_TYPES:
    out_file.write(_MARSHAL_BASIC_TYPE % {'type': basic_type})
  for tpm_type in types:
    tpm_type.OutputMarshalImpl(out_file, marshalled_types, typemap)
  out_file.close()
  call(['clang-format', '-i', '-style=Chromium', 'tpm_generated.c'])
