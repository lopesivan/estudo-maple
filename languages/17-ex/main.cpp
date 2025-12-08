/* maple_plot_final.cpp - Teste final de plotagem com opção 'file'
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include "maplec.h"

// ===========================================
// CALLBACKS
// ===========================================
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

// ===========================================
// CLASSE MAPLEKERNEL (Com fix de encapsulamento C++)
// ===========================================

class MapleKernel
{
  private:
    MKernelVector kv;

  public:
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

    ~MapleKernel()
    {
        if(kv != nullptr)
        {
            std::cout << "\n🛑 Encerrando Kernel Maple...\n";
            StopMaple(kv);
        }
    }

    // Getter Público: Necessário para usar MapleALGEB_Printf (C
    // API)
    MKernelVector getKernelVector() const
    {
        return kv;
    }

    ALGEB executeCommand(const std::string& command)
    {
        return EvalMapleStatement(kv, command.c_str());
    }
};

// ===========================================
// FUNÇÃO DE PLOTAGEM PARA TESTE
// ===========================================

void run_plotting_example(MapleKernel& maple)
{
    std::cout << "\n=== Geração de Gráficos (PNG/JPG) via Opção "
                 "'file' do Plot ===\n";

    // CARREGAR PACOTE 'plots'
    maple.executeCommand("with(plots):");
    std::cout << "✓ Pacote 'plots' carregado.\n";

    // --- 1. Plotagem 2D e Exportação para PNG ---

    std::cout << "\n## 1. Plotando uma Senóide (2D) e salvando "
                 "como 'senoid.png'\n";
    const std::string filename_2d = "senoid.png";

    // Comando CORRIGIDO: Usa a opção 'file'
    const std::string plot_command_2d =
        "plot(sin(x^2)/x, x=1..10, title=\"Senóide Modulada\", "
        "axes=box, file=\"" +
        filename_2d + "\");";

    maple.executeCommand(plot_command_2d);

    std::cout << "✅ Gráfico 2D salvo em: **" << filename_2d
              << "**\n";

    // --- 2. Plotagem 3D e Exportação para JPG ---

    std::cout << "\n## 2. Plotando uma Superfície (3D) e salvando "
                 "como 'superficie.jpg'\n";
    const std::string filename_3d = "superficie.jpg";

    // Comando CORRIGIDO: Usa a opção 'file'
    const std::string plot_command_3d =
        "plot3d(sin(x)*cos(y), x=-3..3, y=-3..3, "
        "title=\"Superfície Senoidal\", file=\"" +
        filename_3d + "\");";

    maple.executeCommand(plot_command_3d);

    std::cout << "✅ Gráfico 3D salvo em: **" << filename_3d
              << "**\n";
}

// ===========================================
// MAIN
// ===========================================

int main(int argc, char* argv[])
{
    try
    {
        // SOLUÇÃO 1: Crie uma instância nomeada (lvalue)
        MapleKernel maple_instance{argc, argv};

        // Agora, passe a instância nomeada
        run_plotting_example(maple_instance);

        std::cout << "\n✅ Teste de plotagem finalizado com "
                     "sucesso! Verifique os arquivos PNG/JPG.\n";
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "\nFATAL EXCEPTION: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
