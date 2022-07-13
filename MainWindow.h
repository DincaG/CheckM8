#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "RoundedRectangleShape.h"
#include "Button.h"
#include <windows.h>

class MainWindow
{
public:

    enum class Color
    {
        White,
        Black
    };

    enum class Turn
    {
        White,
        Black,
        Neither
    };

    enum class Type
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
        Color color{};
        Type type{};
    };

    struct MovingPiece
    {
        Piece* piece;
        sf::Vector2i startTile;
        sf::Vector2i finalTile;
    };

    sf::RenderWindow window;
    WINDOWPLACEMENT m_wpPrev{ sizeof(m_wpPrev) };
    std::string resourcesPath;
    int edge{};

    Piece* chessboard[8][8]{ nullptr };
    bool possibleMoves[8][8]{ false };

    sf::Sound pieceMovedSound;
    sf::Sound pieceTakenSound;
    sf::Sound sliderSound;
    sf::Sound mouseOverSound;

    sf::Sprite background;
    sf::RectangleShape hoveredSquare;
    sf::RectangleShape hoveredPossibleMove;
    sf::RectangleShape pausedBackground;

    Turn turn{ Turn::White };
    Color playingAs{ Color::White };

    MovingPiece movingPiece{};

    sf::Font Youthome;
    sf::Font AndikaNewBasic;
    sf::Color themeColor{ 110, 159, 194 };

    float pieceMovementSpeed;
    int averageFrameRate;
    int possibleMoveXIndex{ -1 };
    int possibleMoveYIndex{ -1 };
    int heldPieceXIndex{ -1 };
    int heldPieceYIndex{ -1 };
    int endGame{ -1 };

    bool holdingSliderArrow{ false };
    bool fpsCapChanged{ false };
    bool paused{ false };
    bool pieceMoving{ false };
    bool audioOn;
    
    sf::String newFpsValue;
    sf::String theme{ "Theme Stylish" };

    Button resume;
    Button settings;
    Button fullscreen;
    Button fullscreenOption;
    Button vSync;
    Button vSyncOption;
    Button fpsCap;
    Button fpsValue;
    Button leftArrow;
    Button rightArrow;
    Button audio;
    Button audioOption;
    Button pieceSpeed;
    Button slider;
    Button sliderArrow;
    Button pieceSpeedValue;
    Button themes;
    Button classic;
    Button stylish;
    Button newGame;
    Button exitGame;

    sf::Texture backgroundTexture;
    sf::Texture arrowTexture;
    sf::Texture sliderArrowTexture;
    sf::Texture whitePawn;
    sf::Texture whiteKnight;
    sf::Texture whiteBishop;
    sf::Texture whiteRook;
    sf::Texture whiteQueen;
    sf::Texture whiteKing;
    sf::Texture blackPawn;
    sf::Texture blackKnight;
    sf::Texture blackBishop;
    sf::Texture blackRook;
    sf::Texture blackQueen;
    sf::Texture blackKing;

    MainWindow(int width, int height, const char* title);
    void SetIcon();
    void UpdateTextures();
    void InitializeTexturesAndResources();
    void InitializeUIAndSettings();
    void InitializeChessboard();
    void InitializePiece(Color color, Type type, int x, int y);
    void CalculateLayout();
    void GameLoop();
    void Paint();
    void UpdateFrameRate();
    void FpsTransition();
    void Move();
    void SwitchScreenMode();
    void OnMouseMove(int xPos, int yPos);
    void OnMouseDown(int xPos, int yPos);
    void OnMouseUp(int xPos, int yPos);
    void OnEscapeDown();
    void FindPossibleMoves(int x, int y);
    void ShowPossibleMoves();
    void ClearPossibleMoves();
    void CheckIfMoveExposesKing(sf::Vector2i before, sf::Vector2i after, Color kingColor);
    bool CheckBoardState(Color color);
    void CheckEndgame(Color color);
    sf::Vector2i GetTile(int xPos, int yPos);
    sf::Vector2i CurrentlyHoveredTile(int x, int y, bool set);
    sf::Vector2i CurrentlyHeldTile(int x, int y, bool set);
    sf::Vector2i CurrentlyClickedTile(int x, int y, bool set);
    template <typename T> std::string toStr(const T& t);
};