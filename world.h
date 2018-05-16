//
//  world.h
//  enroocraft
//
//  Created by Andrew Farm on 5/3/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef world_h
#define world_h

#include <map>
#include <vector>

#include "entity.h"

#define CHUNK_SIZE 16

#define WATER_LEVEL 64

typedef int16_t blocktype;

const blocktype BLOCK_NOT_LOADED = -1;
const blocktype BLOCK_AIR        = 0;
const blocktype BLOCK_GRASS      = 1;
const blocktype BLOCK_DIRT       = 2;
const blocktype BLOCK_STONE      = 3;
const blocktype BLOCK_COBBLE     = 4;
const blocktype BLOCK_BEDROCK    = 5;
const blocktype BLOCK_GLASS      = 6;
const blocktype BLOCK_WATER      = 7;
const blocktype BLOCK_SAND       = 8;

const int TEXTURE_ATLAS_SIZE = 8;
const float TEXTURE_ATLAS_SIZE_RECIPROCAL = 1.0f / TEXTURE_ATLAS_SIZE;

struct blockinfo {
    int tex_nx, tex_px, tex_ny, tex_py, tex_nz, tex_pz;
    bool solid;
    bool opaque;
};

const blockinfo blockInfos[] = {
    {-1, -1, -1, -1, -1, -1, false, false}, //AIR
    {2, 2, 3, 1, 2, 2, true,  true},  //GRASS
    {3, 3, 3, 3, 3, 3, true,  true},  //DIRT
    {4, 4, 4, 4, 4, 4, true,  true},  //STONE
    {6, 6, 6, 6, 6, 6, true,  true},  //COBBLE
    {5, 5, 5, 5, 5, 5, true,  true},  //BEDROCK
    {7, 7, 7, 7, 7, 7, true,  false}, //GLASS
    {8, 8, 8, 8, 8, 8, false, false}, //WATER
    {9, 9, 9, 9, 9, 9, true,  true},  //SAND
};

static inline bool isSolid(int block) {
    return (block > 0) && blockInfos[block].solid;
}

class World {
    std::map<std::pair<int, int>, std::vector<blocktype>> chunks;
    std::vector<Entity> entities;
    
public:
    void genesis(int chunkX, int chunkZ);
    blocktype getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, blocktype block);
    std::map<std::pair<int, int>, std::vector<blocktype>> *getChunks();
    std::vector<Entity> *getEntities();
};

#endif /* world_h */
