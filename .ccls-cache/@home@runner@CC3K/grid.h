#ifndef __GRID_H_
#define __GRID_H_
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include "coordinate.h"
#include "subject.h"

class Cell;
class TextDisplay;

class Grid: public Subject {
    std::vector<std::vector<Cell*>> theGrid;
    std::vector<std::vector<Coordinate>> chambers;
    int h, w;
    void addChamber(std::vector<std::vector<Cell*>> &tempGrid, Coordinate c, std::vector<Coordinate>& tempChamber);
    std::vector<Coordinate> countNeighbour(Coordinate& cdn);

    Coordinate PCLocation;
    TextDisplay* td;
	unsigned seed;
	int gameDiffLevel;
	// 0: easy mode (more gold, NPC 1/2 HP)
	// 1: medium mode (normal mode)
	// 2: hard mode (less gold, NPC 1.5X Atk) (to be implemented)
public:
    Grid(std::string fileName, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(), char PCName = 'h', bool barrierSuit = false);
    void init(char c);
    Coordinate& getPCLocation() const;
    void updatePlayer();
    void updateGrid();
    bool canMoveTo(Coordinate& cdn);
    bool canMoveToNPC(Coordinate& cdn);
    bool moveTo(Coordinate& newCdn);
    void usePotion(Coordinate& cdn);
    void PCAttack(Coordinate& cdn);
    ~Grid();
    int randomInt(int x); // returns a random number between 0 and x-1

};

#endif