# Makefile para exemplos Maple C API
MAPLE_DIR = /opt/maple2021
MAPLE_BIN = $(MAPLE_DIR)/bin.X86_64_LINUX

CC = gcc
CFLAGS = -Wall -Wextra -g -I$(MAPLE_DIR)/extern/include

# Bibliotecas base do Maple
MAPLE_LIBS = -L$(MAPLE_BIN) -lmaplec -lrt -lm

# Bibliotecas OpenGL (para line.c)
OPENGL_LIBS = -lglut -lGLU -lGL

LDFLAGS = -Wl,-rpath,$(MAPLE_BIN) -Wl,-rpath,$$ORIGIN

# Targets
TARGETS = line

all: $(TARGETS)

# Compilar line.c (com OpenGL)
line: line.c
	@echo "Compilando line.c (requer OpenGL/GLUT)..."
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(MAPLE_LIBS) $(OPENGL_LIBS) -o $@

license:
	ln -s /opt/maple2021/license ..

# Executar exemplos
run: line
	@echo "=== Executando line (OpenMaple + OpenGL) ==="
	@echo "Instruções:"
	@echo "  - Clique botão esquerdo: adicionar pontos"
	@echo "  - Clique botão direito: menu de interpolação"
	@echo "  - Tecla 'c': limpar tela"
	@echo "  - Tecla 'q': sair"
	@echo ""
	@$(call setup_libs)
	@LD_LIBRARY_PATH=$(MAPLE_BIN):.:$$LD_LIBRARY_PATH ./line

# Verificar se OpenGL está disponível
check-opengl:
	@echo "=== Verificando OpenGL/GLUT ==="
	@if pkg-config --exists glut 2>/dev/null; then \
		echo "✓ OpenGL/GLUT encontrado"; \
		pkg-config --modversion glut 2>/dev/null || echo "  (versão não disponível via pkg-config)"; \
	elif ldconfig -p | grep -q libglut; then \
		echo "✓ libglut encontrado via ldconfig"; \
	else \
		echo "❌ OpenGL/GLUT não encontrado!"; \
		echo ""; \
		echo "Instalar no Ubuntu/Debian:"; \
		echo "  sudo apt-get install freeglut3-dev"; \
		echo ""; \
		echo "Instalar no Fedora/RedHat:"; \
		echo "  sudo dnf install freeglut-devel"; \
	fi

# Verificar dependências
check-libs: $(TARGETS)
	@echo "=== Verificando main ==="
	@ldd main | grep -E "(maple|imf|svml|irng|intlc)" || true
	@echo ""
	@if [ -f line ]; then \
		echo "=== Verificando line ==="; \
		ldd line | grep -E "(maple|glut|GL)" || true; \
	fi

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
	@echo "Para remover symlinks: make dry"

dry: clean
	rm -f *.so *.so.*
	rm -f ../license

# Ajuda
help:
	@echo "Targets disponíveis:"
	@echo "  make all           - Compila todos (main + line)"
	@echo "  make line          - Compila line.c (requer OpenGL)"
	@echo "  make run           - Executa main"
	@echo "  make check-opengl  - Verifica se OpenGL está instalado"
	@echo "  make check-libs    - Verifica dependências"
	@echo "  make setup-libs    - Cria symlinks das bibliotecas"
	@echo "  make license       - Cria symlink da licença"
	@echo "  make clean         - Remove executáveis"
	@echo "  make dry           - Remove tudo (executáveis + symlinks)"

.PHONY: all run check-opengl check-libs setup-libs license clean dry help

# Função auxiliar para setup
define setup_libs
	if [ ! -L libmaple.so ]; then \
		for lib in $(MAPLE_BIN)/*.so*; do \
			ln -sf $$lib . 2>/dev/null || true; \
		done; \
	fi
endef
