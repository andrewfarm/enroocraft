//
//  main.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "controls.h"
#include "world.h"

int main(int argc, const char * argv[]) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW. I tried, but their agents of Evil are too strong.\n");
        return EXIT_FAILURE;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 500;
    
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "enroocraft", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwMakeContextCurrent(window);
    
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }
    
    World world;
    for (int z = -4; z < 4; z++) {
        for (int x = -4; x < 4; x++) {
            world.genesis(x, z);
        }
    }
    
    Renderer renderer;
    renderer.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer.setWorld(&world);
    
    Controls controls(window, &renderer, &world);
    
    std::chrono::high_resolution_clock::time_point start, end;
    double deltaTime;
    end = std::chrono::high_resolution_clock::now();
    do {
        start = end;
        end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
        
        controls.update(deltaTime);
        
        renderer.render();
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));
    
    return EXIT_SUCCESS;
}
