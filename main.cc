#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include "grid.h"
#include "termcodes.h"
using namespace std;

Coordinate convertCdn(const Coordinate& oldCdn, string direction) {
     if (direction == "no") {
        return Coordinate{oldCdn.x, oldCdn.y - 1};
     } else if (direction == "so") {
        return Coordinate{oldCdn.x, oldCdn.y + 1};
     } else if (direction == "ea") {
        return Coordinate{oldCdn.x - 1, oldCdn.y};
     } else if (direction == "we") {
        return Coordinate{oldCdn.x + 1, oldCdn.y};
     } else if (direction == "ne") {
        return Coordinate{oldCdn.x - 1, oldCdn.y + 1};
     } else if (direction == "nw") {
        return Coordinate{oldCdn.x - 1, oldCdn.y - 1};
     } else if (direction == "se") {
        return Coordinate{oldCdn.x + 1, oldCdn.y + 1};
     } else if (direction == "sw") {
        return Coordinate{oldCdn.x + 1, oldCdn.y - 1};
     }
}




int main(int argc, char* argv[]) {
    std::cout << "WELCOME TO THE GAME OF CHAMBERCRAWLER3000+!" << std::endl;
    std::cout << "Please select your PC character:" << std::endl;
    std::cout << ">>> Options:" << std::endl;
    std::cout << "(1) Human" << YELLOW << "(h)" << RESET << ": HP:140  Attack:20  Defence:20" << std::endl;
    std::cout << "(2) Dwarf" << YELLOW << "(d)" << RESET << ": HP:100  Attack:20  Defence:30    <gold X2>" << std::endl;
    std::cout << "(3) Elves" << YELLOW << "(e)" << RESET << ": HP:140  Attack:30  Defence:10    <negative potions have positive effects>" << std::endl;
    std::cout << "(4) Orc  " << YELLOW << "(o)" << RESET << ": HP:180  Attack:30  Defence:25    <gold X0.5>" << std::endl;
    std::cout << "Please enter 'h', 'd', 'e', or 'o'" << std::endl;
    char pc;
    try {
        // Player character selection
        cin >> pc;
        Grid* g;
        unsigned defaultSeed = std::chrono::system_clock::now().time_since_epoch().count();
        if (argc == 3) {
            string s = argv[2];
            istringstream iss{s};
            unsigned seed;
            iss >> seed;
            g = new Grid{argv[1], seed, pc};

        } else if (argc == 2) {
            g = new Grid{argv[1], defaultSeed, pc};
        } else {
            g = new Grid{"defaultFloor.txt", defaultSeed, pc};
        }
        // Game starts
        char cmd;
        while (cin >> cmd) {
            if (cmd == 'n' || cmd == 's' || cmd == 'e' || cmd == 'w') {
                string direction = "";
                direction += cmd;
                cin >> cmd;
                direction += cmd;
                Coordinate destination = convertCdn(g->getPCLocation(), direction);
                if (g->canMoveTo(destination)) {
                    // TO BE CONTINUED
                }
            }
            

        }




        delete g;

    } catch ( ... ) {
        return 1;
    }




}



