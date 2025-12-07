/* ode_solver.c - Resolução de EDOs com Maple C API */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maplec.h"

static void M_DECL textCallBack(void*       data,
                                int         tag,
                                const char* output)
{
    printf("%s\n", output);
}

static void M_DECL errorCallBack(void*       data,
                                 M_INT       offset,
                                 const char* msg)
{
    fprintf(stderr, "❌ Erro [%ld]: %s\n", (long)offset, msg);
}

/* ============================================
 * EDO de Primeira Ordem
 * ============================================ */
void solve_first_order_ode(MKernelVector kv)
{
    printf("\n=== EDO de 1ª Ordem ===\n");
    printf("dy/dx = x + y, y(0) = 1\n\n");

    // Resolver analiticamente
    ALGEB solution = EvalMapleStatement(
        kv, "dsolve({diff(y(x), x) = x + y(x), y(0) = 1}, y(x));");

    printf("Solução analítica:\n");
    MapleALGEB_Printf(kv, "%a\n\n", solution);

    // Resolver numericamente
    printf("Solução numérica (método Runge-Kutta):\n");
    ALGEB numeric = EvalMapleStatement(
        kv,
        "dsol := dsolve({diff(y(x), x) = x + y(x), y(0) = 1}, "
        "y(x), numeric, method=rkf45);");

    // Avaliar em pontos específicos
    printf("\nValores em pontos específicos:\n");
    for(double x = 0.0; x <= 2.0; x += 0.5)
    {
        char cmd[256];
        sprintf(cmd, "dsol(%.1f);", x);
        ALGEB point = EvalMapleStatement(kv, cmd);
        printf("x = %.1f: ", x);
        MapleALGEB_Printf(kv, "%a\n", point);
    }
}

/* ============================================
 * Sistema de EDOs
 * ============================================ */
void solve_system_odes(MKernelVector kv)
{
    printf("\n\n=== Sistema de EDOs (Presa-Predador de "
           "Lotka-Volterra) ===\n");
    printf("dx/dt = x - 0.1*x*y\n");
    printf("dy/dt = -y + 0.02*x*y\n");
    printf("x(0) = 40, y(0) = 9\n\n");

    ALGEB system = EvalMapleStatement(
        kv,
        "sys := {diff(x(t), t) = x(t) - 0.1*x(t)*y(t), "
        "       diff(y(t), t) = -y(t) + 0.02*x(t)*y(t), "
        "       x(0) = 40, y(0) = 9}:");

    ALGEB sol = EvalMapleStatement(
        kv, "dsol := dsolve(sys, {x(t), y(t)}, numeric);");

    printf("Evolução temporal do sistema:\n");
    printf("%-10s %-15s %-15s\n",
           "t",
           "x(t) [Presa]",
           "y(t) [Predador]");
    printf("-------------------------------------------\n");

    for(int i = 0; i <= 20; i++)
    {
        char cmd[128];
        sprintf(cmd, "dsol(%d);", i);
        ALGEB point = EvalMapleStatement(kv, cmd);
        MapleALGEB_Printf(kv, "%a\n", point);
    }
}

/* ============================================
 * EDO de Segunda Ordem (Oscilador Harmônico)
 * ============================================ */
void solve_harmonic_oscillator(MKernelVector kv)
{
    printf("\n\n=== Oscilador Harmônico Amortecido ===\n");
    printf("d²y/dx² + 2*γ*dy/dx + ω²*y = 0\n");
    printf("γ = 0.5 (amortecimento), ω = 2 (frequência)\n");
    printf("y(0) = 1, y'(0) = 0\n\n");

    // Definir parâmetros
    EvalMapleStatement(kv, "gamma := 0.5: omega := 2:");

    // Resolver
    ALGEB solution = EvalMapleStatement(
        kv,
        "dsolve({diff(y(t), t$2) + 2*gamma*diff(y(t), t) + "
        "omega^2*y(t) = 0, "
        "        y(0) = 1, D(y)(0) = 0}, y(t));");

    printf("Solução analítica:\n");
    MapleALGEB_Printf(kv, "%a\n", solution);

    // Análise de frequência
    printf("\nAmplitude em t=5:\n");
    ALGEB amp =
        EvalMapleStatement(kv,
                           "eval(rhs(dsolve({diff(y(t), t$2) + "
                           "2*0.5*diff(y(t), t) + 4*y(t) = 0, "
                           "y(0) = 1, D(y)(0) = 0}, y(t))), t=5);");
    MapleALGEB_Printf(kv, "%a\n", amp);
}

/* ============================================
 * EDO com Entrada Forçada
 * ============================================ */
void solve_forced_oscillator(MKernelVector kv)
{
    printf("\n\n=== Oscilador Forçado (Ressonância) ===\n");
    printf("d²y/dt² + y = cos(t)\n");
    printf("y(0) = 0, y'(0) = 0\n\n");

    ALGEB solution = EvalMapleStatement(
        kv,
        "dsolve({diff(y(t), t$2) + y(t) = cos(t), "
        "        y(0) = 0, D(y)(0) = 0}, y(t));");

    printf("Solução:\n");
    MapleALGEB_Printf(kv, "%a\n", solution);

    // Verificar ressonância
    printf("\nVerificando amplitude em t=10*Pi:\n");
    ALGEB resonance = EvalMapleStatement(
        kv,
        "eval(rhs(dsolve({diff(y(t), t$2) + y(t) = cos(t), "
        "y(0) = 0, D(y)(0) = 0}, y(t))), t=10*Pi);");
    MapleALGEB_Printf(kv, "%a\n", resonance);
}

