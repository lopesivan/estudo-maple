# Makefile para exemplos Maple C API
MAPLE_DIR = /opt/maple2021
MAPLE_BIN = $(MAPLE_DIR)/bin.X86_64_LINUX

CC=gcc
CFLAGS  = -Wall -Wextra -g -I$(MAPLE_DIR)/extern/include -Wall -Wextra
LDLIBS  = -L$(MAPLE_BIN) -lmaplec -lrt -lm
LDFLAGS = -Wl,-rpath,$(MAPLE_BIN) -Wl,-rpath,$(MAPLE_BIN)

# Targets
TARGETS = main

all: $(TARGETS)

main: main.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

license:
	ln -s /opt/maple2021/license ..

# Executar exemplos
run: main
	@echo "=== Executando exemplo mains ==="
	@$(call setup_libs)
	@LD_LIBRARY_PATH=$(MAPLE_BIN):.:$$LD_LIBRARY_PATH ./$^

# Verificar dependências
check-libs: $(TARGETS)
	@echo "=== Verificando main ==="
	@ldd main | grep -E "(maple|imf|svml|irng|intlc)" || true
	@echo ""
	@echo "=== Verificando advanced_maple ==="
	@ldd advanced_maple | grep -E "(maple|imf|svml|irng|intlc)" || true

# Setup de bibliotecas
setup-libs:
	@if [ ! -L libmaple.so ]; then \
		echo "Criando symlinks das bibliotecas Maple..."; \
		for lib in $(MAPLE_BIN)/*.so*; do \
			ln -sf $$lib . 2>/dev/null || true; \
		done; \
		echo "✅ Symlinks criados"; \
	fi

# Limpar
clean:
	rm -f $(TARGETS) *.o
	@echo "Para remover symlinks: make clean-all"

dry: clean
	rm -f *.so *.so.*
	rm ../license

# Ajuda
help:
	@echo "Targets disponíveis:"
	@echo "  make all           - Compila todos os exemplos"
	@echo "  make main        - Compila apenas exemplo mains"
	@echo "  make run           - Executa todos"
	@echo "  make check-libs    - Verifica dependências"
	@echo "  make setup-libs    - Cria symlinks das bibliotecas"
	@echo "  make clean         - Remove executáveis"
	@echo "  make clean-all     - Remove tudo (incluindo symlinks)"

.PHONY: all run run-main run-advanced check-libs setup-libs clean clean-all help

# Função auxiliar para setup
define setup_libs
	if [ ! -L libmaple.so ]; then \
		for lib in $(MAPLE_BIN)/*.so*; do \
			ln -sf $$lib . 2>/dev/null || true; \
		done; \
	fi
endef
