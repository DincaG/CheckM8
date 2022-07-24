#include "Chess.h"

sf::Texture* pieceSet1Texture;
sf::Texture* pieceSet2Texture;
sf::Texture* pieceSet3Texture;
sf::Texture* board1Texture;
sf::Texture* board2Texture;
sf::Texture* board3Texture;
sf::Texture* arrowTexture;
sf::Texture* sliderArrowTexture;
sf::Texture* languageTexture;
sf::Texture* wPawnTexture;
sf::Texture* wKnightTexture;
sf::Texture* wBishopTexture;
sf::Texture* wRookTexture;
sf::Texture* wQueenTexture;
sf::Texture* wKingTexture;
sf::Texture* bPawnTexture;
sf::Texture* bKnightTexture;
sf::Texture* bBishopTexture;
sf::Texture* bRookTexture;
sf::Texture* bQueenTexture;
sf::Texture* bKingTexture;

sf::Sound* pieceMovedSound;
sf::Sound* pieceTakenSound;
sf::Sound* sliderSound;
sf::Sound* mouseOverSound;

std::vector<MovingPiece> movingPieces;
std::vector<sf::Vector2i> possibleMoves;
int endGame;
bool audioOn;


void InitializeChessboard(Node*& root, Color color)
{
    if (root)
    {
        for (int i{ 0 }; i < 8; i++)
        {
            for (int j{ 0 }; j < 8; j++)
            {
                delete root->chessboard[i][j];
                root->chessboard[i][j] = nullptr;
            }
        }
        delete root;
    }

    root = new Node;
    root->lastMove = { sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };
    root->wKingMoved = false;
    root->bKingMoved = false;
    root->wKSRookMoved = false;
    root->bKSRookMoved = false;
    root->wQSRookMoved = false;
    root->bQSRookMoved = false;
    root->turn = White;
    root->orientation = color;

    root->materials[std::to_string(Pawn) + std::to_string(White)] = 8;
    root->materials[std::to_string(Knight) + std::to_string(White)] = 2;
    root->materials[std::to_string(Bishop) + std::to_string(White)] = 2;
    root->materials[std::to_string(Rook) + std::to_string(White)] = 2;
    root->materials[std::to_string(Queen) + std::to_string(White)] = 1;
    root->materials[std::to_string(King) + std::to_string(White)] = 1;
    root->materials[std::to_string(Pawn) + std::to_string(Black)] = 8;
    root->materials[std::to_string(Knight) + std::to_string(Black)] = 2;
    root->materials[std::to_string(Bishop) + std::to_string(Black)] = 2;
    root->materials[std::to_string(Rook) + std::to_string(Black)] = 2;
    root->materials[std::to_string(Queen) + std::to_string(Black)] = 1;
    root->materials[std::to_string(King) + std::to_string(Black)] = 1;

    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            root->chessboard[i][j] = nullptr;
        }
    }

    Color enemyColor{ (Color)!color };
    InitializePiece(root, color, Pawn, 0, 6);
    InitializePiece(root, color, Pawn, 1, 6);
    InitializePiece(root, color, Pawn, 2, 6);
    InitializePiece(root, color, Pawn, 3, 6);
    InitializePiece(root, color, Pawn, 4, 6);
    InitializePiece(root, color, Pawn, 5, 6);
    InitializePiece(root, color, Pawn, 6, 6);
    InitializePiece(root, color, Pawn, 7, 6);
    InitializePiece(root, color, Knight, 1, 7);
    InitializePiece(root, color, Knight, 6, 7);
    InitializePiece(root, color, Bishop, 2, 7);
    InitializePiece(root, color, Bishop, 5, 7);
    InitializePiece(root, color, Rook, 0, 7);
    InitializePiece(root, color, Rook, 7, 7);
    InitializePiece(root, enemyColor, Pawn, 0, 1);
    InitializePiece(root, enemyColor, Pawn, 1, 1);
    InitializePiece(root, enemyColor, Pawn, 2, 1);
    InitializePiece(root, enemyColor, Pawn, 3, 1);
    InitializePiece(root, enemyColor, Pawn, 4, 1);
    InitializePiece(root, enemyColor, Pawn, 5, 1);
    InitializePiece(root, enemyColor, Pawn, 6, 1);
    InitializePiece(root, enemyColor, Pawn, 7, 1);
    InitializePiece(root, enemyColor, Knight, 1, 0);
    InitializePiece(root, enemyColor, Knight, 6, 0);
    InitializePiece(root, enemyColor, Bishop, 2, 0);
    InitializePiece(root, enemyColor, Bishop, 5, 0);
    InitializePiece(root, enemyColor, Rook, 0, 0);
    InitializePiece(root, enemyColor, Rook, 7, 0);
    
    if (color == White)
    {
        InitializePiece(root, White, Queen, 3, 7);
        InitializePiece(root, White, King, 4, 7);
        InitializePiece(root, Black, Queen, 3, 0);
        InitializePiece(root, Black, King, 4, 0);
        root->wKingPos = sf::Vector2i(4, 7);
        root->bKingPos = sf::Vector2i(4, 0);
    }
    else
    {
        InitializePiece(root, White, Queen, 4, 0);
        InitializePiece(root, White, King, 3, 0);
        InitializePiece(root, Black, Queen, 4, 7);
        InitializePiece(root, Black, King, 3, 7);
        root->wKingPos = sf::Vector2i(3, 0);
        root->bKingPos = sf::Vector2i(3, 7);
    }

    endGame = -1;
    possibleMoves.clear();
}

