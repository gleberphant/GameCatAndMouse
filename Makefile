# Nome do executável
TARGET = game

# Compilador
CC = gcc

# Linker
LD = gcc

# Diretório de inclusão
INCLUDE_DIRS = -I./raylib/include -I./src

# Diretório de bibliotecas
LIB_DIRS = -L./raylib/lib

# Bibliotecas
LIBS = -lraylib -lopengl32 -lgdi32 -luser32 -lwinmm

# Diretório dos arquivos fonte
SRC_DIR = src

# Diretório dos arquivos objeto
OBJ_DIR = src/out

# Arquivos com fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto (gerados automaticamente a partir dos arquivos fonte)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Flags do compilador
CFLAGS = -Wall -Wextra -g $(INCLUDE_DIRS)

# Flags do linker
LDFLAGS = $(LIB_DIRS) $(LIBS) -Wl,-subsystem,console

# Regra padrão
all: $(TARGET)

# Cria o diretório build se ele não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regra para linkar os arquivos objeto e gerar o executável
$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Regra genérica para compilar arquivos .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -rf $(OBJ_DIR)

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Declara clean como um target phony
.PHONY: clean