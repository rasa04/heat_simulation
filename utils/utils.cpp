#include "utils.h"
#include <vector>
#include <algorithm>
static inline unsigned char toByte(float v, float maxT) {
    float n = std::clamp(v / maxT, 0.0f, 1.0f);
    return static_cast<unsigned char>(n * 255.0f);
}

void updateTexture(GLuint texID, const float* data, int texSize) {
    float maxTemp = 0.0f;
    for (int i = 0; i < texSize * texSize; ++i) maxTemp = std::max(maxTemp, data[i]);
    if (maxTemp < 1e-3f) maxTemp = 1.0f;

    std::vector<unsigned char> pixels(texSize * texSize * 3);
    for (int i = 0; i < texSize * texSize; ++i) {
        unsigned char c = toByte(data[i], maxTemp);
        pixels[i * 3 + 0] = c;
        pixels[i * 3 + 1] = 0;
        pixels[i * 3 + 2] = 255 - c;
    }

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void renderTexture(GLuint texID, int texSize, int winWidth, int winHeight) {
    GLint oldViewport[4];
    glGetIntegerv(GL_VIEWPORT, oldViewport);

    glViewport(0, 0, winWidth, winHeight);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, texSize, 0, texSize, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0,       0);
        glTexCoord2f(1, 0); glVertex2f(texSize, 0);
        glTexCoord2f(1, 1); glVertex2f(texSize, texSize);
        glTexCoord2f(0, 1); glVertex2f(0,       texSize);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}
