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
#include "include/c_api/context_c.h"
#include "common/common_test.h"

namespace mindspore {
class ContextCTest : public mindspore::CommonTest {
 public:
  ContextCTest() {}
};

TEST_F(ContextCTest, common_test) {
  OH_AI_DeviceInfoHandle npu_device_info = OH_AI_DeviceInfoCreate(OH_AI_DEVICETYPE_KIRIN_NPU);
  ASSERT_TRUE(npu_device_info != nullptr);
  ASSERT_EQ(OH_AI_DeviceInfoGetDeviceType(npu_device_info), OH_AI_DEVICETYPE_KIRIN_NPU);

  OH_AI_DeviceInfoSetProvider(npu_device_info, "vendor name");
  ASSERT_STREQ(OH_AI_DeviceInfoGetProvider(npu_device_info), "vendor name");

  OH_AI_DeviceInfoSetProviderDevice(npu_device_info, "npu_a");
  ASSERT_STREQ(OH_AI_DeviceInfoGetProviderDevice(npu_device_info), "npu_a");

  OH_AI_DeviceInfoSetFrequency(npu_device_info, 3);
  ASSERT_EQ(OH_AI_DeviceInfoGetFrequency(npu_device_info), 3);

  OH_AI_ContextHandle context = OH_AI_ContextCreate();
  ASSERT_TRUE(context != nullptr);

  OH_AI_ContextSetThreadNum(context, 4);
  ASSERT_EQ(OH_AI_ContextGetThreadNum(context), 4);

  OH_AI_ContextSetThreadAffinityMode(context, 2);
  ASSERT_EQ(OH_AI_ContextGetThreadAffinityMode(context), 2);

  constexpr size_t core_num = 4;
  int32_t core_list[core_num] = {1, 3, 2, 0};
  OH_AI_ContextSetThreadAffinityCoreList(context, core_list, core_num);
  size_t ret_core_num;
  const int32_t *ret_core_list = nullptr;
  ret_core_list = OH_AI_ContextGetThreadAffinityCoreList(context, &ret_core_num);
  ASSERT_EQ(ret_core_num, core_num);
  for (size_t i = 0; i < ret_core_num; i++) {
    ASSERT_EQ(ret_core_list[i], core_list[i]);
  }

  OH_AI_ContextSetEnableParallel(context, true);
  ASSERT_EQ(OH_AI_ContextGetEnableParallel(context), true);

  OH_AI_DeviceInfoHandle cpu_device_info = OH_AI_DeviceInfoCreate(OH_AI_DEVICETYPE_CPU);
  OH_AI_DeviceInfoDestroy(&cpu_device_info);
  cpu_device_info = OH_AI_DeviceInfoCreate(OH_AI_DEVICETYPE_CPU);

  OH_AI_DeviceInfoSetEnableFP16(cpu_device_info, true);
  ASSERT_EQ(OH_AI_DeviceInfoGetEnableFP16(cpu_device_info), true);

  OH_AI_ContextAddDeviceInfo(context, cpu_device_info);
  OH_AI_ContextAddDeviceInfo(context, npu_device_info);
  OH_AI_ContextDestroy(&context);
}
}  // namespace mindspore
