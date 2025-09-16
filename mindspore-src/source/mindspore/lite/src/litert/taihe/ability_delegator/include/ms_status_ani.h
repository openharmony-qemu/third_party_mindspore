/*
 * Copyright 2025 Huawei Technologies Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MS_STATUS_ANI
#define MS_STATUS_ANI

namespace mindspore_ani {

enum MSLiteStatusANI : int32_t {
  MS_SUCCESS_ANI = 0,                  // Success
  MS_ERROR_ANI = 1000199,              // Fail
  MS_ILLEGAL_STATE_ANI = 1000198,      // Status error
  MS_INVALID_PARAM_ANI = 1000197,      // Invalid parameter
  MS_NOT_EXISTED_PARAM_ANI = 1000196,  // Not existed parameter
  MS_INVALID_OPERATION_ANI = 1000195   // Invalid operation
};

enum MSLiteContextParamANI : int {
  MS_UNSET_VALUE_ANI = -1,
  MS_PARAM0_ANI = 0,
  MS_PARAM1_ANI = 1,
  MS_PARAM2_ANI = 2,
  MS_PARAM3_ANI = 3,
  MS_PARAM4_ANI = 4
};

enum MSLiteErrorCodeANI : int32_t {
  MS_LOAD_PATH_ERROR_PREDICT = 1000000,
  MS_LOAD_CONTEXT_ERROR = 1000001,
  MS_LOAD_NATIVE_ERROR_PREDICT = 1000002,
  MS_LOAD_WAY_ERROR_PREDICT = 1000003,
  MS_LOAD_BUFFER_ERROR_PREDICT = 1000004,
  MS_LOAD_BUFFER_NATIVE_ERROR_PREDICT = 1000005,
  MS_LOAD_FD_NATIVE_ERROR_PREDICT = 1000007,
  MS_LOAD_PATH_ERROR_TRAIN = 1000008,
  MS_LOAD_PATH_NATIVE_ERROR_TRAIN = 1000009,
  MS_LOAD_BUFFER_ERROR_TRAIN = 1000010,
  MS_LOAD_BUFFER_NATIVE_ERROR_TRAIN = 1000011,
  MS_LOAD_FD_ERROR_TRAIN = 1000012,
  MS_SETDATA_SIZE_ERROR_MSTENSOR = 1000013,
};

static const std::unordered_map<MSLiteErrorCodeANI, std::string> MSLiteErrorCodeInfoANI{
  {MS_LOAD_PATH_ERROR_PREDICT, "Model Path error, Possible causes: 1.model path is null; 2.model path is not exists."},
  {MS_LOAD_CONTEXT_ERROR, "Invalid context, Possible causes: 1.context target error; 2.device info error."},
  {MS_LOAD_NATIVE_ERROR_PREDICT,
   "Create native model fail from path: 1.insufficient permission to access the model path; 2.the model file is "
   "corrupted."},
  {MS_LOAD_WAY_ERROR_PREDICT, "Load model way error, Possible causes: 1.Load way must be path, buffer, fd."},
  {MS_LOAD_BUFFER_ERROR_PREDICT, "Model Buffer error, Possible causes: 1.buffer size is 0; 2.buffer is nullptr."},
  {MS_LOAD_BUFFER_NATIVE_ERROR_PREDICT,
   "Create native model fail from buffer: 1.buffer size is wrong. 2.buffer file is damaged."},
  {MS_LOAD_FD_NATIVE_ERROR_PREDICT, "Create native model fail from fd: 1.file fd is wrong. 2.model file is damaged."},
  {MS_LOAD_PATH_ERROR_TRAIN,
   "Invalid model path in tarin, Possible causes: 1.model path is null; 2.model path is not exists."},
  {MS_LOAD_PATH_NATIVE_ERROR_TRAIN,
   "Create native train model fail from path: 1.model file is wrong. 2.train cfg is wrong."},
  {MS_LOAD_BUFFER_ERROR_TRAIN,
   "Invalid model buffer in tarin, Possible causes: 1.model buffer size error; 2.model buffer is null."},
  {MS_LOAD_BUFFER_NATIVE_ERROR_TRAIN,
   "Create native train model fail from buffer: 1.model buffer is wrong. 2.train cfg is wrong."},
  {MS_LOAD_FD_ERROR_TRAIN,
   "Create native train model fail from fd: 1.model file or fd is wrong. 2.train cfg is wrong."},
  {MS_SETDATA_SIZE_ERROR_MSTENSOR, "Set mstensor data fail: 1.input array buffer size wrong."}};

}  // namespace mindspore_ani
#endif
