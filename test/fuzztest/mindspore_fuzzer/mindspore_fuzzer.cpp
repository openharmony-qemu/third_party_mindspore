/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#include <cstdio>
#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include "mindspore_fuzzer.h"
#include "../data.h"
#include "include/c_api/context_c.h"
#include "include/c_api/model_c.h"

bool MSPreparedModelFuzzTest(const uint8_t* data, size_t size) {
    if (data == nullptr) {
        return false;
    }

    OH_AI_ContextHandle context = OH_AI_ContextCreate();
    if (context == nullptr) {
        printf("create context failed.\n");
        return false;
    }
    OH_AI_ModelHandle model = OH_AI_ModelCreate();
    if (model == nullptr) {
        printf("create model failed.\n");
        return false;
    }

    OH_AI_ContextDestroy(&context);
    OH_AI_ModelDestroy(&model);
    return true;
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (data == nullptr) {
        LOGE("Pass data is nullptr.");
        return 0;
    }

    if (size < 4) {
        LOGE("Pass size is too small.");
        return 0;
    }

    MSPreparedModelFuzzTest(data, size);
    return 0;
}