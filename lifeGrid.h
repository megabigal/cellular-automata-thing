#ifndef LIFEGRID_H
#define LIFEGRID_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "rules.h" 
#include <iostream>
class lifeGrid {
protected:
    int width, height;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> next; //holds the next iteration
    bool useMoore = true;
    bool wrapping = true;
public:
    
    lifeGrid(int w, int h);
    virtual ~lifeGrid() = default;
    bool isMoore() const { return useMoore; }
    void toggleHood() { useMoore = !useMoore; }
    void setHood(std::string hood);
    void toggleWrapping() { wrapping = !wrapping; }

     int getWidth() const { return width; }
    int getHeight() const { return height; }
    inline int getIndex(int x, int y) const { return y * width + x; }

    virtual uint8_t getCellState(int x, int y) const;
    virtual uint8_t getCellStateWrapping(int x, int y) const;
    virtual void setCell(int x, int y, uint8_t v);
    virtual void clear();

    int countNeighboursVonNeumann(int x, int y) const;
    virtual int countNeighbours(int x, int y) const;
    virtual void update(basicAutomataRule* currentRule);

    virtual void populate(float perc);
    virtual void invert();
};
class BihamMiddletonLevineGrid : public lifeGrid {
public:
    BihamMiddletonLevineGrid(int w, int h) : lifeGrid(w, h) {}
    void update(basicAutomataRule* currentRule) override;
    void calcNext(int x, int y, bool goingRight);
    void populate(float perc) override;
};
#endif
class NagelSchreckenbergGrid : public lifeGrid {
private:
    //uint8_t velocity = 1;
	int maxVelocity = 5;
    float p = 0.5f;
public:
    NagelSchreckenbergGrid(int w, int h) : lifeGrid(w, h) {}
    void update(basicAutomataRule* currentRule) override;
    void calcNext(int x, int y);
    void setMaxVelocity(int maxV);
};