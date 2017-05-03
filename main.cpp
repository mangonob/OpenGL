#include <iostream>
using namespace std;


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "LoadShaders.h"


#define BUFFER_OFFSET(offset) ((void *)(offset))


enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init(void) {
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    GLfloat vertices[NumVertices][2] = {
            { -0.90, -0.90 }, // Triangle 1
            {  0.85, -0.90 },
            { -0.90,  0.85 },
            {  0.90, -0.85 }, // Triangle 3
            {  0.90,  0.90 },
            { -0.85,  0.90 },
    };

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderInfo shaders[] = {
            { GL_VERTEX_SHADER, "triangles.vert" },
            { GL_FRAGMENT_SHADER, "triangles.frag" },
            { GL_NONE, NULL },
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

int main(int argc, const char ** argv) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);

    glutMainLoop();
}