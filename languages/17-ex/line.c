/* 
 * plotfunc.c - Plotador Interativo de Funções com OpenMaple + OpenGL
 *
 * Funcionalidades:
 * - Digite uma função matemática e veja seu gráfico
 * - Calcule integral definida da função
 * - Calcule derivada e plote junto
 * - Menu com funções pré-definidas
 *
 * Controles:
 * - Tecla 'i': Mostrar/esconder integral
 * - Tecla 'd': Mostrar/esconder derivada
 * - Tecla 'c': Limpar
 * - Tecla 'q': Sair
 * - Botão direito: Menu de funções
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "maplec.h"

#define FONT (void *)GLUT_BITMAP_8_BY_13
#define MAX_POINTS 500

#ifdef _MSC_VER
  #define CDECL __cdecl
#else
  #define CDECL 
#endif

/* Variáveis globais */
double width = 640, height = 480;
MKernelVector kv;

/* Dados da função */
char *current_function = NULL;
double *func_points = NULL;
double *deriv_points = NULL;
int num_points = 0;

/* Estados de visualização */
int show_derivative = 0;
int show_integral = 0;
double integral_value = 0.0;
double x_min = -5.0, x_max = 5.0;

/* Funções pré-definidas */
static char *PresetFunctions[] = {
    "sin(x)",
    "cos(x)",
    "x^2",
    "x^3 - 3*x",
    "exp(-x^2)",
    "1/(1+x^2)",
    "sin(x)/x",
    NULL
};

/* ============================================
 * CALLBACKS MAPLE
 * ============================================ */

static void M_DECL textCallBack(void *data, int tag, const char *output)
{
    /* Silencioso - não imprime saída do Maple */
    (void)data; (void)tag; (void)output;
}

static void M_DECL errorCallBack(void *data, M_INT offset, const char *msg)
{
    fprintf(stderr, "Maple Error: %s\n", msg);
    (void)data; (void)offset;
}

/* ============================================
 * FUNÇÕES DE CÁLCULO COM MAPLE
 * ============================================ */

void calculate_function_points(const char *function_str)
{
    ALGEB result;
    char maple_cmd[512];
    int i;
    double x, step;
    
    if (!function_str) return;
    
    printf("Calculando função: %s\n", function_str);
    
    /* Limpar pontos antigos */
    if (func_points) {
        free(func_points);
        func_points = NULL;
    }
    if (deriv_points) {
        free(deriv_points);
        deriv_points = NULL;
    }
    
    num_points = MAX_POINTS;
    func_points = (double*)malloc(2 * num_points * sizeof(double));
    deriv_points = (double*)malloc(2 * num_points * sizeof(double));
    
    if (!func_points || !deriv_points) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    
    /* Definir a função no Maple */
    sprintf(maple_cmd, "f := x -> %s:", function_str);
    result = EvalMapleStatement(kv, maple_cmd);
    if (!result) {
        printf("Erro ao definir função no Maple\n");
        return;
    }
    
    /* Calcular derivada */
    sprintf(maple_cmd, "df := diff(%s, x):", function_str);
    result = EvalMapleStatement(kv, maple_cmd);
    if (!result) {
        printf("Erro ao calcular derivada\n");
        return;
    }
    
    /* Calcular integral de x_min a x_max */
    sprintf(maple_cmd, "int_val := evalf(int(%s, x=%f..%f)):", 
            function_str, x_min, x_max);
    result = EvalMapleStatement(kv, maple_cmd);
    
    /* Extrair valor da integral */
    result = EvalMapleStatement(kv, "int_val;");
    if (result && !IsMapleNULL(kv, result)) {
        char *str_result = MapleToString(kv, result);
        if (str_result && strlen(str_result) > 0) {
            integral_value = atof(str_result);
            printf("Integral [%.2f, %.2f] = %f\n", x_min, x_max, integral_value);
        }
    }
    
    /* Calcular pontos da função e derivada */
    step = (x_max - x_min) / (num_points - 1);
    
    for (i = 0; i < num_points; i++) {
        x = x_min + i * step;
        
        func_points[2*i] = x;
        deriv_points[2*i] = x;
        
        /* Avaliar f(x) */
        sprintf(maple_cmd, "evalf(subs(x=%f, %s));", x, function_str);
        result = EvalMapleStatement(kv, maple_cmd);
        
        if (result && !IsMapleNULL(kv, result)) {
            char *str_val = MapleToString(kv, result);
            if (str_val && strlen(str_val) > 0) {
                double y = atof(str_val);
                /* Limitar valores extremos */
                if (y > 10.0) y = 10.0;
                else if (y < -10.0) y = -10.0;
                else if (isnan(y) || isinf(y)) y = 0.0;
                func_points[2*i + 1] = y;
            } else {
                func_points[2*i + 1] = 0.0;
            }
        } else {
            func_points[2*i + 1] = 0.0;
        }
        
        /* Avaliar df/dx */
        sprintf(maple_cmd, "evalf(subs(x=%f, df));", x);
        result = EvalMapleStatement(kv, maple_cmd);
        
        if (result && !IsMapleNULL(kv, result)) {
            char *str_val = MapleToString(kv, result);
            if (str_val && strlen(str_val) > 0) {
                double dy = atof(str_val);
                if (dy > 10.0) dy = 10.0;
                else if (dy < -10.0) dy = -10.0;
                else if (isnan(dy) || isinf(dy)) dy = 0.0;
                deriv_points[2*i + 1] = dy;
            } else {
                deriv_points[2*i + 1] = 0.0;
            }
        } else {
            deriv_points[2*i + 1] = 0.0;
        }
    }
    
    printf("✓ %d pontos calculados\n", num_points);
}

