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

enum Plane {X, Y, Z};

struct BlockCoord {
    int x, y, z;
    
    BlockCoord(int x, int y, int z) : x(x), y(y), z(z) {}
    
    bool operator<(const BlockCoord &other) const {
        if (x == other.x) {
            if (y == other.y) {
                return z < other.z;
            }
            return y < other.y;
        }
        return x < other.x;
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
