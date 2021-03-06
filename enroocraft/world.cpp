//
//  world.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/3/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
#include <noise/noise.h>

#include "world.h"

const float TIME_PASSAGE_RATE = 1.0f / 60.0f;

const float TREE_FREQ = 0.01f;

const float DRAG_COEF = 0.03f;
const float MAX_SPEED = 20.0f;

const int TREE_WIDTH = 5;
const int TREE_DEPTH = 5;
const int TREE_HEIGHT = 6;
const int TREE_CENTER_X = 2;
const int TREE_CENTER_Z = 2;
const int TREE_CENTER_Y = -1;
const blocktype STRUCTURE_TREE[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, BLOCK_WOOD, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, BLOCK_WOOD, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_WOOD,   BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_WOOD,   BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES,
    
    0, 0, 0, 0, 0,
    0, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, 0,
    0, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, 0,
    0, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, 0,
    0, 0, 0, 0, 0,
    
    0, 0, 0, 0, 0,
    0, 0, BLOCK_LEAVES, 0, 0,
    0, BLOCK_LEAVES, BLOCK_LEAVES, BLOCK_LEAVES, 0,
    0, 0, BLOCK_LEAVES, 0, 0,
    0, 0, 0, 0, 0,
};

const int PORTAL_FRAME_WIDTH = 4;
const int PORTAL_FRAME_DEPTH = 1;
const int PORTAL_FRAME_HEIGHT = 5;
const int PORTAL_FRAME_CENTER_X = 3;
const int PORTAL_FRAME_CENTER_Z = 0;
const int PORTAL_FRAME_CENTER_Y = 4;
const blocktype STRUCTURE_PORTAL_FRAME[] = {
    BLOCK_OBSIDIAN, BLOCK_OBSIDIAN, BLOCK_OBSIDIAN, BLOCK_OBSIDIAN,
    BLOCK_OBSIDIAN, BLOCK_AIR,      BLOCK_AIR,      BLOCK_OBSIDIAN,
    BLOCK_OBSIDIAN, BLOCK_AIR,      BLOCK_AIR,      BLOCK_OBSIDIAN,
    BLOCK_OBSIDIAN, BLOCK_AIR,      BLOCK_AIR,      BLOCK_OBSIDIAN,
    BLOCK_OBSIDIAN, BLOCK_OBSIDIAN, BLOCK_OBSIDIAN, BLOCK_OBSIDIAN,
};

World::World() :
unlinkedPortalPlane(nullptr),
timeOfDay(0)
{}

void World::genesis(int chunkX, int chunkZ) {
    printf("Generating chunk (%d, %d)\n", chunkX, chunkZ);
    std::vector<blocktype> chunkdata;
    
    noise::module::Perlin noiseModule;
    int maxHeight = WATER_LEVEL; // start at water level
    int heightmap[CHUNK_SIZE][CHUNK_SIZE];
    int height;
    srand((unsigned int) time(NULL));
    int randX = rand(), randZ = rand();
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            heightmap[z][x] = height = WATER_LEVEL + (int) (noiseModule.GetValue(
                    (chunkX * CHUNK_SIZE + randX + x) * 0.01 + 0.5,
                    0.5,
                    (chunkZ * CHUNK_SIZE + randZ + z) * 0.01 + 0.5)
                    * 28.0);
            if (height > maxHeight) {
                maxHeight = height;
            }
        }
    }
    chunkdata.reserve(maxHeight * CHUNK_SIZE * CHUNK_SIZE);
    blocktype block;
    int depth;
    for (int y = 0; y <= maxHeight; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                depth = heightmap[z][x] - y;
                if (y == 0) {
                    block = BLOCK_BEDROCK;
                } else if (depth < 0) {
                    block = (y < WATER_LEVEL) ? BLOCK_WATER : BLOCK_AIR;
                } else if (depth == 0) {
                    block = (heightmap[z][x] < WATER_LEVEL + 1) ? BLOCK_SAND : BLOCK_GRASS;
                } else if (depth <= 2) {
                    block = (heightmap[z][x] < WATER_LEVEL + 1) ? BLOCK_SAND : BLOCK_DIRT;
                } else {
                    block = BLOCK_STONE;
                }
                chunkdata.push_back(block);
            }
        }
    }
    
    chunks.insert(std::make_pair(std::make_pair(chunkX, chunkZ), std::move(chunkdata)));
}

