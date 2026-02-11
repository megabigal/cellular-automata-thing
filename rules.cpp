#include "rules.h"

// conways
std::string ConwayRule::getName() const {
    return "Conway's Game of Life";
}

uint8_t ConwayRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        return (neighbours == 2 || neighbours == 3) ? 1 : 0;
    }
    return (neighbours == 3) ? 1 : 0;
}
//seeds
std::string SeedsRule::getName() const {
    return "Seeds";
}

uint8_t SeedsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 2) return 1;
    return 0;
}

// day and night
std::string DayAndNightRule::getName() const {
    return "Day And Night";
}

uint8_t DayAndNightRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        switch (neighbours) {
        case 3: case 4: case 6: case 7: case 8: return 1;
        default: return 0;
        }
    }
    switch (neighbours) {
    case 3: case 6: case 7: case 8: return 1;
    default: return 0;
    }
}
//life without death
std::string LifeWithoutDeathRule::getName() const { return "Life Without Death"; }
uint8_t LifeWithoutDeathRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 3) return 1;
    if (currentState == 1) return 1;
    return 0;
}
// brians brain
std::string BriansBrainRule::getName() const {
    return "Brian's Brain";
}

uint8_t BriansBrainRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 2) return 1;
    if (currentState == 1) return 2;//dying
    return 0;
}

// ulam-wuarburton
std::string UlamWarburtonRule::getName() const {
    return "Ulam-Warburton";
}

std::string UlamWarburtonRule::getDefaultHood() const {
    return "Neumann";
}

uint8_t UlamWarburtonRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) return 1;
    if (currentState == 0 && neighbours == 1) return 1;
    return 0;
    
}
//mazectric
std::string MazectricRule::getName() const {
    return "Mazectric";
}

uint8_t MazectricRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1 && neighbours > 0 && neighbours < 5) return 1;
    if (currentState == 0 && neighbours == 3) return 1;
    return 0;
}
//Iceballs
std::string IceballsRule::getName() const {
    return "Iceballs";
}
uint8_t IceballsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        switch (neighbours) {
        case 5: case 6: case 7: case 8: return 1;
        default: return 0;
        }
    }
    switch (neighbours) {
    case 2: case 5: case 6: case 7: case 8: return 1;
    default: return 0;
    }
}