/* ============================================
 * FUNÇÕES OPENGL - DESENHO
 * ============================================ */

void CDECL changeSize(int w, int h) 
{
    if (h < 0) h = 1;
    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}

void drawAxes(void)
{
    int i;
    
    glColor3f(0.3f, 0.3f, 0.3f);
    
    /* Eixo X */
    glBegin(GL_LINES);
    glVertex2d(-6.0, 0.0);
    glVertex2d(6.0, 0.0);
    glEnd();
    
    /* Eixo Y */
    glBegin(GL_LINES);
    glVertex2d(0.0, -6.0);
    glVertex2d(0.0, 6.0);
    glEnd();
    
    /* Marcas no eixo X */
    glColor3f(0.2f, 0.2f, 0.2f);
    for (i = -5; i <= 5; i++) {
        if (i == 0) continue;
        glBegin(GL_LINES);
        glVertex2d(i, -0.1);
        glVertex2d(i, 0.1);
        glEnd();
    }
    
    /* Marcas no eixo Y */
    for (i = -5; i <= 5; i++) {
        if (i == 0) continue;
        glBegin(GL_LINES);
        glVertex2d(-0.1, i);
        glVertex2d(0.1, i);
        glEnd();
    }
}

void drawFunction(double *points, int n, float r, float g, float b, float width_line)
{
    int i;
    
    if (!points || n == 0) return;
    
    glColor3f(r, g, b);
    glLineWidth(width_line);
    
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < n; i++) {
        double x = points[2*i];
        double y = points[2*i + 1];
        
        /* Pular valores inválidos */
        if (isnan(y) || isinf(y)) continue;
        
        glVertex2d(x, y);
    }
    glEnd();
    
    glLineWidth(1.0f);
}

void drawText(float x, float y, const char *text, float r, float g, float b)
{
    size_t i;
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(FONT, text[i]);
    }
}

void drawInfo(void)
{
    char buf[256];
    
    /* Função atual */
    if (current_function) {
        sprintf(buf, "f(x) = %s", current_function);
        drawText(-5.8f, 5.5f, buf, 1.0f, 1.0f, 0.0f);
    }
    
    /* Integral */
    if (show_integral) {
        sprintf(buf, "Integral [%.1f, %.1f] = %.4f", x_min, x_max, integral_value);
        drawText(-5.8f, 5.0f, buf, 0.0f, 1.0f, 1.0f);
    }
    
    /* Legenda */
    if (show_derivative) {
        drawText(-5.8f, -5.2f, "Azul: f(x)  Verde: f'(x)", 0.8f, 0.8f, 0.8f);
    }
    
    /* Instruções */
    drawText(-5.8f, -5.7f, "i:integral d:derivada c:limpar q:sair", 0.5f, 0.5f, 0.5f);
}

