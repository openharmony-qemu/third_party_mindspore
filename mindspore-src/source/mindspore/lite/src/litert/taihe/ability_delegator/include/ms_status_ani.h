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
}  // namespace mindspore_ani
#endif
