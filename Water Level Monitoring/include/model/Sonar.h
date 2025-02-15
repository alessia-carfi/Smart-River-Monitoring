#ifndef __SONAR__
#define __SONAR__

const float VS = 343.0;
const int MAXDIST = 20; // da cambiare
const int MINDIST = 5;  // da cambiare
const float SOUND_SPEED = 331.5 + 0.6 * 20;

class Sonar
{
private:
    int echo;
    int trig;

public:
    Sonar(int echo, int trig);
    bool isOk();
    float getDistance();
};

#endif
