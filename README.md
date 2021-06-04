# 2048 AI Solvers
AI solvers for the puzzle game [2048](https://play2048.co/).

Python to C++ ported code from [codebase](https://github.com/dwu1200/2048) made by 
[Daniel Wu](https://github.com/dwu1200), 
[Nathaniel Wine](https://github.com/NathanWine), and 
[Yang Yang](https://github.com/yang573). 

Port done by 
[Nathaniel Wine](https://github.com/NathanWine).

Some modifications/improvements are present with the goal of further improved performance/quality.

## About
The original project (as well as this one) was created with the goal of implementing 
algorithms to achieve as high of a score as possible in the game 2048. Game.cpp 
contains the code for the game logic, Heuristics.cpp contains several heuristic 
metrics/functions to evaluate game state quality, and algorithm implementations are 
split between MonteCarlo.cpp and Minimax.cpp.

If unfamiliar with the game rules, check out [2048](https://play2048.co/).

## Setup
First, navigate to the proper directory and run 
```
make all
```
...and then follow [usage](##Usage). Note that this codebase was written and tested in a _Linux_ environment. 
Other operating systems may need to follow additional steps / procedures in order to run the program.

## Usage 
```
AISolver < flag_1 > < flag_1_val > < flag_2 > < flag_2_val > ...
```

Flag list:
* -a: Integer/String value; Algorithm to run. 0 = MonteCarlo, 1 = Minimax
* -n: Integer value; # times to run the algorithm. Stats displayed at program completion
* -r: Integer value; # runs MonteCarlo completes for each move. Higher=better but slower. Recommend 10-100
* -d: Integer value; # depth level for Minimax / Expectimax
* -p: Integer value; Print level. Higher=more display. 0 = minimal, 1 = medium, 2 = high, 3 = full

Ex: 
```
AISolver -a minimax -n 1 -d 1 -p 3
```