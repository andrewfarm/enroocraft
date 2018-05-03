//
//  loadshaders.h
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef loadshaders_h
#define loadshaders_h

#include <iostream>

#include <OpenGL/gl.h>

GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

#endif /* loadshaders_h */
