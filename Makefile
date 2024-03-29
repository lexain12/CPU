CXX 	 = g++
CXXFLAGS = -O2 -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla 
ASMFILES = Assembler/main.cpp Assembler/assemblerFunc.cpp
CPUFILES = main.cpp CPUfuctions.cpp
CLANGSTZ = clang++  -g -std=c++17 -fsanitize=address,undefined,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero
ASMFILE  = Assembler/bin/

all: cpu asm 

cpu: main.o CPUfunctions.o
	$(CXX) bin/main.o bin/CPUfunctions.o stack.cpp -o CPU

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -c -o bin/main.o 

CPUfunctions.o: CPUfunctions.cpp
	$(CXX) $(CXXFLAGS) CPUfunctions.cpp -c -o bin/CPUfunctions.o

cpusanitize:
	$(CLANGSTZ) main.cpp CPUfunctions.cpp stack.cpp -o CPU

asm: asmmain.o assemblerFunc.o
	$(CXX) $(ASMFILE)main.o $(ASMFILE)assemblerFunc.o -o ASM

asmmain.o: Assembler/main.cpp
	$(CXX) $(CXXFLAGS) Assembler/main.cpp -c -o $(ASMFILE)main.o

assemblerFunc.o: Assembler/assemblerFunc.cpp
	$(CXX) $(CXXFLAGS) Assembler/assemblerFunc.cpp -c -o $(ASMFILE)assemblerFunc.o

asmsanitize: 
	$(CLANGSTZ) Assembler/main.cpp Assembler/assemblerFunc.cpp -o ASM

