Este é um excelente problema de eletrostática que combina coordenadas esféricas com a API Maple C.

Para calcular a **densidade volumétrica de carga** ($\rho$) a partir do potencial elétrico ($V$), usamos a **Equação de Poisson** , que, na ausência de materiais dielétricos ($\epsilon = \epsilon_0$), é dada por:

$$\nabla^2 V = - \frac{\rho}{\epsilon_0}$$

Onde $\nabla^2 V$ é o Laplaciano do potencial $V$. Portanto, a densidade de carga é:

$$\rho = - \epsilon_0 \nabla^2 V$$

Vamos usar o Maple para calcular o **Laplaciano em coordenadas esféricas** e, em seguida, multiplicar por $-\epsilon_0$.

## 💻 Programa C Completo: Densidade Volumétrica de Carga ($\rho$)

Este programa C usará o Maple para:

1.  Definir o potencial $V(r, \theta, \varphi)$.
2.  Carregar o pacote **`VectorCalculus`** (ou similar, dependendo da versão do Maple) e definir o sistema de coordenadas esféricas.
3.  Calcular o Laplaciano de $V$ ($\nabla^2 V$).
4.  Definir $\rho$ pela Equação de Poisson.
5.  Imprimir o resultado simbólico.

### Código C (`main.c`)

```c
/* main.c - Cálculo da Densidade Volumétrica de Carga (Laplaciano em Esféricas) */

#include <stdio.h>
#include <stdlib.h>
#include "maplec.h"

/* Constante de permissividade do vácuo */
#define EPSILON_0 "epsilon[0]" 

/* ============================================
 * CALLBACKS (Comunicação com o Kernel Maple)
 * ============================================ */

static void M_DECL textCallBack(void* data, int tag, const char* output)
{
    (void)data;
    (void)tag;
    printf(">> Maple: %s\n", output);
}

static void M_DECL errorCallBack(void* data, M_INT offset, const char* msg)
{
    (void)data;
    (void)offset;
    fprintf(stderr, "❌ Erro Maple: %s\n", msg);
}

/* ============================================
 * FUNÇÃO DE CÁLCULO: Densidade de Carga (rho)
 * ============================================ */
void calculate_charge_density(MKernelVector kv) {
    
    printf("\n=== CÁLCULO DA DENSIDADE VOLUMÉTRICA DE CARGA (RHO) ===\n");
    
    // 1. Carregar o pacote e configurar coordenadas esféricas
    printf("1. Configurando o Maple para coordenadas esféricas...\n");
    
    // O pacote Physics[VectorCalculus] ou apenas Physics é o mais confiável para Maple 2021
    EvalMapleStatement(kv, "with(Physics):"); 
    EvalMapleStatement(kv, "Setup(coordinates = spherical[r, theta, phi], metric = diagonal[1, r^2, r^2*sin(phi)^2]):");
    printf("✓ Pacote e Coordenadas Esféricas configuradas.\n");

    // 2. Definir o Potencial V(r, theta, phi)
    // V(r, θ, φ) = 100 + 50r + 150r*sin(φ)
    const char *potential_V = "V := 100 + 50*r + 150*r*sin(phi)";
    EvalMapleStatement(kv, potential_V);
    printf("\n2. Potencial V definido:\n>> Maple: %s\n", potential_V);

    // 3. Calcular o Laplaciano do Potencial (nabla^2 V)
    // O Laplaciano do Maple é dado por: Laplace(V)
    printf("\n3. Calculando o Laplaciano (Laplace(V)):\n");
    
    // A expressão é armazenada na variável Maple 'laplacian_V'
    EvalMapleStatement(kv, "laplacian_V := Simplify(Laplace(V));");
    
    // Recuperar e imprimir o Laplaciano
    ALGEB laplacian_algeb = EvalMapleStatement(kv, "laplacian_V;");
    printf("Laplaciano (∇²V): ");
    MapleALGEB_Printf(kv, "%a\n", laplacian_algeb);


    // 4. Aplicar a Equação de Poisson: rho = - epsilon_0 * Laplaciano(V)
    printf("\n4. Aplicando a Equação de Poisson: rho = -epsilon[0] * (∇²V)\n");
    
    // NOTA: Usamos o símbolo Maple 'epsilon[0]' para a constante
    EvalMapleStatement(kv, "rho := - " EPSILON_0 " * laplacian_V;");
    
    // Recuperar e imprimir a Densidade de Carga (rho)
    ALGEB rho_algeb = EvalMapleStatement(kv, "rho;");
    printf("Densidade de Carga (ρ): ");
    MapleALGEB_Printf(kv, "%a\n", rho_algeb);

    // 5. Verificação da Superfície (opcional: a esfera está na superfície r=2)
    // O Laplaciano deve ser zero (ρ=0) em todas as regiões livres de carga.
    // Como o Laplaciano que obtivermos será diferente de zero, a carga é volumétrica.
    // Se o potencial fosse 1/r, o Laplaciano seria zero exceto na origem.
    
    // O resultado esperado é: (150 * sin(phi)) / r
    // ρ = - epsilon[0] * (150 * sin(phi)) / r
}

/* ============================================
 * MAIN
 * ============================================ */

int main(int argc, char* argv[])
{
    char err[2048];
    MKernelVector kv;

    MCallBackVectorDesc cb = {
        textCallBack,
        errorCallBack,
        0, 0, 0, 0, 0, 0
    };

    printf("=== Eletrostática: Densidade de Carga em Esféricas ===\n");
    printf("🍁 Inicializando Maple...\n");

    kv = StartMaple(argc, argv, &cb, NULL, NULL, err);
    if(kv == NULL)
    {
        fprintf(stderr, "Erro fatal ao iniciar Maple: %s\n", err);
        return 1;
    }

    printf("✅ Maple inicializado com sucesso!\n");

    /* Configurar libname, caso necessário para o Maple 2021 */
    EvalMapleStatement(kv, "libname := \"/opt/maple2021/lib\", libname;");

    /* Executar o cálculo */
    calculate_charge_density(kv);

    printf("\n✅ Cálculo finalizado.\n");

    StopMaple(kv);
    return 0;
}
```

