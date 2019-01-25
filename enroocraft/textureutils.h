//
//  textureutils.h
//  enroocraft
//
//  Created by Andrew Farm on 5/4/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef textureutils_h
#define textureutils_h

#include <GL/glew.h>

GLuint loadTexture2D(const char *img_filepath);

GLuint loadTexture2D(const char **img_filepaths);

#endif /* textureutils_h */
