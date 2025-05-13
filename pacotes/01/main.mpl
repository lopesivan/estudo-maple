restart;                          # limpa a memória
# libname := libname, "/workspace/estudo-maple/pacotes/01":   # aponte para a pasta onde salvou
read "MeuPacote.mpl":             # carrega o código (ou: with(MeuPacote) se já instalou no lib)
with(MeuPacote):                  # torna os exports acessíveis sem prefixo


Soma(2, 3);                       # retorna 5
Soma(2*x, 3*y);                   # retorna 2 x + 3 y (nota: Maple é simbólico!)
