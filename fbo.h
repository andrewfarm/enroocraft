//
//  fbo.h
//  enroocraft
//
//  Created by Andrew Farm on 5/26/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef fbo_h
#define fbo_h

#include <GL/glew.h>

class FBO {
    GLuint fboID;
    GLuint colorAttachment;
    GLuint depthAttachment;
    
public:
    FBO();
    FBO(GLsizei width, GLsizei height);
    ~FBO();
    
    void setSize(GLsizei width, GLsizei height);
    void bind();
    void bindColorAttachment();
    void bindDepthAttachment();
};

#endif /* fbo_h */
