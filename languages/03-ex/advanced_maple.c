/* advanced_maple.c - Exemplos avançados da API Maple C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "maplec.h"

/* ============================================
 * CALLBACKS
 * ============================================ */

static void M_DECL textCallBack(void* data, int tag, const char* output) {
    printf("%s\n", output);
}

static void M_DECL errorCallBack(void* data, M_INT offset, const char* msg) {
    fprintf(stderr, "❌ Erro Maple [offset %ld]: %s\n", (long)offset, msg);
}

/* ============================================
 * EXEMPLO 1: Operações Matriciais
 * ============================================ */
void example_matrix_operations(MKernelVector kv) {
    printf("\n=== EXEMPLO 1: Operações Matriciais ===\n");
    
    // Carregar módulo LinearAlgebra
    EvalMapleStatement(kv, "with(LinearAlgebra):");
    
    // Criar matriz 3x3
    ALGEB matrix = EvalMapleStatement(kv, 
        "M := Matrix([[1,2,3],[4,5,6],[7,8,9]]);");
    
    printf("Matriz original:\n");
    MapleALGEB_Printf(kv, "%a\n", matrix);
    
    // Calcular determinante
    ALGEB det_cmd = EvalMapleStatement(kv, "Determinant(M);");
    printf("\nDeterminante: ");
    MapleALGEB_Printf(kv, "%a\n", det_cmd);
    
    // Transposta
    ALGEB trans = EvalMapleStatement(kv, "Transpose(M);");
    printf("\nTransposta:\n");
    MapleALGEB_Printf(kv, "%a\n", trans);
    
    // Criar matriz identidade 4x4
    ALGEB identity = EvalMapleStatement(kv, "IdentityMatrix(4);");
    printf("\nMatriz Identidade 4x4:\n");
    MapleALGEB_Printf(kv, "%a\n", identity);
}

/* ============================================
 * EXEMPLO 2: Resolução de Equações
 * ============================================ */
void example_solve_equations(MKernelVector kv) {
    printf("\n=== EXEMPLO 2: Resolução de Equações ===\n");
    
    // Equação quadrática
    printf("Resolver: x^2 - 5*x + 6 = 0\n");
    ALGEB sol1 = EvalMapleStatement(kv, "solve(x^2 - 5*x + 6 = 0, x);");
    MapleALGEB_Printf(kv, "Soluções: %a\n", sol1);
    
    // Sistema de equações lineares
    printf("\nSistema de equações:\n");
    printf("  2x + y = 5\n");
    printf("  x - y = 1\n");
    ALGEB sol2 = EvalMapleStatement(kv, 
        "solve({2*x + y = 5, x - y = 1}, {x, y});");
    MapleALGEB_Printf(kv, "Solução: %a\n", sol2);
    
    // Equação trigonométrica
    printf("\nResolver: sin(x) = 0.5 no intervalo [0, 2*Pi]\n");
    ALGEB sol3 = EvalMapleStatement(kv, 
        "solve(sin(x) = 0.5, x);");
    MapleALGEB_Printf(kv, "Soluções: %a\n", sol3);
}

/* ============================================
 * EXEMPLO 3: Cálculo Diferencial e Integral
 * ============================================ */
void example_calculus(MKernelVector kv) {
    printf("\n=== EXEMPLO 3: Cálculo Diferencial e Integral ===\n");
    
    // Derivada
    printf("f(x) = x^3 * sin(x)\n");
    ALGEB deriv = EvalMapleStatement(kv, "diff(x^3 * sin(x), x);");
    MapleALGEB_Printf(kv, "f'(x) = %a\n", deriv);
    
    // Segunda derivada
    ALGEB deriv2 = EvalMapleStatement(kv, "diff(x^3 * sin(x), x$2);");
    MapleALGEB_Printf(kv, "f''(x) = %a\n", deriv2);
    
    // Integral definida
    printf("\nIntegral de x^2 de 0 a 5:\n");
    ALGEB integral = EvalMapleStatement(kv, "int(x^2, x=0..5);");
    MapleALGEB_Printf(kv, "∫₀⁵ x² dx = %a\n", integral);
    
    // Integral indefinida
    printf("\nIntegral indefinida de e^x * cos(x):\n");
    ALGEB indef = EvalMapleStatement(kv, "int(exp(x)*cos(x), x);");
    MapleALGEB_Printf(kv, "∫ eˣ·cos(x) dx = %a\n", indef);
    
    // Limite
    printf("\nLimite de sin(x)/x quando x→0:\n");
    ALGEB limit = EvalMapleStatement(kv, "limit(sin(x)/x, x=0);");
    MapleALGEB_Printf(kv, "lim(x→0) sin(x)/x = %a\n", limit);
}

