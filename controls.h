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

#include <GLFW/glfw3.h>

void updateControls(
        GLFWwindow *window,
        Renderer &renderer,
        double *prevMouseX,
        double *prevMouseY,
        double deltaTime);

#endif /* controls_h */
