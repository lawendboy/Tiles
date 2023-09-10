//
// Created by Michal Walczyk on 08/09/2023.
//

#ifndef TILES_TERRAINGENERATOR_HPP
#define TILES_TERRAINGENERATOR_HPP


class TerrainGenerator {
private:
    int seed;
public:
    int getSeed() const;
    void setSeed(int seed);
};


#endif //TILES_TERRAINGENERATOR_HPP
