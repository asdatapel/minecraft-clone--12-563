#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() {


    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "prahics manager before" << err << std::endl;
    }

    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, textureCount);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "prahics manager after" << err << std::endl;
    }

}


GraphicsManager::~GraphicsManager() {
}

int
GraphicsManager::newShaderResource(std::string vertShader, std::string fragShader, std::list<Attribute> attributes) {
    ShaderResource shader;
    const char *vert = vertShader.c_str();
    GLuint vertexShader3D = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader3D, 1, &vert, NULL);
    glCompileShader(vertexShader3D);

    char log[300];
    glGetShaderInfoLog(vertexShader3D, 300, NULL, log);
    std::cout << log << '\n';

    const char *frag = fragShader.c_str();
    GLuint fragmentShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3D, 1, &frag, NULL);
    glCompileShader(fragmentShader3D);


    glGetShaderInfoLog(fragmentShader3D, 300, NULL, log);
    std::cout << log << '\n';

    shader.handle = glCreateProgram();
    glAttachShader(shader.handle, vertexShader3D);
    glAttachShader(shader.handle, fragmentShader3D);
    glLinkProgram(shader.handle);
    glUseProgram(shader.handle);

    glDetachShader(shader.handle, vertexShader3D);
    glDetachShader(shader.handle, fragmentShader3D);
    glDeleteShader(vertexShader3D);
    glDeleteShader(fragmentShader3D);

    for (std::list<Attribute>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
        it->handle = glGetAttribLocation(shader.handle, it->name.c_str());
        shader.attributes.push_back(*it);
    }

    int next = 0;
    for (auto &i : shaderResources) {
        if (i.first != next)
            break;
        ++next;
    }
    shaderResources.insert({next, shader});
    return next;
}

void GraphicsManager::bindShader(int id) {
    glUseProgram(shaderResources[id].handle);
}

int GraphicsManager::getUniform(int shaderId, std::string name) {
    GLint handle = glGetUniformLocation(shaderResources[shaderId].handle, name.c_str());
    return handle;
}

int GraphicsManager::getArrayTexture(std::string name) {

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "getarraytex beginning: " << err << std::endl;
    }

    if (arrayTextureResources.find(name) != arrayTextureResources.end()) {
        return arrayTextureResources[name].handle;
    }

    ArrayTextureResource tex;

    tex.image.loadFromFile(name);
    tex.image.flipVertically();

    int next = 0;
    for (auto &x : arrayTextureResources) {
        if (x.second.handle >= next)
            next = x.second.handle + 1;
    }


    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, next, tex.image.getSize().x, tex.image.getSize().y, 1, GL_RGBA,
                    GL_UNSIGNED_BYTE, tex.image.getPixelsPtr());


    tex.handle = next;
    arrayTextureResources.insert({name, tex});

    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "getarraytexend: " << err << std::endl;
    }

    return next;
}

int GraphicsManager::getTexture(std::string name) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "getTexture beginning: " << err << std::endl;
    }

    int next = 0;
    for (auto &x : textureResources) {
        if (x.second.name == name)
            return x.first;
        if (x.first != next)
            break;
        ++next;
    }

    TextureResource tex;

    tex.image.loadFromFile(name);
    tex.image.flipVertically();

    glGenTextures(1, &(tex.handle));
    glBindTexture(GL_TEXTURE_2D, tex.handle);
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            tex.image.getSize().x, tex.image.getSize().y,
            0,
            GL_RGBA, GL_UNSIGNED_BYTE, tex.image.getPixelsPtr()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "getTexture end: " << err << std::endl;
    }


    textureResources.insert({next, tex});

    return next;
}

void GraphicsManager::bindArrayTexture() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
}

void GraphicsManager::bindTexture(int id) {
    glBindTexture(GL_TEXTURE_2D, textureResources[id].handle);
}

int GraphicsManager::newBuffer(int shaderId) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (auto const &attrib : shaderResources[shaderId].attributes) {
        glEnableVertexAttribArray(attrib.handle);
        glVertexAttribPointer(attrib.handle, attrib.length, GL_FLOAT, GL_FALSE, attrib.stride * sizeof(GLfloat),
                              (void *) (attrib.distanceToFirst * sizeof(float)));
    }

    int next = 0;
    for (auto &x : bufferResources) {
        if (x.first != next)
            break;
        ++next;
    }

    bufferResources.insert(std::pair<int, BufferResource>(next, {vbo, vao}));
    return next;
}

void GraphicsManager::updateBuffer(int id, float *mesh, int length) {
    BufferResource b = bufferResources[id];
    glBindBuffer(GL_ARRAY_BUFFER, b.vbo);

    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), mesh, GL_STATIC_DRAW);
}

void GraphicsManager::renderBuffer(int id, int points) {
    if (points != 0) {
        BufferResource b = bufferResources[id];
        glBindVertexArray(b.vao);

        glDrawArrays(GL_TRIANGLES, 0, points);
    }
}

void GraphicsManager::deleteBuffer(int id) {
    BufferResource b = bufferResources[id];
    glDeleteBuffers(1, &b.vbo);
    glDeleteBuffers(1, &b.vao);
}
