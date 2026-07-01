#include "Board.h"
#include "ObjectFactory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void Board::LoadLevel(const std::string& filename, b2World& world,
                      std::vector<Block*>& blocks, std::vector<Target*>& targets,
                      std::queue<std::string>& magazine,
                      sf::FloatRect& outBounds) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open level file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "MAGAZINE") {
            std::string typeStr;
            while (iss >> typeStr)
                magazine.push(typeStr);
        }
        else if (keyword == "BOUNDS") {
            float minX, minY, maxX, maxY;
            if (iss >> minX >> minY >> maxX >> maxY) {
                outBounds = sf::FloatRect({ minX, minY },
                                         { maxX - minX, maxY - minY });
            }
        }
        else if (keyword == "WOOD" || keyword == "ICE" || keyword == "STONE") {
            blocks.push_back(ObjectFactory::CreateBlock(iss, keyword, world));
        }
        else if (keyword == "TARGET") {
            targets.push_back(ObjectFactory::CreateTarget(iss, world));
        }
    }
}
