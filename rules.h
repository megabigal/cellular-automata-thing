#pragma once
#include <string>
#include <cstdint>
#include <array>

// big daddy class
class basicAutomataRule {
public:
    virtual ~basicAutomataRule() = default; //deconstructor
    virtual std::string getName() const = 0; //basic definitions for methods, getname and apply required
    virtual std::string getDefaultHood() const { return "Moore"; }
    virtual uint8_t apply(uint8_t currentState, int neighbours) = 0;
    virtual std::array<float, 3> getDefaultColour(uint8_t state)  {
        switch (state) {
        case 1: return { 0.0f, 1.0f, 1.0f };
        default: return { 0.0,0.0f,1.0f };
        }

    }
    virtual int getMaxState() const { return 1; }
    
};
//conways
class ConwayRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
//seeds
class SeedsRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
//day and night
class DayAndNightRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
};
//life without death
class LifeWithoutDeathRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
//brians brain
class BriansBrainRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;
};
//ulam-warburton
class UlamWarburtonRule : public basicAutomataRule {
public:
    std::string getName() const override;
    std::string getDefaultHood() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
//mazectric
class MazectricRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
//ice balls
class IceballsRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
};
//star wars
class StarWarsRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;
};
//Greenberg Hastings
class GreenbergHastingsRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::string getDefaultHood() const override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;
};
//biham-middleton-levine traffic
class BihamMiddletonLevineTrafficRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;

};
//nagel-schreckenberg
class NagelSchreckenbergTrafficRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;

};
//wireworld
class WireWorldRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;

};
//diffusion limited aggregation
class DiffusionLimitedAggregationRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
    std::array<float, 3> getDefaultColour(uint8_t state) override;
    int getMaxState() const override;

};