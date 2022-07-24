#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <random>
#include "RoundedRectangleShape.h"
#include "Button.h"
#include "Network.h"
#include "Chess.h"

class MainWindow
{
public:
    enum class Mode
    {
        Normal,
        Online,
        AI
    };

    sf::RenderWindow window;
    WINDOWPLACEMENT wpPrev{ sizeof(wpPrev) };
    std::string resourcesPath;
    int edge{ 0 };

    sf::Thread networkingThread;
    sf::Mutex lock;
    std::mt19937 mt;

    sf::Sprite background;
    sf::RectangleShape selectedSquareHighlight;
    sf::RectangleShape hoveredPMHighlight;
    sf::RectangleShape pausedBackground;
    sf::Vector2i hoveredPMSquare;
    sf::Vector2i heldPieceSquare;

    // Game state
    Node* root;
    GameInstance* session;
    Mode mode;

    sf::Font Youthome;
    sf::Font AndikaNewBasic;
    sf::Color themeColor;
    sf::Color windowColor;
    sf::String newFpsValue;
    sf::String pieceSet;

    float pieceMovementSpeed;
    int averageFrameRate;
    int menuDepth;

    bool sessionUpdated;
    bool holdingSliderArrow;
    bool fpsCapChanged;
    bool paused;

    // User Interface
    Button resume;
    Button exit;
    Button language;
    Button english;
    Button romanian;

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
    Button board1;
    Button board2;
    Button board3;
    Button pieceSet1;
    Button pieceSet2;
    Button pieceSet3;

    Button newGame;
    Button playerVsPlayer;
    Button playerVsComputer;
    Button online;
    Button offline;
    Button createGame;
    Button joinGame;
    Button playAsWhite;
    Button playAsBlack;
    Button chooseColor;
    Button gameId;
    Button id;
    Button enterGameId;
    Button inputId;
    Button errorMessage;

    // Create and initialize the window
    MainWindow(int width, int height, const char* title);

    // Set window icons
    void SetIcon();

    // Updates the piece textures with the given chess set pieces
    void UpdateTextures();

    // Gets resources filepath and initializes them
    void InitializeTexturesAndResources();

    // Initializes the user interface and applies default settings
    void InitializeUIAndSettings();

    // Calculates the layout of the elements within the window
    void CalculateLayout();

    // The main loop
    void GameLoop();

    // Draws the chessboard, pieces and UI
    void Paint();

    // Calculates the average frame rate
    void UpdateFrameRate();

    // Performs the visual transition of the fps counter
    void FpsTransition();

    // Performs the visual movement of the pieces
    void MoveTransition();

    // Switches from Windowed to Fullscreen and vice versa
    void SwitchScreenMode();

    // Called when the mouse moves within the window
    void OnMouseMove(int xPos, int yPos);

    // Called when the mouse left button is pressed 
    void OnMouseDown(int xPos, int yPos);

    // Called when the mouse left button is released
    void OnMouseUp(int xPos, int yPos);

    // Called when the keyboard Escape key is pressed
    void OnEscapeDown();

    // Called when window receives text input
    void TextEntered(unsigned int character);

    // Draws across the board the selected piece possible moves
    void ShowPossibleMoves();

    // Check and execute opponent move in online mode
    void Online();

    // Returns the row and column of the board relative to window coordinates
    // Returns (-1,-1) if outside the board
    sf::Vector2i GetSquare(int xPos, int yPos);

    // Returns the currently hovered piece or (-1,-1) if none hovered
    // Can save the currently hovered piece as (x,y) according to the (set) flag
    sf::Vector2i CurrentlyHoveredSquare(int x, int y, bool set);

    // Returns the currently held piece or (-1,-1) if none are being held
    // Can save the currently held piece as (x,y) according to the (set) flag
    sf::Vector2i CurrentlyHeldSquare(int x, int y, bool set);

    // Returns the currently selected piece or (-1,-1) if none selected
    // Can save the currently selected piece as (x,y) according to the (set) flag
    // If updated, calls FindPossibleMoves for the newly selected piece
    sf::Vector2i CurrentlySelectedSquare(int x, int y, bool set);
};