/* ============================================
 * EXEMPLO 4: Manipulação de Listas e Sets
 * ============================================ */
void example_lists_and_sets(MKernelVector kv) {
    printf("\n=== EXEMPLO 4: Listas e Conjuntos ===\n");
    
    // Criar lista programaticamente
    ALGEB list = MapleListAlloc(kv, 5);
    MapleListAssign(kv, list, 1, ToMapleInteger(kv, 10));
    MapleListAssign(kv, list, 2, ToMapleInteger(kv, 20));
    MapleListAssign(kv, list, 3, ToMapleInteger(kv, 30));
    MapleListAssign(kv, list, 4, ToMapleFloat(kv, 3.14));
    MapleListAssign(kv, list, 5, ToMapleString(kv, "texto"));
    
    printf("Lista criada: ");
    MapleALGEB_Printf(kv, "%a\n", list);
    
    // Acessar elemento
    ALGEB elem = MapleListSelect(kv, list, 2);
    printf("Segundo elemento: ");
    MapleALGEB_Printf(kv, "%a\n", elem);
    
    // Operações com listas via Maple
    MapleAssign(kv, ToMapleName(kv, "mylist", TRUE), list);
    
    ALGEB sorted = EvalMapleStatement(kv, "sort([5,2,8,1,9]);");
    printf("\nLista ordenada: ");
    MapleALGEB_Printf(kv, "%a\n", sorted);
    
    // Map - aplicar função a todos elementos
    ALGEB mapped = EvalMapleStatement(kv, "map(x -> x^2, [1,2,3,4,5]);");
    printf("Quadrados: ");
    MapleALGEB_Printf(kv, "%a\n", mapped);
    
    // Select - filtrar elementos
    ALGEB filtered = EvalMapleStatement(kv, 
        "select(x -> x > 5, [1,3,6,8,2,9,4]);");
    printf("Maiores que 5: ");
    MapleALGEB_Printf(kv, "%a\n", filtered);
}

/* ============================================
 * EXEMPLO 5: Procedimentos e Funções Custom
 * ============================================ */
void example_procedures(MKernelVector kv) {
    printf("\n=== EXEMPLO 5: Procedimentos Customizados ===\n");
    
    // Definir procedimento Fibonacci
    EvalMapleStatement(kv, 
        "fib := proc(n) "
        "  if n <= 1 then return n; "
        "  else return fib(n-1) + fib(n-2); "
        "  end if; "
        "end proc:");
    
    printf("Sequência de Fibonacci:\n");
    for(int i = 0; i < 10; i++) {
        char cmd[64];
        sprintf(cmd, "fib(%d);", i);
        ALGEB result = EvalMapleStatement(kv, cmd);
        printf("F(%d) = ", i);
        MapleALGEB_Printf(kv, "%a\n", result);
    }
    
    // Procedimento que retorna múltiplos valores
    printf("\nProcedimento com múltiplos retornos:\n");
    EvalMapleStatement(kv,
        "stats := proc(lst) "
        "  local avg, mx, mn; "
        "  avg := add(lst)/nops(lst); "
        "  mx := max(lst); "
        "  mn := min(lst); "
        "  return avg, mx, mn; "
        "end proc:");
    
    ALGEB stats = EvalMapleStatement(kv, "stats([10, 25, 5, 30, 15]);");
    printf("Média, Máximo, Mínimo: ");
    MapleALGEB_Printf(kv, "%a\n", stats);
}

