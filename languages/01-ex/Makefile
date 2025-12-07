# Makefile: A standard Makefile for simple.c
MAPLE_DIR = /opt/maple2021
MAPLE_BIN = $(MAPLE_DIR)/bin.X86_64_LINUX

CFLAGS    = -I$(MAPLE_DIR)/extern/include
LDLIBS    = -L$(MAPLE_BIN) -lmaplec -lrt

# Adiciona rpath para evitar problemas com LD_LIBRARY_PATH
# O $$ escapa o $ no Makefile, e ORIGIN deve estar entre aspas simples
LDFLAGS=-Wl,-rpath,$(MAPLE_BIN) -Wl,-rpath,$$ORIGIN

all: simple

simple: simple.c
	$(CC) $(CFLAGS) simple.c $(LDFLAGS) $(LDLIBS) -o simple

license:
	ln -s /opt/maple2021/license ..

run: simple
	@echo "Checking Maple libraries..."
	@if [ ! -L libmaple.so ]; then \
		echo "Creating symlinks to Maple libraries..."; \
		for lib in $(MAPLE_BIN)/*.so*; do \
			ln -sf $$lib . 2>/dev/null || true; \
		done; \
	fi
	@echo "Running simple..."
	@LD_LIBRARY_PATH=$(MAPLE_BIN):.:$$LD_LIBRARY_PATH ./simple

check-libs: simple
	@echo "=== Checking library dependencies ==="
	@ldd simple
	@echo ""
	@echo "=== Available Maple libraries ==="
	@ls -la $(MAPLE_BIN)/*.so 2>/dev/null | head -10

clean:
	/bin/rm -rf simple simple.o
	@echo "To remove symlinks, run: make clean-all"

clean-all: clean
	/bin/rm -f *.so *.so.*

.PHONY: all run clean clean-all check-libs

# END OF FILE
