//
//  textureutils.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/4/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "textureutils.h"

#define STB_IMAGE_IMPLEMENTATION // we're not having any linker errors today!
#include <stb/stb_image.h>

GLuint loadTexture(const char *img_filepath) {
    int width, height, bpp;
    unsigned char *rgb_data = stbi_load(img_filepath, &width, &height, &bpp, 4);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    stbi_image_free(rgb_data);
    return texture;
}
