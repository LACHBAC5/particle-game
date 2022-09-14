#pragma once

#include <vector>
#include "particle.hpp"
#include "shape/line.hpp"
#include <GLFW/glfw3.h>

namespace lb{
    class Game{
        public:
        struct Settings{
            int scrwidth, scrheight;
            lb::Line borders[4]; // top left, top right, bottom left, bottom right
            bool collision_detection_wall; bool collision_detection_circle;
            glm::mat4 projection;
        };
        void set_state(const Settings& state);
        const Settings& get_state();
        static void init();
        static void fina();

        void draw();
        void update(double time);
        void input(GLFWwindow* window);

        const Particle& view_particle(int index);
        void add_particle(const Particle& prt);
        void remove_particle(int index);
        void set_particle_position(int index, int position[2]);
        void set_particle_attck_angle(int index, double attck);

        private:
        std::vector<Particle> _particles;
        Settings _state;
    };
}
