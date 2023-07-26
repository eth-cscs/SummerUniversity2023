/*
 * MIT License
 *
 * Copyright (c) 2021 CSCS, ETH Zurich
 *               2021 University of Basel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <type_traits>
#include <vector>

#if defined(USE_CUDA) || defined(__CUDACC__)

#include <thrust/device_vector.h>
#include <cstdio>
#include <cuda_runtime.h>

#include "annotation.hpp"

inline void checkErr(cudaError_t err, const char* filename, int lineno, const char* funcName)
{
    if (err != cudaSuccess)
    {
        const char* errName = cudaGetErrorName(err);
        const char* errStr  = cudaGetErrorString(err);
        fprintf(stderr, "CUDA Error at %s:%d. Function %s returned err %d: %s - %s\n", filename, lineno, funcName, err,
                errName, errStr);
        exit(EXIT_FAILURE);
    }
}

#define checkGpuErrors(errcode) checkErr((errcode), __FILE__, __LINE__, #errcode)

template<class T, class Alloc>
T* rawPtr(thrust::device_vector<T, Alloc>& p)
{
    return thrust::raw_pointer_cast(p.data());
}

template<class T, class Alloc>
const T* rawPtr(const thrust::device_vector<T, Alloc>& p)
{
    return thrust::raw_pointer_cast(p.data());
}

#endif

namespace thrust
{

template<class T>
class device_allocator;

template<class T, class Alloc>
class device_vector;

} // namespace thrust

template<class T, class Alloc>
T* rawPtr(std::vector<T, Alloc>& p)
{
    return p.data();
}

template<class T, class Alloc>
const T* rawPtr(const std::vector<T, Alloc>& p)
{
    return p.data();
}

//! @brief ceil(dividend/divisor) for integers
HOST_DEVICE_FUN constexpr unsigned iceil(size_t dividend, unsigned divisor)
{
    return (dividend + divisor - 1) / divisor;
}