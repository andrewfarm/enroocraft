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

#include "world.h"

#define FACE_GEOMETRY_LENGTH 18
#define FACE_GEOMETRY_STRIDE 3

static const float nxGeometry[] = {
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
};
static const float pxGeometry[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};
static const float nyGeometry[] = {
    1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};
static const float pyGeometry[] = {
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
};
static const float nzGeometry[] = {
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
};
static const float pzGeometry[] = {
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
};

static void translateGeometry(float *geometry, size_t length,
        float x, float y, float z) {
    for (int i = 0; i < length; i += FACE_GEOMETRY_STRIDE) {
        geometry[i]     += x;
        geometry[i + 1] += y;
        geometry[i + 2] += z;
    }
}

static void copyTranslatedIntoVector(
        std::vector<float> &dest,
        float *temp,
        const float *src,
        size_t length,
        float translateX,
        float translateY,
        float translateZ) {
    memcpy(temp, src, length * sizeof(*src));
    translateGeometry(temp, length, translateX, translateY, translateZ);
    size_t size = dest.size();
    dest.resize(size + length);
    for (size_t i = 0; i < length; i++) {
        dest[size + i] = temp[i];
    }
}

void World::genesis(int chunkX, int chunkY) {
    std::vector<blocktype> chunkdata;
    chunkdata.resize(1026);
    for (int i = 0; i < 1026; i++) {
        chunkdata[i] = BLOCK_SOLID;
    }
    chunks.insert(std::make_pair(std::make_pair(chunkX, chunkY), std::move(chunkdata)));
}

blocktype World::getBlock(int x, int y, int z) {
    std::pair<int, int> chunkKey = std::make_pair(
            floor((float) x / CHUNK_SIZE), floor((float) z / CHUNK_SIZE));
    if (chunks.count(chunkKey) == 0) {
        return BLOCK_NOT_LOADED;
    }
    std::vector<blocktype> chunk = chunks[chunkKey];
    int index = (y * CHUNK_SIZE * CHUNK_SIZE) + ((z % CHUNK_SIZE) * CHUNK_SIZE) + (x % CHUNK_SIZE);
    if ((index < 0) || (index > chunk.size())) {
        return BLOCK_AIR;
    }
    return chunk[index];
}

std::vector<float>World::mesh() {
    std::vector<float> vertices;
    std::pair<int, int> chunkCoords;
    int internalX, internalY, internalZ;
    unsigned int chunkHeight;
    int index;
    int x, y, z;
    float tmpGeometry[FACE_GEOMETRY_LENGTH];
    for (auto& entry : chunks) {
        chunkCoords = entry.first;
        chunkHeight = (unsigned int) ceil((float) entry.second.size() / (CHUNK_SIZE * CHUNK_SIZE));
        for (internalY = 0; internalY < chunkHeight; internalY++) {
            for (internalZ = 0; internalZ < CHUNK_SIZE; internalZ++) {
                for (internalX = 0; internalX < CHUNK_SIZE; internalX++) {
                    index = (internalY * CHUNK_SIZE * CHUNK_SIZE) +
                            (internalZ * CHUNK_SIZE) +
                            internalX;
                    if ((index < entry.second.size()) &&
                        (entry.second[index] > BLOCK_AIR)) {
                        x = chunkCoords.first  * CHUNK_SIZE + internalX;
                        y = internalY;
                        z = chunkCoords.second * CHUNK_SIZE + internalZ;
                        
                        if (getBlock(x - 1, y, z) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nxGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x + 1, y, z) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    pxGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y - 1, z) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nyGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y + 1, z) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    pyGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y, z - 1) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nzGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y, z + 1) < BLOCK_SOLID) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    pzGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                    }
                }
            }
        }
    }
    return vertices;
}
