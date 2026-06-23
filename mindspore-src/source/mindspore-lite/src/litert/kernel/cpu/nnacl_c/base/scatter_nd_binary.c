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

#include "nnacl_c/base/scatter_nd_binary.h"
#include <string.h>
#include <stdio.h>
#include "nnacl_c/errorcode.h"
#include "nnacl_c/scatter_nd_binary_simd.h"

int ScatterNDAdd(const void *update, void *output, int *output_unit_offsets, const ScatterNDParameter *param, int type,
                 int task_id) {
  if (update == NULL || output == NULL || output_unit_offsets == NULL || param == NULL) {
    return NNACL_NULL_PTR;
  }
  if (param->op_parameter.thread_num_ == 0) {
    return NNACL_ERR;
  }
  int unit_per_thread = UP_DIV(param->num_unit, param->op_parameter.thread_num_);
  int begin = unit_per_thread * task_id;
  int end = MSMIN(begin + unit_per_thread, param->num_unit);
  if (type == 0) {
    float *update_fp32 = (float *)update;
    float *output_fp32 = (float *)output;
    for (int i = begin; i < end; i++) {
      const float *update_data = update_fp32 + i * param->unit_size;
      float *output_data = output_fp32 + output_unit_offsets[i];
      int j = 0;

      SIMD_RUN_NO_SCALAR(ScatterNDAddFp32, j, update_data, param->unit_size, output_data);
      for (; j < param->unit_size; j++) {
        output_data[j] += update_data[j];
      }
    }
  } else {
    int *update_int32 = (int *)update;
    int *output_int32 = (int *)output;
    for (int i = begin; i < end; i++) {
      const int *update_data = update_int32 + i * param->unit_size;
      int *output_data = output_int32 + output_unit_offsets[i];
      int j = 0;

      SIMD_RUN_NO_SCALAR(ScatterNDAddInt32, j, update_data, param->unit_size, output_data);
      for (; j < param->unit_size; j++) {
        output_data[j] += update_data[j];
      }
    }
  }
  return NNACL_OK;
}

int ScatterNDUpdate(void *output, size_t output_size_bytes, const void *update, int *output_unit_offsets,
                    const ScatterNDParameter *param, int task_id) {
  if (output == NULL || update == NULL || output_unit_offsets == NULL || param == NULL) {
    return NNACL_NULL_PTR;
  }
  if (param->op_parameter.thread_num_ == 0) {
    return NNACL_ERR;
  }
  int unit_per_thread = UP_DIV(param->num_unit, param->op_parameter.thread_num_);
  int begin = unit_per_thread * task_id;
  int end = MSMIN(begin + unit_per_thread, param->num_unit);

  int data_type_len = param->data_type_len;
  if (param->unit_size < 0 || data_type_len < 0) {
    return NNACL_ERR;
  }
  NNACL_CHECK_INT_MUL_NOT_OVERFLOW(param->unit_size, data_type_len, NNACL_ERR);
  int copy_size_bytes = param->unit_size * data_type_len;

  for (int i = begin; i < end; i++) {
    if (output_unit_offsets[i] < 0) {
      return NNACL_ERR;
    }
    NNACL_CHECK_INT_MUL_NOT_OVERFLOW(output_unit_offsets[i], data_type_len, NNACL_ERR);
    int output_offset_bytes = output_unit_offsets[i] * data_type_len;
    if ((size_t)output_offset_bytes > output_size_bytes ||
        (size_t)copy_size_bytes > output_size_bytes - (size_t)output_offset_bytes) {
      return NNACL_ERR;
    }
    NNACL_CHECK_INT_MUL_NOT_OVERFLOW(i, copy_size_bytes, NNACL_ERR);
    (void)memcpy((int8_t *)output + output_offset_bytes, (int8_t *)update + i * copy_size_bytes, copy_size_bytes);
  }
  return NNACL_OK;
}

int ScatterNDMax(const void *update, void *output, int *output_unit_offsets, const ScatterNDParameter *param, int type,
                 int task_id) {
  if (update == NULL || output == NULL || output_unit_offsets == NULL || param == NULL) {
    return NNACL_NULL_PTR;
  }
  if (param->op_parameter.thread_num_ == 0) {
    return NNACL_ERR;
  }
  int unit_per_thread = UP_DIV(param->num_unit, param->op_parameter.thread_num_);
  int begin = unit_per_thread * task_id;
  int end = MSMIN(begin + unit_per_thread, param->num_unit);
  if (type == 0) {
    float *update_fp32 = (float *)update;
    float *output_fp32 = (float *)output;
    for (int i = begin; i < end; i++) {
      const float *update_data = update_fp32 + i * param->unit_size;
      float *output_data = output_fp32 + output_unit_offsets[i];
      int j = 0;

      SIMD_RUN_NO_SCALAR(ScatterNDMaxFp32, j, update_data, param->unit_size, output_data);
      for (; j < param->unit_size; j++) {
        output_data[j] = fmaxf(update_data[j], output_data[j]);
      }
    }
  }
  return NNACL_OK;
}
