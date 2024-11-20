#include <QDebug>

#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

#include "graph.h" //includes vector

using namespace std;

// Board (object should never be created without a vector)
Board::Board(shared_ptr<Board> p, const vector<vector<int>> &v, const vector<vector<int>> &goal, int calc, int prevM) {
    board = v;
    previousMove = prevM;

    if (p == nullptr) {
        blanknum = size*size;
        g = 0;
    }
    else p->getConstants(size, blanknum, g);

    parent = p;
    h = calculateH(board, goal, calc);
    f = h + g;
}

void Board::getConstants(int &s, int &bN, int &d) {
    // this will be called by the constructor
    s = size;
    bN = blanknum;
    d = g+1;
}

void Board::findPos(const vector<vector<int>> &v, int &pos1, int &pos2, int num) {
    //Find position of number
    for (unsigned int i = 0; i < v.size(); i++) {
        for (unsigned int j = 0; j < v.at(i).size(); j++) {
            if (v.at(i).at(j) == num) {
                pos1 = i;
                pos2 = j;
                return;
            }
        }
    }
    qDebug() << "Could not find blank space. Returning (0,0)";
    pos1 = 0;   pos2 = 0;
}

bool Board::isMoveValid(int pos1, int pos2, int move) {
    switch(move) {
        case (0) : { //up
            // board.at(pos1-1).at(pos2);
            if (pos1-1 < 0) return false;
            else return true;
            break;
        }
        case (1) : { //down
            // board.at(pos1+1).at(pos2);
            if (pos1+1 > (int)board.size()-1) return false;
            else return true;
            break;
        }
        case (2) : { //left
            // board.at(pos1).at(pos2-1);
            if (pos2-1 < 0) return false;
            else return true;
            break;
        }
        case (3) : { //right
            // board.at(pos1).at(pos2+1);
            if (pos2+1 > (int)board.size()-1) return false;
            else return true;
            break;
        }
        default: {
            qDebug() << "SWITCH-CASE ERROR WHEN VALIDATING MOVE";
            return false;
            break;
        }
    }
}

vector<vector<int>> Board::move(int move) {
    //This will be what we return
    vector<vector<int>> tempBoard = board;

    // get blank's current position
    int pos1, pos2;
    findPos(tempBoard, pos1, pos2, board.size() * tempBoard.size());

    // Check for valitiy before moving
    if (!isMoveValid(pos1, pos2, move)) {
        qDebug() << "Move is not valid, returning";
        return tempBoard;
    }

    // position of blank
    int blank = tempBoard.at(pos1).at(pos2);

    // make a move depending on where the blank is
    switch(move) {
        case (0) : { //up
            int temp = tempBoard.at(pos1-1).at(pos2);
            tempBoard.at(pos1-1).at(pos2) = blank;
            tempBoard.at(pos1).at(pos2) = temp;
            break;
        }
        case (1) : { //down
            int temp = tempBoard.at(pos1+1).at(pos2);
            tempBoard.at(pos1+1).at(pos2) = blank;
            tempBoard.at(pos1).at(pos2) = temp;
            break;
        }
        case (2) : { //left
            int temp = tempBoard.at(pos1).at(pos2-1);
            tempBoard.at(pos1).at(pos2-1) = blank;
            tempBoard.at(pos1).at(pos2) = temp;
            break;
        }
        case (3) : { //right
            int temp = tempBoard.at(pos1).at(pos2+1);
            tempBoard.at(pos1).at(pos2+1) = blank;
            tempBoard.at(pos1).at(pos2) = temp;
            break;
        }
        default: {
            qDebug() << "SWITCH-CASE ERROR WHEN MOVING";
            break;
        }
    }

    return tempBoard;
};

