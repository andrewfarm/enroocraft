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

class World {
    std::map<std::pair<int, int>, std::vector<blocktype>> chunks;
    
public:
    void genesis(int chunkX, int chunkY);
    blocktype getBlock(int x, int y, int z);
    std::vector<float> mesh();
};

#endif /* world_h */
