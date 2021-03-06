//
//  renderer.h
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include <map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "world.h"
#include "shaderprogram.h"
#include "mesh.h"
#include "indexedmesh.h"
#include "portal.h"
#include "fbo.h"

struct chunkMesh {
    std::shared_ptr<Mesh> opaqueMesh;
    std::shared_ptr<Mesh> transparentMesh;
};

class Renderer {
    World *world;
    
    int width, height;
    
    glm::vec3 camPos;
    float camPitch, camYaw;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    
    glm::mat4 skyRotationMatrix;
    
    glm::vec3 lightDirection;
    glm::mat4 lightMvpMatrix;
    glm::mat4 lightBiasMvpMatrix;
    
    bool drawSelectionCube;
    glm::mat4 selectionModelMatrix;
    
    ShaderProgram shadowMapShaderProgram;
    ShaderProgram blockShaderProgram;
    ShaderProgram screenShaderProgram;
    ShaderProgram crosshairShaderProgram;
    ShaderProgram simpleShaderProgram;
    ShaderProgram skyShaderProgram;
    ShaderProgram sunShaderProgram;
    ShaderProgram portalShaderProgram;
    
    std::map<std::pair<int, int>, chunkMesh> chunkMeshes;
    Mesh screenMesh;
    Mesh crosshairMesh;
    IndexedMesh selectionMesh;
    IndexedMesh skyboxMesh;
    Mesh sunMesh;
    std::map<std::shared_ptr<PortalPlane>, std::shared_ptr<Mesh>>
            portalPlaneMeshes;
    
    GLuint textureAtlas;
    GLuint starfieldTexture;
    
    FBO shadowMapFBO;
    FBO screenFBO;
    FBO portalFBO;
    
    void updateSkyRotationMatrix();
    void updateLightMvpMatrix();
    
    void mesh(
            std::vector<float> &opaqueMeshData,
            std::vector<float> &transparentMeshData,
            int chunkX,
            int chunkZ,
            const std::vector<blocktype> &blocks);
    void loadChunkMesh(int chunkX, int chunkZ, const std::vector<blocktype> &blocks);
    
    void meshPortalPlane(Mesh &mesh, const PortalPlane &pp);
    void updatePortalPlaneMeshes();
    
public:
    Renderer();
    void setSize(float width, float height);
    void setWorld(World *world);
    
    float getCamX();
    float getCamY();
    float getCamZ();
    float getCamPitch();
    float getCamYaw();
    void setCamPos(glm::vec3 pos);
    void setCamPos(float x, float y, float z);
    void setCamPitch(float pitch);
    void setCamYaw(float yaw);
    
    void setDrawSelectionCube(bool drawSelected);
    void setSelectedBlock(int x, int y, int z);
    
    void updateViewMatrix();
    void updateProjectionMatrix();
    
    void updateMesh(int chunkX, int chunkZ);
    void updateMesh(int x, int y, int z);
    
    void renderFrom(glm::mat4 viewMatrix, FBO &fbo, bool renderPortals);
    void render();
};

#endif /* renderer_h */
