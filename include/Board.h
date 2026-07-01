#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <queue>
#include <box2d/box2d.h>
#include "Block.h"
#include "Target.h"

class Board {
public:
    Board() = default;
    ~Board() = default;

    // outBounds is set from the level file's BOUNDS line; the caller must
    // initialise it with a sensible default before calling, in case the line is absent.
    void LoadLevel(const std::string& filename, b2World& world,
                   std::vector<Block*>& blocks, std::vector<Target*>& targets,
                   std::queue<std::string>& magazine,
                   sf::FloatRect& outBounds);
};
