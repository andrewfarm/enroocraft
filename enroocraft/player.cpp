//
//  player.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/16/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "player.h"

void Player::setLookYaw(float lookYaw) {
    this->lookYaw = lookYaw;
}

void Player::setLookPitch(float lookPitch) {
    this->lookPitch = lookPitch;
}

float Player::getLookYaw() {
    return lookYaw;
}

float Player::getLookPitch() {
    return lookPitch;
}
