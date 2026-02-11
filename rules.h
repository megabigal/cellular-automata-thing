#pragma once
#include <string>
#include <cstdint>

// big daddy class
class basicAutomataRule {
public:
    virtual ~basicAutomataRule() = default; //deconstructor
    virtual std::string getName() const = 0; //basic definitions for methods, getname and apply required
    virtual std::string getDefaultHood() const { return "Moore"; }
    virtual uint8_t apply(uint8_t currentState, int neighbours) = 0;
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
};
//ulam-warburton
class UlamWarburtonRule : public basicAutomataRule {
public:
    std::string getName() const override;
    std::string getDefaultHood() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
class MazectricRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};
class IceballsRule : public basicAutomataRule {
public:
    std::string getName() const override;
    uint8_t apply(uint8_t currentState, int neighbours) override;
};