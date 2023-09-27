#include <stdlib.h>

#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "Game/Game.h"
#include "GameData/GameData.h"

using namespace std;

const int SIZE = 4;

void readConfigFile(char *inputFile, Game *initConf, Game *goalConf);
void runAlgorithm(string algorithm, Game *initConf, Game *goalConf);

bool checkIfSolvable(Game *initConf, Game *goalConf);
void debugBoard(Game *initConf, Game *goalConf);

void BFS(Game *initConf, Game *goalConf);

bool DFS(Game *initConf, Game *goalConf, int depth, int flag);
void IDFS(Game *initConf, Game *goalConf);

void AStarMisplacedTiles(Game *initConf, Game *goalConf);
void AStarManhattanDistance(Game *initConf, Game *goalConf);

void GreedyManhattan(Game *initConf, Game *goalConf);
void GreedyMisplacedTiles(Game *initConf, Game *goalConf);

int main(int argc, char *argv[])
{
    system("clear");

    if (argc == 1 || argc > 3)
    {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    string algorithm = argv[1];

    Game *initConf = new Game(SIZE);
    Game *goalConf = new Game(SIZE);
    readConfigFile(argv[2], initConf, goalConf);

    if (!checkIfSolvable(initConf, goalConf))
    {
        printf("The initial configuration is not solvable!\n");
        return 1;
    }

    runAlgorithm(algorithm, initConf, goalConf);

    return 0;
}

void runAlgorithm(string algorithm, Game *initConf, Game *goalConf)
{
    if (algorithm == "BFS")
        BFS(initConf, goalConf);
    else if (algorithm == "DFS")
        DFS(initConf, goalConf, 20, 0);
    else if (algorithm == "IDFS")
        IDFS(initConf, goalConf);
    else if (algorithm == "A*-misplaced")
        AStarMisplacedTiles(initConf, goalConf);
    else if (algorithm == "A*-Manhattan")
        AStarManhattanDistance(initConf, goalConf);
    else if (algorithm == "Greedy-misplaced")
        GreedyMisplacedTiles(initConf, goalConf);
    else if (algorithm == "Greedy-Manhattan")
        GreedyManhattan(initConf, goalConf);
    else
    {
        printf("Invalid algorithm!\n");
        exit(1);
    }
}

void readConfigFile(char *confFile, Game *initConf, Game *goalConf)
{
    // Open the file
    ifstream inputFile(confFile);

    // Check if the file was opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error opening file " << confFile << "\n";
        exit(1);
    }

    // Read the file
    string lineINIT, lineGOAL;
    getline(inputFile, lineINIT);
    getline(inputFile, lineGOAL);

    // Parse the file
    stringstream ssINIT(lineINIT);
    stringstream ssGOAL(lineGOAL);

    // Read the initial configuration
    vector<int> init;
    int num = 0;
    while (ssINIT >> num)
    {
        init.push_back(num);
    }

    // Read the goal configuration
    vector<int> goal;
    while (ssGOAL >> num)
    {
        goal.push_back(num);
    }

    // Add the values to the board
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            initConf->setPiece(i, j, init.front());
            goalConf->setPiece(i, j, goal.front());

            init.erase(init.begin());
            goal.erase(goal.begin());
        }
    }
    inputFile.close();
}

bool checkIfSolvable(Game *initConf, Game *goalConf)
{
    return initConf->solvability() == goalConf->solvability();
}

void debugBoard(Game *initConf, Game *goalConf)
{
    printf("Initial configuration:\n");
    // initConf->setVisited();
    initConf->printBoard();
    // printf("%d\n", initConf->isVisited());

    printf("--------------------\n");

    printf("Goal configuration:\n");
    goalConf->printBoard();

    printf("--------------------\n");

    // printf("Possible moves:\n");
    // vector<Game*> possibleMoves = initConf->possibleMoves();

    // for (int i = 0; i < possibleMoves.size(); i++) {
    //     printf("--------------------\n");
    //     possibleMoves[i]->printBoard();
    //     cout << possibleMoves[i]->getPath() << " " << possibleMoves[i]->isVisited() << endl;
    // }
}

void BFS(Game *initConf, Game *goalConf)
{
    queue<Game *> q;
    q.push(initConf);

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!q.empty())
    {
        Game *cur = q.front();
        q.pop();

        data.incrementExpandedNodes();

        if (cur->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur->printBoard();
            data.setPath(cur->getPath());
            data.printData();
            return;
        }

        vector<Game *> possibleMoves = cur->possibleMoves();
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            Game *nextMove = possibleMoves[i];
            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                q.push(nextMove);
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
}

