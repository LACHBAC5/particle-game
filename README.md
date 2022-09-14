# Particle game

**purpose:** Implementing some collision physics for circles.

The visuals are mostly for debugging and there is no real ui.

### compilation 

Set up dependencies -> [stdOpenGL](https://github.com/LACHBAC5/stdOpenGL)

Compile main.cpp -> make stdOpenGL=*path-to-stdOpenGL*

### some reminder

Object position is updated every n seconds.

Firstly the closest collision is found with result y in seconds.

If y < n then the positions are updated with y seconds and a new

angle of attack is calculated for the circles involved.

Repeat until y >= n.

**How is the closest collision (timewise) caculated?**

For each two seperate circles a formula is used where t(time) is the end result.

For each collision t can be t>=0 or t<=0 where a collision will happen if we go backwards.

The formula is derived from ->

pow(f.r+s.r,2) = pow(f.x+cos(f.attck_angle)\*f.velocity\*time)-(s.x+cos(s.attck_angle)\*s.velocity\*time), 2) + pow(f.y-sin(f.attck_angle)\*f.velocity\*time)-(s.x-sin(s.attck_angle)\*s.velocity\*time, 2)

or.. delta_pos^2 = (f.change_of_x_per_second - s.change_of_x_per_second)^2 + (f.change_of_y_per_second - s.change_of_y_per_second)^2

...then solve for time

**How is a collision processed?**

An attack angle vector is constructed with beginning at (0,0)

A collision angle vector is constructed from the tip of the attack angle vector

with the direction of the collision angle.

Then a perpendicular is constructed from the beginning of the attack angle vector

to the collision angle vector (which represents a line in this case).

Then the the collision angle vector length is recalculated as 2 * the length of the line

from the perpendicular to the beginning of the collision angle vector (named later scalar).

Finally the attack angle is assigned to be a vector from (0,0) to the tip of the collision angle vector.

There is a formula for everything ->

Firstly a dot product is calculated. The [dot product](https://en.wikipedia.org/wiki/Dot_product) of two vectors results in

the cosine of the angle between them.

scalar = cos(attck_angle\*M_PI/180)\*cos(cllsn\*M_PI/180)+sin(attck_angle*\M_PI/180)\*sin(cllsn\*M_PI/180);

The formula for dot product comes from the [law of cosines](https://en.wikipedia.org/wiki/Law_of_cosines) applied to vectors.

attck_angle = atan2(

    sin(attck_angle*M_PI/180)-sin(cllsn*M_PI/180)\*2\*scalar,

    cos(attck_angle*M_PI/180)-cos(cllsn*M_PI/180)\*2\*scalar

)/M_PI*180;

Then sin(new_attck_angle) is equal to sin(attck_angle) which is "moved" by sin(cllsn_angle) scaled by the 2*scalar.

Then cos(new_attck_angle) is equal to cos(attck_angle) which is "moved" by cos(cllsn_angle) scaled by the 2*scalar.

### problems

Border collision is broken and sometimes has objects escaping. (likely a precision problem though)
