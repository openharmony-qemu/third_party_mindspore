/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <inttypes.h>
#include <securec.h>
#include "gtest/gtest.h"
#include "include/c_api/context_c.h"
#include "include/c_api/model_c.h"
#include "include/c_api/types_c.h"
#include "include/c_api/status_c.h"
#include "include/c_api/data_type_c.h"
#include "include/c_api/tensor_c.h"
#include "include/c_api/format_c.h"
#include "../utils/common.h"

class MSLiteTest: public testing::Test {
protected:
    static void SetUpTestCase(void) {}
    static void TearDownTestCase(void) {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

// function before callback
bool PrintBeforeCallback(const OH_AI_TensorHandleArray inputs, const OH_AI_TensorHandleArray outputs,
                         const OH_AI_CallBackParam kernel_Info) {
    std::cout << "Before forwarding " << kernel_Info.node_name << " " << kernel_Info.node_type << std::endl;
    return true;
}

// function after callback
bool PrintAfterCallback(const OH_AI_TensorHandleArray inputs, const OH_AI_TensorHandleArray outputs,
                        const OH_AI_CallBackParam kernel_Info) {
    std::cout << "After forwarding " << kernel_Info.node_name << " " << kernel_Info.node_type << std::endl;
    return true;
}

void AddContextDeviceCPU(OH_AI_ContextHandle context) {
    OH_AI_DeviceInfoHandle cpu_device_info = OH_AI_DeviceInfoCreate(OH_AI_DEVICETYPE_CPU);
    ASSERT_NE(cpu_device_info, nullptr);
    OH_AI_DeviceType device_type = OH_AI_DeviceInfoGetDeviceType(cpu_device_info);
    printf("==========device_type:%d\n", device_type);
    ASSERT_EQ(device_type, OH_AI_DEVICETYPE_CPU);
    OH_AI_ContextAddDeviceInfo(context, cpu_device_info);
}

// fill data to inputs tensor
void FillInputsData(OH_AI_TensorHandleArray inputs, std::string model_name, bool is_transpose) {
    for (size_t i = 0; i < inputs.handle_num; ++i) {
        printf("==========ReadFile==========\n");
        size_t size1;
        size_t *ptr_size1 = &size1;
        std::string input_data_path = "/data/test/resource/" + model_name + "_" + std::to_string(i) + ".input";
        const char *imagePath = input_data_path.c_str();
        char *imageBuf = ReadFile(imagePath, ptr_size1);
        ASSERT_NE(imageBuf, nullptr);
        OH_AI_TensorHandle tensor = inputs.handle_list[i];
        int64_t element_num = OH_AI_TensorGetElementNum(tensor);
        printf("Tensor name: %s. \n", OH_AI_TensorGetName(tensor));
        float *input_data = reinterpret_cast<float *>(OH_AI_TensorGetMutableData(inputs.handle_list[i]));
        ASSERT_NE(input_data, nullptr);
        if (is_transpose) {
            printf("==========Transpose==========\n");
            size_t shape_num;
            const int64_t *shape = OH_AI_TensorGetShape(tensor, &shape_num);
            auto imageBuf_nhwc = new char[size1];
            PackNCHWToNHWCFp32(imageBuf, imageBuf_nhwc, shape[0], shape[1] * shape[2], shape[3]);
            memcpy_s(input_data, size1, imageBuf_nhwc, size1);
            delete[] imageBuf_nhwc;
        } else {
            memcpy_s(input_data, size1, imageBuf, size1);
        }
        printf("input data after filling is: ");
        for (int j = 0; j < element_num && j <= 20; ++j) {
            printf("%f ", input_data[j]);
        }
        printf("\n");
        delete[] imageBuf;
    }
}

// compare result after predict
void CompareResult(OH_AI_TensorHandleArray outputs, std::string model_name, float atol = 0.01, float rtol = 0.01) {
    printf("==========GetOutput==========\n");
    for (size_t i = 0; i < outputs.handle_num; ++i) {
        OH_AI_TensorHandle tensor = outputs.handle_list[i];
        int64_t element_num = OH_AI_TensorGetElementNum(tensor);
        printf("Tensor name: %s .\n", OH_AI_TensorGetName(tensor));
        float *output_data = reinterpret_cast<float *>(OH_AI_TensorGetMutableData(tensor));
        printf("output data is:");
        for (int j = 0; j < element_num && j <= 20; ++j) {
            printf("%f ", output_data[j]);
        }
        printf("\n");
        printf("==========compFp32WithTData==========\n");
        std::string output_file = "/data/test/resource/" + model_name + std::to_string(i) + ".output";
        bool result = compFp32WithTData(output_data, output_file, atol, rtol, false);
        EXPECT_EQ(result, true);
    }
}

void ModelPredict(OH_AI_ModelHandle model, OH_AI_ContextHandle context, std::string model_name,
                  OH_AI_ShapeInfo shape_infos, bool build_by_graph, bool is_transpose, bool is_callback) {
    std::string model_path = "/data/test/resource/" + model_name + ".ms";
    const char *graphPath = model_path.c_str();
    OH_AI_Status ret = OH_AI_STATUS_SUCCESS;
    if (build_by_graph) {
        printf("==========Build model by graphBuf==========\n");
        size_t size;
        size_t *ptr_size = &size;
        char *graphBuf = ReadFile(graphPath, ptr_size);
        ASSERT_NE(graphBuf, nullptr);
        ret = OH_AI_ModelBuild(model, graphBuf, size, OH_AI_MODELTYPE_MINDIR, context);
        delete[] graphBuf;
    } else {
        printf("==========Build model==========\n");
        ret = OH_AI_ModelBuildFromFile(model, graphPath, OH_AI_MODELTYPE_MINDIR, context);
    }
    printf("==========build model return code:%d\n", ret);
    ASSERT_EQ(ret, OH_AI_STATUS_SUCCESS);
    printf("==========GetInputs==========\n");
    OH_AI_TensorHandleArray inputs = OH_AI_ModelGetInputs(model);
    ASSERT_NE(inputs.handle_list, nullptr);
    if (shape_infos.shape_num != 0) {
        printf("==========Resizes==========\n");
        OH_AI_Status resize_ret = OH_AI_ModelResize(model, inputs, &shape_infos, inputs.handle_num);
        printf("==========Resizes return code:%d\n", resize_ret);
        ASSERT_EQ(resize_ret, OH_AI_STATUS_SUCCESS);
    }

    FillInputsData(inputs, model_name, is_transpose);
    OH_AI_TensorHandleArray outputs;
    OH_AI_Status predict_ret = OH_AI_STATUS_SUCCESS;
    if (is_callback) {
        printf("==========Model Predict Callback==========\n");
        OH_AI_KernelCallBack before_call_back = PrintBeforeCallback;
        OH_AI_KernelCallBack after_call_back = PrintAfterCallback;
        predict_ret = OH_AI_ModelPredict(model, inputs, &outputs, before_call_back, after_call_back);
    }else {
        printf("==========Model Predict==========\n");
        predict_ret = OH_AI_ModelPredict(model, inputs, &outputs, nullptr, nullptr);
    }
    printf("==========Model Predict End==========\n");
    ASSERT_EQ(predict_ret, OH_AI_STATUS_SUCCESS);
    printf("=========CompareResult===========\n");
    CompareResult(outputs, model_name);
    printf("=========OH_AI_ModelDestroy===========\n");
    OH_AI_ModelDestroy(&model);
    printf("=========OH_AI_ModelDestroy End===========\n");
}

/*
 * @tc.name: end_to_end_0001
 * @tc.desc: Test MindSpore Runtime.
 * @tc.type: FUNC
 */
HWTEST(MSLiteTest, end_to_end_0001, testing::ext::TestSize.Level0) {
    printf("==========Init Context==========\n");
    OH_AI_ContextHandle context = OH_AI_ContextCreate();
    ASSERT_NE(context, nullptr);

    AddContextDeviceCPU(context);

    printf("==========Create model==========\n");
    OH_AI_ModelHandle model = OH_AI_ModelCreate();
    ASSERT_NE(model, nullptr);

    ModelPredict(model, context, "ml_face_isface", {}, false, true, false);
}