#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include "grid.h"
#include "termcodes.h"
#include "pc.h"
#include "info.h"
#include "coordinate.h"
using namespace std;

// Helper
void printIntroMsg() {
	std::cout << "Please select your PC character:" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "(1) Human" << YELLOW << "(h)" << RESET << ": HP:140  Attack:20  Defence:20" << std::endl;
    std::cout << "(2) Dwarf" << YELLOW << "(d)" << RESET << ": HP:100  Attack:20  Defence:30    <gold X2>" << std::endl;
    std::cout << "(3) Elves" << YELLOW << "(e)" << RESET << ": HP:140  Attack:30  Defence:10    <negative potions have positive effects>" << std::endl;
    std::cout << "(4) Orc  " << YELLOW << "(o)" << RESET << ": HP:180  Attack:30  Defence:25    <gold X0.5>" << std::endl;
    std::cout << ">>> Please enter 'h', 'd', 'e', or 'o'" << std::endl;
}


void printMaps(vector<vector<string>>& maps) {
	for (size_t i = 0; i < maps.size(); i++) {
		std::cout << GREEN << "Floor: " << i + 1 << RESET << std::endl;
		std::cout << "Size: " << maps[i].size() << std::endl;
		for (size_t j = 0; j < maps[i].size(); j++) {
			std::cout << maps[i][j] << std::endl;
		}
	}

}

void printMap(vector<string>& m) {
	std::cout << "Size: " << m.size() << std::endl;
	for (size_t i = 0; i < m.size(); i++) {
		std::cout << GREEN << m[i] << RESET << std::endl;
	}

}

void printInfoList(vector<vector<Info>> lst) {
	std::cout << "Level num: " << lst.size() << std::endl;
	for (size_t i = 0; i < lst.size(); i++) {
		std::cout << "Level: " << i << std::endl;
		std::cout << "Contents: " << lst[i].size() << std::endl;
		for (size_t j = 0; j < lst[i].size(); j++) {
			Info curr = lst[i][j];
			std::cout << "Coordinate: " << curr.cdn << " Item: " << curr.item << " effect: " << curr.effectCode << std::endl;
		}
	}
}





