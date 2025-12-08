/* test_lazy.c
 *
 * Teste: Integral com estratégia LAZY de carregamento de biblioteca
 * Baseado na abordagem do line.c
 *
 * Testa se consegue calcular integral sem configurar libname,
 * e configura apenas se necessário (como line.c faz)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maplec.h"

/* ============================================
 * CALLBACKS
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
    fprintf(stderr, "Erro: %s\n", msg);
}

/* ============================================
 * FUNÇÃO QUE TENTA CALCULAR INTEGRAL
 * Abordagem LAZY (como line.c)
 * ============================================ */

ALGEB calculate_integral_lazy(MKernelVector kv, const char* expression)
{
    ALGEB result;
    char *maple_dir;
    
    printf("\n=== Tentando calcular: %s ===\n", expression);
    
    /* PRIMEIRO: Tentar sem configurar nada */
    printf("1. Tentativa SEM configurar libname...\n");
    result = EvalMapleStatement(kv, expression);
    
    /* Verificar se funcionou */
    if (!result || IsMapleNULL(kv, result)) {
        printf("   ❌ Retornou NULL - falhou!\n");
        
        /* SEGUNDO: Configurar libname e tentar novamente */
        printf("\n2. Detectada falha - configurando libname...\n");
        
        /* Tentar via variável de ambiente (como line.c) */
        if ((maple_dir = getenv("MAPLE")) || (maple_dir = getenv("MAPLE_ROOT"))) {
            char *libpath;
            libpath = (char*)malloc((5 + strlen(maple_dir)) * sizeof(char));
            sprintf(libpath, "%s/lib", maple_dir);
            
            printf("   Configurando via $MAPLE: %s\n", libpath);
            MapleLibName(kv, ToMapleString(kv, libpath));
            free(libpath);
        } else {
            /* Fallback: caminho hardcoded */
            printf("   $MAPLE não encontrado, usando hardcoded...\n");
            EvalMapleStatement(kv, "libname := \"/opt/maple2021/lib\", libname;");
        }
        
        printf("   ✓ Libname configurado\n");
        
        /* TERCEIRO: Tentar novamente */
        printf("\n3. Nova tentativa APÓS configurar libname...\n");
        result = EvalMapleStatement(kv, expression);
        
        if (!result || IsMapleNULL(kv, result)) {
            printf("   ❌ Ainda falhou após configurar libname!\n");
            return NULL;
        } else {
            printf("   ✅ SUCESSO após configurar libname!\n");
        }
    } else {
        printf("   ✅ SUCESSO sem precisar configurar libname!\n");
    }
    
    return result;
}

/* ============================================
 * TESTES
 * ============================================ */

void run_tests(MKernelVector kv)
{
    ALGEB result;
    
    printf("\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("  TESTE: Calculando Integrais com Lazy Loading\n");
    printf("════════════════════════════════════════════════════════\n");
    
    /* Teste 1: Integral simples */
    result = calculate_integral_lazy(kv, "int(x^2, x=0..2);");
    if (result) {
        printf("\n>>> Resultado: ");
        MapleALGEB_Printf(kv, "%a\n", result);
    }
    
    printf("\n" "────────────────────────────────────────────────────────\n");
    
    /* Teste 2: Segunda integral (deve funcionar sem reconfigurar) */
    printf("\n=== Segunda integral (biblioteca já deve estar carregada) ===\n");
    result = EvalMapleStatement(kv, "int(sin(x), x=0..Pi);");
    if (result) {
        printf(">>> Resultado: ");
        MapleALGEB_Printf(kv, "%a\n", result);
    } else {
        printf("❌ Falhou\n");
    }
    
    printf("\n" "────────────────────────────────────────────────────────\n");
    
    /* Teste 3: Valor numérico */
    printf("\n=== Terceira integral (com evalf) ===\n");
    result = EvalMapleStatement(kv, "evalf(int(1/(1+x^2), x=0..1));");
    if (result) {
        printf(">>> Resultado: ");
        MapleALGEB_Printf(kv, "%a\n", result);
    } else {
        printf("❌ Falhou\n");
    }
    
    printf("\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("  FIM DOS TESTES\n");
    printf("════════════════════════════════════════════════════════\n");
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
        0, 0, 0, 0, 0, 0
    };
    
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  Teste: Lazy Loading de Bibliotecas Maple             ║\n");
    printf("║  Estratégia do line.c aplicada a integrais            ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    printf("\n🍁 Inicializando Maple...\n");
    
    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if (kv == NULL) {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }
    
    printf("✅ Maple inicializado\n");
    
    /* Verificar variável de ambiente */
    char *maple_env = getenv("MAPLE");
    if (maple_env) {
        printf("ℹ️  Variável $MAPLE detectada: %s\n", maple_env);
    } else {
        printf("⚠️  Variável $MAPLE não encontrada\n");
    }
    
    /* Executar testes */
    run_tests(kv);
    
    printf("\n✅ Programa finalizado\n");
    
    StopMaple(kv);
    return 0;
}