void World::plant() {
    blocktype block;
    int x, y, z;
    int treeStartX, treeStartY, treeStartZ;
    int currSetBlockX, currSetBlockY, currSetBlockZ;
    blocktype treeBlock;
    for (const auto &chunkEntry : chunks) {
        for (int internalY = 0; internalY < ceil((float) chunkEntry.second.size() / (CHUNK_SIZE * CHUNK_SIZE)); internalY++) {
            for (int internalX = 0; internalX < CHUNK_SIZE; internalX++) {
                for (int internalZ = 0; internalZ < CHUNK_SIZE; internalZ++) {
                    x = chunkEntry.first.first * CHUNK_SIZE + internalX;
                    y = internalY;
                    z = chunkEntry.first.second * CHUNK_SIZE + internalZ;
                    block = getBlock(x, y, z);
                    if ((block == BLOCK_GRASS) && ((float) rand() / RAND_MAX < TREE_FREQ)) {
                        treeStartX = x - TREE_CENTER_X;
                        treeStartY = y - TREE_CENTER_Y;
                        treeStartZ = z - TREE_CENTER_Z;
                        for (int treeX = 0; treeX < TREE_WIDTH; treeX++) {
                            for (int treeY = 0; treeY < TREE_HEIGHT; treeY++) {
                                for (int treeZ = 0; treeZ < TREE_DEPTH; treeZ++) {
                                    currSetBlockX = treeStartX + treeX;
                                    currSetBlockY = treeStartY + treeY;
                                    currSetBlockZ = treeStartZ + treeZ;
                                    treeBlock = STRUCTURE_TREE[(treeY * TREE_WIDTH * TREE_DEPTH) + (treeX * TREE_DEPTH) + treeZ];
                                    if (getBlock(currSetBlockX, currSetBlockY, currSetBlockZ) == BLOCK_AIR) {
                                        setBlock(currSetBlockX, currSetBlockY, currSetBlockZ, treeBlock);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static inline int mod(int a, int b) {
    return (a % b + b) % b;
}

bool World::isLastBlockOfPortalFrame(int x, int y, int z) {
    // portal can only be in one particular configuration for now
    blocktype shouldBe;
    int portalFrameStartX = x - PORTAL_FRAME_CENTER_X;
    int portalFrameStartY = y - PORTAL_FRAME_CENTER_Y;
    for (int portalFrameX = 0; portalFrameX < PORTAL_FRAME_WIDTH; portalFrameX++) {
        for (int portalFrameY  = 0; portalFrameY < PORTAL_FRAME_HEIGHT; portalFrameY++) {
            shouldBe = STRUCTURE_PORTAL_FRAME[
                    (portalFrameY * PORTAL_FRAME_WIDTH * PORTAL_FRAME_DEPTH) +
                    (portalFrameX * PORTAL_FRAME_DEPTH)];
            if (getBlock(portalFrameStartX + portalFrameX, portalFrameStartY + portalFrameY, z) != shouldBe) {
                return false;
            }
        }
    }
    return true;
}

blocktype World::getBlock(int x, int y, int z) {
    const std::pair<int, int> chunkKey(
            floorf((float) x / CHUNK_SIZE), floorf((float) z / CHUNK_SIZE));
    auto result = chunks.find(chunkKey);
    if (result == chunks.end()) {
        return BLOCK_NOT_LOADED;
    }
    std::vector<blocktype> &chunk = result->second;
    int index = (y * CHUNK_SIZE * CHUNK_SIZE) + (mod(z, CHUNK_SIZE) * CHUNK_SIZE) + mod(x, CHUNK_SIZE);
    if ((index < 0) || (index > chunk.size())) {
        return BLOCK_AIR;
    }
    return chunk[index];
}
// TODO reduce reuse of code in setBlock and getBlock
void World::setBlock(int x, int y, int z, blocktype block) {
    const std::pair<int, int> chunkKey(
            floorf((float) x / CHUNK_SIZE), floorf((float) z / CHUNK_SIZE));
    auto result = chunks.find(chunkKey);
    if (result != chunks.end()) {
        std::vector<blocktype> &chunk = result->second;
        int index = (y * CHUNK_SIZE * CHUNK_SIZE) + (mod(z, CHUNK_SIZE) * CHUNK_SIZE) + mod(x, CHUNK_SIZE);
        if (index + 1 > chunk.size()) {
            chunk.resize(index + (CHUNK_SIZE * CHUNK_SIZE), BLOCK_AIR);
        }
        chunk[index] = block;
        
        // possibly create portal
        if (block == BLOCK_OBSIDIAN) {
            if (isLastBlockOfPortalFrame(x, y, z)) {
                std::shared_ptr<PortalPlane> ppp = std::make_shared<PortalPlane>();
                ppp->plane = Z;
                ppp->planeOrdinate = z;
                int portalFrameStartX = x - PORTAL_FRAME_CENTER_X;
                int portalFrameStartY = y - PORTAL_FRAME_CENTER_Y;
                for (int portalFrameX = 1; portalFrameX < PORTAL_FRAME_WIDTH - 1; portalFrameX++) {
                    for (int portalFrameY = 1; portalFrameY < PORTAL_FRAME_HEIGHT - 1; portalFrameY++) {
                        BlockCoord from(portalFrameStartX + portalFrameX, portalFrameStartY + portalFrameY, z);
                        BlockCoord   to(portalFrameStartX + portalFrameX, portalFrameStartY + portalFrameY, z - 1);
                        ppp->betweenBlocks.push_back(std::make_pair(from, to));
                    }
                }
                if (unlinkedPortalPlane) {
                    std::shared_ptr<Portal> pp = std::make_shared<Portal>();
                    
                    pp->first = unlinkedPortalPlane;
                    pp->second = ppp;
                    
                    BlockCoord firstPlaneCorner = pp->first->betweenBlocks[0].first;
                    BlockCoord secondPlaneCorner = pp->second->betweenBlocks[0].first;
                    glm::vec3 diff(
                            (float) (secondPlaneCorner[0] - firstPlaneCorner[0]),
                            (float) (secondPlaneCorner[1] - firstPlaneCorner[1]),
                            (float) (secondPlaneCorner[2] - firstPlaneCorner[2])
                    );
                    pp->first->translationMatrix  = glm::translate(diff);
                    pp->second->translationMatrix = glm::translate(-diff);
                    
                    //TODO support portals that transform orientation
                    pp->first->rotationMatrix  = glm::mat4();
                    pp->second->rotationMatrix = glm::mat4();
                    
                    portals.push_back(pp);
                    portalLookupTable.addPortalPlane(*unlinkedPortalPlane);
                    portalLookupTable.addPortalPlane(*ppp);
                    
                    unlinkedPortalPlane = nullptr;
                } else {
                    unlinkedPortalPlane = ppp;
                }
                printf("Portal created\n");
            }
        }
    }
}

std::map<std::pair<int, int>, std::vector<blocktype>> *World::getChunks() {
    return &chunks;
}

std::vector<Entity> *World::getEntities() {
    return &entities;
}

std::vector<std::shared_ptr<Portal>> *World::getPortals() {
    return &portals;
}

void World::setTimeOfDay(float time) {
    timeOfDay = fmod(time, 1.0);
}

float World::getTimeOfDay() {
    return timeOfDay;
}

void World::update(float deltaTime) {
//    setTimeOfDay(timeOfDay + (TIME_PASSAGE_RATE * deltaTime));
    setTimeOfDay(0.4f);
    
    glm::vec3 vel;
    for (Entity &e : entities) {
        vel = e.getVel();
        if (glm::length2(vel) > MAX_SPEED * MAX_SPEED) {
            vel = glm::normalize(e.getVel()) * MAX_SPEED;
            e.setVel(vel);
        }
        float v2 = glm::length2(vel);
        if (v2 > 0) {
            glm::vec3 drag = -glm::normalize(vel) * glm::length2(vel) * DRAG_COEF;
            if (v2 > glm::length2(drag)) {
                e.acc(drag);
            } else {
                e.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
        
        glm::vec3 beforePos = e.getPos();
        e.move(e.getVel() * deltaTime);
        glm::vec3 afterPos = e.getPos();
        BlockCoord beforeCoord(
                (int) floorf(beforePos[0]),
                (int) floorf(beforePos[1]),
                (int) floorf(beforePos[2]));
        BlockCoord afterCoord(
                (int) floorf(afterPos[0]),
                (int) floorf(afterPos[1]),
                (int) floorf(afterPos[2]));
        PortalPlane *ppp = portalLookupTable.getPortalPlane(beforeCoord, afterCoord);
        if (ppp) {
            e.setPos(glm::vec3(ppp->translationMatrix * glm::vec4(e.getPos(), 1.0f)));
            //TODO support portal planes that transform orientation
        }
    }
}