int main(int argc, char* argv[]) {
    string fileName = "defaultFloor.txt";
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	if (argc >= 2) {
		fileName = argv[1];
	}
	if (argc >= 3) {
		istringstream iss{argv[2]};
		iss >> seed;
	}

    // Step 1: Read in maps and store them in vector<vetor<string>> maps where each string represents each line
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    string s;
	std::vector<string> tempRecord;
	while (getline(ifs, s)) {
		tempRecord.emplace_back(s);
	}
	cin.clear();


    string startLine;
    vector<vector<string>> maps;
    vector<vector<Info>> mapInfo; // Info: {Coordinate cdn, char itemName, int effectCode}
    int floorIndex = 0;
    int PCFloorIndex = 6;
    int index = 0;
    vector<string> tempMap;
    vector<Info> tempInfo;
    bool infoLine = false;
    for (size_t i = 0; i < tempRecord.size(); i++) {
		s = tempRecord[i];
        if (!infoLine) { // If this is a map layout line
            if (index == 0) {
                startLine = s;
            } else if (s == startLine) {
				std::cout << "Graph " << floorIndex << " read in success" << std::endl;
                tempMap.emplace_back(s);
                maps.emplace_back(tempMap);
                tempMap.clear();
                floorIndex++;
                index = 0;
				if (i + 1 < tempRecord.size() && tempRecord[i + 1][0] != '|') {
					infoLine = true;
				} else {
					mapInfo.emplace_back(tempInfo);
					tempInfo.clear();
				}
				continue;
            } 
        	tempMap.emplace_back(s);
            index++;
        }


        else { // If this is an info line
            // check whether the next line is also a line of info
			std::cout << YELLOW << s << RESET << std::endl;
            int x, y, effectCode = 0;
            char item;
            istringstream iss{s};
            iss >> x >> y >> item;
			std::cout << YELLOW << "Coordinate: (" << x << "," << y << ")  Item: " 
			<< item << "  Effect Code: " << effectCode << RESET << std::endl;

            tempInfo.emplace_back(Info{Coordinate{x, y}, item, effectCode});

			if (item == '@') {
				PCFloorIndex = floorIndex;
			}
			if (i + 1 < tempRecord.size() && tempRecord[i + 1][0] == '|') { // EOF or not an Info line
				mapInfo.emplace_back(tempInfo);
                tempInfo.clear();
                infoLine = false;
			}
        }

    }

	// Debugger
	// std::cout << YELLOW << "PCFloorIndex: " << PCFloorIndex << RESET << std::endl;
	//printMaps(maps);
	//printInfoList(mapInfo);


	// Step 2: Call the corresponding constructor to initialize the game Grid


	// CASE 1: If no floor file argument, then play the random-generated game
	// Print out welcome message
	std::cout << "WELCOME TO THE GAME OF CHAMBERCRAWLER3000+!" << std::endl;
	printIntroMsg();
	char pc;
	// Player character selection
	while (cin >> pc) {
		if (pc == 'h' || pc == 'd' || pc == 'e' || pc == 'o') {
			break;
		} else {
			std::cout << "Invalid input, please input the correct character" << std::endl;
		}
	}


	int currFloor = 1; // Current floor number
	if (PCFloorIndex != 6) {
		currFloor = PCFloorIndex;
	}

	std::vector<int> n;
	for (int i = 1; i <= 5; i++) {
		n.emplace_back(i);
	}
	std::shuffle(n.begin(), n.end(), std::default_random_engine{seed});
	int barrierFloor = n[0];
	n.clear();

	// Debugger
	//std::cout << "Barrier Suit floor: " << barrierFloor << std::endl;
	//std::cout << "PCFloorIndex: " << PCFloorIndex << std::endl;
	//std::cout << "Seed: " << seed << std::endl;
	//std::cout << "PC: " << pc << std::endl;

	// Create new Grid object (game)
	Grid* g;

	if (PCFloorIndex == 6) {
		// Debugger
		std::cout << "Maps size: " << maps.size() << std::endl;
		g = new Grid{maps[0], seed, pc, currFloor == barrierFloor};
	} else {
		g = new Grid(maps[PCFloorIndex - 1], mapInfo[PCFloorIndex - 1], seed, pc);
	}

	g->printState(currFloor);


	// Game starts
	char cmd;
	while (cin >> cmd) {
		// Moving Player Character
		if (cmd == 'n' || cmd == 's' || cmd == 'e' || cmd == 'w') {
			try {
				string direction = "";
				direction += cmd;
				cin >> cmd;
				if (cin.fail()) throw runtime_error("Incorrect direction format!");
				direction += cmd;
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				if (g->moveTo(destination)) {

					if (currFloor == 5) {
						cout << YELLOW << "You Win! Your Score is: " << PC::totalCoin << endl;
						break;
					}

					currFloor += 1;
					std::cout << GREEN << "You found the stairs! ENTERING LEVEL " << currFloor << " >>>" << RESET << std::endl;
					delete g;

					g = new Grid{maps[currFloor - 1], ++seed, pc, currFloor == barrierFloor};
					/*
					if (PCFloorIndex == 6) {
						g = new Grid{maps[currFloor - 1], seed, pc, currFloor == barrierFloor};
					} else {
						g = new Grid(maps[currFloor - 1], mapInfo[currFloor - 1], seed, pc);
					}
					*/

					g->printState(currFloor);
				}
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}
		} else if (cmd == 'u') {
			try{
				string direction;
				cin >> direction;
				if (cin.fail()) throw runtime_error("Incorrect direction format!");
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				// Debugger
				std::cout << "Current PC Location: " << g->getPCLocation() << std::endl;
				std::cout << "Accessing cdn: " << destination << std::endl;
				std::cout << direction << std::endl;

				g->usePotion(destination);
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}

		} else if (cmd == 'a') {
			try{
				string dir;
				cin >> dir;
				if (cin.fail() || (dir != "no" && dir != "so" && dir != "we" && dir != "ea" && dir != "nw" && dir != "ne" && dir != "sw" && dir != "se")) {
					throw runtime_error("Incorrect direction format!");
				}
				Coordinate destination = convertCdn(g->getPCLocation(), dir);
				g->PCAttack(destination);
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}

		} else if (cmd == 'q') {
			std::cout << CYAN << "##### " << left << setw(45) << setfill(' ') << "GAME OVER! Defeated! (Player quits the game)" << right << setw(5) << "#####" << std::endl;
			std::cout << "##### TOTAL SCORE: " << left << setw(32) << setfill(' ')  << PC::totalCoin << right << setw(5) << "#####" << RESET << std::endl;
			break;


		} else if (cmd == 'r') {
			currFloor = 1;
			if (PCFloorIndex != 6) {
				currFloor = PCFloorIndex;
			}

			std::vector<int> n;
			for (int i = 1; i <= 5; i++) {
				n.emplace_back(i + 1);
			}
			std::shuffle(n.begin(), n.end(), std::default_random_engine{seed});
			int barrierFloor = n[0];
			n.clear();
			delete g;
			printIntroMsg();
			// Player character reselection
			while (cin >> pc) {
				if (pc == 'h' || pc == 'd' || pc == 'e' || pc == 'o') {
					break;
				} else {
					std::cout << "Invalid input, please input the correct character" << std::endl;
				}
			}
			// Reassign/regenerate seed
			/*
			if (argc == 3) {
				string s = argv[2];
				istringstream iss{s};
				iss >> seed;
			} else { // if not specified, use random seed
				seed = std::chrono::system_clock::now().time_since_epoch().count();
			}
			*/
			if (PCFloorIndex == 6) {
				g = new Grid{maps[currFloor - 1], seed, pc, currFloor == barrierFloor};
			} else {
				g = new Grid(maps[currFloor - 1], mapInfo[currFloor - 1], seed, pc);
			}
			g->printState(currFloor);
			continue;
		}
		
		else if (cmd == 'b') {
			string s;
			cin >> cmd;
			s += cmd;
			cin >> cmd;
			s += cmd;
			g->buyPotion(s);
			g->updateGrid();
		} else {
			std::cout << "Invalid command, please try again!" << std::endl;
			continue;
		}

		// NPC's round & Grid update
		g->updateGrid();
		g->printState(currFloor);
		if (g->getHP() == 0) {
			std::cout << CYAN << "##### " << left << setw(22) << setfill(' ') << "GAME OVER! Defeated!" << right << setw(5) << "#####" << std::endl;
			std::cout << "##### TOTAL SCORE: " << left << setw(9) << setfill(' ')  << PC::totalCoin << right << setw(5) << "#####" << RESET << std::endl;
			break;
		}

	}
	delete g;
	return 0;



	// CASE 2: If user-defined floor file provided, read in maps and load the game state based on the given info

}


