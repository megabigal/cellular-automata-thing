#pragma once 
#include <string>
#include <cstdint>

class basicAutomataRule { //the daddy class
public:
    virtual ~basicAutomataRule() = default;//deconstructor
    virtual std::string getName() const = 0; 
    virtual uint8_t apply(uint8_t currentState, int neighbours) = 0;
};

class ConwayRule : public basicAutomataRule {
public:
    std::string getName() const override { return "Conway's Game of Life"; }
    uint8_t apply(uint8_t currentState, int neighbours) override {
        if (currentState == 1) {
            return (neighbours == 2 || neighbours == 3) ? 1 : 0;
        }
        return (neighbours == 3) ? 1 : 0;
    }
};
class SeedsRule : public basicAutomataRule {
public:
    std::string getName() const override { return "Seeds"; }
    uint8_t apply(uint8_t currentState, int neighbours) override {
        if (currentState == 0 && neighbours == 2) return 1;
        return 0;
    }
}; 
class DayAndNightRule : public basicAutomataRule {
public:
    std::string getName() const override { return "Day And Night"; }
    uint8_t apply(uint8_t currentState, int neighbours) override {
        if (currentState == 1) {
            return (neighbours == 3 || neighbours == 4 || neighbours == 6 || neighbours == 7 || neighbours == 8) ? 1 : 0;
            //make this better IMPORTANT
        }
        return (neighbours == 3 || neighbours == 6 || neighbours == 7 || neighbours == 8) ? 1 : 0;
    }
};
class LifeWithoutDeathRule : public basicAutomataRule {
public:
    std::string getName() const override { return "Life Without Death"; }
    uint8_t apply(uint8_t currentState, int neighbours) override {
        if (currentState == 0 && neighbours == 3) return 1;
        if (currentState == 1) return 1;
        return 0;
    }
};
class BriansBrainRule : public basicAutomataRule {
public:
    std::string getName() const override { return "Brians Brain"; }
    uint8_t apply(uint8_t currentState, int neighbours) override {
        if (currentState == 0 && neighbours == 2) return 1;
        if (currentState == 1) return 2;
        return 0;
    }
};