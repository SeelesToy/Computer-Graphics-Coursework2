#ifndef MODEL_H
#define MODEL_H


#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#undef TINYOBJLOADER_IMPLEMENTATION

#include "texture.h"

#include <string>
#include <vector>
#include <iostream>

struct Model {
    GLuint VAO;
    GLuint VBO;
    GLuint texture;      // diffuse texture ID
    GLsizei vertexCount; // number of vertices
};

// Load an OBJ model (with MTL) and its diffuse texture
inline bool loadModel(const std::string& objPath,
    const std::string& texPath,
    Model& model)
{
    // 1. Load OBJ and materials
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials,
        &warn, &err,
        objPath.c_str(),
        objPath.substr(0, objPath.find_last_of("/\\") + 1).c_str());
    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty())  std::cerr << "ERR:  " << err << std::endl;
    if (!ret) return false;

    // 2. Flatten vertex data: position(3), normal(3), uv(2)
    std::vector<float> data;
    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            // position
            data.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            data.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            data.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
            // normal
            data.push_back(attrib.normals[3 * idx.normal_index + 0]);
            data.push_back(attrib.normals[3 * idx.normal_index + 1]);
            data.push_back(attrib.normals[3 * idx.normal_index + 2]);
            // uv
            data.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
            data.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
        }
    }
    model.vertexCount = static_cast<GLsizei>(data.size() / 8);

    // 3. Create VAO & VBO
    glGenVertexArrays(1, &model.VAO);
    glGenBuffers(1, &model.VBO);
    glBindVertexArray(model.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    // layout(location = 0): position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // layout(location = 1): color (unused, default white)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // layout(location = 2): normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // layout(location = 3): uv
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // 4. Load diffuse texture
    model.texture = setup_texture(texPath.c_str());

    return true;
}

// Render the loaded model with specified shader program and model matrix
inline void renderModel(const Model& m,
    GLuint programID,
    const glm::mat4& modelMat)
{
    glUseProgram(programID);
    // set model matrix
    GLint loc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelMat));

    // bind diffuse texture to unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m.texture);
    glUniform1i(glGetUniformLocation(programID, "diffuseTexture"), 0);

    glUniform1i(glGetUniformLocation(programID, "useTexture"), true);
    // draw geometry
    glBindVertexArray(m.VAO);
    glDrawArrays(GL_TRIANGLES, 0, m.vertexCount);
    glBindVertexArray(0);
}

#endif // MODEL_H
