#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec4 color;

layout (location = 3) in mat4 model_matrix;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