void InitializePiece(Node* root, Color color, Type type, int x, int y)
{
    root->chessboard[x][y] = new Piece;
    root->chessboard[x][y]->color = color;
    root->chessboard[x][y]->type = type;
    if (color == White)
    {
        switch (type)
        {
            case Pawn: root->chessboard[x][y]->sprite.setTexture(*wPawnTexture); break;
            case Bishop: root->chessboard[x][y]->sprite.setTexture(*wBishopTexture); break;
            case Knight: root->chessboard[x][y]->sprite.setTexture(*wKnightTexture); break;
            case Rook: root->chessboard[x][y]->sprite.setTexture(*wRookTexture); break;
            case Queen: root->chessboard[x][y]->sprite.setTexture(*wQueenTexture); break;
            case King: root->chessboard[x][y]->sprite.setTexture(*wKingTexture); break;
        }
    }
    else
    {
        switch (type)
        {
            case Pawn: root->chessboard[x][y]->sprite.setTexture(*bPawnTexture); break;
            case Bishop: root->chessboard[x][y]->sprite.setTexture(*bBishopTexture); break;
            case Knight: root->chessboard[x][y]->sprite.setTexture(*bKnightTexture); break;
            case Rook: root->chessboard[x][y]->sprite.setTexture(*bRookTexture); break;
            case Queen: root->chessboard[x][y]->sprite.setTexture(*bQueenTexture); break;
            case King: root->chessboard[x][y]->sprite.setTexture(*bKingTexture); break;
        }
    }
}

