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
    std::vector<std::vector<Cell*>> backupGrid;
    std::map<Coordinate, int> allFloors;
    Coordinate PCLocation;
    TextDisplay* td;
public:
    Grid(std::string fileName, char PCName = 'h', unsigned seed = std::chrono::system_clock::now().time_since_epoch().count());
    void init(char c);
    void updatePlayer();
    void updateGrid();
    bool canMoveTo(Coordinate& cdn);
    void moveTo(Coordinate& oldCdn, Coordinate& newCdn);
    void usePotion(Coordinate& cdn);
    void PCAttack(Coordinate& cdn);
    ~Grid();

};

#endif
