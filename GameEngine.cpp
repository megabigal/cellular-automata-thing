#include "GameEngine.h"

GameEngine::GameEngine(unsigned int w, unsigned int h)
    : window(sf::VideoMode({ w, h }), "Automata Explorer"), gridMap(w / 10, h / 10)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); //seed
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    rules.push_back(std::make_unique<ConwayRule>()); //allocate memory with uniqueptr
    rules.push_back(std::make_unique<SeedsRule>());
    rules.push_back(std::make_unique<DayAndNightRule>());
    rules.push_back(std::make_unique<LifeWithoutDeathRule>());
    rules.push_back(std::make_unique<BriansBrainRule>());
    rules.push_back(std::make_unique<UlamWarburtonRule>());
    rules.push_back(std::make_unique<MazectricRule>());
    rules.push_back(std::make_unique<IceballsRule>());

    neighbourhood = gridMap.isMoore() ? "Moore" : "Neumann";
}

GameEngine::~GameEngine() {
    ImGui::SFML::Shutdown();
}

void GameEngine::handleEvents() {
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
}

void GameEngine::renderUI(float dt) {
    ImGui::SFML::Update(window, sf::seconds(dt));
    ImGui::Begin("Simulation Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginCombo("Rule", rules[currentRuleIndex]->getName().c_str())) { //rule dropdown
        for (int i = 0; i < (int)rules.size(); i++) {
            if (ImGui::Selectable(rules[i]->getName().c_str(), currentRuleIndex == i)) {
                currentRuleIndex = i;
                gridMap.setHood(rules[currentRuleIndex]->getDefaultHood());
            }
        }
        neighbourhood = gridMap.isMoore() ? "Moore" : "Neumann";
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
    if (ImGui::Button("wrapping")) {
        gridMap.toggleWrapping();
    }
    if (ImGui::Button(neighbourhood)) {
        gridMap.toggleHood();
        neighbourhood = gridMap.isMoore() ? "Moore" : "Neumann";
    }

    ImGui::SliderFloat("Speed", &speed, 0.0f, 0.5f);
    ImGui::SliderFloat("fill ratio", &popValue, 0.0f, 1.0f);
    //ImGui::SliderInt("width", width, 0, 150);
  //  ImGui::SliderInt("height", width, 0, 150);
    ImGui::End();
}

void GameEngine::renderGrid() {
    sf::RectangleShape shape(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));

    for (int y = 0; y < gridMap.getHeight(); y++) {
        for (int x = 0; x < gridMap.getWidth(); x++) {
            uint8_t cellState = gridMap.getCellState(x, y);

            if (cellState > 0) {
                shape.setPosition({ x * cellSize, y * cellSize });
                if (cellState == 1) {
                    shape.setFillColor(sf::Color::Cyan); // Alive
                }
                else if (cellState == 2) {
                    shape.setFillColor(sf::Color::Blue); // Dying
                }
                window.draw(shape);
            }
        }
    }
}

void GameEngine::run() {
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        handleEvents();

        if (state == SimState::Running) {
            timer += dt;
            if (timer >= speed) {
                gridMap.update(rules[currentRuleIndex].get());
                timer = 0;
            }
        }

        renderUI(dt);

        window.clear(sf::Color(30, 30, 30));
        renderGrid();
        ImGui::SFML::Render(window);
        window.display();
    }
}