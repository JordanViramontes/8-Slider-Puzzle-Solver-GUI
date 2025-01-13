
# 8 Slider Puzzle Search GUI

A simple Windows application to find an optimal list of moves to solve an 8 Slider Puzzle. 

![alt text](https://github.com/JordanViramontes/8-Slider-Puzzle-Solver-GUI/blob/main/8SliderPuzzleDemo.png?raw=true)

## Algorithms

In order to find the optimal path, you can choose to use one of the 3 following search algorithms:
* Uniform Cost Search
* A* Search using Euclidean Distance Heuristic
* A* Search using Misplaced Tiles Heuristic

All 3 of these algorithms will find the optimal path, however some work better than others. For example, Uniform Cost Search usually always takes longer than A\* since the algorithm is simpler compared to A\*.

NOTE: Although all these algorithms are functional, A* is unoptimized which may lead to long loading times or crashes. This is because I got busy but one day I'll come back and fix it :)
## Instructions

Download 8 Slider Puzzle Search GUI from the Releases Tab. The exe file is in the "releases" folder.

Once you start running the program, you have the option to input the puzzle as either:
* A single text line, or
* 3 text lines lining up to the 3 rows of the puzzle.

Once you type in the puzzle you want to solve, press the *"Update Puzzle"* button to apply it. The puzzle visual should update as well.

You can choose between 3 Algorithms as described earlier using the radio buttons on the bottom left. It is set to *"Uniform Cost Search"* by default.
