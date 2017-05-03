#include <iostream>
using namespace std;


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "LoadShaders.h"
#include "vmath.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))


enum VAO_IDs { Triangles, NumVAOs };
enum VBO_IDs { VertexBuffer, NumVBOs };
enum EBO_IDs { ElementBuffer, NumEBOs };
enum Attrib_IDs { vPosition, vColor };

GLuint VAOs[NumVAOs];
GLuint VBOs[NumVBOs];
GLuint EBOs[NumEBOs];

const GLuint NumVertices = 6;

void init(void) {
    // four
    static const GLfloat vertex_positions[] =
    {
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
    };

    static const GLfloat vertex_colors[] =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
    };

    static const GLushort vertex_indices[] =
    {
        0, 1, 2,
    };

    // element buffer
    glGenBuffers(NumEBOs, EBOs);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[ElementBuffer]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    // vertex array
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    // array buffer
    glGenBuffers(NumVBOs, VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[VertexBuffer]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

    ShaderInfo shaders[] = {
            { GL_VERTEX_SHADER, "primitive_restart.vs.glsl" },
            { GL_FRAGMENT_SHADER, "primitive_restart.fs.glsl" },
            { GL_NONE, NULL },
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*) sizeof(vertex_positions));
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

//    glBindVertexArray(VAOs[Triangles]);
    vmath::Tmat4 model_matrix;
    model_matrix = vmath::translate(-3.0f, 0.0f, -5.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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