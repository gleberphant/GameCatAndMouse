# Nome do executável
TARGET = game

# Arquivos com fonte
SOURCES = src\animation.c src\actor.c src\itens.c src\lists.c src\maps.c src\main.c

# Flags do compilador
CFLAGS = -Wall -Wextra -g -I.\raylib\include\

# Bibliotecas
LDFLAGS =-lraylib -L.\raylib\lib -lopengl32 -lgdi32 -luser32 -lwinmm

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS) 

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)

# Regra para executar o programa
run: all
	./$(TARGET)