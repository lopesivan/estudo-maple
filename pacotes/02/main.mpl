restart;                          # limpa a memória

# se o .so estiver na mesma pasta da sua worksheet:

lib := "./libmysum.so":                 # caminho para a biblioteca

mysum := define_external(
          'mysum',                      # símbolo exportado no .so
          'a'::float[8],                # 1º argumento  (double)
          'b'::float[8],                # 2º argumento  (double)
          'RETURN'::float[8],           # tipo de retorno
          LIB = lib                     # onde a .so está
         ):

# teste rápido
mysum(2.0, 3.0);       # → 5.0
mysum(evalf(Pi), 2.5); # → 5.641592653...

