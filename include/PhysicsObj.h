//
// Created by Asda Tapel on 11/26/16.
//

#ifndef MINECRAFT_CLONE_PHYSICSOBJ_H
#define MINECRAFT_CLONE_PHYSICSOBJ_H

#include "Chunk.h"


class PhysicsObj {
public:
    virtual bool checkCollision(Chunk* chunk) = 0;

    ivec2 position;
private:

};


#endif //MINECRAFT_CLONE_PHYSICSOBJ_H
