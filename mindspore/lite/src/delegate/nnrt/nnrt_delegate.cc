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
#include "nnrt_delegate.h"
#include "src/common/log_adapter.h"
#include "kits/c/neural_network_runtime_type.h"
#include "kits/c/neural_network_runtime.h"
#include "innerkits/c/neural_network_runtime_inner.h"
#include "nnrt_model_kernel.h"
mindspore::Status mindspore::NNRTDelegate::Build(DelegateModel<schema::Primitive> *model) {
  if (this->lite_graph_.sub_graphs_.empty()) {
    // must have at lease one subgraph
    MS_LOG(ERROR) << "must have at lease one subgraph";
    return mindspore::kLiteError;
  }

  OH_NN_ReturnCode ret_code;
  OH_NNModel *oh_nnmodel = nullptr;
  ret_code = OH_NNModel_BuildFromLiteGraph(oh_nnmodel, &this->lite_graph_);
  if (ret_code != OH_NN_SUCCESS) {
    MS_LOG(ERROR) << "build NNModel failed, OH_NN_ReturnCode = " << ret_code;
    return mindspore::kLiteError;
  }
  MS_LOG(DEBUG) << "NNRTDelegate creates NNModel success.";

  OH_NNCompilation *oh_nn_compilation = nullptr;
  oh_nn_compilation = OH_NNCompilation_Create(oh_nnmodel);

  if (oh_nn_compilation == nullptr) {
    MS_LOG(ERROR) << "create NNCompilation failed";
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kLiteError;
  }
  MS_LOG(DEBUG) << "NNRTDelegate creates NNCompilation success.";

  const size_t *allDevicesID = nullptr;
  uint32_t *device_count;
  ret_code = OH_NNDevice_GetAllDevicesID(&allDevicesID, device_count);
  if (ret_code != OH_NN_SUCCESS) {
    MS_LOG(ERROR) << "NNModel GetAllDevicesID failed, OH_NN_ReturnCode = " << ret_code;
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kLiteError;
  }

  if (*device_count <= 0) {
    MS_LOG(WARNING) << "No HHR Device found, fall back to CPU";
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kSuccess;
  }
  MS_LOG(DEBUG) << "NNRTDelegate GetAllDevicesID success.";

  // check if model ops are supported
  const bool *issupported = nullptr;
  uint32_t *op_count;
  ret_code = OH_NNModel_GetSupportedOperation(oh_nnmodel, allDevicesID[0], &issupported, op_count);
  if (ret_code != OH_NN_SUCCESS) {
    MS_LOG(ERROR) << "NNModel GetSupportedOperation failed, OH_NN_ReturnCode = " << ret_code;
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kLiteError;
  }
  uint32_t supported_op_count;
  for (int i = 0; i < *op_count; i++) {
    if (op_count[i]) {
      supported_op_count++;
    }
  }
  if (*op_count != supported_op_count) {
    MS_LOG(WARNING) << "this model has " << *op_count << "ops, but NNRT only support " << supported_op_count
                    << "ops, fall back to CPU";
    // must support all op, else fall back to CPU
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kSuccess;
  }
  MS_LOG(DEBUG) << "NNRTDelegate supports all op in this model.";

  ret_code = OH_NNCompilation_SetDevice(oh_nn_compilation, allDevicesID[0]);

  if (ret_code != OH_NN_SUCCESS) {
    MS_LOG(ERROR) << "NNCompilation SetDevice failed, OH_NN_ReturnCode = " << ret_code;
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kLiteError;
  }

  MS_LOG(DEBUG) << "NNRTDelegate SetDevice success.";

  OH_NNExecutor *oh_nn_executor = nullptr;
  oh_nn_executor = OH_NNExecutor_Create(oh_nn_compilation);
  if (oh_nn_executor == nullptr) {
    MS_LOG(ERROR) << "NNCompilation SetDevice failed, OH_NN_ReturnCode = " << ret_code;
    OH_NNCompilation_Destroy(&oh_nn_compilation);
    OH_NNModel_Destroy(&oh_nnmodel);
    return mindspore::kLiteError;
  }
  MS_LOG(DEBUG) << "NNRTDelegate creates NNExecutor success.";

  auto nnr_model_kernel = new (std::nothrow) NNRTModelKernel(oh_nn_executor, model->inputs(), model->outputs());
  OH_NNCompilation_Destroy(&oh_nn_compilation);
  OH_NNModel_Destroy(&oh_nnmodel);
  KernelIter from = model->BeginKernelIterator();
  KernelIter end = model->EndKernelIterator();
  model->Replace(from, end, nnr_model_kernel);
  MS_LOG(DEBUG) << "new NNRTModelKernel success.";

  MS_LOG(DEBUG) << "NNRTDelegate build  success.";
  return mindspore::kSuccess;
}

mindspore::Status mindspore::NNRTDelegate::Init() {
  MS_LOG(DEBUG) << "NNRTDelegate init success.";
  return mindspore::kSuccess;
}
const mindspore::lite::LiteGraph &mindspore::NNRTDelegate::getLiteGraph() const { return lite_graph_; }
void mindspore::NNRTDelegate::setLiteGraph(const mindspore::lite::LiteGraph &liteGraph) { lite_graph_ = liteGraph; }

mindspore::NNRTDelegate::~NNRTDelegate() = default;