void FindPossibleMoves(Node* root, bool insertTree, int x, int y, std::vector<sf::Vector2i>* moves)
{
    Piece* piece{ root->chessboard[x][y] };

    if (piece->type == Queen || piece->type == Rook)
    {
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + i, y)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x + i, y));
            }
            if (root->chessboard[x + i][y]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - i, y)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x - i, y));
            }
            if (root->chessboard[x - i][y]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (y + i > 7) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y + i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x, y + i));
            }
            if (root->chessboard[x][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (y - i < 0) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y - i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x, y - i));
            }
            if (root->chessboard[x][y - i]) break;
        }
    }

    if (piece->type == Queen || piece->type == Bishop)
    {
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7 || y + i > 7) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + i, y + i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x + i, y + i));
            }
            if (root->chessboard[x + i][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0 || y + i > 7) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - i, y + i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x - i, y + i));
            }
            if (root->chessboard[x - i][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0 || y - i < 0) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - i, y - i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x - i, y - i));
            }
            if (root->chessboard[x - i][y - i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7 || y - i < 0) break;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + i, y - i)), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(x + i, y - i));
            }
            if (root->chessboard[x + i][y - i]) break;
        }
    }

    if (piece->type == Knight)
    {
        int col[8]{ x + 1, x + 2, x + 2, x + 1, x - 1, x - 2, x - 2, x - 1 };
        int row[8]{ y - 2, y - 1, y + 1, y + 2, y + 2, y + 1, y - 1, y - 2 };

        for (int i{ 0 }; i < 8; i++)
        {
            if (col[i] < 0 || col[i] > 7 || row[i] < 0 || row[i] > 7) continue;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(col[i], row[i])), insertTree) && moves)
            {
                moves->push_back(sf::Vector2i(col[i], row[i]));
            }
        }
    }

    if (piece->type == King)
    {
        int col[8]{ x + 1, x + 1, x, x - 1, x - 1, x - 1, x, x + 1 };
        int row[8]{ y, y + 1, y + 1, y + 1, y, y - 1, y - 1, y - 1 };

        bool canGoLeft{ false };
        bool canGoRight{ false };
        for (int i{ 0 }; i < 8; i++)
        {
            if (col[i] < 0 || col[i] > 7 || row[i] < 0 || row[i] > 7) continue;
            if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(col[i], row[i])), insertTree))
            {
                if (i == 0) canGoRight = true;
                if (i == 4) canGoLeft = true;
                if (moves) moves->push_back(sf::Vector2i(col[i], row[i]));
            }
        }

        // Check for castling
        if (!IsKingThreatened(root))
        {
            bool ksRookMoved{ piece->color == White ? root->wKSRookMoved : root->bKSRookMoved };
            bool qsRookMoved{ piece->color == White ? root->wQSRookMoved : root->bQSRookMoved };
            bool kingMoved{ piece->color == White ? root->wKingMoved : root->bKingMoved };

            if (!kingMoved)
            {
                if (!root->chessboard[x - 2][y] && !root->chessboard[x - 1][y] && root->chessboard[0][y] &&
                    canGoLeft && root->chessboard[0][y]->color == root->chessboard[x][y]->color)
                {
                    if ((root->orientation == White && !root->chessboard[x - 3][y] && !qsRookMoved) ||
                        (root->orientation == Black && !ksRookMoved))
                    {
                        if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - 2, y)), insertTree) && moves)
                        {
                            moves->push_back(sf::Vector2i(x - 2, y));
                        }
                    }
                }
                if (!root->chessboard[x + 2][y] && !root->chessboard[x + 1][y] && root->chessboard[7][y] && 
                    canGoRight && root->chessboard[7][y]->color == root->chessboard[x][y]->color)
                {
                    if ((root->orientation == Black && !root->chessboard[x + 3][y] && !qsRookMoved) ||
                        (root->orientation == White && !ksRookMoved))
                    {
                        if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + 2, y)), insertTree) && moves)
                        {
                            moves->push_back(sf::Vector2i(x + 2, y));
                        }
                    }
                }
            }
        }
    }

    if (piece->type == Pawn)
    {
        if (root->orientation == piece->color)
        {
            if (y - 1 > -1)
            {
                if (!root->chessboard[x][y - 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y - 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x, y - 1));
                    }
                    if (y == 6 && !root->chessboard[x][y - 2])
                    {
                        if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y - 2)), insertTree) && moves)
                        {
                            moves->push_back(sf::Vector2i(x, y - 2));
                        }
                    }
                }
            }
            if (x - 1 > -1 && y - 1 > -1)
            {
                if (root->chessboard[x - 1][y - 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - 1, y - 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x - 1, y - 1));
                    }
                }
            }
            if (x + 1 < 8 && y - 1 > -1)
            {
                if (root->chessboard[x + 1][y - 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + 1, y - 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x + 1, y - 1));
                    }
                }
            }
        }
        else
        {
            if (y + 1 < 8)
            {
                if (!root->chessboard[x][y + 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y + 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x, y + 1));
                    }
                    if (y == 1 && !root->chessboard[x][y + 2])
                    {
                        if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x, y + 2)), insertTree) && moves)
                        {
                            moves->push_back(sf::Vector2i(x, y + 2));
                        }
                    }
                }
            }
            if (x - 1 > -1 && y + 1 < 8)
            {
                if (root->chessboard[x - 1][y + 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x - 1, y + 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x - 1, y + 1));
                    }
                }
            }
            if (x + 1 < 8 && y + 1 < 8)
            {
                if (root->chessboard[x + 1][y + 1])
                {
                    if (ValidateMove(root, Move(sf::Vector2i(x, y), sf::Vector2i(x + 1, y + 1)), insertTree) && moves)
                    {
                        moves->push_back(sf::Vector2i(x + 1, y + 1));
                    }
                }
            }
        }
    }
}

bool ValidateMove(Node* node, const Move& move, bool insertTree)
{
    if (node->chessboard[move.after.x][move.after.y])
    {
        if (node->chessboard[move.after.x][move.after.y]->color == node->chessboard[move.before.x][move.before.y]->color) return false;
    }
    bool promoted{ false };
    if (node->chessboard[move.before.x][move.before.y] && (move.after.y == 0 || move.after.y == 7))
    {
        if(node->chessboard[move.before.x][move.before.y]->type == Pawn) promoted = true;
    }

    Node* next{ new Node };
    *next = *node;
    next->next.clear();
    MakeMove(next, move, false);

    bool valid{ !IsKingThreatened(next) };
    if (valid && insertTree)
    {
        if (IsNew(next))
        {
            next->turn = node->turn == White ? Black : White;
            node->next.push_back(next);
        }
        else delete next;
    }
    else delete next;
    if (promoted && node->chessboard[move.before.x][move.before.y])
    {
        node->chessboard[move.before.x][move.before.y]->type = Pawn;
    }

    return valid;
}

