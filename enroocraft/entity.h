//
//  entity.h
//  enroocraft
//
//  Created by Andrew Farm on 5/16/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef entity_h
#define entity_h

#include <glm/glm.hpp>

class Entity {
    glm::vec3 pos;
    glm::vec3 vel;
    
public:
    void setPos(glm::vec3 pos);
    void setVel(glm::vec3 vel);
    
    void move(glm::vec3 dpos);
    void acc(glm::vec3 dvel);
    
    glm::vec3 getPos();
    glm::vec3 getVel();
};

#endif /* entity_h */
