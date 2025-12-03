#ifndef CAR_H
#define CAR_H
#include <string>

class Car {
private:
    std::string name;
    int lane;
    int speed;
    int position;
    bool isPlayer;
public:
    Car(const std::string& name, bool isPlayer=false);
    std::string getName() const;
    int getLane() const;
    int getSpeed() const;
    int getPosition() const;
    bool getIsPlayer() const;
    void setLane(int newLane);
    void setSpeed(int newSpeed);
    void move();
};
#endif