/* algebra.c
 *
 * Exemplos de uso da Maple C API com Maple 2021:
 *  - Produto de matrizes
 *  - Cálculo de determinantes
 *  - Solução de equação do segundo grau
 *  - Avaliação de polinômio em x=5
 *
 * Baseado nos exemplos line.c e main.c
 * Compilar: make
 * Executar: make run
 */

#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

/* ============================================
 * CALLBACKS (Comunicação com o Kernel Maple)
 * ============================================ */

static void M_DECL textCallBack(void *data, int tag, const char *output)
{
    (void)data;
    (void)tag;
    printf("%s\n", output);
}

static void M_DECL errorCallBack(void *data, M_INT offset, const char *msg)
{
    (void)data;
    (void)offset;
    fprintf(stderr, "❌ Erro: %s\n", msg);
}

/* ============================================
 * EXEMPLOS
 * ============================================ */

static void example_matrices(MKernelVector kv)
{
    ALGEB r;
    
    printf("\n=== PRODUTO DE MATRIZES ===\n");

    /* Carregar o pacote LinearAlgebra */
    printf("Carregando pacote LinearAlgebra...\n");
    EvalMapleStatement(kv, "with(LinearAlgebra):");
    printf("✓ Pacote carregado\n\n");

    /* Definir matriz A (2x3) */
    printf("Matriz A (2x3):\n");
    EvalMapleStatement(kv, "A := Matrix([[1, 2, 3], [4, 5, 6]]);");
    r = EvalMapleStatement(kv, "print(A);");
    printf("\n");

    /* Definir matriz B (3x2) */
    printf("Matriz B (3x2):\n");
    EvalMapleStatement(kv, "B := Matrix([[7, 8], [9, 10], [11, 12]]);");
    r = EvalMapleStatement(kv, "print(B);");
    printf("\n");

    /* Produto A * B (resultado: 2x2) */
    printf("Produto C = A . B (2x2):\n");
    EvalMapleStatement(kv, "C := A . B;");
    r = EvalMapleStatement(kv, "print(C);");
    printf("\n");
}

static void example_determinant(MKernelVector kv)
{
    ALGEB r;
    
    printf("\n=== DETERMINANTE DE MATRIZES ===\n");

    /* Matriz 2x2 */
    printf("Matriz D (2x2):\n");
    EvalMapleStatement(kv, "D := Matrix([[3, 8], [4, 6]]);");
    r = EvalMapleStatement(kv, "print(D);");
    
    printf("\nDeterminante de D:\n");
    EvalMapleStatement(kv, "det_D := Determinant(D);");
    r = EvalMapleStatement(kv, "print(det_D);");
    printf("(Esperado: 3*6 - 8*4 = 18 - 32 = -14)\n\n");

    /* Matriz 3x3 */
    printf("Matriz E (3x3):\n");
    EvalMapleStatement(kv, "E := Matrix([[1, 2, 3], [0, 4, 5], [1, 0, 6]]);");
    r = EvalMapleStatement(kv, "print(E);");
    
    printf("\nDeterminante de E:\n");
    EvalMapleStatement(kv, "det_E := Determinant(E);");
    r = EvalMapleStatement(kv, "print(det_E);");
    printf("\n");

    /* Matriz 4x4 */
    printf("Matriz F (4x4):\n");
    EvalMapleStatement(kv, "F := Matrix([[2, 1, 0, 0], [1, 2, 1, 0], [0, 1, 2, 1], [0, 0, 1, 2]]);");
    r = EvalMapleStatement(kv, "print(F);");
    
    printf("\nDeterminante de F:\n");
    EvalMapleStatement(kv, "det_F := Determinant(F);");
    r = EvalMapleStatement(kv, "print(det_F);");
    printf("\n");
}

static void example_quadratic(MKernelVector kv)
{
    ALGEB r;
    
    printf("\n=== SOLUÇÃO DE EQUAÇÃO DO SEGUNDO GRAU ===\n");

    /* Equação 1: x^2 - 5x + 6 = 0 (raízes: 2 e 3) */
    printf("Equação 1: x² - 5x + 6 = 0\n");
    EvalMapleStatement(kv, "eq1 := x^2 - 5*x + 6 = 0;");
    r = EvalMapleStatement(kv, "print(eq1);");
    
    printf("\nSoluções:\n");
    EvalMapleStatement(kv, "sol1 := solve(eq1, x);");
    r = EvalMapleStatement(kv, "print(sol1);");
    printf("(Esperado: x = 2 ou x = 3)\n\n");

    /* Equação 2: 2x^2 + 3x - 2 = 0 */
    printf("Equação 2: 2x² + 3x - 2 = 0\n");
    EvalMapleStatement(kv, "eq2 := 2*x^2 + 3*x - 2 = 0;");
    r = EvalMapleStatement(kv, "print(eq2);");
    
    printf("\nSoluções (exatas):\n");
    EvalMapleStatement(kv, "sol2 := solve(eq2, x);");
    r = EvalMapleStatement(kv, "print(sol2);");
    
    printf("\nSoluções (numéricas):\n");
    EvalMapleStatement(kv, "sol2_num := evalf(solve(eq2, x));");
    r = EvalMapleStatement(kv, "print(sol2_num);");
    printf("\n");

    /* Equação 3: x^2 + 2x + 5 = 0 (raízes complexas) */
    printf("Equação 3: x² + 2x + 5 = 0 (raízes complexas)\n");
    EvalMapleStatement(kv, "eq3 := x^2 + 2*x + 5 = 0;");
    r = EvalMapleStatement(kv, "print(eq3);");
    
    printf("\nSoluções (complexas):\n");
    EvalMapleStatement(kv, "sol3 := solve(eq3, x);");
    r = EvalMapleStatement(kv, "print(sol3);");
    
    printf("\nSoluções (numéricas):\n");
    EvalMapleStatement(kv, "sol3_num := evalf(solve(eq3, x));");
    r = EvalMapleStatement(kv, "print(sol3_num);");
    printf("\n");

    /* Usando fsolve para solução numérica direta */
    printf("Usando fsolve (força solução numérica):\n");
    printf("fsolve(x² - 5x + 6 = 0): ");
    r = EvalMapleStatement(kv, "print(fsolve(x^2 - 5*x + 6 = 0, x));");
    printf("\n");
}