-----

## 💡 Saída Esperada (Resultado do Laplaciano)

O Laplaciano em coordenadas esféricas ($\nabla^2 V$) é dado por:

$$\nabla^2 V = \frac{1}{r^2} \frac{\partial}{\partial r} \left( r^2 \frac{\partial V}{\partial r} \right) + \frac{1}{r^2 \sin\varphi} \frac{\partial}{\partial \varphi} \left( \sin\varphi \frac{\partial V}{\partial \varphi} \right) + \frac{1}{r^2 \sin^2\varphi} \frac{\partial^2 V}{\partial \theta^2}$$

Para $V(r, \theta, \varphi) = 100 + 50r + 150r \sin\varphi$:

1.  O termo $\theta$ é zero.
2.  **Termo $r$:** $V_r = 50 + 150 \sin\varphi$. $\frac{1}{r^2} \frac{\partial}{\partial r} \left( r^2 V_r \right) = \frac{1}{r^2} \frac{\partial}{\partial r} \left( r^2 (50 + 150 \sin\varphi) \right) = \frac{1}{r^2} (2r (50 + 150 \sin\varphi)) = \frac{100}{r} + \frac{300 \sin\varphi}{r}$.
3.  **Termo $\varphi$:** $\frac{\partial V}{\partial \varphi} = 150r \cos\varphi$.
    $\frac{1}{r^2 \sin\varphi} \frac{\partial}{\partial \varphi} \left( \sin\varphi (150r \cos\varphi) \right)$
    $= \frac{150r}{r^2 \sin\varphi} \frac{\partial}{\partial \varphi} \left( \sin\varphi \cos\varphi \right)$
    $= \frac{150}{r \sin\varphi} (\cos^2\varphi - \sin^2\varphi) = \frac{150 \cos(2\varphi)}{r \sin\varphi}$.

O resultado final é a soma destes termos. O Maple fará a simplificação por você.

O resultado simbólico final da densidade de carga será:

$$\rho(r, \theta, \varphi) = - \epsilon_0 \left( \frac{100}{r} + \frac{300 \sin\varphi}{r} + \frac{150 \cos(2\varphi)}{r \sin\varphi} \right)$$
