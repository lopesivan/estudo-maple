O erro vem do **segundo argumento** que você passou a `define_external`.
Depois do nome da função (`'mysum'`) o Maple espera **ou**:

1. A palavra-chave `MAPLE` (ou `C`, `FORTRAN`, …) que diz qual “estilo” de chamada será usado **antes** de qualquer outra opção **ou**
2. Diretamente a lista de descritores de parâmetros (por exemplo `float[8], float[8]`) e só **depois** as opções escritas como `LIB=…`, `RETURN=…`, …

Quando você entregou apenas o caminho `./libmysum.so`, o
kernel tentou tratá-lo como se fosse uma opção (`unknown
option name ./libmysum.so`).

### Duas formas corretas

```maple
# 1) jeito mais comum: coloque 'MAPLE' logo depois do nome
lib := "./libmysum.so":

mysum := define_external(
          'mysum',                 # símbolo exportado no .so
          'MAPLE',                 # estilo de chamada/wrapper
          LIB      = lib,          # caminho do .so
          RETURN   = float[8],     # double de retorno
          PARAMETERS = [float[8], float[8]]  # dois doubles de entrada
         ):

mysum(2.0, 3.0);     # → 5.0
```

ou

```maple
# 2) sem especificar 'MAPLE': descreva primeiro os parâmetros
lib := "./libmysum.so":

mysum := define_external(
          'mysum',
          float[8], float[8],      # descritores dos 2 argumentos
          RETURN = float[8],
          LIB    = lib
         ):
```

Ambas as sintaxes são aceitas nas versões atuais
do Maple (veja o exemplo da própria documentação
([mapleprimes.com][1])).

---

#### Por que `MAPLE` ajuda

* Deixa o comando mais legível: logo de cara o Maple sabe
que vai gerar um *wrapper* “wrapperless” (`MAPLE`).
* Evita confusão: qualquer string encontrada depois de
`'MAPLE'` só será examinada quando vier com `LIB=`, então
o caminho nunca é interpretado como opção desconhecida.

A página oficial da função (`?define_external`) reforça que
o nome da biblioteca **sempre** deve vir precedido por `LIB=` .

---

### Checklist rápido se ainda der erro

1. **Função exportada?**
   Confirme com `nm -D libmysum.so | grep mysum` que o símbolo realmente existe.
2. **LD\_LIBRARY\_PATH** está visível dentro do Maple?
   Use `kernelopts(`getenv`, "LD_LIBRARY_PATH");`.
3. **Tipos batem?**
   A assinatura em C deve ser

   ```c
   double mysum ( double a, double b );
   ```

   (qualquer coisa diferente exige descritores adequados em Maple).
4. **Permissão de execução** no arquivo `.so`.

Resolvido isso, a chamada `mysum(…)` passa a retornar o
resultado correto. Bons testes!

[1]: https://www.mapleprimes.com/questions/43609--Save-A-Module-Containing-defineexternal "save a module containing \"define_external\" as a maple library"

