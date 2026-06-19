#include "Level.h"
#include "Block.h"
#include <fstream>
#include <iostream>

Level::Level() : Level("") {
}

Level::Level(std::string path1) : nextLevel(""), prevLevel("") {
    std::ifstream level(path1);
    if (!level) {
        std::cerr << "Can't find the level" << std::endl;
    }
    else {
        int lineNumber = -1;
        std::string line;
        while (std::getline(level, line)) {
            if (line == "Next Level") {
                std::getline(level, nextLevel);
                continue;
            }
            if (line == "Previous Level") {
                std::getline(level, prevLevel);
                continue;
            }
            lineNumber++;
            for (int i = 0; i < line.size(); i++) {
                std::string character(1, line[i]);
                if (character == "X") {
                    blocks.push_back(new MBlock(i * 48.f, lineNumber * 48.f));
                }
                else if (character == "F") {
                    blocks.push_back(new EndBlock(i * 48.f, lineNumber * 48.f));
                }
                else if (character == "I") {
                    blocks.push_back(new Item(i * 48.f, lineNumber * 48.f));
                }
                else if (character == "H") {
                    blocks.push_back(new HBlock(i * 48.f, lineNumber * 48.f));
                }
                // '_' et 'W' ignorés pour l'instant
            }
        }
    }
}

Level::~Level() {
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i]) {
            delete blocks[i]; blocks[i] = nullptr;
        }
    }
    blocks.clear();
}

void Level::Render(sf::RenderTarget& target) {
    for (int i = 0; i < blocks.size(); i++) {
        blocks[i]->Render(target);
    }
}

void Level::Update(int currentLvl, int& newLvl, float dt, float now, float playerX, float playerY, float playerW, bool& onHZone){
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i]->GetBlockType() == "EndBlock") { //Next Level
            if (blocks[i]->isNextLevel) {
                newLvl = currentLvl + 1;
                blocks[i]->isNextLevel = false;
            }
        }
        else if (blocks[i]->GetBlockType() == "BegBlock") { //Prev Level
            if (blocks[i]->isPrevLevel) {
                newLvl = currentLvl + 1;
                blocks[i]->isPrevLevel = false;
            }
        }
        else if (blocks[i]->GetBlockType() == "HBlock") {

            if (playerY < 550) {
                float playerL = playerX;
                float playerR = playerX + playerW;
                float blockL = blocks[i]->GetPosX();
                float blockR = blocks[i]->GetRightX();

                if (playerR > blockL && playerL < blockR) {
                    onHZone = true;
                }
            }

        }
    }
}