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

#define BLOCK_NOT_LOADED -1
#define BLOCK_AIR         0
#define BLOCK_SOLID       1

typedef int16_t blocktype;

static const float nxGeometry[] = {
    0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
};
static const float pxGeometry[] = {
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
};
static const float nyGeometry[] = {
    1.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
};
static const float pyGeometry[] = {
    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
};
static const float nzGeometry[] = {
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
};
static const float pzGeometry[] = {
    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
};

const int FACE_GEOMETRY_LENGTH = sizeof(nxGeometry) / sizeof(nxGeometry[0]);
const int FACE_GEOMETRY_STRIDE = 8;

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
