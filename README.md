# 2048 AI Solvers
Python to C++ ported code from [codebase](https://github.com/dwu1200/2048) made by 
[Daniel Wu](https://github.com/dwu1200), 
[Nathaniel Wine](https://github.com/NathanWine), and 
[Yang Yang](https://github.com/yang573). 

Port done by 
[Nathaniel Wine](https://github.com/NathanWine).

Some modifications/improvements are present with the goal of further improved performance/quality.

## Setup
First, navigate to the  run 
```
make all
```
and then follow [usage](##Usage). Note that this codebase was written and tested in a Linux environment. 
Other operating systems may need to follow additional steps / procedures in order to run the program.

## Usage 
```
AISolver < flag_1 > < flag_1_val > < flag_2 > < flag_2_val > ...
```

Flag list:
* -a: Integer/String value; Algorithm to run. 0 = MonteCarlo, 1 = MiniMax
* -n: Integer value; # times to run the algorithm. Stats displayed at program completion
* -r: Integer value; # runs MonteCarlo completes for each move. Higher=better but slower. Recommend 10-100
* -d: Integer value; # depth level for MiniMax / Expectimax
* -p: Integer value; Print level. Higher=more display. 0 = minimal, 1 = medium, 2 = high, 3 = full

Ex: 
```
AISolver -a minimax -n 1 -d 1 -p 3
```