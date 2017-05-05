#include <iostream>
using namespace std;


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <ctime>

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
    static const GLfloat cube_positions[] =
    {
        -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, -1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f,
    };

    static const GLfloat cube_colors[] =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
    };


    // index of triangle_strips
    static const GLushort cube_indices[] =
    {
        0, 1, 2, 3, 6, 7, 4, 5,     // first strip
        0xFFFF,                     // primitive reset index
        2, 6, 0, 4, 1, 5, 3, 7,     // second strip
    };


    // setup OpenGL
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // element buffer
    glGenBuffers(NumEBOs, EBOs);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[ElementBuffer]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // vertex array
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    // array buffer
    glGenBuffers(NumVBOs, VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[VertexBuffer]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

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
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(cube_positions));
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void display(void) {
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    float t = ((clock() / 10) & 0x1FFF) / float(0x1FFF);

    mat4 model_matrix(translate(0.0f, 0.0f, -5.0f) * rotate(t * 360.0f, Y) * rotate(t * 360.0f, Z));
    mat4 projection_matrix(frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));

    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

    glBindVertexArray(VAOs[VertexBuffer]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[ElementBuffer]);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFF);
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);

    glFlush();
}

bool animated = true;

void redisplay_timer(int v) {
    static int a = 0;
    if (animated)
        glutTimerFunc((unsigned int)(1000 / 60), redisplay_timer, a);
    glutPostRedisplay();
    a++;
}

void clickEvent(int button, int state, int x, int y) {
    animated = button != GLUT_LEFT_BUTTON;
    if (animated) redisplay_timer(0);
}

int main(int argc, const char ** argv) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);
    redisplay_timer(0);
    glutMouseFunc(clickEvent);

    glutMainLoop();
}