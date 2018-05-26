//
//  fbo.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/26/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h>

#include "fbo.h"

FBO::FBO() {
    glGenFramebuffers(1, &fboID);
    glGenTextures(1, &colorAttachment);
    glGenTextures(1, &depthAttachment);
    
    bind();
    
    bindColorAttachment();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorAttachment, 0);
    
    bindDepthAttachment();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  depthAttachment, 0);
    
    const GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    // Always check that our framebuffer is ok
    // are you ok, framebuffer?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stdout, "Framebuffer creation successful\n");
    } else {
        fprintf(stderr, "Error creating framebuffer (status: %d)\n", status);
    }
}

FBO::~FBO() {
    glDeleteTextures(1, &colorAttachment);
    glDeleteTextures(1, &depthAttachment);
    glDeleteFramebuffers(1, &fboID);
}

void FBO::setSize(int width, int height) {
    bindColorAttachment();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    bindDepthAttachment();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FBO::bindColorAttachment() {
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
}

void FBO::bindDepthAttachment() {
    glBindTexture(GL_TEXTURE_2D, depthAttachment);
}
