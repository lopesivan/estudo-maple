/*  plotfunc.c  –  Plotador Interativo de Funções com OpenMaple + OpenGL
 *  CORRIGIDO – evita segfault nas funções de desenho e avaliação Maple
 */
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "maplec.h"

#define FONT (void *)GLUT_BITMAP_8_BY_13
#define MAX_POINTS 500

#ifdef _MSC_VER
# define CDECL __cdecl
#else
# define CDECL
#endif

/* ----------  globais  ---------- */
static double width = 640, height = 480;
static MKernelVector kv;

static char *current_function = NULL;
static double *func_points    = NULL;
static double *deriv_points   = NULL;
static int    num_points      = 0;

static int show_derivative = 0;
static int show_integral   = 0;
static double integral_value = 0.0;
static double x_min = -5.0, x_max = 5.0;

static char *PresetFunctions[] = {
    "sin(x)", "cos(x)", "x^2", "x^3 - 3*x",
    "exp(-x^2)", "1/(1+x^2)", "sin(x)/x", NULL
};

/* ----------  Maple callbacks  ---------- */
static void M_DECL textCallBack(void *data, int tag, const char *output)
{ (void)data; (void)tag; (void)output; }

static void M_DECL errorCallBack(void *data, M_INT offset, const char *msg)
{ fprintf(stderr, "Maple Error: %s\n", msg); (void)data; (void)offset; }

/* ----------  desenho – SAFE  ---------- */
static void drawFunction(double *points, int n, float r, float g, float b, float lw)
{
    if (!points || n <= 0) return;          /* 1ª proteção */

    glColor3f(r, g, b);
    glLineWidth(lw);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; ++i) {
        double x = points[2*i];
        double y = points[2*i+1];
        if (isnan(y) || isinf(y)) continue; /* 2ª proteção */
        glVertex2d(x, y);
    }
    glEnd();
    glLineWidth(1.0f);
}

/* ----------  cálculo com Maple – SAFE  ---------- */
static void calculate_function_points(const char *f)
{
    if (!f) return;

    /* libera anterior */
    free(func_points); func_points = NULL;
    free(deriv_points); deriv_points = NULL;

    num_points = MAX_POINTS;
    func_points = calloc(2 * num_points, sizeof(double));
    deriv_points = calloc(2 * num_points, sizeof(double));
    if (!func_points || !deriv_points) { perror("calloc"); exit(EXIT_FAILURE); }

    double step = (x_max - x_min) / (num_points - 1);
    char cmd[512];

    /* ---------- pontos da função ---------- */
    for (int i = 0; i < num_points; ++i) {
        double x = x_min + i * step;
        func_points[2*i] = x;
        deriv_points[2*i] = x;

        sprintf(cmd, "evalf(subs(x=%.15f, %s));", x, f);
        ALGEB res = EvalMapleStatement(kv, cmd);

        double y = 0.0;
        if (res && !IsMapleNULL(kv, res)) {
            const char *s = MapleToString(kv, res);
            if (s && *s) {                /* 3ª proteção */
                y = atof(s);
                if (isnan(y) || isinf(y)) y = 0.0;
                if (y > 10.0) y = 10.0;  /* clipping */
                if (y < -10.0) y = -10.0;
            }
        }
        func_points[2*i+1] = y;
        deriv_points[2*i+1] = 0.0;
    }

    /* ---------- derivada ---------- */
    if (show_derivative) {
        sprintf(cmd, "df := diff(%s, x):", f);
        EvalMapleStatement(kv, cmd);

        for (int i = 0; i < num_points; ++i) {
            double x = func_points[2*i];
            sprintf(cmd, "evalf(subs(x=%.15f, df));", x);
            ALGEB res = EvalMapleStatement(kv, cmd);
            double dy = 0.0;
            if (res && !IsMapleNULL(kv, res)) {
                const char *s = MapleToString(kv, res);
                if (s && *s) {
                    dy = atof(s);
                    if (isnan(dy) || isinf(dy)) dy = 0.0;
                    if (dy > 10.0) dy = 10.0;
                    if (dy < -10.0) dy = -10.0;
                }
            }
            deriv_points[2*i+1] = dy;
        }
    }

    /* ---------- integral ---------- */
    if (show_integral) {
        sprintf(cmd, "evalf(int(%s, x=%.15f..%.15f));", f, x_min, x_max);
        ALGEB res = EvalMapleStatement(kv, cmd);
        if (res && !IsMapleNULL(kv, res)) {
            const char *s = MapleToString(kv, res);
            if (s && *s) integral_value = atof(s);
        }
    }
}

