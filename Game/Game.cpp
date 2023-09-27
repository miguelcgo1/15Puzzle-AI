/*
    Métodos da classe Game:
        - Game(int size): construtor da classe Game
        - Game(const Game& other): construtor de cópia da classe Game
        - solvability(): verifica se o tabuleiro é resolvível
        - setPiece(int i, int j, int value): atribui o valor da peça na posição (i, j)
        - setParent(Game* parent): atribui o nó pai
        - getParent(): retorna o nó pai
        - getPath(): retorna o caminho percorrido até o nó
        - setPath(string p): atribui o caminho percorrido até o nó
        - printBoard(): imprime o tabuleiro
        - misplacedTiles(Game* goal): retorna o número de peças fora do lugar
        - manhattanDistance(Game* goal): retorna a distância de Manhattan
        - setH(int h): atribui o valor da heurística
        - getH(): retorna o valor da heurística
        - setG(int g): atribui o valor do custo
        - getG(): retorna o valor do custo
        - setF(int f): atribui o valor da função f(n)
        - getF(): retorna o valor da função f(n)
*/

#include "Game.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Constructors
Game::Game(int size)
{
    this->rows = size;
    this->cols = size;
    this->blankX = 0;
    this->blankY = 0;

    int k = 1;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            board[i][j] = k++;
        }
    }
    board[this->rows - 1][this->cols - 1] = 0;
}

Game::Game(const Game &other)
{
    this->rows = other.rows;
    this->cols = other.cols;
    this->blankX = other.blankX;
    this->blankY = other.blankY;

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            this->board[i][j] = other.board[i][j];
        }
    }
    this->path = other.path;
    // this->parent = other.parent;
}

// Test the solvability of the board
bool Game::solvability()
{
    int inversions = 0;

    // for(int i: this->pieces)
    //     printf("%d ", i);
    // printf("\n");

    for (int i = 0; i < this->pieces.size(); i++)
    {
        if (this->pieces[i] == 0)
            continue;
        int count = 0;
        for (int j = i + 1; j < this->pieces.size(); j++)
        {
            if (this->pieces[j] == 0)
                continue;
            if (this->pieces[i] > this->pieces[j])
                count++;
        }
        inversions += count;
    }
    // printf("inversions: %d\n", inversions);
    // printf("blankX: %d\n", this->rows - this->blankX);
    return (inversions % 2 == 0) == ((this->rows - this->blankX) % 2 == 1);
}

// Print the board
void Game::printBoard()
{
    int maxDigits = 1;
    int maxValue = this->rows * this->cols;

    while (maxValue >= 10)
    {
        maxDigits++;
        maxValue /= 10;
    }

    for (int i = 0; i < this->rows; i++)
    {
        cout << "| ";
        for (int j = 0; j < this->cols; j++)
        {
            int num = this->board[i][j];
            int numDigits = 1;
            while (num >= 10)
            {
                numDigits++;
                num /= 10;
            }
            int numSpaces = maxDigits - numDigits;
            for (int k = 0; k < numSpaces; k++)
            {
                cout << " ";
            }
            cout << this->board[i][j] << " ";
        }
        cout << "|\n";
    }
}

// Compare two boards to find the solution
bool Game::compareBoards(Game *other)
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if (this->board[i][j] != other->board[i][j])
                return false;
        }
    }
    return true;
}

// Update positions
void Game::setPiece(int i, int j, int value)
{
    this->board[i][j] = value;
    this->pieces.push_back(value);
    if (value == 0)
    {
        this->blankX = i;
        this->blankY = j;
    }
}

void Game::swap(int x1, int y1, int x2, int y2)
{
    int temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}

void Game::updateBlankPosition(int x, int y)
{
    this->blankX = x;
    this->blankY = y;
}

// Generate possible moves from the current board
vector<Game *> Game::possibleMoves()
{
    vector<Game *> moves;

    int x = this->blankX;
    int y = this->blankY;

    // Move blank piece up
    if (x > 0)
    {
        Game *up = new Game(*this);
        up->swap(x, y, x - 1, y);
        up->updateBlankPosition(x - 1, y);
        up->path += 'U';
        up->hashMaker();
        moves.push_back(up);
    }

    // Move blank piece down
    if (x < rows - 1)
    {
        Game *down = new Game(*this);
        down->swap(x, y, x + 1, y);
        down->updateBlankPosition(x + 1, y);
        down->path += 'D';
        down->hashMaker();
        moves.push_back(down);
    }

    // Move blank piece left
    if (y > 0)
    {
        Game *left = new Game(*this);
        left->swap(x, y, x, y - 1);
        left->updateBlankPosition(x, y - 1);
        left->path += 'L';
        left->hashMaker();
        moves.push_back(left);
    }

    // Move blank piece right
    if (y < cols - 1)
    {
        Game *right = new Game(*this);
        right->swap(x, y, x, y + 1);
        right->updateBlankPosition(x, y + 1);
        right->path += 'R';
        right->hashMaker();
        moves.push_back(right);
    }
    return moves;
}

// Hash function to create unique id for each board
void Game::hashMaker()
{
    int hash = 0;
    std::ignore = hash;
    string str = "";
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            str += to_string(this->board[i][j]);
        }
    }
    std::hash<std::string> hasher;
    this->id = hasher(str);
    // printf("%d\n",id);
}

// Heuristic functions
int Game::misplacedTiles(Game *goal)
{
    int count = 0;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if (this->board[i][j] != goal->board[i][j])
                count++;
        }
    }
    return count;
}

int Game::manhattanDistance(Game *goal)
{
    int distance = 0;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            int value = this->board[i][j];
            if (value == 0)
                continue;
            int x = (value - 1) / this->cols;
            int y = (value - 1) % this->cols;
            distance += abs(x - i) + abs(y - j);
        }
    }
    return distance;
}

// Setters and getters for the private variables
size_t Game::getId()
{
    return this->id;
}

string Game::getPath()
{
    return this->path;
}

void Game::setPath(string p)
{
    this->path = p;
}

void Game::setH(int h)
{
    this->h = h;
}

int Game::getH()
{
    return this->h;
}

void Game::setG(int g)
{
    this->g = g;
}

int Game::getG()
{
    return this->g;
}

void Game::setF(int f)
{
    this->f = f;
}

int Game::getF()
{
    return this->f;
}

// void Game::setParent(Game* parent) {
//     this->parent = parent;
// }

// Game* Game::getParent() {
//     return this->parent;
// }