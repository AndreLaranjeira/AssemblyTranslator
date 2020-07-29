# Nome do executável do projeto.

EXE = tradutor

# Nome do compilador, extensão dos arquivos source e dados de compilação
# (flags e bibliotecas).

CC = g++
EXT = .cpp
CFLAGS = -Wall -g -I $(IDIR)
LIBS = -lm

# Caminhos até pastas importantes (arquivos .h, bibliotecas externas,
# arquivos .o, arquivos com testes e, opcionalmente, arquivos do gcov).

IDIR = include
ODIR = src/obj
SDIR = src

# Lista de dependências do projeto (arquivos .h).

_DEPS = asm_file.hpp error_logger.hpp line.hpp translator.hpp instruction.hpp

# Lista de arquivos intermediários de compilação gerados pelo projeto
# (arquivos .o).

_OBJ = asm_file.o error_logger.o main.o translator.o instruction.o line.o

# Lista de arquivos fontes utilizados para compilação.

_SRC = asm_file.cpp error_logger.cpp main.cpp translator.cpp instruction.cpp line.cpp

# Junção dos nomes de arquivos com seus respectivos caminhos.

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

# Atualização de arquivos que foram alterados.

$(ODIR)/%.o: $(SDIR)/%$(EXT) $(DEPS)
	@if ! [ -d $(ODIR) ]; then mkdir -p $(ODIR); fi
	$(CC) -c -o $@ $< $(CFLAGS)

# Compilação do executável do projeto.

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Lista de comandos adicionais do makefile.

.PHONY: test
.PHONY: clean
.PHONY: structure
.PHONY: verification

# Comando para testar automaticamente a compilação

ASM_ODIR = test/obj
ASM_SDIR = test
_ASM_SRC = bin.s fat.s triangulo.s strings.s chars.s overflow_mul.s
ASM_SRC = $(patsubst %,$(ASM_SDIR)/%,$(_ASM_SRC))
_ASM_OBJ = bin.o fat.o triangulo.o strings.o chars.o overflow_mul.o
ASM_OBJ = $(patsubst %,$(ASM_ODIR)/%,$(_ASM_OBJ))
_ASM_OUT = bin.out fat.out triangulo.out strings.out chars.out overflow_mul.out
ASM_OUT = $(patsubst %,$(ASM_SDIR)/%,$(_ASM_OUT))

test: $(ASM_OUT) $(ASM_OBJ) $(ASM_SRC)

$(ASM_SDIR)/%.s: $(ASM_SDIR)/%.asm $(EXE)
	./$(EXE) $(basename $<)

$(ASM_ODIR)/%.o: $(ASM_SDIR)/%.s
	@if ! [ -d $(ASM_ODIR) ]; then mkdir -p $(ASM_ODIR); fi
	nasm -f elf -o $@ $<

$(ASM_SDIR)/%.out: $(ASM_ODIR)/%.o
	ld -m elf_i386 -o $@ $<

# Comando para limpar o executável do projeto e os arquivos .o.

clean:
	@rm -f $(ODIR)/*.o *~ core
	@rm -f $(ASM_ODIR)/*.o *~ core
	@rm -f $(ASM_SDIR)/*.s *~ core
	@rm -f $(ASM_SDIR)/*.out *~ core
	@if [ -d $(ODIR) ]; then rmdir $(ODIR); fi
	@if [ -d $(ASM_ODIR) ]; then rmdir $(ASM_ODIR); fi
	@if [ -f $(EXE) ]; then rm $(EXE) -i; fi

# Comando para gerar a estrutura inicial do projeto.

structure:

	# Criação das pastas do projeto.

	mkdir include
	mkdir src
	mkdir src/obj

	# Movimentação dos arquivos existentes para suas respectivas pastas.

	if [ -f *.h ]; then mv *.h $(IDIR); fi
	if [ -f *$(EXT) ]; then mv *$(EXT) $(SDIR); fi
	if [ -f *.o ]; then mv *.o $(ODIR); fi

# Comando para verificar os testes utilizando o cppcheck e o valgrind.

verification:
	cppcheck $(SRC) ./$(EXE) --enable=all
	valgrind --leak-check=full ./$(EXE)
