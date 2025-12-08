#include <iostream>
#include <string>
#include <stdexcept>
#include "maplec.h"

// Constantes para Callbacks (evitando warnings de parâmetros não
// utilizados)
static void M_DECL textCallBack(void* /* data */,
                                int /* tag */,
                                const char* output)
{
    std::cout << ">> Maple: " << output << "\n";
}

static void M_DECL errorCallBack(void* /* data */,
                                 M_INT /* offset */,
                                 const char* msg)
{
    std::cerr << "❌ Maple Error: " << msg << "\n";
}

// --- CLASSE CORRIGIDA ---
class MapleKernel
{
  private:
    MKernelVector kv;  // Membro privado

  public:
    // Construtor (Inicialização do Kernel)
    MapleKernel(int argc, char** argv)
    {
        char                err[2048];
        MCallBackVectorDesc cb = {
            textCallBack, errorCallBack, 0, 0, 0, 0, 0, 0};

        std::cout << "🍁 Inicializando Kernel Maple...\n";
        kv = StartMaple(argc, argv, &cb, nullptr, nullptr, err);

        if(kv == nullptr)
        {
            throw std::runtime_error(
                std::string("Falha ao iniciar Maple: ") + err);
        }

        executeCommand(
            "libname := \"/opt/maple2021/lib\", libname;");
        std::cout << "✅ Kernel Maple inicializado com sucesso!\n";
    }

    // Destrutor (Encerramento do Kernel)
    ~MapleKernel()
    {
        if(kv != nullptr)
        {
            std::cout << "\n🛑 Encerrando Kernel Maple...\n";
            StopMaple(kv);
        }
    }

    // *** MÉTODO GETTER PÚBLICO CORRIGIDO ***
    // Permite que funções externas acessem o kv de forma
    // controlada.
    MKernelVector getKernelVector() const
    {
        return kv;
    }

    ALGEB executeCommand(const std::string& command)
    {
        return EvalMapleStatement(kv, command.c_str());
    }

    double extractDouble(ALGEB result) const
    {
        return MapleToFloat64(kv, result);
    }
};

/**
 * @brief Exemplo de cálculo usando a classe MapleKernel.
 */
void run_maple_example(MapleKernel& maple)
{
    std::cout << "\n=== Exemplo C++ Moderno + Maple (Cálculo "
                 "Simbólico) ===\n";

    // Obtém o kv de forma segura (pública)
    MKernelVector kv = maple.getKernelVector();

    // 1. Definir a função no Maple
    const auto func_command = "f := x -> x^2-12*x+17;";
    maple.executeCommand(func_command);
    std::cout << "Função definida: f(x) = x^2 -12*x + 17\n";

    // 2. Calcular a Derivada (Simbólica)
    const auto deriv_command = "df := diff(f(x), x);";
    maple.executeCommand(deriv_command);

    // Recuperar e imprimir a derivada
    const auto df_algeb = maple.executeCommand("df;");
    std::cout << "Derivada f'(x): ";
    // *** USO CORRIGIDO: Agora usamos 'kv' (a variável local) ***
    MapleALGEB_Printf(kv, "%a\n", df_algeb);

    // 3. Avaliar a Derivada em um Ponto Específico (x = 1.0)
    const double x_val = 1.0;
    const auto   eval_command =
        "eval(df, x = " + std::to_string(x_val) + ");";
    const auto result_algeb = maple.executeCommand(eval_command);

    // 4. Extrair o valor para um tipo C++ nativo (double)
    const auto result_double = maple.extractDouble(result_algeb);

    std::cout << "\nResultado Maple em C++:\n";
    std::cout << "f'(1.0) Simbólico (ALGEB): ";
    // *** USO CORRIGIDO: Agora usamos 'kv' (a variável local) ***
    MapleALGEB_Printf(kv, "%a\n", result_algeb);
    std::cout << "f'(1.0) Convertido para C++ Double: **"
              << result_double << "**\n";

    // Exemplo de lógica C++ usando o resultado do Maple:
    if(result_double < 0)
    {
        std::cout
            << "=> Lógica C++: A função está decrescendo em x = "
            << x_val << ".\n";
    }
}

int main(int argc, char* argv[])
{
    try
    {
        MapleKernel maple{argc, argv};
        run_maple_example(maple);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "\nFATAL EXCEPTION: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
