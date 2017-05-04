#include <iostream>
using namespace std;


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "vmath.h"
using namespace vmath;

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))


enum VAO_IDs { Triangles, NumVAOs };
enum VBO_IDs { VertexBuffer, NumVBOs };
enum EBO_IDs { ElementBuffer, NumEBOs };
enum Attrib_IDs { vPosition, vColor };

GLuint VAOs[NumVAOs];
GLuint VBOs[NumVBOs];
GLuint EBOs[NumEBOs];

GLint render_model_matrix_loc;
GLint render_projection_matrix_loc;

float aspect = 1.0f;

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

    render_model_matrix_loc = glGetUniformLocation(program, "model_matrix");
    render_projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*) sizeof(vertex_positions));
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    mat4 model_matrix;
    model_matrix = translate(-3.0f, 0.0f, -5.0f);

    mat4 projection_matrix(frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f));
    glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

    model_matrix = translate(-3.0f, 0.0f, -5.0f);
    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[ElementBuffer]);

    model_matrix = translate(-1.0f, 0.0f, -5.0f);
    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

    model_matrix = translate(1.0f, 0.0f, -5.0f);
    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 0);

    model_matrix = translate(3.0f, 0.0f, -5.0f);
    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

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