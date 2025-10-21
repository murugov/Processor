FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

COMMON_INCLUDES = -I./COMMON/headers
GEN_INCLUDES  	= -I./GENERATOR/headers -I./GENERATOR/input -I./GENERATOR/output
ASM_INCLUDES  	= -I./COMPILER/headers
SPU_INCLUDES  	= -I./SPU/headers
STK_INCLUDES  	= -I./SPU/STACK/headers

COMMON_FILES = COMMON/IsBadPtr.cpp COMMON/LineCounter.cpp COMMON/SizeFile.cpp COMMON/TXTreader.cpp
GEN_FILES 	 = COMPILER/HashCmd.cpp
ASM_FILES 	 = COMPILER/ArrPtrCtor.cpp COMPILER/AsmArgParser.cpp COMPILER/AsmCmdWrt.cpp COMPILER/AsmErrPrint.cpp COMPILER/AsmVerifySort.cpp COMPILER/CodeCtor.cpp COMPILER/FirstCompilation.cpp COMPILER/HashCmd.cpp COMPILER/SecondCompilation.cpp
SPU_FILES 	 = SPU/CalcFunc.cpp SPU/spuCtor.cpp SPU/spuErrPrint.cpp SPU/spuExecutor.cpp SPU/SpuVerifySort.cpp SPU/SpuArgParser.cpp
STK_FILES 	 = SPU/STACK/HashFunc.cpp

DEFAULT_INPUT  ?= CompileFiles/source.asm
DEFAULT_OUTPUT ?= CompileFiles/bytecode.asm


all: help

gen: GENERATOR/main_gen.cpp $(COMMON_FILES) $(GEN_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o gen_program $(FLAGS) GENERATOR/main_gen.cpp $(COMMON_INCLUDES) $(GEN_INCLUDES) $(ASM_INCLUDES) $(COMMON_FILES) $(GEN_FILES)
	@echo "-----------------------------------------------------------------------------------------"

comp: COMPILER/main_comp.cpp $(COMMON_FILES) $(ASM_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o comp_program $(FLAGS) COMPILER/main_comp.cpp $(COMMON_INCLUDES) $(GEN_INCLUDES) $(ASM_INCLUDES) $(COMMON_FILES) $(ASM_FILES)
	@echo "-----------------------------------------------------------------------------------------"

spu: SPU/main_spu.cpp $(COMMON_FILES) $(STK_FILES) $(SPU_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o spu_program $(FLAGS) SPU/main_spu.cpp $(COMMON_INCLUDES) $(SPU_INCLUDES) $(STK_INCLUDES) $(COMMON_FILES) $(GEN_INCLUDES) $(STK_FILES) $(SPU_FILES)
	@echo "-----------------------------------------------------------------------------------------"


run-gen: gen
	./gen_program

run-comp: comp
	./comp_program $(DEFAULT_INPUT) $(DEFAULT_OUTPUT)

run-spu: spu
	./spu_program

run: run-spu

run-comp-args: comp
	@if [ "$(ARGS)" = "" ]; then \
		echo "Usage: make run-comp-args ARGS=\"input.asm output.asm\""; \
	fi
	./comp_program $(ARGS)

clean:
	rm -f gen_program comp_program spu_program

help:
	@echo "Available commands:"
	@echo "  make gen                     - compile a generator (main_gen.cpp)"
	@echo "  make comp                    - compile a compiler (main_comp.cpp)"
	@echo "  make spu                     - compile a spu (main_spu.cpp)"
	@echo ""
	@echo "  make run-gen                 - compile and run generator"
	@echo ""
	@echo "  make run-comp                - compile and run compiler with default files ($(DEFAULT_INPUT) $(DEFAULT_OUTPUT))"
	@echo "  make run-comp-args           - run compiler with any arguments:"
	@echo "                                 make run-comp-args ARGS=\"input.asm output.asm\""
	@echo "  make run-spu                 - compile and run spu"
	@echo "  make run                     - compile and run spu (default)"
	@echo ""
	@echo "  make clean                   - remove compiled programs"

.PHONY: gen comp spu run-gen run-comp run-spu run run-comp-args clean help