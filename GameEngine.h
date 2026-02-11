#pragma once
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "lifeGrid.h"
#include <memory>
#include <ctime>
#include <vector>

#include <string>
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
    std::array<float, 3>  userColour = { 0.0f,1.0f,1.0f };
   // int* width = &gridMap.getWidth();
   // int* height = &gridMap.getHeight();
    enum class automataMenu {LifeLike, OneDimension, Odd2D};
    automataMenu currentType = automataMenu::LifeLike;
    std::vector<std::unique_ptr<basicAutomataRule>> rules;
    int currentRuleIndex = 0;

    // Helper functions for the split logic
    void handleEvents();
    void changeMode(automataMenu a);
    void renderUI(float dt);
    void renderGrid();
    void render2DUI(float dt);
    void renderOdd2DUI(float dt);
    void render1DUI(float dt);
    std::string getTypeName(automataMenu current) {
        switch (current) {
        case automataMenu::LifeLike: return "LifeLike";
        case automataMenu::OneDimension: return "OneDimension";
        case automataMenu::Odd2D: return "Odd2D";
        }
    }
public:
    GameEngine(unsigned int w, unsigned int h);
    ~GameEngine();

    void run();
};