#ifndef GAME_H
#define GAME_H
#include <vector>
#include "Car.h"
class Game {
private:
    int lanes;
    int trackLength;
    int fps;
    bool running;
    Car* player;
    std::vector<Car> aiCars;
public:
    Game(int lanes=5, int trackLength=1000, int fps=10);
    ~Game();
    void init();
    void spawnPlayer();
    void spawnAI(int numAI);
    void run();
    void update();
    void render();
    bool checkCollision();
};
#endif