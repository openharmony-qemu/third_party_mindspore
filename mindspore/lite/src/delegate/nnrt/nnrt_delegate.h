/**
 * Copyright 2022 Huawei Technologies Co., Ltd
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
#ifndef MINDSPORE_NNR_DELEGATE_H
#define MINDSPORE_NNR_DELEGATE_H
#include <vector>
#include <map>
#include "include/api/delegate.h"
#include "include/context.h"
#include "include/model.h"
#include "kits/c/neural_network_runtime_type.h"
namespace mindspore {
class NNRTDelegate : public Delegate {
 public:
  NNRTDelegate() : Delegate(){};

  ~NNRTDelegate() override;

  Status Init() override;

  Status Build(DelegateModel<schema::Primitive> *model) override;

  const lite::LiteGraph &getLiteGraph() const;
  void setLiteGraph(const lite::LiteGraph &liteGraph);

 protected:
  lite::LiteGraph lite_graph_ = {};

  OH_NNCompilation *oh_nncompilation{};
  OH_NNExecutor *oh_nnexecutor{};
};

}  // namespace mindspore

#endif  // MINDSPORE_NNR_DELEGATE_H
