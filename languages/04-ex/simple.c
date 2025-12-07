/* advanced_maple_optimization.c - Exemplo Completo de Otimização
 * Simbólica e Numérica com Maple C API */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maplec.h"

/* ============================================
 * CALLBACKS (Comunicação com o Kernel Maple)
 * ============================================ */

// Callback para mensagens de texto e saída padrão do Maple
static void M_DECL textCallBack(void* /* data */,
                                int /* tag */,
                                const char* output)
{
    printf(">> Maple: %s\n", output);
}

// Callback para mensagens de erro do Maple
static void M_DECL errorCallBack(void* /* data */,
                                 M_INT       offset,
                                 const char* msg)
{
    fprintf(stderr,
            "❌ Erro Maple [offset %ld]: %s\n",
            (long)offset,
            msg);
}

/* ============================================
 * FUNÇÃO DE EXEMPLO: Otimização
 * CORREÇÃO FINAL: Removido MapleGetVariable; usamos
 * EvalMapleStatement para recuperar o valor da variável Maple após
 * o cálculo.
 * ============================================ */
void example_optimization(MKernelVector kv)
{
    printf("\n=== EXEMPLO COMPLETO: Otimização Simbólica e "
           "Numérica ===\n");

    // Carregar o pacote de otimização
    EvalMapleStatement(kv, "with(Optimization):");
    printf("Pacote 'Optimization' carregado.\n");

    // --- 1. Otimização Simbólica (Mínimo de uma Função) ---
    printf("\n## 1. Otimização Simbólica (Função de 1 Variável)\n");

    // 1. Definir a função no Maple
    EvalMapleStatement(kv, "f := x -> x^4 - 4*x^2 + 1;");

    // 2. Executar Minimize e ARMAZENAR o resultado na variável
    // Maple 'min_result' O EvalMapleStatement retorna o valor da
    // última expressão.
    EvalMapleStatement(
        kv, "min_result := Minimize(f(x), x=-3..3, 'location');");

    // 3. RECUPERAR o valor avaliando a variável 'min_result' no
    // Maple
    ALGEB min_simbolico_result =
        EvalMapleStatement(kv, "min_result;");

    printf("Função: f(x) = x^4 - 4x^2 + 1\n");
    printf("Mínimo Simbólico: ");
    MapleALGEB_Printf(kv, "%a\n", min_simbolico_result);

    // --- 2. Otimização Numérica com Restrições ---
    printf("\n## 2. Otimização Numérica com Restrições "
           "(Não-Linear)\n");

    printf("Maximizar: g(x, y) = x*y\n");
    printf("Restrição: x^2 + y^2 <= 1\n");

    // Executar Maximize e ARMAZENAR o resultado na variável Maple
    // 'max_result'
    EvalMapleStatement(kv,
                       "max_result := Maximize(x*y, {x^2 + y^2 <= "
                       "1}, initialpoint = [x=0.5, y=0.5]);");

    // RECUPERAR o valor avaliando a variável 'max_result' no Maple
    ALGEB max_numerico_result =
        EvalMapleStatement(kv, "max_result;");

    printf("Resultado Numérico Completo: ");
    MapleALGEB_Printf(kv, "%a\n", max_numerico_result);

    // ... dentro de example_optimization(MKernelVector kv) ...

    // --- 3. Extraindo Valor e Coordenadas da Solução ---
    printf("\n## 3. Extraindo Valor e Coordenadas da Solução "
           "(Convertendo para C) ===\n");

    // 1. Extrair o valor máximo (primeiro elemento de max_result)
    // Executamos a expressão no Maple:
    ALGEB max_value_maple =
        EvalMapleStatement(kv, "max_result[1];");
    double max_value_c =
        MapleToFloat64(kv, max_value_maple);  // CONVERSÃO PARA C

    printf("Valor Máximo Encontrado (C double): %.8f\n",
           max_value_c);

    // 2. Extrair o valor de X (segundo elemento de max_result, que
    // é um set/list de atribuições) Precisamos acessar o valor x do
    // conjunto de resultados: max_result[2][1]
    ALGEB x_assign_maple = EvalMapleStatement(
        kv, "op(1, max_result[2]);");  // Pega a primeira atribuição
                                       // {x=...}

    // Agora, extraímos o valor real do lado direito (rhs) da
    // atribuição
    ALGEB x_value_maple =
        EvalMapleStatement(kv, "rhs(op(1, max_result[2]));");
    double x_value_c =
        MapleToFloat64(kv, x_value_maple);  // CONVERSÃO PARA C

    printf("Coordenada X (C double): %.8f\n", x_value_c);

    // 3. (Opcional) Se você quiser manter a saída simbólica do
    // conjunto de coordenadas:
    ALGEB coordenadas_simbolica =
        EvalMapleStatement(kv, "max_result[2];");
    printf("Coordenadas no Máximo (Saída Simbólica): ");
    MapleALGEB_Printf(kv, "%a\n", coordenadas_simbolica);
}

/* ============================================
 * MAIN
 * ============================================ */
int main(int argc, char* argv[])
{
    char          err[2048];
    MKernelVector kv;

    // Definir o vetor de Callbacks
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

    // Inicializar Maple
    printf("🍁 Inicializando Maple...\n");
    if((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL)
    {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado com sucesso!\n");

    // Executar o exemplo de otimização
    example_optimization(kv);

    printf("\n✅ Exemplo de Otimização executado com sucesso!\n");

    StopMaple(kv);
    return 0;
}
