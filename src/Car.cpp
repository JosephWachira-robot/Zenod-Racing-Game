#include "Car.h"
Car::Car(const std::string& name, bool isPlayer) : name(name), lane(3), speed(0), position(0), isPlayer(isPlayer) {}
std::string Car::getName() const { return name; }
int Car::getLane() const { return lane; }
int Car::getSpeed() const { return speed; }
int Car::getPosition() const { return position; }
bool Car::getIsPlayer() const { return isPlayer; }
void Car::setLane(int newLane) { if(newLane>=1 && newLane<=5) lane=newLane; }
void Car::setSpeed(int newSpeed){ if(newSpeed>=0) speed=newSpeed; }
void Car::move(){ position += speed/10; }