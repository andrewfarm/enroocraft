//
//  shaderprogram.h
//  enroocraft
//
//  Created by Andrew Farm on 5/5/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef shaderprogram_h
#define shaderprogram_h

#include <string>
#include <unordered_map>

#include <GL/glew.h>

class ShaderProgram {
    GLuint programID;
    
public:
    std::unordered_map<std::string, GLint> uniforms;
    
    ~ShaderProgram();
    void load(const char *vertex_file_path, const char *fragment_file_path);
    void useProgram();
    GLint getUniformLocation(std::string name);
};

#endif /* shaderprogram_h */
