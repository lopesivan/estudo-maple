/* main.c - Cálculo da Densidade Volumétrica de Carga (Laplaciano em
 * Esféricas) */

#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

/* Constante de permissividade do vácuo */
#define EPSILON_0 "epsilon[0]"

/* ============================================
 * CALLBACKS (Comunicação com o Kernel Maple)
 * ============================================ */

static void M_DECL textCallBack(void*       data,
                                int         tag,
                                const char* output)
{
    (void)data;
    (void)tag;
    printf(">> Maple: %s\n", output);
}

static void M_DECL errorCallBack(void*       data,
                                 M_INT       offset,
                                 const char* msg)
{
    (void)data;
    (void)offset;
    fprintf(stderr, "❌ Erro Maple: %s\n", msg);
}

/* ============================================
 * FUNÇÃO DE CÁLCULO: Densidade de Carga (rho)
 * ============================================ */
void calculate_charge_density(MKernelVector kv)
{

    printf("\n=== CÁLCULO DA DENSIDADE VOLUMÉTRICA DE CARGA (RHO) "
           "===\n");

    // 1. Carregar o pacote e configurar coordenadas esféricas
    // CORREÇÃO: Usamos o pacote VectorCalculus para Laplaciano 3D
    // simples.
    printf("1. Configurando o Maple com VectorCalculus para "
           "Laplaciano em Esféricas...\n");

    EvalMapleStatement(kv, "with(VectorCalculus):");
    // Configura o sistema de coordenadas esféricas padrão 3D (r,
    // phi, theta).
    EvalMapleStatement(kv,
                       "SetCoordinates(spherical[r, phi, theta]);");
    printf("✓ Pacote e Coordenadas Esféricas configuradas (r, phi, "
           "theta).\n");

    // 2. Definir o Potencial V(r, phi, theta)
    // O Maple no VectorCalculus define V como uma função de r, phi
    // (polar), e theta (azimutal). V(r, φ, θ) = 100 + 50r +
    // 150r*sin(φ)
    const char* potential_V = "V := 100 + 50*r + 150*r*sin(phi);";
    EvalMapleStatement(kv, potential_V);

    printf("\n2. Potencial V definido:\n>> Maple: %s\n",
           potential_V);

    // 3. Calcular o Laplaciano do Potencial (nabla^2 V)
    printf("\n3. Calculando o Laplaciano (Laplacian(V)):\n");
    // Armazena e simplifica o Laplaciano na variável Maple
    // 'laplacian_V' A função é 'Laplacian' (Laplaciano) no
    // VectorCalculus.
    EvalMapleStatement(kv,
                       "laplacian_V := simplify(Laplacian(V));");

    // Recuperar e imprimir o Laplaciano
    ALGEB laplacian_algeb = EvalMapleStatement(kv, "laplacian_V;");
    printf("Laplaciano (∇²V) Simbólico: ");
    MapleALGEB_Printf(kv, "%a\n", laplacian_algeb);


    // 4. Aplicar a Equação de Poisson: rho = - epsilon_0 *
    // Laplaciano(V)
    printf("\n4. Aplicando a Equação de Poisson: rho = -epsilon[0] "
           "* (∇²V)\n");

    EvalMapleStatement(kv, "rho := - " EPSILON_0 " * laplacian_V;");

    // Recuperar e imprimir a Densidade de Carga (rho)
    ALGEB rho_algeb = EvalMapleStatement(kv, "rho;");
    printf("Densidade de Carga (ρ) Simbólica: ");
    MapleALGEB_Printf(kv, "%a\n", rho_algeb);

    printf("\nO resultado final é dado por Laplace(V) * "
           "(-epsilon[0]).\n");
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

    printf(
        "=== Eletrostática: Densidade de Carga em Esféricas ===\n");
    printf("🍁 Inicializando Maple...\n");

    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if(kv == NULL)
    {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado com sucesso!\n");

    /* Configurar libname, caso necessário para o Maple 2021 */
    EvalMapleStatement(
        kv, "libname := \"/opt/maple2021/lib\", libname;");

    /* Executar o cálculo */
    calculate_charge_density(kv);

    printf("\n✅ Cálculo finalizado.\n");

    StopMaple(kv);
    return 0;
}
