#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

static void M_DECL textCallBack(void*       data,
                                int         tag,
                                const char* output)
{
    (void)data;
    (void)tag;
    printf("%s\n", output);
}

static void M_DECL errorCallBack(void*       data,
                                 M_INT       offset,
                                 const char* msg)
{
    (void)data;
    (void)offset;
    fprintf(stderr, "Erro: %s\n", msg);
}

int main(int argc, char* argv[])
{
    char                err[2048];
    MKernelVector       kv;
    MCallBackVectorDesc cb = {
        textCallBack, errorCallBack, 0, 0, 0, 0, 0, 0};
    ALGEB r;

    if((kv = StartMaple(argc, argv, &cb, NULL, NULL, err)) == NULL)
    {
        printf("Erro fatal: %s\n", err);
        return 1;
    }

    printf("=== INTEGRAIS DEFINIDAS - Maple C API ===\n\n");

    /* CRÍTICO: Configurar libname corretamente */
    printf("Configurando bibliotecas Maple...\n");
    EvalMapleStatement(
        kv, "libname := \"/opt/maple2021/lib\", libname;");
    printf("✓ Bibliotecas configuradas\n\n");

    printf("Exemplo 1: ∫ sin(x) dx\n");
    printf("Resultado simbólico: ");
    r = EvalMapleStatement(kv, "int(sin(x), x=0..2*Pi);");
    printf("\n");
    r = EvalMapleStatement(kv, "int(sin(x), x=0..Pi);");

    printf("\n");

    printf("✅ Todos os exemplos executados com sucesso!\n");

    StopMaple(kv);
    return 0;
}
