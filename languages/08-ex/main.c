/* main.c - Exemplo de Otimização Simbólica e Numérica com extração
 * de valores em C */

#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

/* ============================================
 * CALLBACKS (Comunicação com o Kernel Maple)
 * ============================================ */

// Usamos (void) para evitar warnings de parâmetros não utilizados
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
 * FUNÇÃO DE EXEMPLO: Otimização (Extração de Valores em C)
 * ============================================ */
void example_optimization_extraction(MKernelVector kv)
{

    // Variáveis ALGEB para armazenar resultados Maple
    ALGEB max_value_maple, x_value_maple, y_value_maple;

    // Variáveis C para armazenar resultados convertidos
    double max_value_c, x_value_c, y_value_c;

    printf(
        "\n=== EXEMPLO DE EXTRAÇÃO DE VALORES (ALGEB -> C) ===\n");

    // Carregar o pacote de otimização
    EvalMapleStatement(kv, "with(Optimization):");
    printf("Pacote 'Optimization' carregado.\n");

    // --- 1. Otimização Numérica com Restrições ---
    printf("\n## 1. Maximização de x*y em x^2 + y^2 <= 1\n");

    // Executar Maximize e ARMAZENAR o resultado na variável Maple
    // 'max_result' O resultado é uma lista: [valor máximo,
    // {coordenadas}]
    EvalMapleStatement(kv,
                       "max_result := Maximize(x*y, {x^2 + y^2 <= "
                       "1}, initialpoint = [x=0.5, y=0.5]);");

    printf("Executado: max_result := Maximize(x*y, {x^2 + y^2 <= "
           "1}, ...)\n");

    // --- 2. Extração do Valor Máximo (Componente Numérico) ---
    printf("\n## 2. Extraindo Valor Máximo (max_result[1])\n");

    // 2.1. Avaliar max_result[1] para obter o resultado ALGEB do
    // valor máximo (0.5)
    max_value_maple = EvalMapleStatement(kv, "max_result[1];");

    // 2.2. CONVERTER o resultado ALGEB (número racional/float) para
    // double do C
    max_value_c = MapleToFloat64(kv, max_value_maple);

    printf("Valor Máximo Encontrado (C double): **%.10f**\n",
           max_value_c);

    // --- 3. Extração das Coordenadas (Componentes Numéricos) ---
    printf("\n## 3. Extraindo Coordenadas X e Y\n");

    // As coordenadas estão no formato {x=valor, y=valor}.

    // 3.1. Extrair o valor de X (rhs da primeira atribuição)
    // op(1, max_result[2]) -> 'x = valor'
    // rhs(...) -> 'valor'
    x_value_maple =
        EvalMapleStatement(kv, "rhs(op(1, max_result[2]));");
    x_value_c = MapleToFloat64(kv, x_value_maple);

    // 3.2. Extrair o valor de Y (rhs da segunda atribuição)
    y_value_maple =
        EvalMapleStatement(kv, "rhs(op(2, max_result[2]));");
    y_value_c = MapleToFloat64(kv, y_value_maple);

    printf("Coordenada X (C double): **%.10f**\n", x_value_c);
    printf("Coordenada Y (C double): **%.10f**\n", y_value_c);

    // Exemplo de como usar os valores em C (lógica)
    if(x_value_c > 0.707 && y_value_c > 0.707)
    {
        printf("\n=> Lógica C: Coordenadas estão no primeiro "
               "quadrante. Confirmação do cálculo.\n");
    }
    else
    {
        printf("\n=> Lógica C: Algo estranho aconteceu com as "
               "coordenadas.\n");
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
        textCallBack,
        errorCallBack,
        0, /* statusCallBack */
        0, /* readLineCallBack */
        0, /* redirectCallBack */
        0, /* streamCallBack */
        0, /* queryInterrupt */
        0  /* callBackCallBack */
    };

    printf(
        "=== Exemplos de Otimização (Extração de Valores C) ===\n");
    printf("🍁 Inicializando Maple...\n");

    // Inicialização do kernel Maple
    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if(kv == NULL)
    {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado com sucesso!\n");

    /* Configurar libname, caso necessário para o Maple 2021 */
    printf("\nConfigurando bibliotecas Maple...\n");
    EvalMapleStatement(
        kv, "libname := \"/opt/maple2021/lib\", libname;");
    printf("✓ Bibliotecas configuradas\n");

    /* Executar o novo exemplo focado na extração */
    example_optimization_extraction(kv);

    printf("\n✅ Exemplo de Extração executado com sucesso!\n");

    StopMaple(kv);
    return 0;
}
