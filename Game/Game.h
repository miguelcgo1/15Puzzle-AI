/*
    Classe que representa o jogo 15-puzzle:
        - board: matriz que representa o tabuleiro
        - pieces: vetor que armazena os valores do tabuleiro
        - id: identificador do nó (hash do tabuleiro)
        - rows/cols: número de linhas/colunas do tabuleiro
        - blankX/blankY: posição x/y do espaço em branco
        - path: caminho percorrido até o nó
        - parent: pointer para o nó pai
        - h: valor da heurística
        - g: custo para chegar ao nó (número de movimentos feitos até o nó)
        - f: f(n) = g(n) + h(n)
*/

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Game {
   public:
    Game(int size);
    Game(const Game& other);

    bool compareBoards(Game* other);
    vector<Game*> possibleMoves();
    void swap(int x1, int y1, int x2, int y2);
    void updateBlankPosition(int x, int y);
    void hashMaker();
    size_t getId();

    bool solvability();
    void setPiece(int i, int j, int value);

    // void setParent(Game* parent);
    // Game* getParent();

    string getPath();
    void setPath(string p);
    void printBoard();

    int misplacedTiles(Game* goal);
    int manhattanDistance(Game* goal);
    void setH(int h);
    int getH();

    void setG(int g);
    int getG();

    void setF(int f);
    int getF();

   private:
    int board[4][4];
    vector<int> pieces;
    int id;
    int rows, cols;
    int blankX, blankY;
    string path;
    int h, g, f;
    // Game* parent;
};
#endif