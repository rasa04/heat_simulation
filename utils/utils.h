#pragma once
#include <GL/glew.h>

void updateTexture(GLuint texID, const float* data, int texSize);
void renderTexture(GLuint texID, int texSize, int winWidth, int winHeight);
