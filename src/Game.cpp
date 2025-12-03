#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

Game::Game(int lanes,int trackLength,int fps) : lanes(lanes),trackLength(trackLength),fps(fps),running(false){
    std::srand(std::time(nullptr));
}
Game::~Game(){ delete player; }
void Game::init(){ spawnPlayer(); spawnAI(3); running=true; }
void Game::spawnPlayer(){ player = new Car("Player",true); player->setLane(lanes/2+1); player->setSpeed(100); }
void Game::spawnAI(int numAI){ for(int i=0;i<numAI;++i){ Car ai("AI"+std::to_string(i+1)); ai.setLane(1+std::rand()%lanes); ai.setSpeed(80+std::rand()%40); aiCars.push_back(ai); } }
void Game::run(){
    while(running){
        update(); render();
        if(checkCollision()){ std::cout<<"
Collision! Game Over.
"; running=false; break; }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }
    std::cout<<"Race Finished!
";
}
void Game::update(){ player->move(); for(auto &ai:aiCars) ai.move(); }
void Game::render(){
    std::cout<<"
----- Zenod Racing -----
";
    std::cout<<"Player lane: "<<player->getLane()<<" | Position: "<<player->getPosition()<<" km
";
    for(auto &ai:aiCars){ std::cout<<ai.getName()<<" lane: "<<ai.getLane()<<" | Position: "<<ai.getPosition()<<" km
"; }
}
bool Game::checkCollision(){ for(auto &ai:aiCars){ if(ai.getLane()==player->getLane() && abs(ai.getPosition()-player->getPosition())<5) return true; } return false; }