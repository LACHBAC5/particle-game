#pragma once

#include "shape/circle.hpp"

namespace lb{
    class Particle : public Circle {
        public:
        void collide(double unit);
        void advance(double time);

        double attck_angle;
        static constexpr double velocity=20;
    };
}
