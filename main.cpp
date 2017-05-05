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

    object.BindVertexArray();

    glGenBuffers(NumVAOs, VAOs);
    glBufferData(GL_ARRAY_BUFFER, 4 * INSTANCE_COUNT, NULL, GL_DYNAMIC_DRAW);

    vec4 colors[INSTANCE_COUNT];

    for (int n = 0; n < INSTANCE_COUNT; ++n) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;

        colors[n][0] = 0.5f * (sinf(a + 1.0f) + 1.0f);
        colors[n][1] = 0.5f * (sinf(b + 2.0f) + 1.0f);
        colors[n][2] = 0.5f * (sinf(c + 3.0f) + 1.0f);
        colors[n][3] = 1.0f;
    }

    glGenBuffers(2, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[WeightBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[ColorBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    glVertexAttribDivisor(4, 1);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    // Unbind object's array buffer
    glBindVertexArray(0);
}

void display(void) {
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    float t = ((clock() / 10) & 0x1FFF) / float(0x1FFF);

    vec4 weights[INSTANCE_COUNT];

    for (int n = 0; n < INSTANCE_COUNT; ++n) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;

        weights[n][0] = 0.5f * (sinf(t * 6.28318531f * 8.0f + a) + 1.0f);
        weights[n][1] = 0.5f * (sinf(t * 6.28318531f * 26.0f + b) + 1.0f);
        weights[n][2] = 0.5f * (sinf(t * 6.28318531f * 21.0f + c) + 1.0f);
        weights[n][3] = 0.5f * (sinf(t * 6.28318531f * 13.0f + a + b) + 1.0f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[WeightBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(weights), weights, GL_DYNAMIC_DRAW);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    mat4 model_matrix[4];

    for (int n = 0; n < 4; ++n) {
        model_matrix[n] = (vmath::scale(5.0f) *
                           vmath::rotate(t * 360.0f * 40.0f + float(n + 1) * 29.0f, 0.0f, 1.0f, 0.0f) *
                           vmath::rotate(t * 360.0f * 20.0f + float(n + 1) * 35.0f, 0.0f, 0.0f, 1.0f) *
                           vmath::rotate(t * 360.0f * 30.0f + float(n + 1) * 67.0f, 0.0f, 1.0f, 0.0f) *
                           vmath::translate((float)n * 10.0f - 15.0f, 0.0f, 0.0f) *
                           vmath::scale(0.01f));
    }

    glUniformMatrix4fv(render_model_matrix_loc, 4, GL_FALSE, model_matrix[0]);

    mat4 projection_matrix(frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) * translate(0.0f, 0.0f, -100.0f));

    glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

    object.Render(0, INSTANCE_COUNT);

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