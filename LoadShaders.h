//
// Created by Trinity on 2017/5/2.
//

#ifndef OPENGL_LOADSHADERS_H
#define OPENGL_LOADSHADERS_H

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

#endif //OPENGL_LOADSHADERS_H
