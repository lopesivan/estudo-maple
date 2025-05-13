# Lista de arquivos-base (sem extensão)
TARGETS := 03 04

# Regra genérica: gerar PDF a partir de PS
%.pdf: %.ps
	epspdf $<
	convert $< $@

# Regra genérica: gerar PS a partir de MPL
%.ps: %.mpl
	maple -q < $<

# Alvo principal: gera todos os PDFs
all: $(TARGETS:%=%.pdf)

# Exibe o PDF com zathura
show%: %.pdf
	zathura $*.pdf

# Remove arquivos gerados
clean:
	rm -f $(TARGETS:%=%.ps) $(TARGETS:%=%.pdf) $(TARGETS:%=%.png)

# Alvo de ajuda
help:
	@echo "Makefile de geração automática com Maple:"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  make            Gera todos os PDFs: $(TARGETS)"
	@echo "  make 03.pdf     Gera somente 03.pdf"
	@echo "  make show03     Abre 03.pdf com zathura"
	@echo "  make clean      Remove arquivos .ps, .pdf e .png"
	@echo "  make help       Mostra esta ajuda"
