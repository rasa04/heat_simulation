#pragma once

// Заглушка: пока просто копирует данные (для компиляции),
// потом сюда перебросим настоящий CUDA-код из heat_gpu.cu
inline void stepHeatGPU(const float* in, float* out, int size) {
    for (int i = 0, N = size*size; i < N; ++i)
        out[i] = in[i];
}
