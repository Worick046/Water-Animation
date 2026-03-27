#pragma once
#include <vector>


struct Vertex
{
    float x;
    float y;
    float z;
};

struct mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};