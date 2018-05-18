//
//  renderer.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h> //TODO remove
#include <math.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "renderer.h"
#include "textureutils.h"


#define LEN_STATIC(xs) (sizeof(xs) / sizeof(xs[0]))


///////////////////////////////
// cube face geometries
///////////////////////////////

// cube face geometries
static const float nxGeometry[] = {
    0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float pxGeometry[] = {
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float nyGeometry[] = {
    1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float pyGeometry[] = {
    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float nzGeometry[] = {
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,   0.0f,
};
static const float pzGeometry[] = {
    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,
};

// cube face geometries (with flipped diagonal)
static const float nxGeometryFlipped[] = {
    0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float pxGeometryFlipped[] = {
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float nyGeometryFlipped[] = {
    1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float pyGeometryFlipped[] = {
    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
};
static const float nzGeometryFlipped[] = {
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,   0.0f,
};
static const float pzGeometryFlipped[] = {
    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,
};

#define FACE_GEOMETRY_STRIDE 9
#define FACE_GEOMETRY_LENGTH (FACE_GEOMETRY_STRIDE * 6)

#define FACE_GEOMETRY_POSITION 0
#define FACE_GEOMETRY_NORMAL   3
#define FACE_GEOMETRY_UV       6
#define FACE_GEOMETRY_AO       8

static const VertexAttrib faceAttribs[] = {
    {0, 3, FACE_GEOMETRY_POSITION}, // position
    {1, 3, FACE_GEOMETRY_NORMAL},   // normal
    {2, 2, FACE_GEOMETRY_UV},       // texture UVs
    {3, 1, FACE_GEOMETRY_AO},       // ambient light
};

///////////////////////////////
// screen geometry
///////////////////////////////

static const float screenGeometry[] = {
     1.0f,  1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,   0.0f, 1.0f,
     1.0f, -1.0f,   1.0f, 0.0f,
    -1.0f, -1.0f,   0.0f, 0.0f,
};

static const VertexAttrib screenAttribs[] = {
    {0, 2, 0}, // position
    {1, 2, 2}, // texture UVs
};

///////////////////////////////
// crosshair point geometry
///////////////////////////////

static const float crosshairGeometry[] = {0.0f, 0.0f};

static const VertexAttrib crosshairAttribs[] = {
    {0, 2, 0}, // position
};

///////////////////////////////
// selection cube geometry
///////////////////////////////

static const float selectionGeometry[] = {
    -0.005f,  1.005f, -0.005f,
     1.005f,  1.005f, -0.005f,
     1.005f,  1.005f,  1.005f,
    -0.005f,  1.005f,  1.005f,
    -0.005f, -0.005f, -0.005f,
     1.005f, -0.005f, -0.005f,
     1.005f, -0.005f,  1.005f,
    -0.005f, -0.005f,  1.005f,
};

static const unsigned char selectionIndices[] = {
    3, 0, 7, 4, 7, 0,
    1, 2, 5, 6, 5, 2,
    6, 7, 5, 4, 5, 7,
    1, 0, 2, 3, 2, 0,
    0, 1, 4, 5, 4, 1,
    2, 3, 6, 7, 6, 3,
};

static const VertexAttrib selectionAttribs[] = {
    {0, 3, 0}, // position
};

///////////////////////////////
// skybox geometry
///////////////////////////////

static const float skyboxGeometry[] = {
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
};

static const unsigned char skyboxIndices[] = {
    0, 3, 4, 7, 4, 3,
    2, 1, 6, 5, 6, 1,
    5, 4, 6, 7, 6, 4,
    2, 3, 1, 0, 1, 3,
    1, 0, 5, 4, 5, 0,
    3, 2, 7, 6, 7, 2,
};

static const VertexAttrib skyboxAttribs[] = {
    {0, 3, 0}, // position
};

///////////////////////////////
// sun geometry
///////////////////////////////

static const float sunGeometry[] = {
     1.0f,  0.05f,  0.05f,
     1.0f,  0.05f, -0.05f,
     1.0f, -0.05f,  0.05f,
     1.0f, -0.05f, -0.05f,
};

static const VertexAttrib sunAttribs[] = {
    {0, 3, 0}, // position
};


#define SHADOWMAP_SIZE 4096

const glm::mat4 lightBiasMatrix(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f);

const float TWO_PI = (float) (2 * M_PI);
const float HALF_PI = (float) (0.5 * M_PI);

const float MIDNIGHT_AMBIENT_LIGHT = 0.2f;
const float DAWN_AMBIENT_LIGHT = 0.5f;

const glm::mat4 skyboxCorrectionMatrix = glm::rotate(glm::mat4(), HALF_PI, glm::vec3(-1.0f, 0.0f, 0.0f));

Renderer::Renderer() :
framebufferCreated(false),
drawSelectionCube(false),
screenMesh(screenAttribs, LEN_STATIC(screenAttribs),
        GL_STATIC_DRAW, GL_TRIANGLE_STRIP),
crosshairMesh(crosshairAttribs, LEN_STATIC(crosshairAttribs),
        GL_STATIC_DRAW, GL_POINTS),
selectionMesh(selectionAttribs, LEN_STATIC(selectionAttribs),
        GL_STATIC_DRAW, GL_STATIC_DRAW, GL_TRIANGLES),
skyboxMesh(skyboxAttribs, LEN_STATIC(skyboxAttribs),
        GL_STATIC_DRAW, GL_STATIC_DRAW, GL_TRIANGLES),
sunMesh(sunAttribs, LEN_STATIC(sunAttribs),
        GL_STATIC_DRAW, GL_TRIANGLE_STRIP)
{
    updateViewMatrix();
    
    updateLightMvpMatrix();
    
    shadowMapShaderProgram.load(
            "shaders/shadowmapvertexshader.glsl",
            "shaders/shadowmapfragmentshader.glsl");
    
    blockShaderProgram.load(
            "shaders/blockvertexshader.glsl",
            "shaders/blockfragmentshader.glsl");
    
    screenShaderProgram.load(
            "shaders/screenvertexshader.glsl",
            "shaders/screenfragmentshader.glsl");
    
    crosshairShaderProgram.load(
            "shaders/crosshairvertexshader.glsl",
            "shaders/crosshairfragmentshader.glsl");
    
    simpleShaderProgram.load(
            "shaders/simplevertexshader.glsl",
            "shaders/simplefragmentshader.glsl");
    
    skyShaderProgram.load(
            "shaders/skyvertexshader.glsl",
            "shaders/skyfragmentshader.glsl");
    
    sunShaderProgram.load(
            "shaders/sunvertexshader.glsl",
            "shaders/simplefragmentshader.glsl");
    
    printf("Loading texture atlas\n");
    textureAtlas = loadTexture2D("res/textures.png");
    printf("Loading starfield texture\n");
    const char *imgFilepaths[] = {
        "res/starmap_8k_4.png",
        "res/starmap_8k_3.png",
        "res/starmap_8k_6.png",
        "res/starmap_8k_5.png",
        "res/starmap_8k_2.png",
        "res/starmap_8k_1.png",
    };
    starfieldTexture = loadTexture2D(imgFilepaths);
    
    screenMesh.setData(screenGeometry, LEN_STATIC(screenGeometry));
    
    crosshairMesh.setData(crosshairGeometry, LEN_STATIC(crosshairGeometry));
    
    selectionMesh.setData(selectionGeometry, LEN_STATIC(selectionGeometry));
    selectionMesh.setIndices(selectionIndices, sizeof(selectionIndices),
            GL_UNSIGNED_BYTE, sizeof(selectionIndices[0]));
    
    skyboxMesh.setData(skyboxGeometry, LEN_STATIC(skyboxGeometry));
    skyboxMesh.setIndices(skyboxIndices, sizeof(skyboxIndices),
            GL_UNSIGNED_BYTE, sizeof(skyboxIndices[0]));
    
    sunMesh.setData(sunGeometry, LEN_STATIC(sunGeometry));
    
    // create shadowmap framebuffer
    
    printf("Creating shadowmap framebuffer\n");
    glGenFramebuffers(1, &shadowMapFBO);
    glGenTextures(1, &shadowMapColorTexture);
    glGenTextures(1, &shadowMapDepthTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    
    glBindTexture(GL_TEXTURE_2D, shadowMapColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowMapColorTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, shadowMapDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  shadowMapDepthTexture, 0);
    
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    // Always check that our framebuffer is ok
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer creation successful\n");
    } else {
        printf("Error creating framebuffer (status: %d)\n", status);
    }
}

void Renderer::setSize(float width, float height) {
    width  *= 2; //TODO replace hardcoded pixel density
    height *= 2;
    this->width  = width;
    this->height = height;
    
    updateProjectionMatrix();
    
    printf("Creating screen framebuffer\n");
    if (!framebufferCreated) {
        glGenFramebuffers(1, &framebuffer);
        glGenTextures(1, &renderedColorTexture);
        glGenTextures(1, &renderedDepthTexture);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glBindTexture(GL_TEXTURE_2D, renderedColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedColorTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, renderedDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  renderedDepthTexture, 0);
    
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    framebufferCreated = true;
    
    // Always check that our framebuffer is ok
    // are you ok, framebuffer?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer creation successful\n");
    } else {
        printf("Error creating framebuffer (status: %d)\n", status);
    }
}

//TODO inline?
static void translateGeometry(float *geometry, size_t length,
                              float x, float y, float z) {
    for (int i = FACE_GEOMETRY_POSITION; i < length; i += FACE_GEOMETRY_STRIDE) {
        geometry[i]     += x;
        geometry[i + 1] += y;
        geometry[i + 2] += z;
    }
}

static void translateUV(float *geometry, size_t length, int textureNumber) {
    for (int i = FACE_GEOMETRY_UV; i < length; i += FACE_GEOMETRY_STRIDE) {
        geometry[i] = (geometry[i] + (textureNumber % TEXTURE_ATLAS_SIZE)) *
        TEXTURE_ATLAS_SIZE_RECIPROCAL;
        geometry[i + 1] = (geometry[i + 1] + (textureNumber / TEXTURE_ATLAS_SIZE)) *
        TEXTURE_ATLAS_SIZE_RECIPROCAL;
    }
}

// ambient occlusion values are for vertices clockwise from top left
static void setAO(float *geometry, float ao1, float ao2, float ao3, float ao4) {
    geometry[FACE_GEOMETRY_STRIDE * 1 + FACE_GEOMETRY_AO] = ao1;
    geometry[FACE_GEOMETRY_STRIDE * 5 + FACE_GEOMETRY_AO] = ao1;
    
    geometry[FACE_GEOMETRY_AO] = ao2;
    
    geometry[FACE_GEOMETRY_STRIDE * 2 + FACE_GEOMETRY_AO] = ao3;
    geometry[FACE_GEOMETRY_STRIDE * 4 + FACE_GEOMETRY_AO] = ao3;
    
    geometry[FACE_GEOMETRY_STRIDE * 3 + FACE_GEOMETRY_AO] = ao4;
}

static void setAOFlipped(float *geometry, float ao1, float ao2, float ao3, float ao4) {
    geometry[FACE_GEOMETRY_STRIDE * 5 + FACE_GEOMETRY_AO] = ao1;
    
    geometry[FACE_GEOMETRY_AO] = ao2;
    geometry[FACE_GEOMETRY_STRIDE * 4 + FACE_GEOMETRY_AO] = ao2;
    
    geometry[FACE_GEOMETRY_STRIDE * 2 + FACE_GEOMETRY_AO] = ao3;
    
    geometry[FACE_GEOMETRY_STRIDE * 1 + FACE_GEOMETRY_AO] = ao4;
    geometry[FACE_GEOMETRY_STRIDE * 3 + FACE_GEOMETRY_AO] = ao4;
}

static void copyTranslatedIntoVector(
        std::vector<float> &dest,
        float *temp,
        const float *geometry,
        const float *geometryFlipped,
        size_t length,
        float translateX,
        float translateY,
        float translateZ,
        int textureNumber,
        float ao1, float ao2, float ao3, float ao4) {
    if (ao2 + ao4 > ao1 + ao3) {
        memcpy(temp, geometryFlipped, length * sizeof(*geometryFlipped));
        setAOFlipped(temp, ao1, ao2, ao3, ao4);
    } else {
        memcpy(temp, geometry, length * sizeof(*geometry));
        setAO(temp, ao1, ao2, ao3, ao4);
    }
    translateGeometry(temp, length, translateX, translateY, translateZ);
    translateUV(temp, length, textureNumber);
    size_t size = dest.size();
    dest.resize(size + length);
    for (size_t i = 0; i < length; i++) {
        dest[size + i] = temp[i];
    }
}

static bool shouldMeshFace(int adjacentBlock, int thisBlock, bool thisBlockOpaque) {
    return  !(blockInfos[adjacentBlock].opaque) &&
            (thisBlockOpaque || (adjacentBlock != thisBlock));
}

static inline bool isOpaque(int block) {
    return (block >= BLOCK_AIR) && blockInfos[block].opaque;
}

static inline float vertexAO(bool side1, bool side2, bool corner) {
    return (float) ((side1 && side2) ? 0 : (3 - (side1 + side2 + corner))) / 3.0f;
}

void Renderer::mesh(
        std::vector<float> &opaqueMeshData,
        std::vector<float> &transparentMeshData,
        int chunkX,
        int chunkZ,
        const std::vector<blocktype> &blocks)
{
    int internalX, internalY, internalZ;
    int index;
    int x, y, z;
    float tmpGeometry[FACE_GEOMETRY_LENGTH];
    blocktype block;
    blockinfo binfo;
    blocktype kernel[3][3][3];
    blocktype temp;
    bool aoKernel[3][3][3];
    int kernelX, kernelY, kernelZ;
    printf("Meshing chunk (%d, %d)\n", chunkX, chunkZ);
    int chunkHeight = (int) ceil((float) blocks.size() / (CHUNK_SIZE * CHUNK_SIZE));
    for (internalY = 0; internalY < chunkHeight; internalY++) {
        for (internalZ = 0; internalZ < CHUNK_SIZE; internalZ++) {
            for (internalX = 0; internalX < CHUNK_SIZE; internalX++) {
                index = (internalY * CHUNK_SIZE * CHUNK_SIZE) +
                        (internalZ * CHUNK_SIZE) +
                        internalX;
                if ((index < blocks.size()) &&
                    (blocks[index] > BLOCK_AIR)) {
                    
                    block = blocks[index];
                    x = chunkX * CHUNK_SIZE + internalX;
                    y = internalY;
                    z = chunkZ * CHUNK_SIZE + internalZ;
                    binfo = blockInfos[block];
                    std::vector<float> &targetVector =
                    binfo.opaque ? opaqueMeshData : transparentMeshData;
                    
                    for (kernelX = 0; kernelX < 3; kernelX++) {
                        for (kernelY = 0; kernelY < 3; kernelY++) {
                            for (kernelZ = 0; kernelZ < 3; kernelZ++) {
                                temp = world->getBlock(x + kernelX - 1, y + kernelY - 1, z + kernelZ - 1);
                                //TODO blocks really only need to be stored in one or the other,
                                //since face-adjacent blocks are used by meshing and
                                //edge and corner blocks are used by ambient occlusion
                                kernel[kernelX][kernelY][kernelZ] = temp;
                                aoKernel[kernelX][kernelY][kernelZ] = isOpaque(temp);
                            }
                        }
                    }
                    
                    // negative x
                    if (shouldMeshFace(kernel[0][1][1], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                nxGeometry, nxGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_nx,
                                vertexAO(aoKernel[0][1][0], aoKernel[0][2][1], aoKernel[0][2][0]),
                                vertexAO(aoKernel[0][2][1], aoKernel[0][1][2], aoKernel[0][2][2]),
                                vertexAO(aoKernel[0][1][2], aoKernel[0][0][1], aoKernel[0][0][2]),
                                vertexAO(aoKernel[0][0][1], aoKernel[0][1][0], aoKernel[0][0][0]));
                    }
                    // positive x
                    if (shouldMeshFace(kernel[2][1][1], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                pxGeometry, pxGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_px,
                                vertexAO(aoKernel[2][1][2], aoKernel[2][2][1], aoKernel[2][2][2]),
                                vertexAO(aoKernel[2][2][1], aoKernel[2][1][0], aoKernel[2][2][0]),
                                vertexAO(aoKernel[2][1][0], aoKernel[2][0][1], aoKernel[2][0][0]),
                                vertexAO(aoKernel[2][0][1], aoKernel[2][1][2], aoKernel[2][0][2]));
                    }
                    // negative y
                    if (shouldMeshFace(kernel[1][0][1], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                nyGeometry, nyGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_ny,
                                vertexAO(aoKernel[0][0][1], aoKernel[1][0][2], aoKernel[0][0][2]),
                                vertexAO(aoKernel[1][0][2], aoKernel[2][0][1], aoKernel[2][0][2]),
                                vertexAO(aoKernel[2][0][1], aoKernel[1][0][0], aoKernel[2][0][0]),
                                vertexAO(aoKernel[1][0][0], aoKernel[0][0][1], aoKernel[0][0][0]));
                    }
                    // positive y
                    if (shouldMeshFace(kernel[1][2][1], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                pyGeometry, pyGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_py,
                                vertexAO(aoKernel[0][2][1], aoKernel[1][2][0], aoKernel[0][2][0]),
                                vertexAO(aoKernel[1][2][0], aoKernel[2][2][1], aoKernel[2][2][0]),
                                vertexAO(aoKernel[2][2][1], aoKernel[1][2][2], aoKernel[2][2][2]),
                                vertexAO(aoKernel[1][2][2], aoKernel[0][2][1], aoKernel[0][2][2]));
                    }
                    // negative z
                    if (shouldMeshFace(kernel[1][1][0], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                nzGeometry, nzGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_nz,
                                vertexAO(aoKernel[2][1][0], aoKernel[1][2][0], aoKernel[2][2][0]),
                                vertexAO(aoKernel[1][2][0], aoKernel[0][1][0], aoKernel[0][2][0]),
                                vertexAO(aoKernel[0][1][0], aoKernel[1][0][0], aoKernel[0][0][0]),
                                vertexAO(aoKernel[1][0][0], aoKernel[2][1][0], aoKernel[2][0][0]));
                    }
                    // positive z
                    if (shouldMeshFace(kernel[1][1][2], block, binfo.opaque)) {
                        copyTranslatedIntoVector(targetVector, tmpGeometry,
                                pzGeometry, pzGeometryFlipped, FACE_GEOMETRY_LENGTH, x, y, z, binfo.tex_pz,
                                vertexAO(aoKernel[0][1][2], aoKernel[1][2][2], aoKernel[0][2][2]),
                                vertexAO(aoKernel[1][2][2], aoKernel[2][1][2], aoKernel[2][2][2]),
                                vertexAO(aoKernel[2][1][2], aoKernel[1][0][2], aoKernel[2][0][2]),
                                vertexAO(aoKernel[1][0][2], aoKernel[0][1][2], aoKernel[0][0][2]));
                    }
                }
            }
        }
    }
}

void Renderer::loadChunkMesh(int chunkX, int chunkZ, const std::vector<blocktype> &blocks) {
    // get pointer to mesh struct, and create new one if necessary
    const auto chunkMeshEntry = chunkMeshes.find(std::pair<int, int>(chunkX, chunkZ));
    chunkMesh *p_chunkMesh;
    if (chunkMeshEntry != chunkMeshes.end()) {
        // mesh for this chunk already exists
        p_chunkMesh = &(chunkMeshEntry->second);
    } else {
        // create new mesh entry
        chunkMesh temp;
        std::pair<int, int> key(chunkX, chunkZ);
        chunkMeshes.insert(std::make_pair(key, temp));
        p_chunkMesh = &(chunkMeshes.find(key)->second);
        p_chunkMesh->opaqueMesh = std::make_shared<Mesh>(
                faceAttribs, LEN_STATIC(faceAttribs), GL_DYNAMIC_DRAW, GL_TRIANGLES);
        p_chunkMesh->transparentMesh = std::make_shared<Mesh>(
                faceAttribs, LEN_STATIC(faceAttribs), GL_DYNAMIC_DRAW, GL_TRIANGLES);
    }
    
    // generate mesh
    std::vector<float> opaqueMeshData;
    std::vector<float> transparentMeshData;
    mesh(opaqueMeshData, transparentMeshData, chunkX, chunkZ, blocks);
    
    // send vertex data to OpenGL
    p_chunkMesh->opaqueMesh->setData(
            &opaqueMeshData[0], opaqueMeshData.size());
    p_chunkMesh->transparentMesh->setData(
            &transparentMeshData[0], transparentMeshData.size());
}

void Renderer::updateMesh(int chunkX, int chunkZ) {
    const auto chunks = world->getChunks();
    const auto &chunkEntry = chunks->find(std::pair<int, int>(chunkX, chunkZ));
    if (chunkEntry != chunks->end()) {
        loadChunkMesh(chunkX, chunkZ, chunkEntry->second);
    }
}

void Renderer::updateMesh(int x, int y, int z) {
    int chunkX = (int) floor((float) x / CHUNK_SIZE);
    int chunkZ = (int) floor((float) z / CHUNK_SIZE);
    updateMesh(chunkX, chunkZ);
    
    //update adjacent chunks
    int internalX = x - (chunkX * CHUNK_SIZE);
    int internalZ = z - (chunkZ * CHUNK_SIZE);
    if (internalX == 0) {
        updateMesh(chunkX - 1, chunkZ);
    } else if (internalX == CHUNK_SIZE - 1) {
        updateMesh(chunkX + 1, chunkZ);
    }
    if (internalZ == 0) {
        updateMesh(chunkX, chunkZ - 1);
    } else if (internalZ == CHUNK_SIZE - 1) {
        updateMesh(chunkX, chunkZ + 1);
    }
}

void Renderer::setWorld(World *world) {
    this->world = world;
    
    const auto chunks = world->getChunks();
    const auto chunksEnd = chunks->end();
    for (auto chunkEntry = chunks->begin(); chunkEntry != chunksEnd; ++chunkEntry) {
        loadChunkMesh(chunkEntry->first.first, chunkEntry->first.second, chunkEntry->second);
    }
}

float Renderer::getCamX() { return camPos[0]; }
float Renderer::getCamY() { return camPos[1]; }
float Renderer::getCamZ() { return camPos[2]; }
float Renderer::getCamPitch() { return camPitch; }
float Renderer::getCamYaw() { return camYaw; }
void Renderer::setCamPos(glm::vec3 pos) { camPos = pos; }
void Renderer::setCamPos(float x, float y, float z) { camPos = glm::vec3(x, y, z); }
void Renderer::setCamPitch(float pitch) { camPitch = pitch; }
void Renderer::setCamYaw(float yaw) { camYaw = yaw; }

void Renderer::setDrawSelectionCube(bool drawSelectionCube) {
    this->drawSelectionCube = drawSelectionCube;
}

void Renderer::setSelectedBlock(int x, int y, int z) {
    selectionModelMatrix = glm::translate(glm::mat4(), glm::vec3((float) x, (float) y, (float) z));
}

void Renderer::render() {
    updateSkyRotationMatrix();
    lightDirection = glm::mat3(skyRotationMatrix) * glm::vec3(1.0f, 0.0f, -0.2f);
    updateLightMvpMatrix();
    
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    
    shadowMapShaderProgram.useProgram();
    glUniformMatrix4fv(shadowMapShaderProgram.uniforms["u_LightMvpMatrix"], 1, GL_FALSE, &lightMvpMatrix[0][0]);
    
    for (auto &chunkMeshEntry : chunkMeshes) {
        chunkMesh &chunkMesh = chunkMeshEntry.second;
        chunkMesh.opaqueMesh->draw();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    skyShaderProgram.useProgram();
    glUniformMatrix4fv(skyShaderProgram.uniforms["u_VpRotationMatrix"], 1,
            GL_FALSE, &(skyVpRotationMatrix * skyboxCorrectionMatrix)[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, starfieldTexture);
    glUniform1i(skyShaderProgram.uniforms["u_Texture"], 0);
    
    skyboxMesh.draw();
    
    sunShaderProgram.useProgram();
    glUniformMatrix4fv(sunShaderProgram.uniforms["u_VpRotationMatrix"], 1, GL_FALSE, &skyVpRotationMatrix[0][0]);
    glUniform4f(sunShaderProgram.uniforms["u_Color"], 1.0f, 0.97f, 0.90f, 1.0f);
    
    sunMesh.draw();

    blockShaderProgram.useProgram();
    glUniformMatrix4fv(blockShaderProgram.uniforms["u_MvpMatrix"], 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(blockShaderProgram.uniforms["u_LightBiasMvpMatrix"], 1, GL_FALSE, &lightBiasMvpMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas);
    glUniform1i(blockShaderProgram.uniforms["u_Texture"], 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapDepthTexture);
    glUniform1i(blockShaderProgram.uniforms["u_ShadowMap"], 1);
    glUniform3fv(blockShaderProgram.uniforms["u_LightDirection"], 1, &lightDirection[0]);
    float ambientLight = fmax(
            (sinf(world->getTimeOfDay() * TWO_PI)) * (1.0f - DAWN_AMBIENT_LIGHT) + DAWN_AMBIENT_LIGHT,
            MIDNIGHT_AMBIENT_LIGHT);
    glUniform1f(blockShaderProgram.uniforms["u_AmbientLight"], ambientLight);
    
    for (auto &chunkMeshEntry : chunkMeshes) {
        chunkMesh &chunkMesh = chunkMeshEntry.second;
        chunkMesh.opaqueMesh->draw();
    }
    
    glDepthMask(GL_FALSE);
    for (auto &chunkMeshEntry : chunkMeshes) {
        chunkMesh &chunkMesh = chunkMeshEntry.second;
        chunkMesh.transparentMesh->draw();
    }
    
    if (drawSelectionCube) {
        simpleShaderProgram.useProgram();
        glUniformMatrix4fv(simpleShaderProgram.uniforms["u_MvpMatrix"], 1, GL_FALSE,
                &(mvpMatrix * selectionModelMatrix)[0][0]);
        glUniform4f(simpleShaderProgram.uniforms["u_Color"], 1.0f, 1.0f, 1.0f, 0.25f);
        
        selectionMesh.draw();
    }
    glDepthMask(GL_TRUE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    screenShaderProgram.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedColorTexture);
    glUniform1i(screenShaderProgram.uniforms["u_ColorTexture"], 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderedDepthTexture);
    glUniform1i(screenShaderProgram.uniforms["u_DepthTexture"], 1);
    if (camPos.y < WATER_LEVEL) {
        glUniform4f(screenShaderProgram.uniforms["u_ColorOverlay"], 0.0f, 0.0f, 1.0f, 0.25f);
    } else {
        glUniform4f(screenShaderProgram.uniforms["u_ColorOverlay"], 0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    screenMesh.draw();
    
    glPointSize(50.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
    
    crosshairShaderProgram.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas);
    glBindTexture(crosshairShaderProgram.uniforms["u_Texture"], 0);
    glUniform2f(crosshairShaderProgram.uniforms["u_StartUV"], 0.0f, 0.0f);
    glUniform1f(crosshairShaderProgram.uniforms["u_TextureSize"], TEXTURE_ATLAS_SIZE_RECIPROCAL);
    
    crosshairMesh.draw();
}

void Renderer::updateViewMatrix() {
    viewMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(), -camPitch, glm::vec3(1.0f, 0.0f, 0.0f)), -camYaw, glm::vec3(0.0f, 1.0f, 0.0f)), -camPos);
    updateMvpMatrix();
    updateVpRotationMatrix();
}

void Renderer::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(
            glm::radians(60.0f), (float) width / (float) height, 0.1f, 1000.0f);
    updateMvpMatrix();
    updateVpRotationMatrix();
}

void Renderer::updateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix;
}

void Renderer::updateVpRotationMatrix() {
    vpRotationMatrix = viewMatrix;
    
    // remove translation components of matrix
    vpRotationMatrix[3][0] = 0.0f;
    vpRotationMatrix[3][1] = 0.0f;
    vpRotationMatrix[3][2] = 0.0f;
    
    vpRotationMatrix = projectionMatrix * vpRotationMatrix;
}

void Renderer::updateSkyRotationMatrix() {
    skyRotationMatrix = glm::rotate(world->getTimeOfDay() * TWO_PI, glm::vec3(0.0f, 0.0f, 1.0f));
    updateSkyVpRotationMatrix();
}

void Renderer::updateSkyVpRotationMatrix() {
    skyVpRotationMatrix = vpRotationMatrix * skyRotationMatrix;
}

void Renderer::updateLightMvpMatrix() {
    lightMvpMatrix = glm::ortho(-128.0f, 128.0f, -128.0f, 128.0f, -512.0f, 256.0f) * glm::lookAt(lightDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightBiasMvpMatrix = lightBiasMatrix * lightMvpMatrix;
}
