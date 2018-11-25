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
	$(CC) -c -o $@ $< $(CFLAGS)

# Compilação do executável do projeto.

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Lista de comandos adicionais do makefile.

.PHONY: clean
.PHONY: structure
.PHONY: verification
.PHONY: test

# Comando para limpar o executável do projeto e os arquivos .o.

clean:
	@rm -f $(ODIR)/*.o *~ core
	@rm -f $(ASM_ODIR)/%.o *~ core
	@rm -f $(ASM_SDIR)/%.s *~ core
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

# Comando para testar automaticamente a compilação

ASM_ODIR = test/obj
ASM_SDIR = test
_ASM_SRC = bin.s bin2.s fat.s test.s triangulo.s
ASM_SRC = $(patsubst %,$(ASM_SDIR)/%,$(_ASM_SRC))
_ASM_OBJ = bin.o bin2.o fat.o test.o triangulo.o
ASM_OBJ = $(patsubst %,$(ASM_ODIR)/%,$(_ASM_OBJ))
_ASM_OUT = bin2.out fat.out triangulo.out test.out bin.out
ASM_OUT = $(patsubst %,$(ASM_SDIR)/%,$(_ASM_OUT))

test: $(ASM_OUT) $(ASM_OBJ) $(ASM_SRC)

$(ASM_SDIR)/%.s: $(ASM_SDIR)/%.asm $(EXE)
	./$(EXE) $(basename $<)

$(ASM_ODIR)/%.o: $(ASM_SDIR)/%.s
	nasm -f elf -o $@ $<

$(ASM_SDIR)/%.out: $(ASM_ODIR)/%.o
	ld -m elf_i386 -o $@ $<
