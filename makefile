FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

COMMON_INCLUDES = -I./COMMON/headers
GEN_INCLUDES  = -I./GENFILES/files
ASM_INCLUDES  = -I./COMPILER/headers
SPU_INCLUDES  = -I./SPU/headers
STK_INCLUDES  = -I./SPU/STACK/headers

COMMON_FILES = COMMON/IsBadPtr.cpp COMMON/LineCounter.cpp COMMON/SizeFile.cpp COMMON/TXTreader.cpp
ASM_FILES = COMPILER/ArrPtrCtor.cpp COMPILER/AsmCmdWrt.cpp COMPILER/AsmErrPrint.cpp COMPILER/Assembler.cpp COMPILER/HashCmd.cpp
SPU_FILES = SPU/CalcFunc.cpp SPU/spuErrPrint.cpp SPU/spuExecutor.cpp
STK_FILES = SPU/STACK/headers/hash.h SPU/STACK/headers/stack.h SPU/STACK/HashFunc.hpp SPU/STACK/StackFunc.hpp SPU/STACK/StackVerify.hpp SPU/STACK/StackDump.hpp

all: help

comp: COMPILER/main_comp.cpp $(COMMON_FILES) $(ASM_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ $(FLAGS) COMPILER/main_comp.cpp $(COMMON_FILES) $(ASM_FILES) $(COMMON_INCLUDES) $(GEN_INCLUDES) $(ASM_INCLUDES) -o comp_program
	@echo "-----------------------------------------------------------------------------------------"

spu: SPU/main_spu.cpp $(COMMON_FILES) $(STK_FILES) $(SPU_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ $(FLAGS) $(COMMON_INCLUDES) $(SPU_INCLUDES) $(STK_INCLUDES) SPU/main_spu.cpp $(COMMON_FILES) $(STK_FILES) $(SPU_FILES)
	@echo "-----------------------------------------------------------------------------------------"

run-comp: comp
	./comp_program

run-spu: spu
	./spu_program

run: run-spu

clean:
	rm -f comp_program spu_program

help:
	@echo "Available commands:"
	@echo "  make comp      - compile a compiler (main_comp.cpp)"
	@echo "  make spu      - compile a spu (main_spu.cpp)"
	@echo "  make run-comp  - compile and run the compiler"
	@echo "  make run-spu  - compile and run the spu"
	@echo "  make run       - compile and run the spu (default)"
	@echo "  make clean     - remove compiled programs"

.PHONY: comp spu run-comp run-spu run clean help