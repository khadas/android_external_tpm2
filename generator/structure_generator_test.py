#!/usr/bin/python2

# Copyright 2015 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Unit tests for the libtpm2 structure_generator."""

from __future__ import print_function

import StringIO
import unittest

import structure_generator


class TestGenerators(unittest.TestCase):
  """Test structure_generator classes."""

  def testTypedefMarshal(self):
    """Test generation of marshaling code for typedefs."""
    marshalled_types = set(['int'])
    typedef = structure_generator.Typedef('int', 'INT')
    typedef2 = structure_generator.Typedef('INT', 'INT2')
    typemap = {'INT': typedef}
    out_file = StringIO.StringIO()
    typedef2.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('INT', marshalled_types)
    self.assertIn('INT2', marshalled_types)
    out_file.close()

  def testConstantTypeMarshal(self):
    """Test generation of marshaling code for constant types."""
    marshalled_types = set(['int'])
    typedef = structure_generator.Typedef('int', 'UINT16')
    constant = structure_generator.ConstantType('UINT16', 'TPM_TYPE')
    constant.valid_values.append('VALUE0')
    constant.valid_values.append('VALUE1')
    typemap = {'UINT16': typedef}
    out_file = StringIO.StringIO()
    constant.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('UINT16', marshalled_types)
    self.assertIn('TPM_TYPE', marshalled_types)
    out_file.close()

  def testAttributeStructureMarshal(self):
    """Test generation of marshaling code for attribute structures."""
    marshalled_types = set(['int'])
    typedef = structure_generator.Typedef('int', 'UINT16')
    attributeStruct = structure_generator.AttributeStructure(
        'UINT16', 'TPM_TYPE')
    attributeStruct.reserved.append('4_7')
    attributeStruct.reserved.append('1')
    typemap = {'UINT16': typedef}
    out_file = StringIO.StringIO()
    attributeStruct.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('UINT16', marshalled_types)
    self.assertIn('TPM_TYPE', marshalled_types)
    out_file.close()

  def testInterfacemarshal(self):
    """test generation of marshaling code for interfaces."""
    marshalled_types = set(['int'])
    typedef = structure_generator.Typedef('int', 'UINT16')
    interface = structure_generator.Interface('UINT16', 'TPM_TYPE')
    interface.conditional = 'TPM_VALUE_NULL'
    interface.bounds.append(('TPM_MIN', 'TPM_MAX'))
    interface.valid_values.append('VALUE0')
    interface.valid_values.append('VALUE1')
    typemap = {'UINT16': typedef}
    out_file = StringIO.StringIO()
    interface.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('UINT16', marshalled_types)
    self.assertIn('TPM_TYPE', marshalled_types)
    out_file.close()

  def testStructMarshal(self):
    """Test generation of marshaling code for structures."""
    marshalled_types = set(['int'])
    struct = structure_generator.Structure('TEST_STRUCT')
    struct.AddField('UINT16', 'type', None, False)
    struct.AddField('TPMI_TYPE', 'interfaceField0', 'TRUE', False)
    struct.AddField('TPMI_TYPE', 'interfaceField1', 'FALSE', False)
    struct.AddField('TPMU_SYM_MODE', 'unionField', 'type', False)
    struct.AddField('UINT16', 'arrayField', 'MAX_VALUE', True)
    typedef = structure_generator.Typedef('int', 'UINT16')
    interface = structure_generator.Interface('UINT16', 'TPMI_TYPE')
    # Choose TPMU_SYM_MODE because it exists in selectors definition and it
    # has few fields.
    union = structure_generator.Union('TPMU_SYM_MODE')
    union.AddField('UINT16', 'aes', None)
    union.AddField('UINT16', 'SM4', None)
    typemap = {
        'UINT16': typedef,
        'TPMI_TYPE': interface,
        'TPMU_SYM_MODE': union
    }
    out_file = StringIO.StringIO()
    struct.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('UINT16', marshalled_types)
    self.assertIn('TPMI_TYPE', marshalled_types)
    self.assertIn('TPMU_SYM_MODE', marshalled_types)
    self.assertIn('TEST_STRUCT', marshalled_types)
    out_file.close()

  def testUnionMarshal(self):
    """Test generation of marshaling code for unions."""
    marshalled_types = set(['int'])
    union = structure_generator.Union('TPMU_SYM_MODE')
    union.AddField('UINT16', 'aes', None)
    union.AddField('UINT16', 'SM4', None)
    typedef = structure_generator.Typedef('int', 'UINT16')
    typemap = {'UINT16': typedef}
    out_file = StringIO.StringIO()
    union.OutputMarshalImpl(out_file, marshalled_types, typemap)
    self.assertIn('UINT16', marshalled_types)
    self.assertIn('TPMU_SYM_MODE', marshalled_types)
    out_file.close()

  def _MakeArg(self, arg_type, arg_name):
    return {'type': arg_type,
            'name': arg_name,
            'command_code': None,
            'description': None}


