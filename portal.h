//
//  portal.h
//  enroocraft
//
//  Created by Andrew Farm on 5/23/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef portal_h
#define portal_h

#include <vector>
#include <memory>

#include <glm/glm.hpp>

enum Plane {X=0, Y=1, Z=2};

struct BlockCoord {
    int c[3];
    
    BlockCoord(int x, int y, int z) {
        c[0] = x;
        c[1] = y;
        c[2] = z;
    }
    
    int operator[](int index) const {
        return c[index];
    }
    
    bool operator<(const BlockCoord &other) const {
        if (c[0] == other[0]) {
            if (c[1] == other[1]) {
                return c[2] < other[2];
            }
            return c[1] < other[1];
        }
        return c[0] < other[0];
    }
};

struct PortalPlane {
    Plane plane;
    int planeOrdinate;
    std::vector<std::pair<BlockCoord, BlockCoord>> betweenBlocks;
    glm::mat4 translationMatrix;
    glm::mat4 rotationMatrix;
};

typedef std::pair<std::shared_ptr<PortalPlane>, std::shared_ptr<PortalPlane>> Portal;

#endif /* portal_h */
