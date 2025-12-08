**🎉 EUREKA! DESCOBRIMOS A VERDADE!**

## 🔍 Análise dos Resultados:

```
ℹ️  Variável $MAPLE detectada: /opt/maple2021
✅ SUCESSO sem precisar configurar libname!
>>> Resultado: 8/3
```

### A Revelação:

**O Maple FUNCIONOU sem configurar `libname` porque `$MAPLE` está definido!**

Isso significa:

1. ✅ **O Maple usa `$MAPLE` internamente** durante `StartMaple()`
2. ✅ **Se `$MAPLE` está definido, ele encontra `lib/` automaticamente**
3. ✅ **Não precisa de `EvalMapleStatement(kv, "libname := ...")`**

## 🧪 Vamos Confirmar - Teste Final:

Rode este comando para testar **SEM** `$MAPLE`:

```bash
env -u MAPLE -u MAPLE_ROOT ./main
```

Isso vai:
- Remover `$MAPLE` do ambiente
- Executar o programa
- Revelar se realmente precisa configurar `libname` manualmente

## 🎯 Previsão:

**Cenário 1: Se falhar sem $MAPLE**
```
❌ Retornou NULL - falhou!
Configurando via hardcoded...
✅ SUCESSO após configurar libname!
```
→ Confirma que `$MAPLE` é necessário OU precisa configurar manualmente

**Cenário 2: Se funcionar sem $MAPLE**
```
✅ SUCESSO sem precisar configurar libname!
```
→ Significa que o `rpath` do Makefile é suficiente!

## 💡 Por que seus códigos anteriores falharam?

Provavelmente porque:
1. `$MAPLE` **não estava** configurado quando você testou antes
2. O `rpath` sozinho não é suficiente para o Maple encontrar `lib/`
3. Você precisou do `libname := "/opt/maple2021/lib"` manual

**Rode o teste com `env -u MAPLE` e me mostre o resultado!** 🔬
