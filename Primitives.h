#pragma once
#include "Mesh.h"
#include "vec.h"

namespace Primitives
{
    Vertex cubeVertices[] = {
        // Front face
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{0.0f, 1.0f} },

        // Back face
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Left face
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Right face
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{0.0f, 1.0f} },

        // Top face
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Bottom face
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{0.0f, 1.0f} }
    };

    unsigned int cubeIndices[] = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Back
        4, 5, 6,
        4, 6, 7,

        // Left
        8, 9, 10,
        8, 10, 11,

        // Right
        12, 13, 14,
        12, 14, 15,

        // Top
        16, 17, 18,
        16, 18, 19,

        // Bottom
        20, 21, 22,
        20, 22, 23
    };
}
