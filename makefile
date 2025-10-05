FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

GEN_INCLUDES = -I./GenFiles
ASM_INCLUDES = -I./COMPILER/headers
CPU_INCLUDES = -I./CPU/headers

GEN_FILES = GenFiles/IsBadPtr.cpp GenFiles/SizeFile.cpp
ASM_FILES = COMPILER/AsmErrPrint.cpp COMPILER/Assembly.cpp COMPILER/Compiler.cpp COMPILER/HashCmd.cpp
CPU_FILES = CPU/CalcErrPrint.cpp CPU/Calculator.cpp CPU/HashFunc.cpp CPU/StackDump.cpp CPU/StackFunc.cpp CPU/StackVerify.cpp

all: help

comp: COMPILER/main_comp.cpp $(GEN_FILES) $(ASM_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ $(FLAGS) COMPILER/main_comp.cpp $(GEN_FILES) $(ASM_FILES) $(GEN_INCLUDES) $(ASM_INCLUDES) -o comp_program
	@echo "-----------------------------------------------------------------------------------------"

cpu: CPU/main_cpu.cpp $(GEN_FILES) $(CPU_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ $(FLAGS) CPU/main_cpu.cpp $(GEN_FILES) $(CPU_FILES) $(GEN_INCLUDES) $(CPU_INCLUDES) -o cpu_program
	@echo "-----------------------------------------------------------------------------------------"

run-comp: comp
	./comp_program

run-cpu: cpu
	./cpu_program

run: run-cpu

clean:
	rm -f comp_program cpu_program

help:
	@echo "Available commands:"
	@echo "  make comp      - compile a compiler (main_comp.cpp)"
	@echo "  make cpu      - compile a cpu version (main_cpu.cpp)"
	@echo "  make run-comp  - compile and run the compiler"
	@echo "  make run-cpu  - compile and run the cpu version"
	@echo "  make run       - compile and run the cpu version (default)"
	@echo "  make clean     - remove compiled programs"