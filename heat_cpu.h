#pragma once
#include <vector>

void initHeat(std::vector<float>& grid, int size, float sourceTemp);
void stepHeatCPU(const std::vector<float>& in, std::vector<float>& out, int size);
