#include "GameEngine.h"
#include <chrono>

GameEngine::GameEngine(unsigned int w, unsigned int h)
    : window(sf::VideoMode({ w, h }), "Automata Explorer")
{
    gridMap = std::make_unique<lifeGrid>(w / 10, h / 10);

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
    rules.push_back(std::make_unique<StarWarsRule>());
	rules.push_back(std::make_unique<GreenbergHastingsRule>());
    rules.push_back(std::make_unique<WireWorldRule>());

    neighbourhood = gridMap->isMoore() ? "Moore" : "Neumann";
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
        gridMap->setCell(mPos.x / (int)cellSize, mPos.y / (int)cellSize, currentInput);
        if (state == SimState::Empty) state = SimState::Paused;
    }
}
void GameEngine::renderHelper(float dt) {
    if (ImGui::BeginCombo("Rule", rules[currentRuleIndex]->getName().c_str())) { //rule dropdown
        for (int i = 0; i < (int)rules.size(); i++) {
            if (ImGui::Selectable(rules[i]->getName().c_str(), currentRuleIndex == i)) {
                std::string ruleName = rules[i]->getName();

                if (ruleName == "Nagel–Schreckenberg traffic Model" && currentRuleIndex != i) {
                    gridMap = std::make_unique<NagelSchreckenbergGrid>(window.getSize().x / 10, window.getSize().y / 10);
                }
                else if (ruleName == "Biham–Middleton–Levine traffic Model" && currentRuleIndex != i) {
                    gridMap = std::make_unique<BihamMiddletonLevineGrid>(window.getSize().x / 10, window.getSize().y / 10);
                    
                }
                else if (ruleName == "Diffusion-Limited-Aggregation" && currentRuleIndex != i) {
                    gridMap = std::make_unique<BrownianGrid>(window.getSize().x / 10, window.getSize().y / 10);
                }
                currentRuleIndex = i;
                gridMap->setHood(rules[currentRuleIndex]->getDefaultHood());
                // userColour = rules[currentRuleIndex]->getDefaultColour();
            }
        }
        neighbourhood = gridMap->isMoore() ? "Moore" : "Neumann";

        ImGui::EndCombo();
    }

    const char* label = (state == SimState::Running) ? "Pause" : "Start";
    if (ImGui::Button(label, ImVec2(100, 0))) {
        if (state == SimState::Empty) {
            gridMap->populate(popValue);
            state = SimState::Running;
        }
        else {
            state = (state == SimState::Running) ? SimState::Paused : SimState::Running;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        gridMap->update(rules[currentRuleIndex].get());
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset", ImVec2(100, 0))) {
        gridMap->clear();
        state = SimState::Empty;
    }
    ImGui::SliderFloat("fill ratio", &popValue, 0.0f, 1.0f);
    ImGui::SliderFloat("Speed", &speed, 0.0f, 0.5f);
    ImGui::SliderInt("Input", &currentInput, 0, rules[currentRuleIndex].get()->getMaxState());


}
void  GameEngine::render2DUI(float dt) {
	renderHelper(dt);
    if (ImGui::Button("invert")) {
        gridMap->invert();
    }
    if (ImGui::Button("wrapping")) {
        gridMap->toggleWrapping();
    }


    if (ImGui::Button(neighbourhood)) {
        gridMap->toggleHood();
        neighbourhood = gridMap->isMoore() ? "Moore" : "Neumann";
    }

    
    //ImGui::SliderInt("width", width, 0, 150);
  //  ImGui::SliderInt("height", width, 0, 150);
    ImGui::ColorEdit3("Primary Color", &(userColour[0]));
    static float values[90] = { 0 };
    static int values_offset = 0;

    // Update the array with the latest raw value
    values[values_offset] = (float)lastUpdateMs;
    values_offset = (values_offset + 1) % 90;

    ImGui::Text("Raw: %.3f ms", lastUpdateMs);
    ImGui::Text("Avg: %.3f ms", smoothedUpdateMs);

    // This graph will show you the "jitter" visually
    ImGui::PlotLines("Jitter", values, 90, values_offset, "ms", 0.0f, 1.0f, ImVec2(0, 80));

}
void  GameEngine::render1DUI(float dt) {
    return;
}
void  GameEngine::renderOdd2DUI(float dt) {
    renderHelper(dt);
    if (ImGui::Button("car")) {
        if (currentInput == 1)
            currentInput = 2;
        else
            currentInput = 1;
    }
    
    
}
void GameEngine::renderUI(float dt) {
    ImGui::SFML::Update(window, sf::seconds(dt));
    ImGui::Begin("Main Menu");
    if (ImGui::BeginCombo("Automata Type", getTypeName(currentType).c_str())) {
        if (ImGui::Selectable("Life-like (2D)", currentType == automataMenu::LifeLike)) {
            changeMode(automataMenu::LifeLike);
        }
        if (ImGui::Selectable("Odd 2D", currentType == automataMenu::Odd2D)) {
            changeMode(automataMenu::Odd2D);
        }
        ImGui::EndCombo();
    }
    ImGui::Separator();

    // switch case for menu
    switch (currentType) {
    case automataMenu::LifeLike:
        render2DUI(dt); // Move your current UI code here
        break;
    case automataMenu::OneDimension:
        render1DUI(dt); // New UI for 1D rules
        break;

    case automataMenu::Odd2D:
        renderOdd2DUI(dt);
       
        break;

    }
    ImGui::End();
}
void GameEngine::changeMode(automataMenu a) {
    if (currentType == a) return;
    currentType = a;
    state = SimState::Empty;
    gridMap->clear();
    rules.clear();
    if (currentType == automataMenu::LifeLike) {
        gridMap = std::make_unique<lifeGrid>(window.getSize().x / 10, window.getSize().y / 10);
        rules.push_back(std::make_unique<ConwayRule>()); //allocate memory with uniqueptr
        rules.push_back(std::make_unique<SeedsRule>());
        rules.push_back(std::make_unique<DayAndNightRule>());
        rules.push_back(std::make_unique<LifeWithoutDeathRule>());
        rules.push_back(std::make_unique<BriansBrainRule>());
        rules.push_back(std::make_unique<UlamWarburtonRule>());
        rules.push_back(std::make_unique<MazectricRule>());
        rules.push_back(std::make_unique<IceballsRule>());
        rules.push_back(std::make_unique<StarWarsRule>());
        rules.push_back(std::make_unique<GreenbergHastingsRule>());
        rules.push_back(std::make_unique<WireWorldRule>());
        
    }
    if (currentType == automataMenu::Odd2D) {
        gridMap = std::make_unique<BihamMiddletonLevineGrid>(window.getSize().x / 10, window.getSize().y / 10);
        rules.push_back(std::make_unique<BihamMiddletonLevineTrafficRule>());
        rules.push_back(std::make_unique<NagelSchreckenbergTrafficRule>());
        rules.push_back(std::make_unique<DiffusionLimitedAggregationRule>());

    }

    neighbourhood = gridMap->isMoore() ? "Moore" : "Neumann";
}


void GameEngine::renderGrid() {
    int width = gridMap->getWidth();
    int height = gridMap->getHeight();

    gridVertices.resize(width * height * 6);
    size_t vertexIndex = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t cellState = gridMap->getCellState(x, y);

            if (cellState > 0) {
                
                float left = x * cellSize;
                float top = y * cellSize;
                float right = left + cellSize - 1.0f;
                float bottom = top + cellSize - 1.0f;

                
                auto rgb = rules[currentRuleIndex]->getDefaultColour(cellState);
                sf::Color color(
                    static_cast<uint8_t>(rgb[0] * 255.0f),
                    static_cast<uint8_t>(rgb[1] * 255.0f),
                    static_cast<uint8_t>(rgb[2] * 255.0f)
                );

                // triangle 1
                gridVertices[vertexIndex++].position = { left, top };
                gridVertices[vertexIndex - 1].color = color;

                gridVertices[vertexIndex++].position = { right, top };
                gridVertices[vertexIndex - 1].color = color;

                gridVertices[vertexIndex++].position = { left, bottom };
                gridVertices[vertexIndex - 1].color = color;

                // triangle 2
                gridVertices[vertexIndex++].position = { left, bottom };
                gridVertices[vertexIndex - 1].color = color;

                gridVertices[vertexIndex++].position = { right, top };
                gridVertices[vertexIndex - 1].color = color;

                gridVertices[vertexIndex++].position = { right, bottom };
                gridVertices[vertexIndex - 1].color = color;
            }
        }
    }

    //draw them all 
    if (vertexIndex > 0) {
        window.draw(&gridVertices[0], vertexIndex, sf::PrimitiveType::Triangles);
    }
}

void GameEngine::run() {
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        handleEvents();
        
        if (state == SimState::Running) {
            timer += dt;
            if (timer >= speed) {
                
                gridMap->update(rules[currentRuleIndex].get());
                

                timer = 0;
            }
        }

        renderUI(dt);
        
        window.clear(sf::Color(0, 0, 0));
        auto start = std::chrono::high_resolution_clock::now();
        renderGrid();
        
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end - start;
        lastUpdateMs = elapsed.count();


        smoothedUpdateMs = (smoothedUpdateMs * 0.9) + (lastUpdateMs * 0.1);
        ImGui::SFML::Render(window);
        window.display();
    }
}