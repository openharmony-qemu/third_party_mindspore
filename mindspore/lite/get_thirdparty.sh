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
rm -rf v2.0.0.tar.gz*

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
