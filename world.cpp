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
        if (index + 1 > chunk.size()) {
            chunk.resize(index + (CHUNK_SIZE * CHUNK_SIZE), BLOCK_AIR);
        }
        chunk[index] = block;
    }
}

std::map<std::pair<int, int>, std::vector<blocktype>> *World::getChunks() {
    return &chunks;
}
