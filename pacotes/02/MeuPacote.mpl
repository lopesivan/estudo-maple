# ===== arquivo: MeuPacote.mpl =====
MeuPacote := module()
    option package;          # diz ao Maple que é um pacote
    export Soma, Media;      # nomes que ficarão “visíveis” fora do módulo

    # --- Função Soma: aceita dois argumentos numéricos ou simbólicos
    Soma := proc(a, b)
        return a + b;
    end proc;

    # --- Só para ilustrar algo a mais: média de uma lista numérica
    Media := proc(L::list)
        return add(L[i], i = 1 .. nops(L)) / nops(L);
    end proc;

end module:
# ===== fim do arquivo =====

