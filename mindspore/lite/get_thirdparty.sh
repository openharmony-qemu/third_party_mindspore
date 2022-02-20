#!/bin/bash
# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
set -e

BASEPATH=$(cd "$(dirname $0)"; pwd)
echo ${BASEPATH}

cd ${BASEPATH}/../../third_party/

get_flatbuffers() {
  rm -rf flatbuffers-v2.0.0/
  rm -rf v2.0.0.tar.gz
  wget https://gitee.com/mirrors/flatbuffers/repository/archive/v2.0.0.tar.gz
  sha256_calc=$(sha256sum v2.0.0.tar.gz | awk '{print $1}')
  sha256_value='3d1eabe298ddac718de34d334aefc22486064dcd8e7a367a809d87393d59ac5a'
  if [ "X${sha256_calc}" == "X${sha256_value}" ]; then
    echo -e "Sha256 check ok."
  else
    echo -e "Sha256 check Failed!"
    exit 1
  fi
  tar -xzf v2.0.0.tar.gz
}

get_OpenCL-Headers() {
  rm -rf OpenCL-Headers-v2020.12.18/
  rm -rf v2020.12.18.tar.gz
  wget https://gitee.com/mirrors/OpenCL-Headers/repository/archive/v2020.12.18.tar.gz
  sha256_calc=$(sha256sum v2020.12.18.tar.gz | awk '{print $1}')
  sha256_value='076251b94284b931399ee525527bc9aef3f5f6f3f3b1964ae485218cc88956ba'
  if [ "X${sha256_calc}" == "X${sha256_value}" ]; then
    echo -e "Sha256 check ok."
  else
    echo -e "Sha256 check Failed!"
    exit 1
  fi
  tar -xzf v2020.12.18.tar.gz
}

get_OpenCL-CLHPP() {
  rm -rf OpenCL-CLHPP-v2.0.12/
  rm -rf v2.0.12.tar.gz
  wget https://gitee.com/mirrors/OpenCL-CLHPP/repository/archive/v2.0.12.tar.gz
  sha256_calc=$(sha256sum v2.0.12.tar.gz | awk '{print $1}')
  sha256_value='d5bdbfb614a6494de97abf7297db6d2c88a55a095b12949d797ce562f5d4fdce'
  if [ "X${sha256_calc}" == "X${sha256_value}" ]; then
    echo -e "Sha256 check ok."
  else
    echo -e "Sha256 check Failed!"
    exit 1
  fi
  tar -xzf v2.0.12.tar.gz
}

gene_clhpp() {
  CL_SRC_DIR=$1
  if [ ! -d "${CL_SRC_DIR}" ]; then
    return
  fi
  cd ${CL_SRC_DIR}/
  rm -rf *.inc
  for file_path in "${CL_SRC_DIR}"/*
  do
    if [ -f "${file_path}" ]; then
      file="$(basename ${file_path})"
      inc_file=$(echo ${CL_SRC_DIR}/${file} | sed 's/$/.inc/')
      sed 's/\\/\\\\/g;s/\"/\\\"/g;s/^/\"/;s/$/\\n\" \\/' ${CL_SRC_DIR}/${file} > ${inc_file}
      kernel_name=$(echo ${file} | sed s'/.\{3\}$//')
      sed -i "1i\static const char *${kernel_name}_source =\"\\n\" \\" ${inc_file}
      sed -i '$a\;' ${inc_file}
    fi
  done
}

#===============begin===============#
get_flatbuffers
get_OpenCL-Headers
get_OpenCL-CLHPP
gene_clhpp "${BASEPATH}/src/runtime/kernel/opencl/cl"
gene_clhpp "${BASEPATH}/src/runtime/kernel/opencl/cl/int8"
#===============end=================#
