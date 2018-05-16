//
//  entity.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/16/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "entity.h"

void Entity::setPos(glm::vec3 pos) {
    this->pos = pos;
}

void Entity::setVel(glm::vec3 vel) {
    this->vel = vel;
}

void Entity::move(glm::vec3 dpos) {
    this->pos += dpos;
}

void Entity::acc(glm::vec3 dvel) {
    this->vel += dvel;
}

glm::vec3 Entity::getPos() {
    return pos;
}

glm::vec3 Entity::getVel() {
    return vel;
}