bool IsKingThreatened(Node* node)
{
    sf::Vector2i king;
    if (node->turn == White) king = node->wKingPos;
    else king = node->bKingPos;
    
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x + i > 7) break;
        if (node->chessboard[king.x + i][king.y])
        {
            if (node->chessboard[king.x + i][king.y]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x + i][king.y]->type };
                if (type == Rook || type == Queen || (i == 1 && type == King)) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x - i < 0) break;
        if (node->chessboard[king.x - i][king.y])
        {
            if (node->chessboard[king.x - i][king.y]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x - i][king.y]->type };
                if (type == Rook || type == Queen || (i == 1 && type == King)) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.y + i > 7) break;
        if (node->chessboard[king.x][king.y + i])
        {
            if (node->chessboard[king.x][king.y + i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x][king.y + i]->type };
                if (type == Rook || type == Queen || (i == 1 && type == King)) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.y - i < 0) break;
        if (node->chessboard[king.x][king.y - i])
        {
            if (node->chessboard[king.x][king.y - i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x][king.y - i]->type };
                if (type == Rook || type == Queen || (i == 1 && type == King)) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x + i > 7 || king.y + i > 7) break;
        if (node->chessboard[king.x + i][king.y + i])
        {
            if (node->chessboard[king.x + i][king.y + i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x + i][king.y + i]->type };
                if (type == Bishop || type == Queen || (i == 1 && type == King)) return true;
                if (i == 1 && type == Pawn && node->orientation != node->chessboard[king.x][king.y]->color) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x - i < 0 || king.y + i > 7) break;
        if (node->chessboard[king.x - i][king.y + i])
        {
            if (node->chessboard[king.x - i][king.y + i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x - i][king.y + i]->type };
                if (type == Bishop || type == Queen || (i == 1 && type == King)) return true;
                if (i == 1 && type == Pawn && node->orientation != node->chessboard[king.x][king.y]->color) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x + i > 7 || king.y - i < 0) break;
        if (node->chessboard[king.x + i][king.y - i])
        {
            if (node->chessboard[king.x + i][king.y - i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x + i][king.y - i]->type };
                if (type == Bishop || type == Queen || (i == 1 && type == King)) return true;
                if (i == 1 && type == Pawn && node->orientation == node->chessboard[king.x][king.y]->color) return true;
            }
            break;
        }
    }
    for (int i{ 1 }; i < 8; i++)
    {
        if (king.x - i < 0 || king.y - i < 0) break;
        if (node->chessboard[king.x - i][king.y - i])
        {
            if (node->chessboard[king.x - i][king.y - i]->color != node->chessboard[king.x][king.y]->color)
            {
                Type type{ node->chessboard[king.x - i][king.y - i]->type };
                if (type == Bishop || type == Queen || (i == 1 && type == King)) return true;
                if (i == 1 && type == Pawn && node->orientation == node->chessboard[king.x][king.y]->color) return true;
            }
            break;
        }
    }

    int col[8]{ king.x + 1, king.x + 2, king.x + 2, king.x + 1, king.x - 1, king.x - 2, king.x - 2, king.x - 1 };
    int row[8]{ king.y - 2, king.y - 1, king.y + 1, king.y + 2, king.y + 2, king.y + 1, king.y - 1, king.y - 2 };

    for (int i{ 0 }; i < 8; i++)
    {
        if (col[i] < 0 || col[i] > 7 || row[i] < 0 || row[i] > 7) continue;
        if (node->chessboard[col[i]][row[i]])
        {
            if (node->chessboard[col[i]][row[i]]->color != node->chessboard[king.x][king.y]->color &&
                node->chessboard[col[i]][row[i]]->type == Knight) return true;
        }
    }

    return false;
}

