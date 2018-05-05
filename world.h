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
const blocktype BLOCK_AIR   = 0;
const blocktype BLOCK_GRASS = 1;

struct block_textures {
    int nx, px, ny, py, nz, pz;
};

const block_textures textureNumbers[] = {
    {-1, -1, -1, -1, -1, -1}, //AIR
    {2, 2, 3, 1, 2, 2}        //GRASS
};

#define FACE_GEOMETRY_LENGTH 48
#define FACE_GEOMETRY_STRIDE 8

#define FACE_GEOMETRY_POSITION 0
#define FACE_GEOMETRY_NORMAL   3
#define FACE_GEOMETRY_UV       6

class World {
    std::map<std::pair<int, int>, std::vector<blocktype>> chunks;
    
public:
    void genesis(int chunkX, int chunkY);
    blocktype getBlock(int x, int y, int z);
    std::vector<float> mesh();
};

#endif /* world_h */
