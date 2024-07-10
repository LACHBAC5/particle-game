#include "game.hpp"
#include <iostream>

namespace lb{
    void Game::set_state(const Settings& state){
        _state=state;
    }
    const Game::Settings& Game::get_state(){
        return _state;
    }

    void Game::init(){
        lb::Circle::init();
        lb::Line::init();
    }

    void Game::fina(){
        lb::Line::fina();
        lb::Circle::fina();
    }

    void Game::draw(){
        // draw particles
        for(int i = 0; i < _particles.size(); i++){
            _particles[i].draw(_state.projection);
        }
        // draw borders
        for(int i = 0; i < 4; i++){
            _state.borders[i].draw(_state.projection);
        }
    }

    void Game::update(double time){
        double progress=0.0;
        while(progress<time){
            double delta_time = time-progress;

            std::vector<std::pair<int, double>> collisions;
            for(int i = 0; i < _particles.size(); i++){
                if(_state.collision_detection_circle){
                    for(int o = i+1; o < _particles.size(); o++){
                        // gibberish derived from:
                        // pow(f.r+s.r,2) = pow(f.x+cos(f.attck_angle)*f.velocity*time)-(s.x+cos(s.attck_angle)*s.velocity*time), 2) + pow(f.y-sin(f.attck_angle)*f.velocity*time)-(s.x-sin(s.attck_angle)*s.velocity*time, 2)
                        // or.. delta_pos^2 = (f.change_of_x_per_second - s.change_of_x_per_second)^2 + (f.change_of_y_per_second - s.change_of_y_per_second)^2
                        // then solve for time in seconds

                        const double AB1 = _particles[i].r+_particles[o].r;

                        const double a = pow(cos(_particles[i].attck_angle*M_PI/180)*Particle::velocity-cos(_particles[o].attck_angle*M_PI/180)*Particle::velocity, 2) + pow(sin(_particles[o].attck_angle*M_PI/180)*Particle::velocity-sin(_particles[i].attck_angle*M_PI/180)*Particle::velocity, 2);
                        const double b = (2 * (_particles[i].pos[0]-_particles[o].pos[0]) * (cos(_particles[i].attck_angle*M_PI/180)*Particle::velocity-cos(_particles[o].attck_angle*M_PI/180)*Particle::velocity)) + (2 * (_particles[i].pos[1]-_particles[o].pos[1]) * (sin(_particles[o].attck_angle*M_PI/180)*Particle::velocity-sin(_particles[i].attck_angle*M_PI/180)*Particle::velocity));
                        const double c = pow(_particles[i].pos[0], 2) + pow(_particles[o].pos[0], 2) - 2 * _particles[i].pos[0]*_particles[o].pos[0] + pow(_particles[i].pos[1], 2) + pow(_particles[o].pos[1], 2) - 2 * _particles[i].pos[1]*_particles[o].pos[1] - pow(AB1, 2);

                        const double D = pow(b, 2) - 4 * a * c;

                        // two possible time points... a positive and a negative.
                        const double t1 = (- b - sqrt(D)) / (2 * a);
                        // const double t2 = (- b + sqrt(D)) / (2 * a);

                        if(t1 > 0 && t1 < delta_time){
                            double cllsn = atan2(
                                _particles[i].pos[1]-_particles[o].pos[1],
                                _particles[o].pos[0]-_particles[i].pos[0]
                            )/M_PI*180;
                            collisions={std::make_pair(i, cllsn), std::make_pair(o, 180+cllsn)};
                            delta_time=t1;
                        }
                        else if(t1==delta_time){
                            double cllsn = atan2(
                                _particles[o].pos[1]-_particles[i].pos[1],
                                _particles[o].pos[0]-_particles[i].pos[0]
                            )/M_PI*180;
                            collisions.push_back(std::make_pair(i, cllsn));
                            collisions.push_back(std::make_pair(i, 180-cllsn));
                        }
                    }
                }

                if(_state.collision_detection_wall){
                    double delta_time_wall;
                    if((delta_time_wall = 1.0/cos(_particles[i].attck_angle*M_PI/180)*(_state.borders[2].pos[0]-_particles[i].pos[0])/Particle::velocity) < delta_time && delta_time_wall>0){
                        collisions={std::make_pair(i, 0.0)};
                        delta_time=delta_time_wall;
                    }
                    else if(delta_time_wall==delta_time){
                        collisions.push_back(std::make_pair(i, 0.0));
                    }
                    else if((delta_time_wall = -1.0/cos(_particles[i].attck_angle*M_PI/180)*(_particles[i].pos[0]-_state.borders[0].pos[0])/Particle::velocity) < delta_time && delta_time_wall>0){
                        collisions={std::make_pair(i, 180.0)};
                        delta_time=delta_time_wall;
                    }
                    else if(delta_time_wall==delta_time){
                        collisions.push_back(std::make_pair(i, 180.0));
                    }
                    else if((delta_time_wall = -1.0/sin(_particles[i].attck_angle*M_PI/180)*(_state.borders[2].pos[1]-_particles[i].pos[1])/Particle::velocity) < delta_time && delta_time_wall>0){
                        collisions={std::make_pair(i, 270.0)};
                        delta_time=delta_time_wall;
                    }
                    else if(delta_time_wall==delta_time){
                        collisions.push_back(std::make_pair(i, 270.0));
                    }
                    else if((delta_time_wall = 1.0/sin(_particles[i].attck_angle*M_PI/180)*(_particles[i].pos[1]-_state.borders[0].pos[1])/Particle::velocity) < delta_time && delta_time_wall>0){
                        collisions={std::make_pair(i, 90.0)};
                        delta_time=delta_time_wall;
                    }
                    else if(delta_time_wall==delta_time){
                        collisions.push_back(std::make_pair(i, 90.0));
                    }
                }
            }
            
            // advance
            for(int i = 0; i < _particles.size(); i++){
                _particles[i].advance(delta_time);
            }

            /*
            if(collisions.size()>0){
                std::cout << "new patch:\n";
            }
            */
            // recalculate attck_angle
            for(const auto& [index, cllsn] : collisions){
                //std::cout << "index=" << index << ", cllsn=" << cllsn << ", attck=" << _particles[index].attck_angle;
                _particles[index].collide(cllsn);
                //std::cout << ", reflect=" << _particles[index].attck_angle << '\n';
            }

            // update progress
            progress += delta_time;
        }
    }

    void Game::input(GLFWwindow* window){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
    }

    const Particle& Game::view_particle(int index){
        return _particles[index];
    }

    void Game::add_particle(const Particle& prt){
        _particles.push_back(prt);
    }

    void Game::remove_particle(int index){
        _particles.erase(_particles.begin()+index);
    }

    void Game::set_particle_position(int index, int position[2]){
        _particles[index].pos[0]=position[0]; _particles[index].pos[1]=position[1];
    }

    void Game::set_particle_attck_angle(int index, double attck){
        _particles[index].attck_angle=attck;
    }
}
