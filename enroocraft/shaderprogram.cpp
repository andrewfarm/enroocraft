//
//  shaderprogram.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/5/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "shaderprogram.h"
#include "shaderutils.h"

ShaderProgram::~ShaderProgram() {
    glUseProgram(0);
    glDeleteProgram(programID);
}

void ShaderProgram::load(const char *vertex_file_path, const char *fragment_file_path) {
    programID = loadShaders(vertex_file_path, fragment_file_path);
    
    GLint i;
    GLint count;
    
    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)
    
    const GLsizei bufSize = 32; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length
    
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);
    
    GLint location;
    for (i = 0; i < count; i++) {
        glGetActiveUniform(programID, (GLuint) i, bufSize, &length, &size, &type, name);
        location = glGetUniformLocation(programID, name);
        printf("Uniform #%d Type: %u Name: %s Location: %d\n", i, type, name, location);
        uniforms.insert(std::make_pair(std::string(name), location));
    }
}

void ShaderProgram::useProgram() {
    glUseProgram(programID);
}
