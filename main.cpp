#include "game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <cmath>

int main(){
    lb::Game::Settings state = {
        400, 400,
        {{{50, 50, 350, 50}, {1.0, 1.0, 1.0, 1.0}}, {{350, 50, 350, 350}, {1.0, 1.0, 1.0, 1.0}}, {{350, 350, 50, 350}, {1.0, 1.0, 1.0, 1.0}}, {{50, 350, 50, 50}, {1.0, 1.0, 1.0, 1.0}}},
        true, true,
        glm::ortho(0.0f, 400.0f, 400.0f, 0.0f)
    };
    lb::Game game; game.set_state(state);

    lb::Particle prt;
    prt.seg_angle = 30;
    prt.color={1.0, 1.0, 1.0, 1.0};
    prt.r=10;
    prt.pos[0] = 200;
    prt.pos[1] = 200;

    srand(clock());
    for(int i = 0; i < 15; i++) {
        prt.attck_angle = rand()%360;
        game.add_particle(prt);
    }

    /*
    prt.pos[0]=159; prt.pos[1]=150;
    prt.attck_angle=315.0;
    game.add_particle(prt);
    
    prt.pos[0]=250; prt.pos[1]=250;
    prt.attck_angle=135.0;
    game.add_particle(prt);
    */
    /*
    prt.pos[0]=200; prt.pos[1]=200;
    for(int i = 0; i <= 45; i++){
        prt.attck_angle=i;
        game.add_particle(prt);
    }
    */

    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(state.scrwidth, state.scrheight, "particle-game", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init glad" << std::endl;
    }

    glViewport(0, 0, state.scrwidth, state.scrheight);

    lb::Game::init();

    double start=0, end=0;
    while(!glfwWindowShouldClose(window)){
        end=glfwGetTime();
        if(end-start>0.25f){
            game.update(0.25f);
            start=end;
        }
        // get input
        glfwPollEvents();
        game.input(window);
        // draw scene
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game.draw();
        glfwSwapBuffers(window);
    }

    lb::Game::fina();

    glfwTerminate();
}
