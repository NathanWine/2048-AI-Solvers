CC := g++

CFLAGS := -Wall -Werror -Wno-unused-variable -Wno-unused-function -MMD
COLORF := -DCOLOR
DFLAGS := -g -DDEBUG -DCOLOR
PRINT_STAMENTS := -DERROR -DSUCCESS -DWARN -DINFO

MONTE := MonteCarloSolver

all: $(MONTE)
$(MONTE): Main.o Game.o $(MONTE).o
	$(CC) $(CFLAGS) -o $(MONTE) Main.o Game.o $(MONTE).o

Main.o: Main.cpp Game.hpp MonteCarloSolver.hpp
	$(CC) $(CFLAGS) -c Main.cpp

Game.o: Game.hpp

MonteCarloSolver.o: MonteCarloSolver.hpp

clean:
	$(RM) $(MONTE)