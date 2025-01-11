# Nome do executável
TARGET = main

# Arquivos com fonte
SOURCES = main.c

# Flags do compilador
CFLAGS = -Wall -Wextra -g -I.\include\

# Bibliotecas
LDFLAGS =-lraylib -L.\lib -lopengl32 -lgdi32 -luser32 -lwinmm -mwindows

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