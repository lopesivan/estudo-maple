#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

/* callback used for directing result output */
static void M_DECL textCallBack(void*       data,
                                int         tag,
                                const char* output)
{
    printf("%s\n", output);
}

int main(int argc, char* argv[])
{
    char                err[2048];
    MKernelVector       kv;
    MCallBackVectorDesc cb = {textCallBack, 0, 0, 0, 0, 0, 0, 0};
    ALGEB               r;

    /* initialize Maple */
    if((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL)
    {
        printf("Fatal error, %s\n", err);
        return (1);
    }

    printf("=== Exemplos de Integrais Simples ===\n\n");

    /* Exemplo 1: integral de x^2 de 0 a 2 */
    /* Resultado esperado: 8/3 = 2.666666667 */
    printf("Exemplo 1: int(x^2, x=0..2)\n");
    r = EvalMapleStatement(kv, "int(x^2, x=0..2);");
    MapleALGEB_Printf(kv, "Resultado simbólico: %a\n", r);

    r = MapleEval(kv, r);
    MapleALGEB_Printf(kv, "Resultado avaliado: %a\n", r);

    r = EvalMapleStatement(kv, "evalf(int(x^2, x=0..2));");
    MapleALGEB_Printf(kv, "Resultado numérico: %a\n\n", r);

    /* Exemplo 2: integral de sin(x) de 0 a Pi */
    /* Resultado esperado: 2 */
    printf("Exemplo 2: int(sin(x), x=0..Pi)\n");
    r = EvalMapleStatement(kv, "int(sin(x), x=0..Pi);");
    MapleALGEB_Printf(kv, "Resultado simbólico: %a\n", r);

    r = EvalMapleStatement(kv, "evalf(int(sin(x), x=0..Pi));");
    MapleALGEB_Printf(kv, "Resultado numérico: %a\n\n", r);

    /* Exemplo 3: integral de 1/(1+x^2) de 0 a 1 */
    /* Resultado esperado: Pi/4 = 0.7853981634 */
    printf("Exemplo 3: int(1/(1+x^2), x=0..1)\n");
    r = EvalMapleStatement(kv, "int(1/(1+x^2), x=0..1);");
    MapleALGEB_Printf(kv, "Resultado simbólico: %a\n", r);

    r = EvalMapleStatement(kv, "evalf(int(1/(1+x^2), x=0..1));");
    MapleALGEB_Printf(kv, "Resultado numérico: %a\n\n", r);

    /* Exemplo 4: integral de e^x de 0 a 1 */
    /* Resultado esperado: e - 1 = 1.718281828 */
    printf("Exemplo 4: int(exp(x), x=0..1)\n");
    r = EvalMapleStatement(kv, "int(exp(x), x=0..1);");
    MapleALGEB_Printf(kv, "Resultado simbólico: %a\n", r);

    r = EvalMapleStatement(kv, "evalf(int(exp(x), x=0..1));");
    MapleALGEB_Printf(kv, "Resultado numérico: %a\n\n", r);

    StopMaple(kv);
    return (0);
}
