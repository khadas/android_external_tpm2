#!/bin/bash -eux
# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Builds fuzzers from within a container into /out/ director.
# Expects /src/tpm2 to contain tpm2 checkout.

mkdir /work/tpm2
cd /src/tpm2
make V=1 obj=/work/tpm2 /work/tpm2/libtpm2.a

find /usr/lib -name "*.a"
grep -r BN_CTX_get /usr/lib/

$CXX $CXXFLAGS $FUZZER_LDFLAGS -std=c++11 \
  /src/tpm2/fuzz/execute-command.cc -o /out/tpm2_execute_command_fuzzer \
  -lfuzzer /work/tpm2/libtpm2.a \
  /usr/lib/x86_64-linux-gnu/libcrypto.a /usr/lib/x86_64-linux-gnu/libssl.a \
  -I /src/tpm2
