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
#include <noise/noise.h>

#include "world.h"

static const float nxGeometry[] = {
    0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
};
static const float pxGeometry[] = {
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
};
static const float nyGeometry[] = {
    1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
};
static const float pyGeometry[] = {
    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
};
static const float nzGeometry[] = {
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
};
static const float pzGeometry[] = {
    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
};

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

static void copyTranslatedIntoVector(
        std::vector<float> &dest,
        float *temp,
        const float *src,
        size_t length,
        float translateX,
        float translateY,
        float translateZ,
        int textureNumber) {
    memcpy(temp, src, length * sizeof(*src));
    translateGeometry(temp, length, translateX, translateY, translateZ);
    translateUV(temp, length, textureNumber);
    size_t size = dest.size();
    dest.resize(size + length);
    for (size_t i = 0; i < length; i++) {
        dest[size + i] = temp[i];
    }
}

void World::genesis(int chunkX, int chunkZ) {
    printf("Generating chunk (%d, %d)\n", chunkX, chunkZ);
    std::vector<blocktype> chunkdata;
    
    noise::module::Perlin noiseModule;
    int maxHeight = 0;
    int heightmap[CHUNK_SIZE][CHUNK_SIZE];
    int height;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            heightmap[z][x] = height = 64 + (int) (noiseModule.GetValue(
                    (chunkX * CHUNK_SIZE + x) * 0.01 + 0.5,
                    0.5,
                    (chunkZ * CHUNK_SIZE + z) * 0.01 + 0.5)
                    * 16.0);
            if (height > maxHeight) {
                maxHeight = height;
            }
        }
    }
    chunkdata.reserve(maxHeight * CHUNK_SIZE * CHUNK_SIZE);
    for (int y = 0; y <= maxHeight; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                chunkdata.push_back((y < heightmap[z][x]) ? BLOCK_GRASS : BLOCK_AIR);
            }
        }
    }
    
    chunks.insert(std::make_pair(std::make_pair(chunkX, chunkZ), std::move(chunkdata)));
}

static inline int mod(int a, int b) {
    return (a % b + b) % b;
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
// TODO reduce reuse of code is setBlock and getBlock
void World::setBlock(int x, int y, int z, blocktype block) {
    const std::pair<int, int> chunkKey(
            floorf((float) x / CHUNK_SIZE), floorf((float) z / CHUNK_SIZE));
    auto result = chunks.find(chunkKey);
    if (result != chunks.end()) {
        std::vector<blocktype> &chunk = result->second;
        int index = (y * CHUNK_SIZE * CHUNK_SIZE) + (mod(z, CHUNK_SIZE) * CHUNK_SIZE) + mod(x, CHUNK_SIZE);
        chunk[index] = block;
    }
}

std::vector<float> World::mesh(int chunkX, int chunkZ, const std::vector<blocktype> &blocks) {
    std::vector<float> vertices;
    int internalX, internalY, internalZ;
    int index;
    int x, y, z;
    float tmpGeometry[FACE_GEOMETRY_LENGTH];
    block_textures tex;
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
                    
                    x = chunkX * CHUNK_SIZE + internalX;
                    y = internalY;
                    z = chunkZ * CHUNK_SIZE + internalZ;
                    tex = textureNumbers[blocks[index]];
                    
                    if (getBlock(x - 1, y, z) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                nxGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.nx);
                    }
                    if (getBlock(x + 1, y, z) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                pxGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.px);
                    }
                    if (getBlock(x, y - 1, z) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                nyGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.ny);
                    }
                    if (getBlock(x, y + 1, z) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                pyGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.py);
                    }
                    if (getBlock(x, y, z - 1) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                nzGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.nz);
                    }
                    if (getBlock(x, y, z + 1) == BLOCK_AIR) {
                        copyTranslatedIntoVector(vertices, tmpGeometry,
                                pzGeometry, FACE_GEOMETRY_LENGTH, x, y, z, tex.pz);
                    }
                }
            }
        }
    }
    return vertices;
}

std::map<std::pair<int, int>, std::vector<blocktype>> *World::getChunks() {
    return &chunks;
}
