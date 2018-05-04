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
                chunkdata.push_back((y < heightmap[z][x]) ? BLOCK_SOLID : BLOCK_AIR);
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
            floor((float) x / CHUNK_SIZE), floor((float) z / CHUNK_SIZE));
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

std::vector<float>World::mesh() {
    std::vector<float> vertices;
    int internalX, internalY, internalZ;
    int chunkHeight;
    int index;
    int x, y, z;
    float tmpGeometry[FACE_GEOMETRY_LENGTH];
    for (auto& entry : chunks) {
        // entry.first is the cunk (x, z) coordinates, entry.second is the chunk data
        printf("Meshing chunk (%d, %d)\n", entry.first.first, entry.first.second);
        chunkHeight = (int) ceil((float) entry.second.size() / (CHUNK_SIZE * CHUNK_SIZE));
        for (internalY = 0; internalY < chunkHeight; internalY++) {
            for (internalZ = 0; internalZ < CHUNK_SIZE; internalZ++) {
                for (internalX = 0; internalX < CHUNK_SIZE; internalX++) {
                    index = (internalY * CHUNK_SIZE * CHUNK_SIZE) +
                            (internalZ * CHUNK_SIZE) +
                            internalX;
                    if ((index < entry.second.size()) &&
                        (entry.second[index] > BLOCK_AIR)) {
                        x = entry.first.first  * CHUNK_SIZE + internalX;
                        y = internalY;
                        z = entry.first.second * CHUNK_SIZE + internalZ;
                        
                        if (getBlock(x - 1, y, z) == BLOCK_AIR) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nxGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x + 1, y, z) == BLOCK_AIR) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    pxGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y - 1, z) == BLOCK_AIR) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nyGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y + 1, z) == BLOCK_AIR) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    pyGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y, z - 1) == BLOCK_AIR) {
                            copyTranslatedIntoVector(vertices, tmpGeometry,
                                    nzGeometry, FACE_GEOMETRY_LENGTH, x, y, z);
                        }
                        if (getBlock(x, y, z + 1) == BLOCK_AIR) {
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
