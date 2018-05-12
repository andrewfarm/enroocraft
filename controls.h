//
//  controls.h
//  enroocraft
//
//  Created by Andrew Farm on 5/3/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef controls_h
#define controls_h

#include "renderer.h"
#include "world.h"

#include <GLFW/glfw3.h>

class Controls {
    GLFWwindow *window;
    Renderer *renderer;
    World *world;
    int windowWidth, windowHeight;
    double prevMouseX, prevMouseY;
    bool canBreakBlock, canPlaceBlock;
    
    bool escapeKeyDown;
    bool paused;
    
public:
    Controls(GLFWwindow *window, Renderer *renderer, World *world);
    bool arePaused();
    void update(double deltaTime);
};

#endif /* controls_h */
