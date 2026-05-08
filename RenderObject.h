#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "vec.h"
#include "mat4.h"
#include "quat.h"

struct RenderObject {
    Mesh* mesh;
    Texture* texture;
    Shader* shader;

    vec3 position;
    quat rotation;
    vec3 scale;
    vec3 colour;

    mat4 getModelMatrix() const {
        return mat4::translate(position.x, position.y, position.z) * mat4::rotate(rotation) * mat4::scale(scale.x, scale.y, scale.z);
    }
};