void CDECL renderScene(void) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawAxes();
    
    /* Desenhar função principal */
    if (func_points && num_points > 0) {
        drawFunction(func_points, num_points, 0.0f, 0.5f, 1.0f, 2.0f);
    }
    
    /* Desenhar derivada */
    if (show_derivative && deriv_points && num_points > 0) {
        drawFunction(deriv_points, num_points, 0.0f, 1.0f, 0.0f, 1.5f);
    }
    
    /* Desenhar área da integral (simplificado) */
    if (show_integral && func_points && num_points > 0) {
        int i;
        glColor4f(0.0f, 1.0f, 1.0f, 0.2f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i < num_points; i++) {
            double x = func_points[2*i];
            double y = func_points[2*i + 1];
            if (x >= x_min && x <= x_max && !isnan(y) && !isinf(y)) {
                glVertex2d(x, 0.0);
                glVertex2d(x, y);
            }
        }
        glEnd();
        
        glDisable(GL_BLEND);
    }
    
    drawInfo();
    
    glutSwapBuffers();
}

/* ============================================
 * CONTROLES
 * ============================================ */

void CDECL processKeys(unsigned char key, int x, int y) 
{
    (void)x; (void)y;
    
    if (key == 'q' || key == 'Q') {
        exit(0);
    }
    else if (key == 'c' || key == 'C') {
        if (func_points) free(func_points);
        if (deriv_points) free(deriv_points);
        func_points = NULL;
        deriv_points = NULL;
        num_points = 0;
        current_function = NULL;
        show_integral = 0;
        show_derivative = 0;
    }
    else if (key == 'i' || key == 'I') {
        show_integral = !show_integral;
        printf("Integral: %s\n", show_integral ? "ON" : "OFF");
    }
    else if (key == 'd' || key == 'D') {
        show_derivative = !show_derivative;
        printf("Derivada: %s\n", show_derivative ? "ON" : "OFF");
    }
}

void CDECL pickFunction(int option) 
{
    current_function = PresetFunctions[option];
    calculate_function_points(current_function);
}

/* ============================================
 * INICIALIZAÇÃO
 * ============================================ */

void initOpenGL(int argc, char **argv)
{
    int i;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Plotador de Funções - OpenMaple");
    
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processKeys);

    glutCreateMenu(pickFunction);
    for (i = 0; PresetFunctions[i] != NULL; ++i) 
        glutAddMenuEntry(PresetFunctions[i], i);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    /* Fundo preto */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    /* Configurar projeção ortogonal */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-6.0, 6.0, -6.0, 6.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void initMaple(int argc, char *argv[])
{
    MCallBackVectorDesc cb = { textCallBack, errorCallBack, 0, 0, 0, 0, 0, 0 };
    char err[2048];
    char *maple_dir;

    if ((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL) {
        printf("Erro ao inicializar Maple: %s\n", err);
        exit(1);
    }
    
    printf("✓ Maple inicializado\n");

    /* Configurar libname (lazy, como line.c) */
    if ((maple_dir = getenv("MAPLE")) || (maple_dir = getenv("MAPLE_ROOT"))) {
        char *libpath = malloc((5 + strlen(maple_dir)) * sizeof(char));
        sprintf(libpath, "%s/lib", maple_dir);
        MapleLibName(kv, ToMapleString(kv, libpath));
        free(libpath);
        printf("✓ Libname configurado via $MAPLE\n");
    }
    
    /* Testar com função inicial */
    current_function = PresetFunctions[0];
    calculate_function_points(current_function);
}

/* ============================================
 * MAIN
 * ============================================ */

int main(int argc, char **argv) 
{
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  Plotador Interativo de Funções                       ║\n");
    printf("║  OpenMaple + OpenGL                                    ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    initMaple(argc, argv);
    initOpenGL(argc, argv);
    
    printf("\nControles:\n");
    printf("  i - Mostrar/esconder integral\n");
    printf("  d - Mostrar/esconder derivada\n");
    printf("  c - Limpar\n");
    printf("  q - Sair\n");
    printf("  Botão direito - Menu de funções\n\n");
    
    glutMainLoop();

    return 0;
}