/* ----------  restante do código (sem mudanças)  ---------- */
static void drawAxes(void)
{
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    glVertex2d(-6.0, 0.0); glVertex2d(6.0, 0.0);
    glVertex2d(0.0, -6.0); glVertex2d(0.0, 6.0);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    for (int i = -5; i <= 5; ++i) {
        if (i == 0) continue;
        glBegin(GL_LINES);
        glVertex2d(i, -0.1); glVertex2d(i, 0.1);
        glVertex2d(-0.1, i); glVertex2d(0.1, i);
        glEnd();
    }
}

static void drawText(float x, float y, const char *text, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (size_t i = 0; i < strlen(text); ++i)
        glutBitmapCharacter(FONT, text[i]);
}

static void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxes();

    if (func_points) drawFunction(func_points, num_points, 0.0f, 0.5f, 1.0f, 2.0f);
    if (show_derivative && deriv_points)
        drawFunction(deriv_points, num_points, 0.0f, 1.0f, 0.0f, 1.5f);

    if (show_integral && func_points) {
        glColor4f(0.0f, 1.0f, 1.0f, 0.2f);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < num_points; ++i) {
            double x = func_points[2*i];
            double y = func_points[2*i+1];
            if (x < x_min || x > x_max || isnan(y) || isinf(y)) continue;
            glVertex2d(x, 0.0);
            glVertex2d(x, y);
        }
        glEnd(); glDisable(GL_BLEND);
    }

    char buf[256];
    if (current_function) {
        sprintf(buf, "f(x) = %s", current_function);
        drawText(-5.8f, 5.5f, buf, 1.0f, 1.0f, 0.0f);
    }
    if (show_integral) {
        sprintf(buf, "Integral [%.1f, %.1f] = %.4f", x_min, x_max, integral_value);
        drawText(-5.8f, 5.0f, buf, 0.0f, 1.0f, 1.0f);
    }
    if (show_derivative)
        drawText(-5.8f, -5.2f, "Azul: f(x)  Verde: f'(x)", 0.8f, 0.8f, 0.8f);
    drawText(-5.8f, -5.7f, "i:integral  d:derivada  c:limpar  q:sair", 0.5f, 0.5f, 0.5f);

    glutSwapBuffers();
}

static void changeSize(int w, int h)
{
    if (h == 0) h = 1;
    width = w; height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(-6.0, 6.0, -6.0, 6.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

static void processKeys(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    switch (key) {
    case 'q': case 'Q': exit(0);
    case 'c': case 'C':
        free(func_points); func_points = NULL;
        free(deriv_points); deriv_points = NULL;
        num_points = 0; current_function = NULL;
        show_integral = show_derivative = 0; break;
    case 'i': case 'I':
        show_integral ^= 1;
        if (show_integral && current_function) calculate_function_points(current_function);
        break;
    case 'd': case 'D':
        show_derivative ^= 1;
        if (show_derivative && current_function) calculate_function_points(current_function);
        break;
    }
}

static void pickFunction(int opt) {
    current_function = PresetFunctions[opt];
    calculate_function_points(current_function);
}

static void initMaple(int argc, char *argv[])
{
    MCallBackVectorDesc cb = { textCallBack, errorCallBack, 0,0,0,0,0,0 };
    char err[2048];
    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if (!kv) { printf("Maple init erro: %s\n", err); exit(1); }
    printf("✓ Maple inicializado\n");
}

int main(int argc, char **argv)
{
    printf("Plotador Interativo – OpenMaple + OpenGL (corrigido)\n");
    initMaple(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Plotador de Funções");
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processKeys);
    glutCreateMenu(pickFunction);
    for (int i = 0; PresetFunctions[i]; ++i) glutAddMenuEntry(PresetFunctions[i], i);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glClearColor(0, 0, 0, 1);
    glutMainLoop();
    return 0;
}
