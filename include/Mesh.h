//
// Created by Asda Tapel on 1/1/17.
//

#ifndef MINECRAFT_CLONE_MESH_H
#define MINECRAFT_CLONE_MESH_H

#include <string>
#include <fstream>


class Mesh {
public:
    void loadFromFile(std::string filename);
    void clearMesh();

    float *vertices;
    int vertexCount;

};


#endif //MINECRAFT_CLONE_MESH_H
