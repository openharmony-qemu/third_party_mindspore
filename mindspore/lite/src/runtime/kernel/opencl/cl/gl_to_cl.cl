/* Copyright 2021 Huawei Technologies Co., Ltd
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
__constant sampler_t smp_zero = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void GLTexture2D_to_IMG(__read_only image2d_t imageSrc, __write_only image2d_t imageDst) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    float4 color = read_imagef(imageSrc, smp_zero, (int2)(x, y));
    write_imagef(imageDst, (int2)(x, y), color);
}

__kernel void IMG_to_GLTexture2D(__read_only image2d_t imageSrc, __write_only image2d_t imageDst) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    float4 color = read_imagef(imageSrc, smp_zero, (int2)(x, y));
    write_imagef(imageDst, (int2)(x, y), color);
}