bool DFS(Game *initConf, Game *goalConf, int depth, int flag)
{
    stack<pair<Game *, int>> s; // store current depth of each node
    s.push(make_pair(initConf, 0));

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!s.empty())
    {
        auto cur = s.top();
        s.pop();

        data.incrementExpandedNodes();

        if (cur.first->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur.first->printBoard();
            data.setPath(cur.first->getPath());
            data.printData();
            return true;
        }

        if (cur.second >= depth && flag == 0)
        {
            continue;
        }

        if (cur.second >= depth && flag == 1)
        {
            visited.clear();
            continue;
        }

        vector<Game *> possibleMoves = cur.first->possibleMoves();

        // Add the child nodes to the stack in a reverse order, so that the algorithm explores them in a depth-first order
        for (int i = possibleMoves.size() - 1; i >= 0; i--)
        {
            Game *nextMove = possibleMoves[i];
            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                s.push(make_pair(nextMove, cur.second + 1));
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
    if (flag == 0)
        DFS(initConf, goalConf, depth + 1, 0);
    return false;
}

void IDFS(Game *initConf, Game *goalConf)
{
    int i = 0;
    cout << "Currently exploring depth 0\n";
    while (!DFS(initConf, goalConf, i, 1))
    {
        cout << "Currently exploring depth " << (++i) << '\n';
    }
    return;
}

void AStarMisplacedTiles(Game *initConf, Game *goalConf)
{
    // priority_queue<Game*, vector<Game*>, function<bool(Game*, Game*)>> pq(
    //     [](Game* game1, Game* game2) -> bool {
    //         return game1->getF() > game2->getF();
    //     }
    // );
    struct AStarMisplacedTilesCompare
    {
        bool operator()(Game *game1, Game *game2) const
        {
            return game1->getF() > game2->getF();
        }
    };
    priority_queue<Game *, vector<Game *>, AStarMisplacedTilesCompare> pq;

    initConf->setH(initConf->misplacedTiles(goalConf));
    pq.push(initConf);

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!pq.empty())
    {
        Game *cur = pq.top();
        pq.pop();

        data.incrementExpandedNodes();

        if (cur->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur->printBoard();
            data.setPath(cur->getPath());
            data.printData();
            return;
        }

        vector<Game *> possibleMoves = cur->possibleMoves();
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            Game *nextMove = possibleMoves[i];

            // Calculate the misplaced tiles heuristic for the next move
            int h = nextMove->misplacedTiles(goalConf);
            nextMove->setH(h);

            // Calculate the cost of the next move and set it
            // int g = nextMove->getPath().length();
            int g = cur->getG() + 1;
            nextMove->setG(g);

            // Calculate the f(n) value of the next move and set it
            int f = g + h;
            nextMove->setF(f);

            // Check if the new node has already been visited
            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                pq.push(nextMove);
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
}

void AStarManhattanDistance(Game *initConf, Game *goalConf)
{
    struct AStarManhattanDistanceCompare
    {
        bool operator()(Game *game1, Game *game2) const
        {
            return game1->getF() > game2->getF();
        }
    };
    priority_queue<Game *, vector<Game *>, AStarManhattanDistanceCompare> pq;

    initConf->setH(initConf->manhattanDistance(goalConf));
    pq.push(initConf);

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!pq.empty())
    {
        Game *cur = pq.top();
        pq.pop();

        data.incrementExpandedNodes();

        if (cur->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur->printBoard();
            data.setPath(cur->getPath());
            data.printData();
            return;
        }

        vector<Game *> possibleMoves = cur->possibleMoves();
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            Game *nextMove = possibleMoves[i];

            int g = cur->getG() + 1;
            nextMove->setG(g);

            int h = nextMove->manhattanDistance(goalConf);
            nextMove->setH(h);

            int f = g + h;
            nextMove->setF(f);

            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                pq.push(nextMove);
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
}

void GreedyManhattan(Game *initConf, Game *goalConf)
{
    struct GreedyManhattanCompare
    {
        bool operator()(Game *game1, Game *game2) const
        {
            return game1->getH() > game2->getH();
        }
    };
    priority_queue<Game *, vector<Game *>, GreedyManhattanCompare> pq;

    initConf->setH(initConf->manhattanDistance(goalConf));
    pq.push(initConf);

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!pq.empty())
    {
        Game *cur = pq.top();
        pq.pop();

        data.incrementExpandedNodes();

        if (cur->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur->printBoard();
            data.setPath(cur->getPath());
            data.printData();
            return;
        }

        vector<Game *> possibleMoves = cur->possibleMoves();
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            Game *nextMove = possibleMoves[i];

            int h = nextMove->manhattanDistance(goalConf);
            nextMove->setH(h);

            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                pq.push(nextMove);
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
}

void GreedyMisplacedTiles(Game *initConf, Game *goalConf)
{
    struct GreedyMisplacedTilesCompare
    {
        bool operator()(Game *game1, Game *game2) const
        {
            return game1->getH() > game2->getH();
        }
    };
    priority_queue<Game *, vector<Game *>, GreedyMisplacedTilesCompare> pq;

    initConf->setH(initConf->misplacedTiles(goalConf));
    pq.push(initConf);

    initConf->hashMaker();
    unordered_set<size_t> visited;
    visited.insert(initConf->getId());

    GameData data;

    while (!pq.empty())
    {
        Game *cur = pq.top();
        pq.pop();

        data.incrementExpandedNodes();

        if (cur->compareBoards(goalConf))
        {
            printf("Found the goal configuration!\n");
            cur->printBoard();
            data.setPath(cur->getPath());
            data.printData();
            return;
        }

        vector<Game *> possibleMoves = cur->possibleMoves();
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            Game *nextMove = possibleMoves[i];

            int h = nextMove->misplacedTiles(goalConf);
            nextMove->setH(h);

            if (visited.find(nextMove->getId()) == visited.end())
            {
                visited.insert(nextMove->getId());
                pq.push(nextMove);
                data.incrementMemoryUsed();
            }
            else
            {
                delete nextMove;
            }
        }
    }
}
