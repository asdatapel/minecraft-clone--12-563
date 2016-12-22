//
// Created by Asda Tapel on 11/26/16.
//

#include "Box.h"

Box::Box(GraphicsManager* g, AABB a, int shaderId) {
    graphics = g;
    aabb = a;

    buffer = g->newBuffer(shaderId);
    mesh = cube;
    float red = (float)std::rand() / RAND_MAX;
    float green = (float)std::rand() / RAND_MAX;
    float blue = (float)std::rand() / RAND_MAX;
    for (int i = 0; i < 216; i += 6){
        mesh[i + 3] = red;
        mesh[i + 4] = green;
        mesh[i + 5] = blue;
    }
    g->updateBuffer(buffer, mesh, 216);

    //matrix = glm::translate(glm::scale(glm::mat4(1.0f), {a.size.x, a.size.y, a.size.z}), glm::vec3(aabb.position.x, aabb.position.y, aabb.position.z));
    matrix = glm::scale(glm::translate(glm::mat4(1.0f), {aabb.position.x, aabb.position.y, aabb.position.z}), {a.size.x, a.size.y, a.size.z});

    matrixUniform = g->getUniform(shaderId, "PVM");

    velocity = {0,0,0};

}

void Box::update(){
    matrix = glm::scale(glm::translate(glm::mat4(1.0f), {aabb.position.x, aabb.position.y, aabb.position.z}), {aabb.size.x, aabb.size.y, aabb.size.z});
}

void Box::render(glm::mat4 *PV) {
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(*PV * matrix));
    graphics->renderBuffer(buffer, 36);
}

bool Box::checkCollision(Chunk *chunk) {
    return false;
}

