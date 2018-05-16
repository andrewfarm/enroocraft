//
//  player.h
//  enroocraft
//
//  Created by Andrew Farm on 5/16/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef player_h
#define player_h

#include "entity.h"

class Player : public Entity {
    float lookYaw;
    float lookPitch;
    
public:
    void setLookYaw(float lookYaw);
    void setLookPitch(float lookPitch);
    
    float getLookYaw();
    float getLookPitch();
};

#endif /* player_h */
