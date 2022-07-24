#include "Chess.h"

constexpr double KingScore{ 1300 };
constexpr double QueenScore{ 900 };
constexpr double RookScore{ 500 };
constexpr double BishopScore{ 330 };
constexpr double KnightScore{ 320 };
constexpr double PawnScore{ 100 };

constexpr int MAX_DEPTH{ 2 };
std::vector<std::string> prevMoves;

constexpr int wKingTable[8][8]
{
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};
constexpr int wQueenTable[8][8]
{
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
constexpr int wRookTable[8][8]
{
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};
constexpr int wBishopTable[8][8]
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};
constexpr int wKnightTable[8][8]
{
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
constexpr int wPawnTable[8][8]
{
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int bKingTable[8][8]
{
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};
constexpr int bQueenTable[8][8]
{
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
constexpr int bRookTable[8][8]
{
      0,  0,  0,  5,  5,  0,  0,  0,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      5, 10, 10, 10, 10, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
};
constexpr int bBishopTable[8][8]
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};
constexpr int bKnightTable[8][8]
{
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
constexpr int bPawnTable[8][8]
{
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5,-10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};

double MaterialEvaluation(Node* node)
{
    auto count
    {
        [node](Type type, Color color) -> int
        {
            return node->materials[std::to_string(type) + std::to_string(color)];
        }
    };
    double white
    {
        count(Pawn, White) * PawnScore + count(Knight, White) * KnightScore + count(Bishop, White) * BishopScore +
        count(Rook, White) * RookScore + count(Queen, White) * QueenScore + count(King, White) * KingScore
    };
    double black
    {
        count(Pawn, Black) * PawnScore + count(Knight, Black) * KnightScore + count(Bishop, Black) * BishopScore +
        count(Rook, Black) * RookScore + count(Queen, Black) * QueenScore + count(King, Black) * KingScore
    };

    return node->orientation == White ? white - black : black - white;
}

double BoardEvaluation(Node* node)
{
    double score{ MaterialEvaluation(node) };
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (node->chessboard[i][j])
            {
                switch (node->chessboard[i][j]->type)
                {
                    case Pawn:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wPawnTable[j][i];
                        else score -= bPawnTable[j][i];
                        break;
                    }
                    case Knight:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wKnightTable[j][i];
                        else score -= bKnightTable[j][i];
                        break;
                    }
                    case Bishop:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wBishopTable[j][i];
                        else score -= bBishopTable[j][i];
                        break;
                    }
                    case Rook:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wRookTable[j][i];
                        else score -= bRookTable[j][i];
                        break;
                    }
                    case Queen:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wQueenTable[j][i];
                        else score -= bQueenTable[j][i];
                        break;
                    }
                    case King:
                    {
                        if (node->chessboard[i][j]->color == node->orientation) score += wKingTable[j][i];
                        else score -= bKingTable[j][i];
                        break;
                    }
                }
            }
        }
    }
    return score / 100;
}

double MinMaxAlphaBeta(Node*& root, Node*& best, int depth, bool state, double alpha, double beta)
{
    if (depth >= MAX_DEPTH || root->next.size() == 0) return BoardEvaluation(root);
    else if (state)
    {
        for (std::vector<Node*>::iterator it{ root->next.begin() }; it != root->next.end(); it++)
        {
            double val{ MinMaxAlphaBeta(*it, best, depth + 1, 0, alpha, beta) };
            if (val > alpha)
            {
                if (depth == 0) best = *it;
                alpha = val;
            }
            if (alpha >= beta) break;
        }
        return alpha;
    }
    else
    {
        for (std::vector<Node*>::iterator it{ root->next.begin() }; it != root->next.end(); it++)
        {
            double val{ MinMaxAlphaBeta(*it, best, depth + 1, 1, alpha, beta) };
            if (val < beta)
            {
                if (depth == 0) best = *it;
                beta = val;
            }
            if (alpha >= beta) break;
        }
        return beta;
    }
}

void ConstructTree(Node* root, int depth)
{
    if (depth >= MAX_DEPTH) return;

    for (int x{ 0 }; x < 8; x++)
    {
        for (int y{ 0 }; y < 8; y++)
        {
            if (root->chessboard[x][y])
            {
                if (root->chessboard[x][y]->color == root->turn)
                {
                    FindPossibleMoves(root, true, x, y);
                }
            }
        }
    }

    for (std::vector<Node*>::iterator it{ root->next.begin() }; it != root->next.end(); it++)
    {
        ConstructTree(*it, depth + 1);
    }
}

bool IsNew(Node* test)
{
    std::string boardHash{ "" };
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (test->chessboard[i][j])
            {
                boardHash += std::to_string(test->chessboard[i][j]->type) + std::to_string(test->chessboard[i][j]->color);
            }
            else boardHash += "-1";
        }
    }

    if (std::find(prevMoves.begin(), prevMoves.end(), boardHash) == prevMoves.end())
    {
        prevMoves.push_back(boardHash);
        return true;
    }
    return false;
}

void AI(Node* root)
{
    Node* best{ new Node };
    ConstructTree(root, 0);
    MinMaxAlphaBeta(root, best, 0, 0, -1000, 1000);

    if (!root->next.empty())
    {
        MakeMove(root, best->lastMove, true);
        root->turn = best->turn;
    }

    for (std::vector<Node*>::reverse_iterator rIt{ root->next.rbegin() }; rIt != root->next.rend(); rIt++) delete *rIt;
    while (!root->next.empty()) root->next.pop_back();
    prevMoves.clear();
}