/* ============================================
 * EXEMPLO 6: Álgebra Simbólica
 * ============================================ */
void example_symbolic_algebra(MKernelVector kv) {
    printf("\n=== EXEMPLO 6: Álgebra Simbólica ===\n");
    
    // Expansão
    ALGEB expand = EvalMapleStatement(kv, "expand((x+y)^3);");
    printf("(x+y)³ expandido: ");
    MapleALGEB_Printf(kv, "%a\n", expand);
    
    // Fatoração
    ALGEB factor = EvalMapleStatement(kv, "factor(x^2 - 5*x + 6);");
    printf("x² - 5x + 6 fatorado: ");
    MapleALGEB_Printf(kv, "%a\n", factor);
    
    // Simplificação
    ALGEB simplify = EvalMapleStatement(kv, 
        "simplify(sin(x)^2 + cos(x)^2);");
    printf("sin²(x) + cos²(x) simplificado: ");
    MapleALGEB_Printf(kv, "%a\n", simplify);
    
    // Substituição
    printf("\nSubstituir x=2 em x² + 3x + 1:\n");
    ALGEB subs = EvalMapleStatement(kv, "subs(x=2, x^2 + 3*x + 1);");
    MapleALGEB_Printf(kv, "Resultado: %a\n", subs);
    
    // Resolver simbolicamente
    printf("\nResolver a*x² + b*x + c = 0 para x:\n");
    ALGEB quad = EvalMapleStatement(kv, "solve(a*x^2 + b*x + c = 0, x);");
    MapleALGEB_Printf(kv, "Fórmula de Bhaskara: %a\n", quad);
}

/* ============================================
 * EXEMPLO 7: Gráficos (geração de dados)
 * ============================================ */
void example_plot_data(MKernelVector kv) {
    printf("\n=== EXEMPLO 7: Geração de Dados para Gráfico ===\n");
    
    // Carregar módulo Statistics
    EvalMapleStatement(kv, "with(Statistics):");
    
    // Gerar pontos de sin(x) com evalf
    printf("Gerando pontos para y = sin(x):\n");
    EvalMapleStatement(kv, 
        "points := [seq([evalf(x), evalf(sin(x))], x = 0.0 .. 6.28, 0.5)];");
    
    ALGEB points = EvalMapleStatement(kv, "points;");
    printf("Primeiros 5 pontos: ");
    ALGEB first5 = EvalMapleStatement(kv, "points[1..5];");
    MapleALGEB_Printf(kv, "%a\n", first5);
    
    // Estatísticas dos dados
    printf("\nEstatísticas dos valores y:\n");
    ALGEB yvals = EvalMapleStatement(kv, "ylist := map(p -> p[2], points);");
    ALGEB mean = EvalMapleStatement(kv, "Mean(ylist);");
    ALGEB stdev = EvalMapleStatement(kv, "StandardDeviation(ylist);");
    
    printf("Média: ");
    MapleALGEB_Printf(kv, "%a\n", mean);
    printf("Desvio Padrão: ");
    MapleALGEB_Printf(kv, "%a\n", stdev);
}

/* ============================================
 * EXEMPLO 8: Conversões e Tipos
 * ============================================ */
void example_conversions(MKernelVector kv) {
    printf("\n=== EXEMPLO 8: Conversões de Tipos ===\n");
    
    // Criar diferentes tipos
    ALGEB int_val = ToMapleInteger(kv, 42);
    ALGEB float_val = ToMapleFloat(kv, 3.14159);
    ALGEB bool_val = ToMapleBoolean(kv, 1);
    ALGEB str_val = ToMapleString(kv, "Hello Maple");
    
    // Verificar tipos
    printf("int_val é inteiro? %s\n", 
        IsMapleInteger(kv, int_val) ? "Sim" : "Não");
    printf("float_val é float? %s\n", 
        IsMapleNumeric(kv, float_val) ? "Sim" : "Não");
    printf("bool_val é booleano? %s\n", 
        (bool_val == ToMapleBoolean(kv, 0) || bool_val == ToMapleBoolean(kv, 1)) ? "Sim" : "Não");
    printf("str_val é string? %s\n", 
        IsMapleString(kv, str_val) ? "Sim" : "Não");
    
    // Converter de volta para C
    M_INT c_int = MapleToInteger64(kv, int_val);
    double c_float = MapleToFloat64(kv, float_val);
    M_BOOL c_bool = MapleToM_BOOL(kv, bool_val);
    const char* c_str = MapleToString(kv, str_val);
    
    printf("\nValores convertidos para C:\n");
    printf("int: %ld\n", (long)c_int);
    printf("float: %.5f\n", c_float);
    printf("bool: %d\n", c_bool);
    printf("string: %s\n", c_str);
}

