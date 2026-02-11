#pragma once
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "lifeGrid.h"
#include <memory>
#include <ctime>
#include <vector>

enum class SimState { Empty, Running, Paused }; //yummy state machine

class GameEngine {
private:
    sf::RenderWindow window;
    lifeGrid gridMap;
    sf::Clock deltaClock;
    SimState state = SimState::Empty;
    const char* neighbourhood;

    float speed = 0.1f;
    float timer = 0.0f;
    float popValue = 0.2f;
    const float cellSize = 10.0f;
   // int* width = &gridMap.getWidth();
   // int* height = &gridMap.getHeight();

    std::vector<std::unique_ptr<basicAutomataRule>> rules;
    int currentRuleIndex = 0;

    // Helper functions for the split logic
    void handleEvents();
    void renderUI(float dt);
    void renderGrid();

public:
    GameEngine(unsigned int w, unsigned int h);
    ~GameEngine();

    void run();
};