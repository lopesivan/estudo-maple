#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "maplec.h"

/* ---------- protótipo ---------- */
ALGEB M_DECL NewtonsMethod(MKernelVector kv, ALGEB args);

/* ---------- implementação ---------- */
ALGEB M_DECL NewtonsMethod(MKernelVector kv, ALGEB args)
{
    if (MapleNumArgs(kv, args) != 3) {
        MapleRaiseError(kv, "three arguments expected");
        return NULL;
    }
    M_INT idx1 = 1, idx2 = 2, idx3 = 3;
    ALGEB f_arg  = MapleSelectIndexed(kv, args, 1, &idx1);
    ALGEB x0_arg = MapleSelectIndexed(kv, args, 1, &idx2);
    ALGEB tol_arg= MapleSelectIndexed(kv, args, 1, &idx3);

    /* converte função para procedure se necessário */
    ALGEB f = f_arg;
    if (!IsMapleProcedure(kv, f)) {
        ALGEB indets = EvalMapleProc(kv, ToMapleName(kv, "indets", TRUE), 1, f);
        if (!IsMapleSet(kv, indets) || MapleNumArgs(kv, indets) != 1) {
            MapleRaiseError(kv, "cannot find indeterminate");
            return NULL;
        }
        M_INT one = 1;
        ALGEB xvar = MapleSelectIndexed(kv, indets, 1, &one);
        f = EvalMapleProc(kv, ToMapleName(kv, "unapply", TRUE), 2, f, xvar);
        if (!f || !IsMapleProcedure(kv, f)) {
            MapleRaiseError(kv, "cannot convert to procedure");
            return NULL;
        }
    }

    ALGEB xsym = ToMapleName(kv, "x", FALSE);
    ALGEB fprime = EvalMapleProc(kv, ToMapleName(kv, "unapply", TRUE), 2,
                                 EvalMapleProc(kv, ToMapleName(kv, "diff", TRUE), 2,
                                               ToMapleFunction(kv, f, 1, xsym), xsym),
                                 xsym);
    if (!fprime || !IsMapleProcedure(kv, fprime)) {
        MapleRaiseError(kv, "cannot compute derivative");
        return NULL;
    }

    double guess = MapleEvalhf(kv, x0_arg);
    double tol   = MapleEvalhf(kv, tol_arg);

    for (int i = 0; i < 500; ++i) {
        double fx = EvalhfMapleProc(kv, f, 1, &guess);
        if (fabs(fx) <= tol) break;
        double fpx = EvalhfMapleProc(kv, fprime, 1, &guess);
        if (fpx == 0.0) {
            MapleRaiseError(kv, "derivative vanished");
            return NULL;
        }
        guess -= fx / fpx;
    }
    return ToMapleFloat(kv, guess);
}

/* ---------- cria lista sem parser ---------- */
static ALGEB MakeList3(MKernelVector kv, ALGEB a, ALGEB b, ALGEB c)
{
    ALGEB list = MapleNewList(kv, 3);
    M_INT i1 = 1, i2 = 2, i3 = 3;
    MapleListAssign(kv, list, 1, &i1, a);
    MapleListAssign(kv, list, 1, &i2, b);
    MapleListAssign(kv, list, 1, &i3, c);
    return list;
}

/* ---------- teste ---------- */
int main(int argc, char *argv[])
{
    char err[2048];
    MCallBackVectorDesc cb = { 0 };

    /* 1. garante libname antes de qualquer coisa */
    if (getenv("MAPLE"))
        sprintf(err, "libname := \"%s/lib\":", getenv("MAPLE"));
    else
        strcpy(err, "libname := \"/opt/maple2021/lib\":"); /* fallback */
    EvalMapleStatement(NULL, err); /* NULL = ainda sem kv, mas já define libname */

    MKernelVector kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if (!kv) {
        printf("Erro ao iniciar Maple: %s\n", err);
        return 1;
    }

    /* 2. constrói lista [x^4-1, 2.0, 0.001] */
    ALGEB f_expr = EvalMapleStatement(kv, "x^4 - 1");
    ALGEB list = MakeList3(kv, f_expr,
                           ToMapleFloat(kv, 2.0),
                           ToMapleFloat(kv, 1e-3));

    ALGEB root = NewtonsMethod(kv, list);
    if (root)
        MapleALGEB_Printf(kv, "Raiz: %a\n", root);
    else
        printf("Falhou.\n");

    return 0;
}
