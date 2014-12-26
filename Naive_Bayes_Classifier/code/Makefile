EXECUTABLE1 = NaiveBayes
EXECUTABLE2 = NBAdaBoost

COMPILER = g++
COMPILER_OPTS = -c -g

LINKER = g++
LINKER_OPTS_1 = -o $(EXECUTABLE1)
LINKER_OPTS_2 = -o $(EXECUTABLE2)
#CFLAGS = -DDEBUG_LOGS

.PHONY: all naivebayes adaboost

all: naivebayes adaboost

naivebayes: $(EXECUTABLE1)

adaboost: $(EXECUTABLE2)

OBJS_NAIVE_BAYES = naivebayes.o

#  linking the program.
$(EXECUTABLE1) : $(OBJS_NAIVE_BAYES)
	$(LINKER) $(LINKER_OPTS_1) $(CFLAGS) $(OBJS_NAIVE_BAYES)

OBJS_ADABOOST = adaboost.o

#  linking the program.
$(EXECUTABLE2) : $(OBJS_ADABOOST)
	$(LINKER) $(LINKER_OPTS_2) $(CFLAGS) $(OBJS_ADABOOST)

# Rules for performing the compilation.
naivebayes.o : naivebayes.h naivebayes.cpp
	$(COMPILER) $(COMPILER_OPTS) $(CFLAGS) naivebayes.cpp


# Rules for performing the compilation of each individual object file.
adaboost.o : adaboost.h adaboost.cpp
	$(COMPILER) $(COMPILER_OPTS) $(CFLAGS) adaboost.cpp

clean:
	-rm *.o $(EXECUTABLE1) $(EXECUTABLE2)




