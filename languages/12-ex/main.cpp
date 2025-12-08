#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "maplec.h"

// Constantes para Callbacks
static void M_DECL textCallBack(void* /* data */, int /* tag */, const char* output) {
    std::cout << ">> Maple: " << output << "\n";
}

static void M_DECL errorCallBack(void* /* data */, M_INT /* offset */, const char* msg) {
    std::cerr << "❌ Maple Error: " << msg << "\n";
}

// ===========================================
// CLASSE MAPLEKERNEL (Encapsulamento C++)
// ===========================================

class MapleKernel {
private:
    MKernelVector kv;

public:
    MapleKernel(int argc, char** argv) {
        char err[2048];
        MCallBackVectorDesc cb = {
            textCallBack,
            errorCallBack,
            0, 0, 0, 0, 0, 0
        };

        std::cout << "🍁 Inicializando Kernel Maple...\n";
        kv = StartMaple(argc, argv, &cb, nullptr, nullptr, err);

        if (kv == nullptr) {
            throw std::runtime_error(std::string("Falha ao iniciar Maple: ") + err);
        }
        
        executeCommand("libname := \"/opt/maple2021/lib\", libname;");
        std::cout << "✅ Kernel Maple inicializado com sucesso!\n";
    }

    ~MapleKernel() {
        if (kv != nullptr) {
            std::cout << "\n🛑 Encerrando Kernel Maple...\n";
            StopMaple(kv);
        }
    }

    MKernelVector getKernelVector() const {
        return kv;
    }

    ALGEB executeCommand(const std::string& command) {
        return EvalMapleStatement(kv, command.c_str());
    }

    double extractDouble(ALGEB result) const {
        // Assume que o resultado pode ser convertido para float
        return MapleToFloat64(kv, result);
    }
};

// ===========================================
// FUNÇÃO PRINCIPAL DO EXEMPLO
// ===========================================

void run_series_analysis(MapleKernel& maple) {
    std::cout << "\n=== Análise de Séries (sum, seq) em C++ ===\n";

    MKernelVector kv = maple.getKernelVector();

    // SÉRIE DE EXEMPLO: Série Geométrica Simples (Sum_{k=1}^{infinity} (1/2)^k)
    // Sabemos que esta série converge para 1.
    const std::string series_term = "(1/2)^k";
    std::cout << "Função da Série: a_k = " << series_term << "\n";
    
    // --- 1. Geração dos Primeiros Termos (seq) ---
    std::cout << "\n## 1. Gerando os 5 Primeiros Termos (seq)\n";
    
    const int num_terms = 5;
    // Comando Maple: seq( (1/2)^k, k=1..5 )
    std::string seq_command = "termos := [seq(" + series_term + ", k=1.." + std::to_string(num_terms) + ")];";
    
    maple.executeCommand(seq_command);

    // Recupera a lista de termos. A lista é retornada como um objeto ALGEB.
    const auto termos_algeb = maple.executeCommand("termos;");
    
    std::cout << "Termos (ALGEB): ";
    MapleALGEB_Printf(kv, "%a\n", termos_algeb); // Exibe [1/2, 1/4, 1/8, 1/16, 1/32]

    // --- 2. Cálculo da Soma Parcial (sum) ---
    std::cout << "\n## 2. Cálculo da Soma Parcial S_5 (sum)\n";
    
    // Comando Maple: sum( (1/2)^k, k=1..5 )
    std::string partial_sum_command = "S_5 := sum(" + series_term + ", k=1.." + std::to_string(num_terms) + ");";
    
    maple.executeCommand(partial_sum_command);
    
    // Recupera o resultado simbólico
    const auto s5_algeb = maple.executeCommand("S_5;");
    const auto s5_double = maple.extractDouble(s5_algeb);

    std::cout << "S_5 (ALGEB/Fracão): ";
    MapleALGEB_Printf(kv, "%a\n", s5_algeb); // Exibe 31/32
    std::cout << "S_5 (C++ Double): **" << s5_double << "** (Esperado: 0.96875)\n";


    // --- 3. Cálculo da Soma Infinita (sum com infinity) ---
    std::cout << "\n## 3. Convergência da Série (sum com infinity)\n";
    
    // Comando Maple: sum( (1/2)^k, k=1..infinity )
    std::string infinite_sum_command = "S_inf := sum(" + series_term + ", k=1..infinity);";
    
    maple.executeCommand(infinite_sum_command);
    
    // Recupera o resultado
    const auto s_inf_algeb = maple.executeCommand("S_inf;");
    const auto s_inf_double = maple.extractDouble(s_inf_algeb);

    std::cout << "S_inf (ALGEB): ";
    MapleALGEB_Printf(kv, "%a\n", s_inf_algeb); // Exibe 1
    std::cout << "S_inf (C++ Double): **" << s_inf_double << "**\n";


    // --- 4. Lógica C++ Pós-Cálculo ---
    std::cout << "\n## 4. Lógica C++: Verificação de Convergência\n";
    
    if (s_inf_double == 1.0) {
        std::cout << "=> Sucesso: A série geométrica a_k converge para 1.0, conforme calculado pelo Maple.\n";
    } else {
        std::cout << "=> Aviso: O Maple retornou um valor diferente de 1.0 para a série convergente.\n";
    }
}

// ===========================================
// MAIN
// ===========================================

int main(int argc, char* argv[]) {
    try {
        // Inicialização RAII (C++ moderno)
        MapleKernel maple{argc, argv};
        run_series_analysis(maple);
    } catch (const std::runtime_error& e) {
        std::cerr << "\nFATAL EXCEPTION: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