static void example_polynomial(MKernelVector kv)
{
    ALGEB r;
    
    printf("\n=== AVALIAÇÃO DE POLINÔMIO ===\n");

    /* Definir polinômio: p(x) = 2x^3 - 3x^2 + 5x - 7 */
    printf("Polinômio: p(x) = 2x³ - 3x² + 5x - 7\n");
    EvalMapleStatement(kv, "p := x -> 2*x^3 - 3*x^2 + 5*x - 7;");
    r = EvalMapleStatement(kv, "print(p(x));");
    printf("\n");

    /* Avaliar em x = 5 */
    printf("Avaliação em x = 5:\n");
    printf("p(5) = ");
    EvalMapleStatement(kv, "p5 := p(5);");
    r = EvalMapleStatement(kv, "print(p5);");
    printf("\nCálculo: 2(125) - 3(25) + 5(5) - 7\n");
    printf("       = 250 - 75 + 25 - 7 = 193\n\n");

    /* Avaliar em vários pontos */
    printf("Avaliação em múltiplos pontos:\n");
    printf("p(0) = ");
    r = EvalMapleStatement(kv, "print(p(0));");
    
    printf("p(1) = ");
    r = EvalMapleStatement(kv, "print(p(1));");
    
    printf("p(2) = ");
    r = EvalMapleStatement(kv, "print(p(2));");
    
    printf("p(-1) = ");
    r = EvalMapleStatement(kv, "print(p(-1));");
    printf("\n");

    /* Derivada do polinômio */
    printf("Derivada p'(x):\n");
    EvalMapleStatement(kv, "dp := diff(p(x), x);");
    r = EvalMapleStatement(kv, "print(dp);");
    
    printf("\nDerivada avaliada em x = 5:\n");
    printf("p'(5) = ");
    EvalMapleStatement(kv, "dp5 := eval(dp, x=5);");
    r = EvalMapleStatement(kv, "print(dp5);");
    printf("\n");

    /* Raízes do polinômio */
    printf("Raízes de p(x) = 0:\n");
    EvalMapleStatement(kv, "roots := fsolve(p(x) = 0, x);");
    r = EvalMapleStatement(kv, "print(roots);");
    printf("\n");

    /* Polinômio com coeficientes simbólicos */
    printf("Polinômio simbólico: q(x) = ax² + bx + c\n");
    EvalMapleStatement(kv, "q := (x, a, b, c) -> a*x^2 + b*x + c;");
    
    printf("\nq(5, 2, -3, 1) = ");
    EvalMapleStatement(kv, "q5 := q(5, 2, -3, 1);");
    r = EvalMapleStatement(kv, "print(q5);");
    printf("Cálculo: 2(25) + (-3)(5) + 1 = 50 - 15 + 1 = 36\n\n");
}

/* ============================================
 * CONFIGURAÇÃO INICIAL DO MAPLE
 * ============================================ */

static void init_maple_libraries(MKernelVector kv)
{
    char *maple_dir;
    
    /* Tentar configurar libname via variável de ambiente, como no line.c */
    if ((maple_dir = getenv("MAPLE")) || (maple_dir = getenv("MAPLE_ROOT"))) {
        char *libpath;
        libpath = (char*)malloc((5 + strlen(maple_dir)) * sizeof(char));
        sprintf(libpath, "%s/lib", maple_dir);
        MapleLibName(kv, ToMapleString(kv, libpath));
        free(libpath);
    } else {
        /* Fallback: configuração direta */
        EvalMapleStatement(kv, "libname := \"/opt/maple2021/lib\", libname;");
    }
}

/* ============================================
 * MAIN
 * ============================================ */

int main(int argc, char *argv[])
{
    char err[2048];
    MKernelVector kv;

    MCallBackVectorDesc cb = {
        textCallBack,
        errorCallBack,
        0, /* statusCallBack */
        0, /* readLineCallBack */
        0, /* redirectCallBack */
        0, /* streamCallBack */
        0, /* queryInterrupt */
        0  /* callBackCallBack */
    };

    printf("=== Exemplos de Álgebra e Polinômios ===\n");
    printf("🍁 Inicializando Maple...\n");

    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if (kv == NULL) {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado com sucesso!\n");

    /* Configurar bibliotecas (método do line.c) */
    printf("\nConfigurando bibliotecas Maple...\n");
    init_maple_libraries(kv);
    printf("✓ Bibliotecas configuradas\n");

    /* Executar exemplos */
    example_matrices(kv);
    example_determinant(kv);
    example_quadratic(kv);
    example_polynomial(kv);

    printf("\n✅ Todos os exemplos executados com sucesso!\n");

    StopMaple(kv);
    return 0;
}
