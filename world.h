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

#define CHUNK_SIZE 16

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
    bool opaque;
};

const blockinfo blockInfos[] = {
    {-1, -1, -1, -1, -1, -1, false}, //AIR
    {2, 2, 3, 1, 2, 2, true},  //GRASS
    {3, 3, 3, 3, 3, 3, true},  //DIRT
    {4, 4, 4, 4, 4, 4, true},  //STONE
    {6, 6, 6, 6, 6, 6, true},  //COBBLE
    {5, 5, 5, 5, 5, 5, true},  //BEDROCK
    {7, 7, 7, 7, 7, 7, false}, //GLASS
    {8, 8, 8, 8, 8, 8, false}, //WATER
    {9, 9, 9, 9, 9, 9, true},  //SAND
};

#define FACE_GEOMETRY_LENGTH 48
#define FACE_GEOMETRY_STRIDE 8

#define FACE_GEOMETRY_POSITION 0
#define FACE_GEOMETRY_NORMAL   3
#define FACE_GEOMETRY_UV       6

class World {
    std::map<std::pair<int, int>, std::vector<blocktype>> chunks;
    
public:
    void genesis(int chunkX, int chunkZ);
    blocktype getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, blocktype block);
    void mesh(
            std::vector<float> &opaqueMeshData,
            std::vector<float> &transparentMeshData,
            int chunkX,
            int chunkZ,
            const std::vector<blocktype> &blocks);
    std::map<std::pair<int, int>, std::vector<blocktype>> *getChunks();
};

#endif /* world_h */
