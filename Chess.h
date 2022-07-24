#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>

extern sf::Texture* pieceSet1Texture;
extern sf::Texture* pieceSet2Texture;
extern sf::Texture* pieceSet3Texture;
extern sf::Texture* board1Texture;
extern sf::Texture* board2Texture;
extern sf::Texture* board3Texture;
extern sf::Texture* arrowTexture;
extern sf::Texture* sliderArrowTexture;
extern sf::Texture* languageTexture;
extern sf::Texture* wPawnTexture;
extern sf::Texture* wKnightTexture;
extern sf::Texture* wBishopTexture;
extern sf::Texture* wRookTexture;
extern sf::Texture* wQueenTexture;
extern sf::Texture* wKingTexture;
extern sf::Texture* bPawnTexture;
extern sf::Texture* bKnightTexture;
extern sf::Texture* bBishopTexture;
extern sf::Texture* bRookTexture;
extern sf::Texture* bQueenTexture;
extern sf::Texture* bKingTexture;

extern sf::Sound* pieceMovedSound;
extern sf::Sound* pieceTakenSound;
extern sf::Sound* sliderSound;
extern sf::Sound* mouseOverSound;

enum Color
{
    White,
    Black
};

enum Type
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Piece
{
    sf::Sprite sprite;
    Color color;
    Type type;
};

struct Move
{
    sf::Vector2i before;
    sf::Vector2i after;
};

struct MovingPiece
{
    Piece* piece;
    Move move;
};

struct Node
{
    Piece* chessboard[8][8];
    Color orientation;
    Color turn;
    std::map<std::string, int> materials;
    sf::Vector2i wKingPos;
    sf::Vector2i bKingPos;
    Move lastMove;
    bool wKingMoved;
    bool bKingMoved;
    bool wKSRookMoved;
    bool bKSRookMoved;
    bool wQSRookMoved;
    bool bQSRookMoved;
    std::vector<Node*> next;
    ~Node()
    {
        if (next.empty()) return;
        for (std::vector<Node*>::reverse_iterator rIt{ next.rbegin() }; rIt != next.rend(); rIt++)
        {
            delete *rIt;
        }
    }
};

extern std::vector<MovingPiece> movingPieces;
extern std::vector<sf::Vector2i> possibleMoves;
extern int endGame;
extern bool audioOn;

// Finds the best move that the opponent can react with the lowest score
double MinMaxAlphaBeta(Node*& root, Node*& best, int depth, bool state, double alpha, double beta);

// Constructs the tree of board variations
void ConstructTree(Node* root, int depth);

// Checks if (test) has been evaluated yet
bool IsNew(Node* test);

// Executes the AI moves
void AI(Node* root);

// Initializes the board and the chess pieces according to the (color) being played
void InitializeChessboard(Node*& root, Color color);

// Creates and initializes a piece based on color, type and position
void InitializePiece(Node* root, Color color, Type type, int x, int y);

// Finds the possible moves for the (x,y) positioned piece and copies them into a vector (moves) if given one
// Can insert the nodes into (root) according to the (insertTree) flag
void FindPossibleMoves(Node* root, bool insertTree, int x, int y, std::vector<sf::Vector2i>* moves = nullptr);

// Validates the move
bool ValidateMove(Node* node, const Move& move, bool insertTree);

// Applies the (move) to the (root) node
// Displays the move according to the (display) flag
void MakeMove(Node* root, const Move& move, bool display);

// Returns true if the king is in check, false otherwise
bool IsKingThreatened(Node* node);

// Checks the state of the board and returns
// -1 if the game hasn't ended yet
// 0 if white won
// 1 if black won
// 2 if draw by stalemate
// 3 if draw by insufficient material
int CheckEndgame(Node* node);