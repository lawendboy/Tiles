//
// Created by Michal Walczyk on 12/09/2023.
//
#include "iostream"
#include "LogicalChunk.hpp"

LogicalChunk::LogicalChunk() {
    for(int i : middleLayer)
        std::cout << i << std::endl;
}

LogicalChunk::~LogicalChunk() {

}