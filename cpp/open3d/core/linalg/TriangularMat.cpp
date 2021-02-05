// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/core/linalg/TriangularMat.h"

#include "open3d/core/Tensor.h"
#include "open3d/core/linalg/TriangularMatImpl.h"

namespace open3d {
namespace core {

void Thiu(const Tensor& A, Tensor& output, const int diagonal) {
    core::Device device = A.GetDevice();

    // Check dimensions.
    SizeVector A_shape = A.GetShape();
    if (A_shape.size() != 2) {
        utility::LogError("Tensor A must be 2D, but got {}D.", A_shape.size());
    }
    if (A_shape[0] == 0 || A_shape[1] == 0) {
        utility::LogError(
                "Tensor shapes should not contain dimensions with zero.");
    }
    if (diagonal <= -1 * A_shape[0] || diagonal >= A_shape[1]) {
        utility::LogError(
                "Diagonal parameter must be between [-{}, {}] for a matrix "
                "with shape {}, but got {}.",
                A_shape[0], A_shape[1], A.GetShape().ToString(), diagonal);
    }

    output = core::Tensor::Zeros(A.GetShape(), A.GetDtype(), device);
    if (device.GetType() == Device::DeviceType::CUDA) {
#ifdef BUILD_CUDA_MODULE
        ThiuCUDA(A, output, diagonal);
#else
        utility::LogError("Unimplemented device.");
#endif
    } else {
        ThiuCPU(A, output, diagonal);
    }
}

void Thil(const Tensor& A, Tensor& output, const int diagonal) {
    core::Device device = A.GetDevice();

    // Check dimensions.
    SizeVector A_shape = A.GetShape();
    if (A_shape.size() != 2) {
        utility::LogError("Tensor A must be 2D, but got {}D.", A_shape.size());
    }
    if (A_shape[0] == 0 || A_shape[1] == 0) {
        utility::LogError(
                "Tensor shapes should not contain dimensions with zero.");
    }
    if (diagonal <= -1 * A_shape[0] || diagonal >= A_shape[1]) {
        utility::LogError(
                "Diagonal parameter must be between [-{}, {}] for a matrix "
                "with shape {}, but got {}.",
                A_shape[0], A_shape[1], A.GetShape().ToString(), diagonal);
    }

    output = core::Tensor::Zeros(A.GetShape(), A.GetDtype(), device);
    if (device.GetType() == Device::DeviceType::CUDA) {
#ifdef BUILD_CUDA_MODULE
        ThilCUDA(A, output, diagonal);
#else
        utility::LogError("Unimplemented device.");
#endif
    } else {
        ThilCPU(A, output, diagonal);
    }
}

void Thiul(const Tensor& A, Tensor& upper, Tensor& lower, const int diagonal) {
    core::Device device = A.GetDevice();

    // Check dimensions.
    SizeVector A_shape = A.GetShape();
    if (A_shape.size() != 2) {
        utility::LogError("Tensor A must be 2D, but got {}D.", A_shape.size());
    }
    if (A_shape[0] == 0 || A_shape[1] == 0) {
        utility::LogError(
                "Tensor shapes should not contain dimensions with "
                "zero.");
    }
    if (diagonal <= -1 * A_shape[0] || diagonal >= A_shape[1]) {
        utility::LogError(
                "Diagonal parameter must be between [-{}, {}] for a matrix "
                "with shape {}, but got {}.",
                A_shape[0] - 1, A_shape[1] - 1, A.GetShape().ToString(),
                diagonal);
    }

    upper = core::Tensor::Zeros(A.GetShape(), A.GetDtype(), device);
    lower = core::Tensor::Zeros(A.GetShape(), A.GetDtype(), device);
    if (device.GetType() == Device::DeviceType::CUDA) {
#ifdef BUILD_CUDA_MODULE
        ThiulCUDA(A, upper, lower, diagonal);
#else
        utility::LogError("Unimplemented device.");
#endif
    } else {
        ThiulCPU(A, upper, lower, diagonal);
    }
}

}  // namespace core
}  // namespace open3d
