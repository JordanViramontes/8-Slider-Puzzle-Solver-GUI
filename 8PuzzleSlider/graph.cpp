#include <QDebug>

#include <vector>
#include <cmath>
#include <memory>

#include "graph.h"

using namespace std;

Graph::Graph(vector<vector<int>> v, int c) {
    // set goal
    int cnt = 1;
    for (int i = 0; i < size; i++) {
        vector<int> tmp;
        for (int j = 0; j < size; j++) {
            tmp.push_back(cnt);
            cnt++;
        }
        goal.push_back(tmp);
    }

    // variables
    initBoard = shared_ptr<Board>(new Board(nullptr, v, goal, c));
    finalBoard = nullptr;
    allBoards.push_back(initBoard);
    calc = c;
}

Graph::~Graph() {
    qDebug() << "in Deconstructor";
}

void Graph::ASearch(std::shared_ptr<Board> b) {
    // check if we've hit our goal
    if (b->getVector() == goal) {
        finalBoard = b;
        return;
    }

    // get all valid children and add them to graph
    vector<std::shared_ptr<Board>> temp = b->ASearch(allBoards, goal, calc);
    b->addChildren(temp);
    for (unsigned int i = 0; i < temp.size(); i++) {
        allBoards.push_back(temp.at(i));
    }

    //check all boards for lower f value
    double minf = -1;
    std::shared_ptr<Board> minBoard;;

    for (unsigned int i = 1; i < allBoards.size(); i++) {
        if (allBoards.at(i)->getExplored()) continue;
        if (minf < 0) {
            minf = allBoards.at(i)->getF();
            minBoard = allBoards.at(i);
        }
        if (allBoards.at(i)->getF() == minf) {
            if (allBoards.at(i)->getDepth() > minBoard->getDepth()) {
                minBoard = allBoards.at(i);
            }
        }
        if (allBoards.at(i)->getF() < minf) {
            minf = allBoards.at(i)->getF();
            minBoard = allBoards.at(i);
        }
    }

    ASearch(minBoard);
}

void Graph::ASearchUniform() {
    // iterate over allboards queue (should be infinite until reached goal)
    for (unsigned int i = 0; i < allBoards.size(); i++) {
        // check if we've hit our goal
        if (allBoards.at(i)->getVector() == goal) {
            finalBoard = allBoards.at(i);
            return;
        }

        // get all valid children and add them to graph and allBoards
        vector<std::shared_ptr<Board>> temp = allBoards.at(i)->ASearchUniform(goal);
        allBoards.at(i)->addChildren(temp);
        for (unsigned int i = 0; i < temp.size(); i++) { allBoards.push_back(temp.at(i)); }

        // check children for goal
        for (unsigned int j = 0; j < temp.size(); j++) {
            if (temp.at(j)->getVector() == goal) {
                finalBoard = temp.at(j);
                return;
            }
        }
    }
}

void Graph::ASearch() {
    if (calc == 0) ASearchUniform();
    else ASearch(initBoard);

    int expanded = 0;
    for (unsigned int i = 0; i < allBoards.size(); i++) {
        if (allBoards.at(i)->getExplored()) expanded++;
    }

    qDebug() << "The total amount of nodes expanded were: " << expanded;
    qDebug() << "The total amount of nodes in the queue were: " << allBoards.size();
    qDebug() << "The depth of the goal was: " << finalBoard->getDepth();
}
