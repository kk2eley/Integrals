# Используемые компиляторы и их флаги
CC=gcc                          # Устанавливает gcc как компилятор для C/C++ кода.
CFLAGS=-Wall -m32 -std=c11      # Устанавливает флаги для компилятора C: -Wall (предупреждения), -m32 (32-битная архитектура), -std=c11 (стандарт C11).
NASM=nasm                       # Устанавливает NASM как ассемблер.
NASMFLAGS=-f elf32              # Устанавливает флаги для NASM: -f elf32 (формат вывода ELF 32-бит).
LD=gcc                          # Указывает gcc как компоновщик (линкер).
LDFLAGS=-m32                    # Устанавливает флаги для компоновщика: -m32 (32-битная архитектура).
SPEC_FILE?=in.txt             # Устанавливает файл спецификации по умолчанию, если он не задан при вызове make.
METHOD=                          # Пустая переменная для условных флагов компиляции.

# Исходные файлы и целевые программы
PARSER_SRC=parser.c             # Устанавливает исходный файл парсера.
PARSER_OUT=parser               # Устанавливает выходное имя файла исполняемого файла парсера.
ASM_SRC_HDL=rpnfunctions.asm 
ASM_OUT_HDL=rpnfunctions.o        # Имя файла для сгенерированного ассемблерного кода.
ASM_SRC=functions.asm
ASM_OUT=functions.o
OBJ_FILES_HDL=main.o functions.o rpnfunctions.o   # Список объектных файлов, которые нужно собрать.
OBJ_FILES=main.o functions.o
TARGET=program                  # Целевой исполняемый файл для всего проекта.
HDL_TARGET=programhdl      # Исполняемый файл для цели handle.

all: $(TARGET)                   # Основная цель, строит целевой исполняемый файл программы.

chord: METHOD="-DCHORD"          # Устанавливает метод для компиляции с определёнными макросами (chord).
chord: all                       # Перестраивает основную цель с методом chord.

tangent: METHOD="-DTANGENT"      # Устанавливает метод для компиляции с определёнными макросами (tangent).
tangent: all                     # Перестраивает основную цель с методом tangent.

handle: METHOD="-DHANDLE" "-DCHORD"         # Устанавливает метод для компиляции с определёнными макросами (chord) для handle.
handle: $(HDL_TARGET)      # Строит исполняемый файл rpnfunctions с методом chord.

$(HANDLE_TARGET): $(OBJ_FILES_HDL)   # Строит файл rpnfunctions из объектных файлов.
	$(LD) $(LDFLAGS) -o $@ $^     # Связывает объектные файлы в исполняемый файл.

main.o: main.c                   # Строит объектный файл main.o из исходника main.c.
	$(CC) $(CFLAGS) $(METHOD) -c -o $@ $<  # Компиляция с переданными флагами.

$(ASM_SRC_HDL): $(PARSER_OUT) $(SPEC_FILE)  # Генерирует ассемблерный код.
	./$(PARSER_OUT) $(SPEC_FILE)  # Запускает парсер.

$(PARSER_OUT): $(PARSER_SRC)     # Строит исполняемый файл парсера из исходника.
	$(CC) $(CFLAGS) -o $@ $<      # Компиляция парсера.

$(ASM_OUT): $(ASM_SRC)          # Строит объектный файл functions.o из ассемблерного кода.
	$(NASM) $(NASMFLAGS) -o $@ $<  # Ассемблирование кода.
	
$(ASM_OUT_HDL): $(ASM_SRC_HDL)
	$(NASM) $(NASMFLAGS) -o $@ $<

$(HDL_TARGET): $(OBJ_FILES_HDL)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(OBJ_FILES)          # Строит основной исполняемый файл из объектных файлов.
	$(LD) $(LDFLAGS) -o $@ $^     # Связывание объектных файлов.

clean:                           # Цель для очистки проекта.
	rm -f $(PARSER_OUT) $(TARGET) $(HDL_TARGET) $(OBJ_FILES) $(ASM_OUT) $(ASM_OUT_HDL)  # Удаление сгенерированных файлов.

.PHONY: all clean chord tangent handle  # Указывает, что перечисленные цели не связаны с файлами.

