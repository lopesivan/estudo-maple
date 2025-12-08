Segmentation fault! Vamos debugar. O problema provavelmente está na função `calculate_function_points`. Vou corrigir:Corrigi os problemas principais:

## 🔧 Correções:

1. **Removida variável não usada** `eqn`
2. **Verificação de NULL** em alocação de memória
3. **Verificação de resultados** do Maple antes de usar
4. **Verificação de strlen** antes de usar string
5. **Uso de `subs`** em vez de `f(valor)` para maior estabilidade
6. **Tipo correto** em `drawText` (`size_t` em vez de `int`)
7. **Adicionado `:` no final** dos comandos Maple para suprimir output

## 🧪 Teste novamente:

```bash
make clean
make line
make run-line
```

Se ainda der segfault, rode com debugger:

```bash
gdb ./line
run
# Quando crashar, digite:
bt
```

E me mostre o backtrace!


$ LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX:. ./line
╔════════════════════════════════════════════════════════╗
║  Plotador Interativo de Funções                       ║
║  OpenMaple + OpenGL                                    ║
╚════════════════════════════════════════════════════════╝

✓ Maple inicializado
✓ Libname configurado via $MAPLE
Calculando função: sin(x)
Falha de segmentação     (imagem do núcleo gravada) LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX:. ./line
ivan*:  /workspace/estudo-maple/languages/17-ex main ⇡6 !2 ?2
$ LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX:.  gdb  ./line
GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./line...
(gdb) run
Starting program: /workspace/estudo-maple/languages/17-ex/line

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.ubuntu.com>
Enable debuginfod for this session? (y or [n]) yes
Debuginfod has been enabled.
To make this setting permanent, add 'set debuginfod enabled on' to .gdbinit.
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libmaplec.so
Downloading separate debug info for /lib/x86_64-linux-gnu/libglut.so.3.12
Downloading separate debug info for /lib/x86_64-linux-gnu/libGL.so.1
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Downloading separate debug info for /lib/x86_64-linux-gnu/libstdc++.so.6
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libimf.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libmaple.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libsvml.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libirng.so
Downloading separate debug info for /lib/x86_64-linux-gnu/libgcc_s.so.1
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libintlc.so.5
Downloading separate debug info for /lib/x86_64-linux-gnu/libX11.so.6
Downloading separate debug info for /lib/x86_64-linux-gnu/libXxf86vm.so.1
Downloading separate debug info for /lib/x86_64-linux-gnu/libXi.so.6
Downloading separate debug info for /lib/x86_64-linux-gnu/libGLdispatch.so.0
Downloading separate debug info for /lib/x86_64-linux-gnu/libGLX.so.0
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/liblmx-altair.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libhf.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libprocessor.so
Downloading separate debug info for /lib/x86_64-linux-gnu/libxcb.so.1
Downloading separate debug info for /lib/x86_64-linux-gnu/libXext.so.6
Downloading separate debug info for /lib/x86_64-linux-gnu/libXau.so.6
Downloading separate debug info for /lib/x86_64-linux-gnu/libXdmcp.so.6
Downloading separate debug info for /lib/x86_64-linux-gnu/libbsd.so.0
Downloading separate debug info for /lib/x86_64-linux-gnu/libmd.so.0
╔════════════════════════════════════════════════════════╗
║  Plotador Interativo de Funções                       ║
║  OpenMaple + OpenGL                                    ║
╚════════════════════════════════════════════════════════╝

Downloading separate debug info for /lib/x86_64-linux-gnu/libnss_sss.so.2
[New Thread 0x7fffedbff6c0 (LWP 445272)]
[New Thread 0x7fffeb7ff6c0 (LWP 445273)]
✓ Maple inicializado
✓ Libname configurado via $MAPLE
Calculando função: sin(x)
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libint.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libnag.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libI77.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libF77.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libclapack.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libmkl_rt.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libmkl_intel_lp64.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libmkl_intel_thread.so
Downloading separate debug info for /opt/maple2021/bin.X86_64_LINUX/libiomp5.so

Thread 1 "line" received signal SIGSEGV, Segmentation fault.
0x00007ffff5e914a3 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
(gdb) bt
#0  0x00007ffff5e914a3 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#1  0x00007ffff5e92227 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#2  0x00007ffff5e95994 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#3  0x00007ffff5e937b1 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#4  0x00007ffff5e92227 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#5  0x00007ffff5e95994 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#6  0x00007ffff5e937b1 in simpl () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#7  0x00007ffff5e9a299 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#8  0x00007ffff60028c8 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#9  0x00007ffff621828a in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#10 0x00007ffff5cb37f6 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#11 0x00007ffff60e062e in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#12 0x00007ffff60d5c40 in ?? () from /opt/maple2021/bin.X86_64_LINUX/libmaple.so
#13 0x0000555555556a15 in calculate_function_points (function_str=0x555555558010 "sin(x)")
    at line.c:138
#14 0x0000555555557c00 in initMaple (argc=1, argv=0x7fffffffcb08) at line.c:446
#15 0x0000555555557c77 in main (argc=1, argv=0x7fffffffcb08) at line.c:460