/* ============================================
 * Análise de Estabilidade
 * ============================================ */
void stability_analysis(MKernelVector kv)
{
    printf("\n\n=== Análise de Estabilidade ===\n");

    // Sistema linearizado em torno de um ponto de equilíbrio
    printf("Sistema: dx/dt = -x + y, dy/dt = -x - y\n\n");

    // Matriz Jacobiana
    ALGEB jacobian =
        EvalMapleStatement(kv,
                           "with(LinearAlgebra): "
                           "J := Matrix([[-1, 1], [-1, -1]]);");

    printf("Matriz Jacobiana:\n");
    MapleALGEB_Printf(kv, "%a\n\n", jacobian);

    // Autovalores
    ALGEB eigenvals = EvalMapleStatement(
        kv, "Eigenvalues(Matrix([[-1, 1], [-1, -1]]));");

    printf("Autovalores (determinam estabilidade):\n");
    MapleALGEB_Printf(kv, "%a\n", eigenvals);

    // Autovetores
    ALGEB eigenvecs = EvalMapleStatement(
        kv, "Eigenvectors(Matrix([[-1, 1], [-1, -1]]));");

    printf("\nAutovetores:\n");
    MapleALGEB_Printf(kv, "%a\n", eigenvecs);
}

/* ============================================
 * Método de Euler Manual
 * ============================================ */
void manual_euler_method(MKernelVector kv)
{
    printf("\n\n=== Implementação Manual do Método de Euler ===\n");
    printf("dy/dx = -2*y, y(0) = 1\n");
    printf("Solução exata: y = e^(-2x)\n\n");

    double h     = 0.1;  // passo
    double x     = 0.0;
    double y     = 1.0;
    int    steps = 20;

    printf("%-10s %-15s %-15s %-15s\n",
           "x",
           "y (Euler)",
           "y (exata)",
           "Erro");
    printf("-------------------------------------------------------"
           "-\n");

    for(int i = 0; i <= steps; i++)
    {
        // Calcular solução exata
        char exact_cmd[128];
        sprintf(exact_cmd, "evalf(exp(-2*%.2f));", x);
        ALGEB  exact   = EvalMapleStatement(kv, exact_cmd);
        double y_exact = MapleToFloat64(kv, exact);

        // Calcular erro
        double error = fabs(y - y_exact);

        printf("%-10.2f %-15.6f %-15.6f %-15.6e\n",
               x,
               y,
               y_exact,
               error);

        // Passo de Euler: y_new = y + h * f(x, y)
        // onde f(x, y) = -2*y
        y = y + h * (-2.0 * y);
        x = x + h;
    }
}

/* ============================================
 * Campos Vetoriais
 * ============================================ */
void vector_field_analysis(MKernelVector kv)
{
    printf("\n\n=== Análise de Campo Vetorial ===\n");
    printf("Sistema: dx/dt = y, dy/dt = -sin(x)\n");
    printf("(Pêndulo simples)\n\n");

    // Encontrar pontos de equilíbrio
    printf("Pontos de equilíbrio (dy/dt = dx/dt = 0):\n");
    ALGEB equilibria = EvalMapleStatement(
        kv, "solve({y = 0, -sin(x) = 0}, {x, y});");
    MapleALGEB_Printf(kv, "%a\n", equilibria);

    // Trajetórias
    printf("\nAlgumas trajetórias:\n");
    for(int i = 0; i < 3; i++)
    {
        char cmd[256];
        sprintf(cmd,
                "dsol := dsolve({"
                "  diff(x(t), t) = y(t), "
                "  diff(y(t), t) = -sin(x(t)), "
                "  x(0) = %d*0.5, y(0) = 0}, "
                "{x(t), y(t)}, numeric, range=0..10);",
                i);

        EvalMapleStatement(kv, cmd);

        printf("\nCondição inicial: x(0) = %.1f, y(0) = 0\n",
               i * 0.5);
        for(double t = 0; t <= 10; t += 2)
        {
            char eval_cmd[64];
            sprintf(eval_cmd, "dsol(%.1f);", t);
            ALGEB point = EvalMapleStatement(kv, eval_cmd);
            printf("t = %.1f: ", t);
            MapleALGEB_Printf(kv, "%a\n", point);
        }
    }
}

/* ============================================
 * MAIN
 * ============================================ */
int main(int argc, char* argv[])
{
    char          err[2048];
    MKernelVector kv;

    MCallBackVectorDesc cb = {
        textCallBack, errorCallBack, 0, 0, 0, 0, 0, 0};

    printf("🍁 Inicializando Maple para análise de EDOs...\n");

    if((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL)
    {
        fprintf(stderr, "Erro fatal: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado!\n");

    // Executar exemplos
    solve_first_order_ode(kv);
    solve_system_odes(kv);
    solve_harmonic_oscillator(kv);
    solve_forced_oscillator(kv);
    stability_analysis(kv);
    manual_euler_method(kv);
    vector_field_analysis(kv);

    printf("\n\n✅ Análise completa!\n");

    StopMaple(kv);
    return 0;
}
