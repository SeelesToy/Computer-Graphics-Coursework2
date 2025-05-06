#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <shadow.h>

GLuint setup_texture(const char* filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w, h, chan;
	stbi_set_flip_vertically_on_load(true);
    unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);
    if (pxls) {
        GLenum internalFmt = (chan == 4 ? GL_RGBA : GL_RGB);
        GLenum dataFmt = internalFmt;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);       // 严格按字节打包
        glTexImage2D(GL_TEXTURE_2D,
            0,
            internalFmt,
            w, h,
            0,
            dataFmt,
            GL_UNSIGNED_BYTE,
            pxls);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

	delete[] pxls;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}

// texture.h
GLuint setup_mipmaps(const char* filename[], int n) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    GLuint texObject;
    glGenTextures(1, &texObject);
    glBindTexture(GL_TEXTURE_2D, texObject);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int w[16], h[16], chan[16];
    unsigned char* pxls[16] = { nullptr };  // 初始化指针数组

    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < n; i++) {
        pxls[i] = stbi_load(filename[i], &w[i], &h[i], &chan[i], 0);  // 正确加载
        if (pxls[i]) {
            glTexImage2D(GL_TEXTURE_2D, i, GL_RGB, w[i], h[i], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[i]);
        }
        else {
            std::cerr << "Failed to load texture: " << filename[i] << std::endl;
        }
    }

    // 释放内存（使用 stbi_image_free）
    for (int i = 0; i < n; i++) {
        if (pxls[i]) stbi_image_free(pxls[i]);
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    return texObject;
}