class TestParser(unittest.TestCase):
  """Test structure parser."""

  FAKE_TYPEDEF = '_BEGIN_TYPES\n_OLD_TYPE uint16_t\n_NEW_TYPE UINT16\n_END\n'
  FAKE_CONSTANT = ('_BEGIN_CONSTANTS\n_OLD_TYPE base_type\n'
                   '_NEW_TYPE const_type\n'
                   '_NAME const_name\n_RETURN return_name\n_END\n')
  FAKE_ATT_STRUCT = ('_BEGIN_ATTRIBUTE_STRUCTS\n_OLD_TYPE base_type\n'
                     '_NEW_TYPE att_type\n_RESERVED 4_7\n_RESERVED 9\n_END\n')
  FAKE_INTERFACE = ('_BEGIN_INTERFACES\n_OLD_TYPE base_type\n'
                    '_NEW_TYPE interface_type\n_NAME const_name\n'
                    '_MIN min_name\n_MAX max_name\n_CONDITIONAL null_name\n'
                    '_RETURN return_name\n_END\n')
  FAKE_STRUCTURE = ('_BEGIN_STRUCTURES\n_STRUCTURE struct_type\n_TYPE UINT16\n'
                    '_NAME field1\n_MIN field1 min\n_TYPE UINT16\n'
                    '_NAME field2\n_TYPE UINT16\n_NAME field3 _ARRAY field1\n'
                    '_MAX field1 max\n'
                    '_TYPE UINT16\n_NAME field4 _UNION field2\n'
                    '_TYPE interface_type\n_NAME field5 _PLUS\n_END\n')
  FAKE_UNION = ('_BEGIN_UNIONS\n_UNION union_type\n_TYPE field1_type\n'
                '_NAME field1\n_TYPE field2_type\n_NAME field2\n_END\n')

  def testStructureParserWithBadData(self):
    """Test the structure parser with invalid data."""
    input_data = 'bad_data'
    in_file = StringIO.StringIO(input_data)
    parser = structure_generator.StructureParser(in_file)
    types, typemap = parser.Parse()
    self.assertIsNotNone(types)
    self.assertIsNotNone(typemap)

  def testStructureParser(self):
    """Test the structure parser with valid data.

       One of each typedef, constant type, attribute structure, interface,
       structure, and union. Should appear in types array in that order.
    """
    input_data = (self.FAKE_TYPEDEF + self.FAKE_CONSTANT +
                  self.FAKE_ATT_STRUCT + self.FAKE_INTERFACE +
                  self.FAKE_STRUCTURE + self.FAKE_UNION)
    in_file = StringIO.StringIO(input_data)
    parser = structure_generator.StructureParser(in_file)
    types, _ = parser.Parse()
    self.assertEqual(len(types), 6)
    self.assertEqual(types[0].old_type, 'uint16_t')
    self.assertEqual(types[0].new_type, 'UINT16')
    self.assertEqual(types[1].old_type, 'base_type')
    self.assertEqual(types[1].new_type, 'const_type')
    self.assertEqual(types[1].valid_values[0], 'const_name')
    self.assertEqual(types[1].error_code, 'return_name')
    self.assertEqual(types[2].old_type, 'base_type')
    self.assertEqual(types[2].new_type, 'att_type')
    self.assertEqual(types[2].reserved[0], '4_7')
    self.assertEqual(types[2].reserved[1], '9')
    self.assertEqual(types[3].old_type, 'base_type')
    self.assertEqual(types[3].new_type, 'interface_type')
    self.assertEqual(types[3].bounds[0][0], 'min_name')
    self.assertEqual(types[3].bounds[0][1], 'max_name')
    self.assertEqual(types[3].valid_values[0], 'const_name')
    self.assertEqual(types[3].conditional_value, 'null_name')
    self.assertEqual(types[3].error_code, 'return_name')
    self.assertEqual(types[4].name, 'struct_type')
    self.assertEqual(types[4].fields[0].field_type, 'UINT16')
    self.assertEqual(types[4].fields[0].field_name, 'field1')
    self.assertEqual(types[4].fields[1].field_type, 'UINT16')
    self.assertEqual(types[4].fields[1].field_name, 'field2')
    self.assertEqual(types[4].fields[2].field_type, 'UINT16')
    self.assertEqual(types[4].fields[2].field_name, 'field3')
    self.assertEqual(types[4].fields[2].extra_argument, 'field1')
    self.assertEqual(types[4].fields[3].field_type, 'UINT16')
    self.assertEqual(types[4].fields[3].field_name, 'field4')
    self.assertEqual(types[4].fields[3].extra_argument, 'field2')
    self.assertEqual(types[4].fields[4].field_type, 'interface_type')
    self.assertEqual(types[4].fields[4].field_name, 'field5')
    self.assertEqual(types[4].fields[4].extra_argument, 'TRUE')
    self.assertEqual(types[4].upper_bounds['field1'], 'max')
    self.assertEqual(types[4].lower_bounds['field1'], 'min')
    self.assertEqual(types[5].name, 'union_type')
    self.assertEqual(types[5].fields[0].field_type, 'field1_type')
    self.assertEqual(types[5].fields[0].field_name, 'field1')
    self.assertEqual(types[5].fields[1].field_type, 'field2_type')
    self.assertEqual(types[5].fields[1].field_name, 'field2')

if __name__ == '__main__':
  unittest.main()
