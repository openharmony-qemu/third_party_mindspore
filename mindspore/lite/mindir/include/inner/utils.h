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
#ifndef MIDIR_LITE_UTILS_H
#define MIDIR_LITE_UTILS_H
#include "mindir.h"
#include "schema/model_generated.h"
namespace mindspore {
namespace lite {

// ********** PrimitiveBase **********
mindspore::schema::PrimitiveType MindIR_GetPrimitiveType(PrimitivePtr prim);

PrimitivePtr MindIR_CreatePrimitiveFromBuilder(flatbuffers::FlatBufferBuilder &fbb);

flatbuffers::Offset<schema::Vec2D> CreateVec2D(flatbuffers::FlatBufferBuilder &fbb,
                                               const std::vector<std::vector<int64_t>> &data);
flatbuffers::Offset<schema::Vec2D> CreateVec2D(flatbuffers::FlatBufferBuilder &fbb,
                                               const mindspore::schema::Vec2D *data);

flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<schema::QuantParam>>> ConvertQuantParams(
  flatbuffers::FlatBufferBuilder &fbb, const std::vector<QuantParam> &quant_params);
flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<schema::QuantParam>>> ConvertQuantParams(
  flatbuffers::FlatBufferBuilder &fbb,
  const flatbuffers::Vector<flatbuffers::Offset<mindspore::schema::QuantParam>> *quant_params);
}  // namespace lite
}  // namespace mindspore
#endif  // MIDIR_LITE_UTILS_H