double Board::distance(const int x1, const int y1, const int x2, const int y2) {
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

double Board::calculateH(const vector<vector<int>> &v,
                         const std::vector<std::vector<int>> &g, int calc) {
    double total = 0;

    // Only need to calculate the H for Euclidean and Misplaced
    switch(calc) {
        case 0: {
            total = 0;
            break;
        }
        case 2: { //Mismatched Tile
            for (unsigned int i = 0; i < v.size(); i++) {
                for (unsigned int j = 0; j < v.at(i).size(); j++) {
                    if (v.at(i).at(j) != g.at(i).at(j)) total++;
                }
            }
            break;
        }
        case 1: { //Euclidean
            for (int i = 0; i < blanknum-1; i++) {
                int x1, x2, y1, y2;
                findPos(v, x1, y1, i+1);
                findPos(g, x2, y2, i+1);
                total += distance(x1, y1, x2, y2);
            }
            break;
        }
        default: {
            qDebug() << "ERROR WITH H CALCULATION, INVALID CALC TYPE";
            break;
        }
    }

    return total;
}

vector<shared_ptr<Board>> Board::ASearch(vector<shared_ptr<Board>> &knowns, const std::vector<std::vector<int>> &goal, int calc) {
    vector<vector<int>> tempVector = board;
    vector<shared_ptr<Board>> returnBoards;

    int pos1, pos2;
    findPos(board, pos1, pos2, blanknum);

    double hU = blanknum*blanknum,
        hD = blanknum*blanknum,
        hL = blanknum*blanknum,
        hR = blanknum*blanknum;

    // Check possible 4 next states
    if (isMoveValid(pos1, pos2, 0)) hU = calculateH(move(0), goal, calc); //Up
    if (isMoveValid(pos1, pos2, 1)) hD = calculateH(move(1), goal, calc); //Down
    if (isMoveValid(pos1, pos2, 2)) hL = calculateH(move(2), goal, calc); //Left
    if (isMoveValid(pos1, pos2, 3)) hR = calculateH(move(3), goal, calc); //Right

    // Sort and Find Possible Moves
    vector<double> possibleMoves;
    vector<moves> m;

    // fill moves vector
    if (hU != blanknum*blanknum) m.push_back(moves(hU, 0));
    if (hD != blanknum*blanknum) m.push_back(moves(hD, 1));
    if (hL != blanknum*blanknum) m.push_back(moves(hL, 2));
    if (hR != blanknum*blanknum) m.push_back(moves(hR, 3));

    // sort moves vector
    sort(m.begin(), m.end(), compare());

    // fill possibleMoves with moves in correct order
    for (unsigned int i = 0; i < m.size(); i++) {
        possibleMoves.push_back(m.at(i).move);
    }

    //iterate over all moves and return all new states
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        vector<vector<int>> temp = move(possibleMoves.at(i));

        // check knowns
        bool isKnown = false;
        for (unsigned int i = 0; i < knowns.size(); i++) {
            if (temp == knowns.at(i)->getVector()) {
                isKnown = true;
                break;
            }
        }

        // if its unique, push back a new board
        if (!isKnown) returnBoards.push_back(shared_ptr<Board>(new Board(getBoardThis(), temp, goal, calc, possibleMoves.at(i))));
    }

    explored = true;
    return returnBoards;
}

vector<shared_ptr<Board>> Board::ASearchUniform(const std::vector<std::vector<int>> &goal) {
    vector<shared_ptr<Board>> returnBoards;

    int pos1, pos2;
    findPos(board, pos1, pos2, blanknum);

    // check next 4 possible moves
    vector<double> possibleMoves;
    if (isMoveValid(pos1, pos2, 0)) possibleMoves.push_back(0); //up
    if (isMoveValid(pos1, pos2, 1)) possibleMoves.push_back(1); //down
    if (isMoveValid(pos1, pos2, 2)) possibleMoves.push_back(2); //left
    if (isMoveValid(pos1, pos2, 3)) possibleMoves.push_back(3); //right

    // check for parent
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        vector<vector<int>> t = move(possibleMoves.at(i));

        // if parent is null
        if (parent == nullptr) {
            returnBoards.push_back(shared_ptr<Board>(new Board(getBoardThis(), move(possibleMoves.at(i)), goal, 0, possibleMoves.at(i))));
            continue;
        }

        //check if our move is the same as the parent
        if (t != parent->getVector()) {
            returnBoards.push_back(shared_ptr<Board>(new Board(getBoardThis(), move(possibleMoves.at(i)), goal, 0, possibleMoves.at(i))));
        }
    }

    explored = true;

    return returnBoards;
}

void Board::addChildren(vector<shared_ptr<Board>> t) {
    for (unsigned int i = 0; i < t.size(); i++) {
        children.push_back(t.at(i));
    }
}

void Board::setParent(shared_ptr<Board> p) {
    parent = p;
}
