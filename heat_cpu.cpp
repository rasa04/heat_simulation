#include "heat_cpu.h"
#include <algorithm>

// Инициализация сетки: устанавливаем центральную точку как источник тепла
void initHeat(std::vector<float>& grid, int size, float sourceTemp) {
    std::fill(grid.begin(), grid.end(), 0.0f);
    int center = size / 2;
    grid[center * size + center] = sourceTemp;
}

// Один шаг симуляции на CPU: рассчитываем новую сетку из входной
void stepHeatCPU(const std::vector<float>& in, std::vector<float>& out, int size) {
    for (int y = 1; y < size - 1; ++y) {
        for (int x = 1; x < size - 1; ++x) {
            int i = y * size + x;
            out[i] = 0.25f * (
                in[i - 1] + in[i + 1] +    // лево/право
                in[i - size] + in[i + size] // вверх/низ
            );
        }
    }
    // Удерживаем источник горячим
    int center = size / 2;
    out[center * size + center] = in[center * size + center];
}