/* ============================================
 * EXEMPLO 9: Tratamento de Erros
 * ============================================ */
void example_error_handling(MKernelVector kv) {
    printf("\n=== EXEMPLO 9: Tratamento de Erros ===\n");
    
    // Usar try-catch do Maple (sem dividir por zero diretamente)
    printf("Tentando operação que pode falhar (divisão por zero):\n");
    ALGEB safe_div = EvalMapleStatement(kv,
        "try "
        "  resultado := 1/0; "
        "catch: "
        "  resultado := \"Erro capturado: divisão por zero\"; "
        "end try; "
        "resultado;");
    
    if(safe_div != NULL) {
        printf("Resultado: ");
        MapleALGEB_Printf(kv, "%a\n", safe_div);
    }
    
    // Outro exemplo: verificar tipo antes de operar
    printf("\nVerificação de tipo antes de operação:\n");
    ALGEB type_check = EvalMapleStatement(kv,
        "if type(x, numeric) then "
        "  x^2; "
        "else "
        "  \"x não é numérico\"; "
        "end if;");
    
    printf("Resultado: ");
    MapleALGEB_Printf(kv, "%a\n", type_check);
}

/* ============================================
 * EXEMPLO 10: Performance - Avaliação em Lote
 * ============================================ */
void example_performance(MKernelVector kv) {
    printf("\n=== EXEMPLO 10: Performance ===\n");
    
    // Criar array de 1000 elementos e processar
    printf("Criando e processando array de 1000 elementos...\n");
    
    // Processar com map (eficiente) - usar evalf para forçar avaliação numérica
    printf("Calculando raízes quadradas com evalf:\n");
    ALGEB processed = EvalMapleStatement(kv,
        "result := map(x -> evalf(sqrt(x)), [seq(i^2, i=1..10)]);");
    
    printf("Primeiros 10 resultados: ");
    MapleALGEB_Printf(kv, "%a\n", processed);
    
    // Estatísticas
    ALGEB sum = EvalMapleStatement(kv, 
        "add(i, i=1..1000);");
    printf("\nSoma de 1 a 1000: ");
    MapleALGEB_Printf(kv, "%a\n", sum);
    
    // Operações vetoriais
    printf("\nOperações vetoriais:\n");
    ALGEB vec_op = EvalMapleStatement(kv,
        "lista := [seq(i, i=1..20)]; "
        "[add(lista), nops(lista), max(lista), min(lista)];");
    printf("Soma, Tamanho, Máx, Mín: ");
    MapleALGEB_Printf(kv, "%a\n", vec_op);
}

/* ============================================
 * MAIN
 * ============================================ */
int main(int argc, char* argv[]) {
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
    
    // Inicializar Maple
    printf("🍁 Inicializando Maple...\n");
    if((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL) {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }
    
    printf("✅ Maple inicializado com sucesso!\n");
    
    // Executar exemplos
    example_matrix_operations(kv);
    example_solve_equations(kv);
    example_calculus(kv);
    example_lists_and_sets(kv);
    example_procedures(kv);
    example_symbolic_algebra(kv);
    example_plot_data(kv);
    example_conversions(kv);
    example_error_handling(kv);
    example_performance(kv);
    
    printf("\n✅ Todos os exemplos executados com sucesso!\n");
    
    StopMaple(kv);
    return 0;
}
