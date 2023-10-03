# 设置编译器
CC = g++

# 设置源文件
SOURCES = CardCombination.cpp DemoBot.cpp main.cpp

# 设置输出文件名
OUTPUT = test

# 默认的目标
all: $(OUTPUT)

# 编译命令
$(OUTPUT): $(SOURCES)
	$(CC) $^ -o $@

# 清理命令
clean:
	rm -f $(OUTPUT)
