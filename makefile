CC = g++
flags = -Wall -std=c++20 -O -fsanitize=address -Wextra
linkFlags = -fsanitize=address

headers = Exception/exception.h Text/text.h Prog/prog.h KMP/KMP.h

exceptionFile = Exception/exception.cpp
exceptionIncludes = $(headers)
exceptionObj = exception.o

textFile = Text/text.cpp 
textIncludes = $(headers)
textObj = text.o

randFile = Text/randomText.cpp 
randIncludes = $(headers) $(textFile)
randObj = randText.o

progFile = Prog/prog.cpp
progIncludes = $(headers)
progObj = prog.o

kmpFile = KMP/KMP.cpp
kmpIncludes = $(headers)
kmpObj = kmp.o

mainFile = work.cpp
objs = $(exceptionObj) $(textObj) $(randObj) $(kmpObj) $(progObj)
exe = textUtils


all: $(exe) clean execution

$(exe):  $(objs) main.o
	@echo -n
	@echo edition des liens :
	@echo -n "   "
	$(CC) $(linkFlags) -o $@ $(objs) main.o

$(textObj) : $(textFile)
	@echo -n
	@echo compilation de $(textFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(textFile)
$@ : $(textIncludes)

$(randObj) : $(randFile)
	@echo -n
	@echo compilation de $(randFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(randFile)
$@ : $(randIncludes)

$(exceptionObj) : $(excetionFile)
	@echo -n
	@echo compilation de $(exceptionFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(exceptionFile)
$@ : $(exceptionIncludes)

$(progObj) : $(progFile)
	@echo -n
	@echo compilation de $(progFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(progFile)
$@ : $(progIncludes)

$(kmpObj) : $(kmpFile)
	@echo -n
	@echo compilation de $(kmpFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(kmpFile)
$@ : $(kmpIncludes)

main.o : $(mainFile)
	@echo -n
	@echo compilation de $(mainFile):
	@echo -n "   "
	$(CC) $(flags) -o $@ -c $(mainFile)

execution:
	@echo -n
	@echo execution :
	@echo -n "   "
	./$(exe)

clean:
	@echo -n
	@echo supperession des intermédiaires:
	@echo -n "   "
	rm -f $(objs) main.o

.PHONY: all execution clean 