void MakeMove(Node* root, const Move& move, bool display)
{
    if (display)
    {
        if (audioOn)
        {
            if(root->chessboard[move.after.x][move.after.y]) pieceTakenSound->play();
            else pieceMovedSound->play();
        }
    }

    if (root->chessboard[move.after.x][move.after.y])
    {
        std::string t{ std::to_string(root->chessboard[move.after.x][move.after.y]->type) };
        std::string c{ std::to_string(root->chessboard[move.after.x][move.after.y]->color) };
        root->materials[t + c]--;
    }
    root->chessboard[move.after.x][move.after.y] = root->chessboard[move.before.x][move.before.y];
    root->chessboard[move.before.x][move.before.y] = nullptr;
    root->lastMove = move;

    if (root->chessboard[move.after.x][move.after.y])
    {
        if (root->chessboard[move.after.x][move.after.y]->type == King)
        {
            if (root->chessboard[move.after.x][move.after.y]->color == White)
            {
                root->wKingPos = move.after;
                root->wKingMoved = true;
            }
            else
            {
                root->bKingPos = move.after;
                root->bKingMoved = true;
            }
            if (move.before.x - move.after.x == 2)
            {
                root->chessboard[move.after.x + 1][move.after.y] = root->chessboard[0][move.after.y];
                root->chessboard[0][move.after.y] = nullptr;
                if (display)
                {
                    Move rookMove{ sf::Vector2i(0, move.after.y), sf::Vector2i(move.after.x + 1, move.after.y) };
                    movingPieces.push_back({ root->chessboard[move.after.x + 1][move.after.y], rookMove });
                }
            }
            else if (move.after.x - move.before.x == 2)
            {
                root->chessboard[move.after.x - 1][move.after.y] = root->chessboard[7][move.after.y];
                root->chessboard[7][move.after.y] = nullptr;
                if (display)
                {
                    Move rookMove{ sf::Vector2i(7, move.after.y), sf::Vector2i(move.after.x - 1, move.after.y) };
                    movingPieces.push_back({ root->chessboard[move.after.x - 1][move.after.y], rookMove });
                }
            }
        }
        else if (root->chessboard[move.after.x][move.after.y]->type == Rook)
        {
            if (root->orientation == White)
            {
                if (move.before == sf::Vector2i(0, 7)) root->wQSRookMoved = true;
                else if (move.before == sf::Vector2i(7, 7)) root->wKSRookMoved = true;
                else if (move.before == sf::Vector2i(0, 0)) root->bQSRookMoved = true;
                else if (move.before == sf::Vector2i(7, 0)) root->bKSRookMoved = true;
            }
            else
            {
                if (move.before == sf::Vector2i(0, 7)) root->bKSRookMoved = true;
                else if (move.before == sf::Vector2i(7, 7)) root->bQSRookMoved = true;
                else if (move.before == sf::Vector2i(0, 0)) root->wKSRookMoved = true;
                else if (move.before == sf::Vector2i(7, 0)) root->wQSRookMoved = true;
            }
        }
        else if (root->chessboard[move.after.x][move.after.y]->type == Pawn)
        {
            if (move.after.y == 0 || move.after.y == 7)
            {
                root->chessboard[move.after.x][move.after.y]->type = Queen;
                std::string t{ std::to_string(Pawn) };
                std::string c{ std::to_string(root->chessboard[move.after.x][move.after.y]->color) };
                root->materials[t + c]--;
                t = std::to_string(Queen);
                root->materials[t + c]++;

                if (display)
                {
                    if (root->chessboard[move.after.x][move.after.y]->color == White)
                    {
                        root->chessboard[move.after.x][move.after.y]->sprite.setTexture(*wQueenTexture);
                    }
                    else root->chessboard[move.after.x][move.after.y]->sprite.setTexture(*bQueenTexture);
                }
            }
        }
        if (display)
        {
            movingPieces.push_back({ root->chessboard[move.after.x][move.after.y], move });
        }
    }
}

int CheckEndgame(Node* node)
{
    auto count
    {
        [node](Type type, Color color) -> int
        {
            return node->materials[std::to_string(type) + std::to_string(color)];
        }
    };
    if (!count(Pawn, White) && !count(Pawn, Black) && !count(Queen, White) && !count(Queen, Black) &&
       (count(Knight, White) + count(Knight, Black) + count(Bishop, White) + count(Bishop, Black) < 2)) return 3;

    for (int x{ 0 }; x < 8; x++)
    {
        for (int y{ 0 }; y < 8; y++)
        {
            if (node->chessboard[x][y])
            {
                if (node->chessboard[x][y]->color == node->turn)
                {
                    std::vector<sf::Vector2i> moves;
                    FindPossibleMoves(node, false, x, y, &moves);
                    if (!moves.empty()) return -1;
                }
            }
        }
    }

    if (!IsKingThreatened(node)) return 2;
    else if (node->turn == White) return 1;
    else return 0;
}