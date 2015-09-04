#!/usr/bin/python2

# Copyright 2015 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import structure_generator
import command_generator

"""A main file for command and structure generators.

Takes in structures.txt and commands.txt as outputted by extract_*.sh, then
passes files as input to structure_generator and command_generator objects.
"""

def main():
  """A main function.

  TPM structures and commands files are parsed and C header and C implementation
  files are generated.

  Positional Args:
    structures_file: The extracted TPM structures file.
    commands_file: The extracted TPM commands file.
  """
  parser = argparse.ArgumentParser(description='TPM 2.0 code generator')
  parser.add_argument('structures_file')
  parser.add_argument('commands_file')
  args = parser.parse_args()
  structure_parser = structure_generator.StructureParser(
      open(args.structures_file))
  _, typemap = structure_parser.Parse()
  types = [typemap[x] for x in sorted(typemap.keys())]
  command_parser = command_generator.CommandParser(open(args.commands_file))
  commands = command_parser.Parse()
  structure_generator.GenerateHeader(types, typemap)
  structure_generator.GenerateImplementation(types, typemap)
  command_generator.GenerateHeader(commands)
  command_generator.GenerateImplementation(commands, typemap)
  print('Processed %d TPM types.' % len(types))
  print('Processed %d commands.' % len(commands))

if __name__ == '__main__':
  main()
