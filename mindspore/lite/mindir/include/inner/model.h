/**
 * Copyright 2021 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MINDSPORE_LITE_MODEL_H
#define MINDSPORE_LITE_MODEL_H
#include <string>
#include <vector>
#include "mindir_types.h"

namespace mindspore {
namespace lite {

struct QuantParam2 {
  int32_t zeroPoint;
  double scale;
  int numBits;
};

struct Tensor {
  std::string name;
  DataType dataType;
  std::vector<int32_t> dims;
  Format format;
  std::vector<uint8_t> data;
  std::vector<QuantParam> quantParams;
};

struct Node {
  std::string name;
  NodeType nodeType;
  std::vector<int8_t> nodeAttr;
  std::vector<uint32_t> inputIndex;
  std::vector<uint32_t> outputIndex;
  QuantType quantType;
};

struct SubGraph {
  std::string name;
  std::vector<uint32_t> inputIndices;
  std::vector<uint32_t> outputIndices;
  std::vector<uint32_t> nodeIndices;
};

}  // namespace lite
}  // namespace mindspore
#endif  // MINDSPORE_LITE_MODEL_H
