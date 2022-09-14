#include "particle.hpp"

namespace lb{
    void Particle::advance(double time){
        pos[0] += cos(attck_angle*M_PI/180)*time*velocity;
        pos[1] -= sin(attck_angle*M_PI/180)*time*velocity;
    }
    void Particle::collide(double cllsn){
        double scalar = cos(attck_angle*M_PI/180)*cos(cllsn*M_PI/180)+sin(attck_angle*M_PI/180)*sin(cllsn*M_PI/180);
        attck_angle = atan2(
            sin(attck_angle*M_PI/180)-sin(cllsn*M_PI/180)*2*scalar,
            cos(attck_angle*M_PI/180)-cos(cllsn*M_PI/180)*2*scalar
        )/M_PI*180;
    }
}
