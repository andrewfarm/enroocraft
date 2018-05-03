//
//  renderer.h
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <glm/glm.hpp>

class Renderer {
    GLuint shaderProgram;
    GLuint vertexArrayID;
    GLuint vertexBuffer;
    
public:
    Renderer();
    void render();
};

#endif /* renderer_h */
