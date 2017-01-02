//
// Created by Asda Tapel on 1/1/17.
//

#include "Mesh.h"

void Mesh::loadFromFile(std::string filename) {
    std::ifstream file;
    file.open(filename);

    int count;
    file >> count;

    vertices = new float[count];
    for (int i = 0; i < count; ++i){
        file >> vertices[i];
    }

    vertexCount = count;
}