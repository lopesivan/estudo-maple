#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

static void M_DECL textCallBack(void *data, int tag, const char *output)
{
    (void)data; (void)tag;
    printf("%s\n", output);
}

static void M_DECL errorCallBack(void *data, M_INT offset, const char *msg)
{
    (void)data; (void)offset;
    fprintf(stderr, "Erro: %s\n", msg);
}

int main(int argc, char *argv[])
{
    char err[2048];
    MKernelVector kv;
    MCallBackVectorDesc cb = { textCallBack, errorCallBack, 0, 0, 0, 0, 0, 0 };
    ALGEB r;

    if ((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL) {
        printf("Erro fatal: %s\n", err);
        return 1;
    }

    printf("=== INTEGRAIS DEFINIDAS - Maple C API ===\n\n");

    /* CRÍTICO: Configurar libname corretamente */
    printf("Configurando bibliotecas Maple...\n");
    EvalMapleStatement(kv, "libname := \"/opt/maple2021/lib\", libname;");
    printf("✓ Bibliotecas configuradas\n\n");

    /* Exemplo 1: int(x^2, x=0..2) - Resultado: 8/3 */
    printf("Exemplo 1: ∫₀² x² dx\n");
    printf("Resultado simbólico: ");
    r = EvalMapleStatement(kv, "int(x^2, x=0..2);");
    
    printf("Resultado numérico: ");
    r = EvalMapleStatement(kv, "evalf(int(x^2, x=0..2));");
    printf("\n");

    /* Exemplo 2: int(sin(x), x=0..Pi) - Resultado: 2 */
    printf("Exemplo 2: ∫₀^π sin(x) dx\n");
    printf("Resultado simbólico: ");
    r = EvalMapleStatement(kv, "int(sin(x), x=0..Pi);");
    
    printf("Resultado numérico: ");
    r = EvalMapleStatement(kv, "evalf(int(sin(x), x=0..Pi));");
    printf("\n");

    /* Exemplo 3: int(1/(1+x^2), x=0..1) - Resultado: Pi/4 */
    printf("Exemplo 3: ∫₀¹ 1/(1+x²) dx\n");
    printf("Resultado simbólico: ");
    r = EvalMapleStatement(kv, "int(1/(1+x^2), x=0..1);");
    
    printf("Resultado numérico: ");
    r = EvalMapleStatement(kv, "evalf(int(1/(1+x^2), x=0..1));");
    printf("\n");

    /* Exemplo 4: int(exp(t), t=0..1) - Resultado: e-1 */
    printf("Exemplo 4: ∫₀¹ eᵗ dt\n");
    printf("Resultado simbólico: ");
    r = EvalMapleStatement(kv, "int(exp(t), t=0..1);");
    
    printf("Resultado numérico: ");
    r = EvalMapleStatement(kv, "evalf(int(exp(t), t=0..1));");
    printf("\n");

    /* Exemplo 5: Integral dupla */
    printf("Exemplo 5: ∫₀¹ ∫₀² u*v dv du\n");
    printf("Resultado: ");
    r = EvalMapleStatement(kv, "int(int(u*v, v=0..2), u=0..1);");
    printf("\n");

    /* Exemplo 6: Usando procedimento customizado */
    printf("Exemplo 6: Procedimento customizado myint\n");
    EvalMapleStatement(kv, 
        "myint := proc(f, var, a, b) "
        "  local F; "
        "  F := int(f, var); "
        "  eval(F, var=b) - eval(F, var=a); "
        "end proc;");
    
    printf("myint(x^3, x, 0, 2) = ");
    r = EvalMapleStatement(kv, "myint(x^3, x, 0, 2);");
    printf("\n");

    printf("✅ Todos os exemplos executados com sucesso!\n");

    StopMaple(kv);
    return 0;
}
