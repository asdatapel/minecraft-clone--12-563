#include <iostream>
#include <fstream>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Types.h"
#include "GraphicsManager.h"
#include "World.h"
#include "Chunk.h"
#include "Player.h"
#include "shaders.hpp"
#include "Perlin.h"
#include "Camera.h"


int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    settings.depthBits = 32;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.stencilBits = 4;
    settings.attributeFlags = settings.Core;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, settings);

    window.setPosition({100, 100});

    glewExperimental = GL_TRUE;
    glewInit();
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClearColor(7 / 255.0, 56 / 255.0, 145 / 255.0, 1);


    GraphicsManager graphics;

    std::list<Attribute> attributes;
    Attribute a = {0, std::string("position"), 3, 11, 0};
    attributes.push_back(a);
    a = {0, std::string("incolor"), 3, 11, 3};
    attributes.push_back(a);
    a = {0, std::string("sunlightlevel"), 1, 11, 6};
    attributes.push_back(a);
    a = {0, std::string("aolevel"), 1, 11, 7};
    attributes.push_back(a);
    a = {0, std::string("texcoord"), 3, 11, 8};
    attributes.push_back(a);
    int shader3d = graphics.newShaderResource(vertexShaderSource, fragmentShaderSource, attributes);
    GLint matrixUniform = graphics.getUniform(shader3d, "PVM");
    GLint sunlightUniform = graphics.getUniform(shader3d, "sunlight");

    attributes.clear();
    a = {0, std::string("position"), 2, 4, 0};
    attributes.push_back(a);
    a = {0, std::string("texcoord"), 2, 4, 2};
    attributes.push_back(a);
    int shader2d = graphics.newShaderResource(vertexShaderSource2D, fragmentShaderSource2D, attributes);
    int crosshairTex = graphics.getTexture("textures/cross.png");

    attributes.clear();
    a = {0, std::string("position"), 3, 5, 0};
    attributes.push_back(a);
    a = {0, std::string("texcoord"), 2, 5, 3};
    attributes.push_back(a);
    int shaderSky = graphics.newShaderResource(vertexShaderSourceSky, fragmentShaderSourceSky, attributes);
    int skyTex = graphics.getTexture("textures/skybox2.png");
    GLint matrixUniformSky = graphics.getUniform(shaderSky, "PVM");
    GLint sunlightUniformSky = graphics.getUniform(shaderSky, "sunlight");

    float crosshairMesh[] = {
            -0.01f, 0.01f, 0.0f, 1.0f,
            0.01f, 0.01f, 1.0f, 1.0f,
            -0.01f, -0.01f, 0.0f, 0.0f,
            -0.01f, -0.01f, 0.0f, 0.0f,
            0.01f, 0.01f, 1.0f, 1.0f,
            0.01f, -0.01f, 1.0f, 0.0f,
    };
    int crosshairBuffer = graphics.newBuffer(shader2d);
    graphics.updateBuffer(crosshairBuffer, crosshairMesh, 24);

    float skyMesh[180] = {
            1.0f, -1.0f, -1.0f, 0.0, 1.0 / 3.0,
            -1.0f, -1.0f, -1.0f, 1.0 / 4.0, 1.0 / 3.0,
            1.0f, 1.0f, -1.0f, 0.0, 2.0 / 3.0,

            -1.0f, -1.0f, -1.0f, 1.0 / 4.0, 1.0 / 3.0,
            -1.0f, 1.0f, -1.0f, 1.0 / 4.0, 2.0 / 3.0,
            1.0f, 1.0f, -1.0f, 0.0, 2.0 / 3.0,

            1.0f, -1.0f, 1.0f, 3.0 / 4.0, 1.0 / 3.0,
            -1.0f, 1.0f, 1.0f, 2.0 / 4.0, 2.0 / 3.0,
            -1.0f, -1.0f, 1.0f, 2.0 / 4.0, 1.0 / 3.0,

            1.0f, -1.0f, 1.0f, 3.0 / 4.0, 1.0 / 3.0,
            1.0f, 1.0f, 1.0f, 3.0 / 4.0, 2.0 / 3.0,
            -1.0f, 1.0f, 1.0f, 2.0 / 4.0, 2.0 / 3.0,

            1.0f, 1.0f, 1.0f, 1.0 / 4.0, 2.0 / 3.0,
            1.0f, -1.0f, -1.0f, 2.0 / 4.0, 1.0 / 3.0,
            1.0f, 1.0f, -1.0f, 2.0 / 4.0, 2.0 / 3.0,

            1.0f, 1.0f, 1.0f, 1.0 / 4.0, 2.0 / 3.0,
            1.0f, -1.0f, 1.0f, 1.0 / 4.0, 1.0 / 3.0,
            1.0f, -1.0f, -1.0f, (float) (2.0 / 4.0), (float) (1.0 / 3.0),

            -1.0f, -1.0f, -1.0f, 3.0 / 4.0, 1.0 / 3.0,
            -1.0f, 1.0f, 1.0f, 4.0 / 4.0, 2.0 / 3.0,
            -1.0f, 1.0f, -1.0f, 3.0 / 4.0, 2.0 / 3.0,

            -1.0f, -1.0f, -1.0f, (float) (3.0 / 4.0), 1.0 / 3.0,
            -1.0f, -1.0f, 1.0f, 4.0 / 4.0, 1.0 / 3.0,
            -1.0f, 1.0f, 1.0f, 4.0 / 4.0, 2.0 / 3.0,

            -1.0f, -1.0f, -1.0f, 2.0 / 4.0, 1.0 / 3.0,
            1.0f, -1.0f, 1.0f, 1.0 / 4.0, 0.0f,
            -1.0f, -1.0f, 1.0f, 2.0 / 4.0, 0.0f,

            -1.0f, -1.0f, -1.0f, 2.0 / 4.0, 1.0 / 3.0,
            1.0f, -1.0f, -1.0f, 1.0 / 4.0, 1.0 / 3.0,
            1.0f, -1.0f, 1.0f, 1.0 / 4.0, 0.0f,

            -1.0f, 1.0f, 1.0f, 1.0 / 4.0, 2.0 / 3.0,
            1.0f, 1.0f, -1.0f, 2.0 / 4.0, 1.0f,
            -1.0f, 1.0f, -1.0f, 1.0 / 4.0, 1.0f,

            -1.0f, 1.0f, 1.0f, 1.0 / 4.0, 2.0 / 3.0,
            1.0f, 1.0f, 1.0f, 2.0 / 4.0, 2.0 / 3.0,
            1.0f, 1.0f, -1.0f, 2.0 / 4.0, 1.0f,
    };
    int skyBuffer = graphics.newBuffer(shaderSky);
    graphics.updateBuffer(skyBuffer, skyMesh, 180);

    //setup
    Player player;
    player.setPosition({0, 0, 80});
    bool canJump;
    Camera camera;
    camera.setPosition(player.getCameraPosition());
    Perlin perlin(6, 4, 1, 51);

    graphics.bindShader(shader3d);
    World *world = new World(&graphics, shader3d);
    world->genChunks(&perlin);

    fvec3 velocity(0, 0, 0);

    float sunlight = 0.0f;
    float sunlightTimer = 0.0f;


    attributes.clear();
    a = {0, std::string("position"), 3, 6, 0};
    attributes.push_back(a);
    a = {0, std::string("incolor"), 3, 6, 3};
    attributes.push_back(a);
    int boxShader = graphics.newShaderResource(boxVertexShaderSource, boxFragmentShaderSource, attributes);
    world->createBoxes(boxShader);

    sf::Clock timer;

    bool cameraControl = true;
    bool running = true;
    timer.restart();
    while (running) {
        float elapsedTime = timer.getElapsedTime().asSeconds();

        timer.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            } else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && canJump) {
                    velocity += fvec3(0.0, 0.0, 0.6);
                } else if (event.key.code == sf::Keyboard::Escape) {
                    cameraControl = !cameraControl;
                } else if (event.key.code == sf::Keyboard::Q) {
                    running = false;
                } else if (event.key.code == sf::Keyboard::M) {
                    world->createRandomBoxes(boxShader);
                } else if (event.key.code == sf::Keyboard::P) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            } else if (event.key.code == sf::Keyboard::O) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    RayCollision ray = world->raycast(camera.getPosition(), camera.getDirectionVector(), 4);
                    std::cout << ray.blockPos.x << "--" << ray.blockPos.z << "--" << ray.blockPos.y << "-----------"
                              << ray.side << "\n";
                    if (ray.isCollision)
                        world->removeBlock(ray.blockPos);
                } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                    RayCollision ray = world->raycast(camera.getPosition(), camera.getDirectionVector(), 4);
                    std::cout << ray.blockPos.x << "--" << ray.blockPos.z << "--" << ray.blockPos.y << "-----------"
                              << ray.side << "\n";
                    if (ray.isCollision && !player.checkCollision(floor(ray.blockPos + adjacent[ray.side])))
                        world->addBlock(ray.blockPos + adjacent[ray.side], 4);
                } else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
                    RayCollision ray = world->raycast(camera.getPosition(), camera.getDirectionVector(), 4);
                    std::cout << ray.blockPos.x << "--" << ray.blockPos.z << "--" << ray.blockPos.y << "-----------"
                              << ray.side << "-----------" << world->getSunlightLevel(ray.blockPos) << "\n";
                    if (ray.isCollision && !player.checkCollision(floor(ray.blockPos + adjacent[ray.side])))
                        world->addBlock(ray.blockPos + adjacent[ray.side], 1);
                }

            }
        }

        //velocity *= fvec3(0.9, 0.9, 1.0) ;
        //velocity += fvec3(0.0, 0.0, -1.5) * elapsedTime;
        float dampening = std::pow(0.01f, elapsedTime);
        float dampeningY = std::pow(0.05f, elapsedTime);
        velocity = {velocity.x * dampening, velocity.z * dampening, velocity.y * dampeningY};
        //std::cout << dampening << " --- " << velocity.x << ", " << velocity.z << " " << velocity.y << "\n";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            velocity += (camera.getForwardVector() * -0.25f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            velocity += (camera.getForwardVector() * 0.25f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocity += (camera.getRightVector() * -0.25f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocity += (camera.getRightVector() * 0.25f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            velocity += fvec3(0.0, 0.0, -1.5);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            velocity += fvec3(0.0, 0.0, 1.5);
        }

        if (sqrt(velocity.x * velocity.x + velocity.z * velocity.z) < 0.05) {
            velocity.x = 0;
            velocity.z = 0;
        } else if (sqrt(velocity.x * velocity.x + velocity.z * velocity.z) > 1.f) {
            float mag = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            velocity.x = velocity.x / mag;
            velocity.z = velocity.z / mag;
        }
        if (velocity.y > 1.f) {
            velocity.y = 1.f;
        } else if (velocity.y < -1.f) {
            velocity.y = -1.f;
        }


        player.setVelocity(velocity);

        fvec3 pos = player.getPosition();

        BoundingBox temp = player.getBB();
        temp.position.x += player.getVelocity().x * elapsedTime * 10;
        if (world->checkCollision(temp))
            velocity.x = 0;
        else
            pos = temp.position;
        temp.position = pos;
        temp.position.z += player.getVelocity().z * elapsedTime * 10;
        if (world->checkCollision(temp))
            velocity.z = 0;
        else
            pos = temp.position;
        temp.position = pos;
        temp.position.y += player.getVelocity().y * elapsedTime * 10;
        if (world->checkCollision(temp)) {
            velocity.y = 0;
            canJump = true;
        } else {
            pos = temp.position;
            canJump = false;
        }

        //pos += player.getVelocity();
        player.setPosition(pos);
        camera.setPosition(player.getCameraPosition());

        world->update(player.getPosition(), elapsedTime);

        //Rendering
        glm::mat4 PV = camera.getPVMat();

        if (cameraControl) {
            sf::Vector2i mouseDelta = sf::Mouse::getPosition() -
                                      (window.getPosition() + sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
            camera.rotate(-mouseDelta.x, -mouseDelta.y + 1600);
            sf::Mouse::setPosition(window.getPosition() + sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
        }

        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        graphics.bindShader(shaderSky);
        graphics.bindTexture(skyTex);
        glUniformMatrix4fv(matrixUniformSky, 1, GL_FALSE, glm::value_ptr(camera.getCenteredPVMat()));
        graphics.renderBuffer(skyBuffer, 36);

        glEnable(GL_DEPTH_TEST);
        graphics.bindShader(shader3d);
        graphics.bindArrayTexture();
        glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(PV * modelMat));

        sunlightTimer += elapsedTime / 2;
        sunlight = 0.03f + (0.03f * std::sin(sunlightTimer));
        //sunlight = 0.05;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            sunlight += 0.0005f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            sunlight -= 0.0005f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            std::cout << sunlight << std::endl;
        }
        if (sunlight < 0.0f)
            sunlight = 0.0f;


        glUniform1f(sunlightUniform, sunlight);

        world->renderChunks();

        graphics.bindShader(boxShader);
        world->renderBoxes(&PV);

        glDisable(GL_DEPTH_TEST);
        graphics.bindShader(shader2d);
        graphics.bindTexture(crosshairTex);
        graphics.renderBuffer(crosshairBuffer, 6);

        window.display();
    }
    // release resources...

    exit(0);
    return 0;
}
