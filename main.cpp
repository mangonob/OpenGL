#include <iostream>
using namespace std;


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <ctime>

#include "vmath.h"
#include "vbm.h"
using namespace vmath;


#include "LoadShaders.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))


enum VAO_IDs { Triangles, NumVAOs };
enum VBO_IDs { WeightBuffer, ColorBuffer, NumVBOs };
enum EBO_IDs { ElementBuffer, NumEBOs };
enum Attrib_IDs { vPosition, vColor };

GLuint VAOs[NumVAOs];
GLuint VBOs[NumVBOs];
GLuint EBOs[NumEBOs];

GLint render_model_matrix_loc;
GLint render_projection_matrix_loc;

float aspect = 1.0f;

const static int INSTANCE_COUNT = 100;

VBObject object;

void init(void) {
    ShaderInfo shaders[] = {
            { GL_VERTEX_SHADER, "shaders/primitive_restart/primitive_restart.vs.glsl" },
            { GL_FRAGMENT_SHADER, "shaders/primitive_restart/primitive_restart.fs.glsl" },
            { GL_NONE, NULL },
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    render_model_matrix_loc = glGetUniformLocation(program, "model_matrix");
    render_projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");
}

void display(void) {
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    float t = ((clock() / 10) & 0x1FFF) / float(0x1FFF);


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