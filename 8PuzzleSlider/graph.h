#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <vector>
#include <string>
#include <memory>

// mini for help when sorting moves (greedy)
// had issue where H = 2 different moves
struct moves {
public:
    double value;
    int move;

    moves(double v, int m) {
        value = v;
        move = m;
    }
};

struct compare {
    inline bool operator() (const moves& struct1, const moves& struct2)
    {
        return (struct1.value < struct2.value);
    }
};

class Board : public std::enable_shared_from_this<Board>{
    private:
        std::vector<std::vector<int>> board; //current board
        std::vector<std::shared_ptr<Board>> children;
        std::shared_ptr<Board> parent;
        bool explored = false;
        double h; int g; double f;
        int size = 3; // 3x3 board
        int blanknum; // should be size*size=9 by default
        int previousMove;

        // helpers
        void findPos(const std::vector<std::vector<int>> &, int &, int &, int); //return 2 ints that show location of blank, use as a helped
        std::vector<std::vector<int>> move(int); //make and return a move and return vector
        bool isMoveValid(int, int, int); //check if move is valid
        double calculateH(const std::vector<std::vector<int>> &,
                          const std::vector<std::vector<int>> &, int); //calculate h, int = type of calculations
        double distance(const int x1, const int y1, const int x2, const int y2);
    public:
        std::shared_ptr<Board> getBoardThis()
        {
            return shared_from_this();
        }
        // constructors
        Board(std::shared_ptr<Board>, const std::vector<std::vector<int>> &, const std::vector<std::vector<int>> &, int, int prevM);
        void  getConstants(int &s, int &bN, int &d);

        // Algorithm
        std::vector<std::shared_ptr<Board>> ASearch(std::vector<std::shared_ptr<Board>> &, const std::vector<std::vector<int>> &, int); //Search algorithm, 2nd argument is which type of A search
        std::vector<std::shared_ptr<Board>> ASearchUniform(const std::vector<std::vector<int>> &);
        void addChildren(std::vector<std::shared_ptr<Board>> t);

        // Get
        std::vector<std::vector<int>> const getVector() { return board; }
        std::shared_ptr<Board> const getParent() { return parent; }
        double getH() { return h; }
        double  getF() { return f; }
        int  getDepth() { return g; }
        bool  getExplored() { return explored; }
        void  setParent(std::shared_ptr<Board>);
        int getPreviousMode() { return previousMove; }
};

class Graph {
    private:
        std::vector<std::shared_ptr<Board>> allBoards; // vector of all created boards in order of creation
        std::shared_ptr<Board> initBoard; // first board
        std::shared_ptr<Board> finalBoard; // final board
        std::vector<std::vector<int>> goal; // goal state
        int calc; // determines the heuristic
        int size = 3;
        int expanded;

        void printRoute(std::shared_ptr<Board>);
        void ASearch(std::shared_ptr<Board>);
        void ASearchUniform();

    public:
        Graph(std::vector<std::vector<int>>, int);
        void ASearch();

        // get
        int getDepth() { return finalBoard->getDepth() + 1; };
        unsigned int  getSize() { return allBoards.size(); }
        std::shared_ptr<Board> getFinalBoard() { return finalBoard; }
        QString getStringOutput();
};

#endif // GRAPH_H
