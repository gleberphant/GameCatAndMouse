# Nome do executável
TARGET = main

# Arquivos com fonte
SOURCES = animation.c actor.c itens.c lists.c maps.c main.c

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