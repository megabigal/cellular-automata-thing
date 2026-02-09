#pragma once
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "lifeGrid.h"
#include <memory>
#include <ctime>

enum class SimState { Empty, Running, Paused };//yummy state machine

class GameEngine {
private:
    sf::RenderWindow window;
    lifeGrid gridMap;
    sf::Clock deltaClock;
    SimState state = SimState::Empty;

    float speed = 0.1f;
    float timer = 0.0f;
    float popValue = 0.2f;
    const float cellSize = 10.0f;

    std::vector<std::unique_ptr<basicAutomataRule>> rules;
    int currentRuleIndex = 0;

public:
    GameEngine(unsigned int w, unsigned int h)
        : window(sf::VideoMode({ w, h }), "Automata Explorer"), gridMap(w / 10, h / 10)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));//seed
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window);

        rules.push_back(std::make_unique<ConwayRule>());//allocate memory with uniqueptr
        rules.push_back(std::make_unique<SeedsRule>());
        rules.push_back(std::make_unique<DayAndNightRule>());
        rules.push_back(std::make_unique<LifeWithoutDeathRule>());
        rules.push_back(std::make_unique<BriansBrainRule>());
    }

    ~GameEngine() { ImGui::SFML::Shutdown(); }

    void run() {
        while (window.isOpen()) {
            sf::Time dt = deltaClock.restart();

            // handling
            while (const std::optional event = window.pollEvent()) {
                ImGui::SFML::ProcessEvent(window, *event); //closing the window
                if (event->is<sf::Event::Closed>()) window.close();
            }

            // input
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !ImGui::GetIO().WantCaptureMouse) {
                sf::Vector2i mPos = sf::Mouse::getPosition(window); //lets me draw :D
                gridMap.setCell(mPos.x / (int)cellSize, mPos.y / (int)cellSize, 1);
                if (state == SimState::Empty) state = SimState::Paused;
            }

            // ui
            ImGui::SFML::Update(window, dt);
            ImGui::Begin("Simulation Controls",nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::BeginCombo("Rule", rules[currentRuleIndex]->getName().c_str())) { //rule dropdown
                for (int i = 0; i < rules.size(); i++) {
                    if (ImGui::Selectable(rules[i]->getName().c_str(), currentRuleIndex == i))
                        currentRuleIndex = i;
                }
                ImGui::EndCombo();
            }

            const char* label = (state == SimState::Running) ? "Pause" : "Start";
            if (ImGui::Button(label, ImVec2(100, 0))) {
                if (state == SimState::Empty) {
                    gridMap.populate(popValue);
                    state = SimState::Running;
                }
                else {
                    state = (state == SimState::Running) ? SimState::Paused : SimState::Running;
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset", ImVec2(100, 0))) {
                gridMap.clear();
                state = SimState::Empty;
            }
            if (ImGui::Button("invert")) {
                gridMap.invert();
                
            }

            ImGui::SliderFloat("Speed", &speed, 0.01f, 0.5f);
            ImGui::SliderFloat("fill ratio", &popValue, 0.0f, 1.0f);
            ImGui::End();

            if (state == SimState::Running) {
                timer += dt.asSeconds();
                if (timer >= speed) {
                    gridMap.update(rules[currentRuleIndex].get());
                    timer = 0;
                }
            }

            // Render
            window.clear(sf::Color(30, 30, 30));
            sf::RectangleShape shape(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));
            shape.setFillColor(sf::Color::Cyan);

            for (int y = 0; y < gridMap.getHeight(); y++) {
                for (int x = 0; x < gridMap.getWidth(); x++) {
                    uint8_t state = gridMap.getCellState(x, y);

                    if (state > 0) {
                        shape.setPosition({ x * cellSize, y * cellSize });
                        if (state == 1) {
                            shape.setFillColor(sf::Color::Cyan); // Alive
                        }
                        else if (state == 2) {
                            shape.setFillColor(sf::Color::Blue); // Dying
                        }
                        window.draw(shape);
                    }
                }
            }
            ImGui::SFML::Render(window);
            window.display();
        }
    }
};