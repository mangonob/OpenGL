//
// Created by Trinity on 2017/5/2.
//


#include "LoadShaders.h"
#include <iostream>

using namespace std;


// load GLSL source code from disk
static const GLchar* ReadShader(const char *filename) {
    FILE* infile = fopen(filename, "rb");

    fseek(infile, 0, SEEK_END);
    long len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    GLchar* source = new GLchar[len+1];
    fread(source, sizeof(GLchar), len, infile);

    source[len] = 0;
    return const_cast<const GLchar*>(source);
}


// load shaders with infos of shader infos
GLuint LoadShaders(ShaderInfo* shaders)
{
    if (!shaders) return 0;

    GLuint program = glCreateProgram();

    ShaderInfo *entry = shaders;
    while (entry->type != GL_NONE) {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const GLchar* source = ReadShader(entry->filename);

        if (!source) {
            for (entry = shaders; entry->type != GL_NONE; ++entry) {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource(shader, 1, &source, NULL);
        delete [] source;

        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
#ifdef DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */

            return 0;
        }

        glAttachShader(program, shader);

        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        for (entry = shaders; entry->type != GL_NONE; ++entry) {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return 0;
    }

    return program;
}