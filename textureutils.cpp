//
//  textureutils.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/4/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h>

#include "textureutils.h"

#define STB_IMAGE_IMPLEMENTATION // we're not having any linker errors today!
#include <stb/stb_image.h>

GLuint loadTexture2D(const char *img_filepath) {
    int width, height, bpp;
    unsigned char *imgdata = stbi_load(img_filepath, &width, &height, &bpp, 4);
    if (imgdata) {
        printf("Successfully read %s\n", img_filepath);
    } else {
        fprintf(stderr, "Error reading %s\n", img_filepath);
        return 0;
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    if (texture == 0) {
        fprintf(stderr, "Could not create texture\n");
        return 0;
    }
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    stbi_image_free(imgdata);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint loadTexture2D(const char *img_filepaths[6]) {
    GLuint texture;
    glGenTextures(1, &texture);
    if (texture == 0) {
        fprintf(stderr, "Could not create texture\n");
        return 0;
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum targets[] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    };
    int width, height, bpp;
    unsigned char *imgdata;
    for (int i = 0; i < 6; i++) {
        imgdata = stbi_load(img_filepaths[i], &width, &height, &bpp, 4);
        if (imgdata[i]) {
            printf("Successfully read %s\n", img_filepaths[i]);
        } else {
            fprintf(stderr, "Error reading %s\n", img_filepaths[i]);
            return 0;
        }
        glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);
        stbi_image_free(imgdata);
    }
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture;
}
