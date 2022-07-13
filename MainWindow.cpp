#include "MainWindow.h"
#include <string.h>
#include <sstream>

// Create and initialize the window
MainWindow::MainWindow(int width, int height, const char* title)
{
    window.create(sf::VideoMode(width, height), title);
    SetIcon();
    InitializeTexturesAndResources();
    InitializeUIAndSettings();
    InitializeChessboard();
}

// Set window icons
void MainWindow::SetIcon()
{
    HICON largeIcon{ (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(102), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED) };
    SetClassLongPtr(window.getSystemHandle(), GCLP_HICON, reinterpret_cast<LONG_PTR>(largeIcon));

    HICON smallIcon{ (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(103), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED) };
    SetClassLongPtr(window.getSystemHandle(), GCLP_HICONSM, reinterpret_cast<LONG_PTR>(smallIcon));
}

// Updates the textures with the given theme
void MainWindow::UpdateTextures()
{
    arrowTexture.loadFromFile(resourcesPath + "/Images/Arrow.png");
    sliderArrowTexture.loadFromFile(resourcesPath + "/Images/Slider Arrow.png");
    whitePawn.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White Pawn.png");
    whiteKnight.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White Knight.png");
    whiteBishop.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White Bishop.png");
    whiteRook.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White Rook.png");
    whiteQueen.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White Queen.png");
    whiteKing.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/White King.png");
    blackPawn.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black Pawn.png");
    blackKnight.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black Knight.png");
    blackBishop.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black Bishop.png");
    blackRook.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black Rook.png");
    blackQueen.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black Queen.png");
    blackKing.loadFromFile(resourcesPath + "/Images/Chess Pieces/" + theme + "/Black King.png");

    sf::Image image;
    image.loadFromFile(resourcesPath + "/Images/Board.png");
    if (playingAs == Color::Black) image.flipHorizontally();
    backgroundTexture.loadFromImage(image);
}

// Gets resources filepath and initializes them
void MainWindow::InitializeTexturesAndResources()
{
    char* relativePath;
    _get_pgmptr(&relativePath);

    constexpr int backslash{ 92 };
    std::string path{ relativePath };
    path.erase(path.find_last_of(backslash));
    path += "/Resources";
    resourcesPath = path;

    UpdateTextures();

    backgroundTexture.setRepeated(true);
    arrowTexture.setSmooth(true);
    sliderArrowTexture.setSmooth(true);
    whitePawn.setSmooth(true);
    whiteKnight.setSmooth(true);
    whiteBishop.setSmooth(true);
    whiteRook.setSmooth(true);
    whiteQueen.setSmooth(true);
    whiteKing.setSmooth(true);
    blackPawn.setSmooth(true);
    blackKnight.setSmooth(true);
    blackBishop.setSmooth(true);
    blackRook.setSmooth(true);
    blackQueen.setSmooth(true);
    blackKing.setSmooth(true);

    background.setTexture(backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 800));
    background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);

    hoveredSquare.setFillColor(themeColor);
    hoveredPossibleMove.setFillColor(themeColor);
    pausedBackground.setFillColor(sf::Color(55, 55, 55, 150));

    Youthome.loadFromFile(resourcesPath + "/Fonts/Youthome.ttf");
    AndikaNewBasic.loadFromFile(resourcesPath + "/Fonts/AndikaNewBasic-R.ttf");

    static sf::SoundBuffer pieceMovedBuffer;
    pieceMovedBuffer.loadFromFile(resourcesPath + "/Sounds/Piece Moved.wav");
    pieceMovedSound.setBuffer(pieceMovedBuffer);

    static sf::SoundBuffer pieceTakenBuffer;
    pieceTakenBuffer.loadFromFile(resourcesPath + "/Sounds/Piece Taken.wav");
    pieceTakenSound.setBuffer(pieceTakenBuffer);

    static sf::SoundBuffer mouseOverBuffer;
    mouseOverBuffer.loadFromFile(resourcesPath + "/Sounds/Mouseover.wav");
    mouseOverSound.setBuffer(mouseOverBuffer);

    static sf::SoundBuffer sliderBuffer;
    sliderBuffer.loadFromFile(resourcesPath + "/Sounds/Slider.wav");
    sliderSound.setBuffer(sliderBuffer);
}

// Initializes the user interface and applies default settings
void MainWindow::InitializeUIAndSettings()
{
    resume.setSize(sf::Vector2f(400.f, 70.f));
    resume.setTextSize(100.f);
    resume.setTextFont(resourcesPath + "/Fonts/AndikaNewBasic-R.ttf");
    resume.setTextString("Resume");
    resume.setTextFillColor(sf::Color(10, 10, 10, 255));
    resume.setFillColor(sf::Color::Transparent);
    resume.setTextOutlineColor(themeColor);
    resume.setTextOutlineThickness(2.f);
    resume.setRelativePosition(50.f, 25.f);
    resume.setParent(background);
    resume.hide();
    resume.setUserData(this);
    resume.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (transition == Button::Transition::In)
                {
                    if (window->audioOn) window->mouseOverSound.play();
                    button->setTextStyle(sf::Text::Bold);
                    button->setTextSize(button->getTextSize() + 6.f);
                }
                else if (transition == Button::Transition::Out)
                {
                    button->setTextStyle(sf::Text::Regular);
                    button->setTextSize(button->getTextSize() - 6.f);
                }
            }
        }
    );
    resume.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->paused = false;
                window->resume.hide();
                window->settings.hide();
                window->themes.hide();
                window->newGame.hide();
                window->exitGame.hide();
                window->CurrentlyHeldTile(-1, -1, true);
            }
        }
    );

    settings = resume;
    settings.setTextString("Settings");
    settings.setRelativePosition(50.f, 37.5f);
    settings.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->resume.hide();
                window->settings.hide();
                window->themes.hide();
                window->newGame.hide();
                window->exitGame.hide();
                window->fullscreen.show();
                window->fullscreenOption.show();
                window->vSync.show();
                window->vSyncOption.show();
                window->fpsCap.show();
                window->fpsValue.show();
                window->leftArrow.show();
                window->rightArrow.show();
                window->audio.show();
                window->audioOption.show();
                window->pieceSpeed.show();
                window->slider.show();
                window->sliderArrow.show();
                window->pieceSpeedValue.show();
            }
        }
    );

    fullscreen = resume;
    fullscreen.setSize(sf::Vector2f(400.f, 50.f));
    fullscreen.setTextString("Fullscreen");
    fullscreen.setRelativePosition(25.f, 25.f);
    fullscreen.setHoverBehavior(nullptr);
    fullscreen.setPressBehavior(nullptr);
    fullscreen.setUserData(nullptr);

    fullscreenOption = resume;
    fullscreenOption.setSize(sf::Vector2f(100.f, 50.f));
    fullscreenOption.setTextString("Off");
    fullscreenOption.setRelativePosition(75.f, 25.f);
    fullscreenOption.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (button->getTextString() == "Off") button->setTextString("On");
                else button->setTextString("Off");
                window->SwitchScreenMode();
            }
        }
    );

    vSync = fullscreen;
    vSync.setTextString("VSync");
    vSync.setRelativePosition(25.f, 37.5f);

    vSyncOption = fullscreenOption;
    vSyncOption.setRelativePosition(75.f, 37.5f);
    vSyncOption.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (button->getTextString() == "Off")
                {
                    button->setTextString("On");
                    window->window.setVerticalSyncEnabled(true);
                }
                else
                {
                    button->setTextString("Off");
                    window->window.setVerticalSyncEnabled(false);
                }
            }
        }
    );

    fpsCap = fullscreen;
    fpsCap.setTextString("FPS Cap");
    fpsCap.setRelativePosition(25.f, 50.f);

    fpsValue = fullscreen;
    fpsValue.setTextString("60");
    fpsValue.setRelativePosition(75.f, 50.f);
    window.setFramerateLimit(60);

    leftArrow.setSize(sf::Vector2f(50.f, 50.f));
    leftArrow.setTexture(&arrowTexture);
    leftArrow.setFillColor(sf::Color(255, 255, 255, 160) * themeColor);
    leftArrow.setRotation(180.f);
    leftArrow.setRelativePosition(55.f, 50.f);
    leftArrow.setParent(background);
    leftArrow.hide();
    leftArrow.setUserData(this);
    leftArrow.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (transition == Button::Transition::In)
                {
                    button->setFillColor(window->themeColor);
                    button->setSize(sf::Vector2f(55.f, 55.f));
                }
                else if (transition == Button::Transition::Out)
                {
                    button->setFillColor(sf::Color(255, 255, 255, 160) * window->themeColor);
                    button->setSize(sf::Vector2f(50.f, 50.f));
                }
            }
        }
    );
    leftArrow.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (window->fpsValue.getTextString() == "Unlimited")
                {
                    window->newFpsValue = "150";
                    window->fpsCapChanged = true;
                }
                else if (window->fpsValue.getTextString() == "150")
                {
                    window->newFpsValue = "60";
                    window->fpsCapChanged = true;
                }
            }
        }
    );

    rightArrow = leftArrow;
    rightArrow.setRelativePosition(95.f, 50.f);
    rightArrow.setRotation(0.f);
    rightArrow.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (window->fpsValue.getTextString() == "60")
                {
                    window->newFpsValue = "150";
                    window->fpsCapChanged = true;
                }
                else if (window->fpsValue.getTextString() == "150")
                {
                    window->newFpsValue = "Unlimited";
                    window->fpsCapChanged = true;
                }
            }
        }
    );

    audio = fullscreen;
    audio.setTextString("Audio");
    audio.setRelativePosition(25.f, 62.5f);

    audioOption = fullscreenOption;
    audioOption.setTextString("On");
    audioOption.setRelativePosition(75.f, 62.5f);
    audioOption.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (button->getTextString() == "Off")
                {
                    button->setTextString("On");
                    window->audioOn = true;
                }
                else
                {
                    button->setTextString("Off");
                    window->audioOn = false;
                }
            }
        }
    );
    audioOn = true;

    pieceSpeed = fullscreen;
    pieceSpeed.setTextString("Piece Speed");
    pieceSpeed.setRelativePosition(25.f, 75.f);

    slider.setSize(sf::Vector2f(200.f, 2.f));
    slider.setCornerRadius(1.f);
    slider.setCornerPointCount(5);
    slider.setFillColor(sf::Color(20, 20, 20, 255));
    slider.setOutlineColor(themeColor);
    slider.setOutlineThickness(1.f);
    slider.setRelativePosition(66.6f, 75.f);
    slider.setParent(background);
    slider.hide();

    sliderArrow = rightArrow;
    sliderArrow.setSize(sf::Vector2f(30.f, 30.f));
    sliderArrow.setTexture(&sliderArrowTexture);
    sliderArrow.setRelativePosition(66.6f + -2 * 1.25f, 73.f);
    sliderArrow.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (transition == Button::Transition::In)
                {
                    button->setFillColor(window->themeColor);
                    button->setSize(sf::Vector2f(35.f, 35.f));
                }
                else if (transition == Button::Transition::Out && !window->holdingSliderArrow)
                {
                    button->setFillColor(sf::Color(255, 255, 255, 160) * window->themeColor);
                    button->setSize(sf::Vector2f(30.f, 30.f));
                }
            }
        }
    );
    sliderArrow.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (transition == Button::Transition::In) window->holdingSliderArrow = true;
                else if (transition == Button::Transition::Out)
                {
                    window->holdingSliderArrow = false;
                    if (!window->sliderArrow.isHovered())
                    {
                        button->setFillColor(sf::Color(255, 255, 255, 160) * window->themeColor);
                        button->setSize(sf::Vector2f(30.f, 30.f));
                    }
                }
            }
        }
    );

    pieceSpeedValue = fullscreen;
    pieceSpeedValue.setSize(sf::Vector2f(100.f, 40.f));
    pieceSpeedValue.setTextString("0.40s");
    pieceSpeedValue.setRelativePosition(90.f, 75.f);
    pieceMovementSpeed = 0.4f;

    themes = resume;
    themes.setTextString("Themes");
    themes.setRelativePosition(50.f, 50.f);
    themes.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->resume.hide();
                window->settings.hide();
                window->themes.hide();
                window->newGame.hide();
                window->exitGame.hide();
                window->classic.show();
                window->stylish.show();
            }
        }
    );

    classic = resume;
    classic.setSize(sf::Vector2f(300.f, 80.f));
    classic.setTextString("Classic");
    classic.setRelativePosition(50.f, 37.5f);
    classic.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (window->theme == "Theme Stylish")
                {
                    window->classic.setTextOutlineThickness(5.f);
                    window->stylish.setTextOutlineThickness(2.f);
                    window->theme = "Theme Classic";
                    window->UpdateTextures();
                }
            }
        }
    );

    stylish = classic;
    stylish.setTextString("Stylish");
    stylish.setRelativePosition(50.f, 62.5f);
    stylish.setTextOutlineThickness(5.f);
    stylish.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (window->theme == "Theme Classic")
                {
                    window->stylish.setTextOutlineThickness(5.f);
                    window->classic.setTextOutlineThickness(2.f);
                    window->theme = "Theme Stylish";
                    window->UpdateTextures();
                }
            }
        }
    );

    newGame = resume;
    newGame.setTextString("New Game");
    newGame.setRelativePosition(50.f, 62.5f);
    newGame.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };

                window->endGame = -1;
                window->CurrentlyHoveredTile(-1, -1, true);
                window->CurrentlyHeldTile(-1, -1, true);
                window->CurrentlyClickedTile(-1, -1, true);
                window->heldPieceXIndex = -1;
                window->heldPieceYIndex = -1;
                window->ClearPossibleMoves();
                window->turn = MainWindow::Turn::White;
                window->InitializeChessboard();

                window->resume.hide();
                window->settings.hide();
                window->themes.hide();
                window->newGame.hide();
                window->exitGame.hide();
                window->paused = false;
            }
        }
    );

    exitGame = resume;
    exitGame.setTextString("Exit Game");
    exitGame.setRelativePosition(50.f, 75.f);
    exitGame.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                for (int i{ 0 }; i < 8; i++)
                {
                    for (int j{ 0 }; j < 8; j++)
                    {
                        if (window->chessboard[i][j])
                        {
                            delete window->chessboard[i][j];
                            window->chessboard[i][j] = nullptr;
                        }
                    }
                }
                DestroyWindow(window->window.getSystemHandle());
            }
        }
    );
}

// Initializes the board and the chess pieces according with the color being played
void MainWindow::InitializeChessboard()
{
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (chessboard[i][j])
            {
                delete chessboard[i][j];
                chessboard[i][j] = nullptr;
            }
        }
    }

    Color enemyColor{ playingAs == Color::White ? Color::Black : Color::White };

    InitializePiece(playingAs, Type::Pawn, 0, 6);
    InitializePiece(playingAs, Type::Pawn, 1, 6);
    InitializePiece(playingAs, Type::Pawn, 2, 6);
    InitializePiece(playingAs, Type::Pawn, 3, 6);
    InitializePiece(playingAs, Type::Pawn, 4, 6);
    InitializePiece(playingAs, Type::Pawn, 5, 6);
    InitializePiece(playingAs, Type::Pawn, 6, 6);
    InitializePiece(playingAs, Type::Pawn, 7, 6);
    InitializePiece(playingAs, Type::Knight, 1, 7);
    InitializePiece(playingAs, Type::Knight, 6, 7);
    InitializePiece(playingAs, Type::Bishop, 2, 7);
    InitializePiece(playingAs, Type::Bishop, 5, 7);
    InitializePiece(playingAs, Type::Rook, 0, 7);
    InitializePiece(playingAs, Type::Rook, 7, 7);
    InitializePiece(playingAs, Type::Queen, 3, 7);
    InitializePiece(playingAs, Type::King, 4, 7);

    InitializePiece(enemyColor, Type::Pawn, 0, 1);
    InitializePiece(enemyColor, Type::Pawn, 1, 1);
    InitializePiece(enemyColor, Type::Pawn, 2, 1);
    InitializePiece(enemyColor, Type::Pawn, 3, 1);
    InitializePiece(enemyColor, Type::Pawn, 4, 1);
    InitializePiece(enemyColor, Type::Pawn, 5, 1);
    InitializePiece(enemyColor, Type::Pawn, 6, 1);
    InitializePiece(enemyColor, Type::Pawn, 7, 1);
    InitializePiece(enemyColor, Type::Knight, 1, 0);
    InitializePiece(enemyColor, Type::Knight, 6, 0);
    InitializePiece(enemyColor, Type::Bishop, 2, 0);
    InitializePiece(enemyColor, Type::Bishop, 5, 0);
    InitializePiece(enemyColor, Type::Rook, 0, 0);
    InitializePiece(enemyColor, Type::Rook, 7, 0);
    InitializePiece(enemyColor, Type::Queen, 3, 0);
    InitializePiece(enemyColor, Type::King, 4, 0);

    sf::Image image;
    image.loadFromFile(resourcesPath + "/Images/Board.png");
    if (playingAs == Color::Black) image.flipHorizontally();
    backgroundTexture.loadFromImage(image);

    CalculateLayout();
}

// Creates and initializes a piece based on color, type and position
void MainWindow::InitializePiece(Color color, Type type, int x, int y)
{
    chessboard[x][y] = new Piece;
    chessboard[x][y]->color = color;
    chessboard[x][y]->type = type;
    if (color == Color::White)
    {
        switch (type)
        {
        case Type::Pawn: chessboard[x][y]->sprite.setTexture(whitePawn); break;
        case Type::Bishop: chessboard[x][y]->sprite.setTexture(whiteBishop); break;
        case Type::Knight: chessboard[x][y]->sprite.setTexture(whiteKnight); break;
        case Type::Rook: chessboard[x][y]->sprite.setTexture(whiteRook); break;
        case Type::Queen: chessboard[x][y]->sprite.setTexture(whiteQueen); break;
        case Type::King: chessboard[x][y]->sprite.setTexture(whiteKing); break;
        }
    }
    else
    {
        switch (type)
        {
        case Type::Pawn: chessboard[x][y]->sprite.setTexture(blackPawn); break;
        case Type::Bishop: chessboard[x][y]->sprite.setTexture(blackBishop); break;
        case Type::Knight: chessboard[x][y]->sprite.setTexture(blackKnight); break;
        case Type::Rook: chessboard[x][y]->sprite.setTexture(blackRook); break;
        case Type::Queen: chessboard[x][y]->sprite.setTexture(blackQueen); break;
        case Type::King: chessboard[x][y]->sprite.setTexture(blackKing); break;
        }
    }
}

// Calculates the layout of the elements within the window
void MainWindow::CalculateLayout()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };

    int length{};
    float offsetX{};
    float offsetY{};

    if (width > height)
    {
        length = height;
        offsetX = (width - height) / 2.f;
    }
    else
    {
        length = width;
        offsetY = (height - width) / 2.f;
    }

    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (chessboard[i][j])
            {
                chessboard[i][j]->sprite.setPosition((length / 8.f) * i + offsetX, (length / 8.f) * j + offsetY);
                chessboard[i][j]->sprite.setScale(length / 1600.f, length / 1600.f);
            }
        }
    }
    if (heldPieceXIndex != -1 && heldPieceYIndex != -1)
    {
        if (chessboard[heldPieceXIndex][heldPieceYIndex])
        {
            chessboard[heldPieceXIndex][heldPieceYIndex]->sprite.scale(0.8f, 0.8f);
            float postLength{ chessboard[heldPieceXIndex][heldPieceYIndex]->sprite.getGlobalBounds().width };

            float offset{ (length / 8.f - postLength) / 2.f };
            chessboard[heldPieceXIndex][heldPieceYIndex]->sprite.move(offset, offset);
        }
    }

    hoveredPossibleMove.setSize(sf::Vector2f(length / 8.f, length / 8.f));
    hoveredPossibleMove.setPosition((length / 8.f) * possibleMoveXIndex + offsetX, (length / 8.f) * possibleMoveYIndex + offsetY);

    hoveredSquare.setSize(sf::Vector2f(length / 8.f, length / 8.f));
    hoveredSquare.setPosition((length / 8.f) * heldPieceXIndex + offsetX, (length / 8.f) * heldPieceYIndex + offsetY);

    pausedBackground.setSize(sf::Vector2f((float)width, (float)height));
    background.setScale(length / 800.f, length / 800.f);
    background.setPosition(width / 2.f, height / 2.f);
}

// The main loop
void MainWindow::GameLoop()
{
    if (pieceMoving) Move();
    if (fpsCapChanged) FpsTransition();
    UpdateFrameRate();

    Paint();
}

// Draws the chessboard, pieces and UI
void MainWindow::Paint()
{
    window.clear(sf::Color(245, 245, 245));
    window.draw(background);

    if (CurrentlyClickedTile(-1, -1, false) != sf::Vector2i(-1, -1)) ShowPossibleMoves();
    if (CurrentlyHeldTile(-1, -1, false) != sf::Vector2i(-1, -1)) window.draw(hoveredSquare);
    if (CurrentlyHoveredTile(-1, -1, false) != sf::Vector2i(-1, -1))
    {
        if (possibleMoves[CurrentlyHoveredTile(-1, -1, false).x][CurrentlyHoveredTile(-1, -1, false).y])
        {
            window.draw(hoveredPossibleMove);
        }
    }
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (chessboard[i][j])
            {
                window.draw(chessboard[i][j]->sprite);
            }
        }
    }

    if (endGame != -1)
    {
        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        rect.setFillColor(sf::Color(255, 255, 255, 100));
        window.draw(rect);

        sf::Text text;
        text.setFont(Youthome);
        text.setFillColor(sf::Color(10, 10, 10, 100));
        text.setCharacterSize(window.getSize().x > window.getSize().y ? window.getSize().y / 5 : window.getSize().x / 5);

        if (endGame == 0) text.setString("White Wins");
        else text.setString("Black Wins");

        text.setPosition((window.getSize().x - text.getGlobalBounds().width) / 2, (window.getSize().y - text.getGlobalBounds().height) / 2);

        window.draw(text);
    }

    if (paused) window.draw(pausedBackground);
    if (resume.isVisible()) window.draw(resume);
    if (settings.isVisible()) window.draw(settings);
    if (fullscreen.isVisible()) window.draw(fullscreen);
    if (fullscreenOption.isVisible()) window.draw(fullscreenOption);
    if (vSync.isVisible()) window.draw(vSync);
    if (vSyncOption.isVisible()) window.draw(vSyncOption);
    if (fpsCap.isVisible()) window.draw(fpsCap);
    if (fpsValue.isVisible()) window.draw(fpsValue);
    if (leftArrow.isVisible()) window.draw(leftArrow);
    if (rightArrow.isVisible()) window.draw(rightArrow);
    if (audio.isVisible()) window.draw(audio);
    if (audioOption.isVisible()) window.draw(audioOption);
    if (pieceSpeed.isVisible()) window.draw(pieceSpeed);
    if (slider.isVisible()) window.draw(slider);
    if (sliderArrow.isVisible()) window.draw(sliderArrow);
    if (pieceSpeedValue.isVisible()) window.draw(pieceSpeedValue);
    if (themes.isVisible()) window.draw(themes);
    if (classic.isVisible()) window.draw(classic);
    if (stylish.isVisible()) window.draw(stylish);
    if (newGame.isVisible()) window.draw(newGame);
    if (exitGame.isVisible()) window.draw(exitGame);

    window.display();
}

// Calculates the average frame rate
void MainWindow::UpdateFrameRate()
{
    static sf::Clock clock;
    static int index{ 0 };
    float timeSinceLastDraw{ clock.restart().asSeconds() };

    static float timeIntervals[60]{ 0 };
    timeIntervals[index++] = timeSinceLastDraw;
    if (index > 59) index = 0;

    float sum{ 0 };
    for (int i{ 0 }; i < 60; i++) sum += timeIntervals[i];
    averageFrameRate = static_cast<int>(1.f / (sum / 60.f));
}

// Performs the visual transition of the fps counter
void MainWindow::FpsTransition()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };
    int length{ width > height ? height : width };

    static constexpr float fadingSpeed{ 0.25f };
    float opacityOffset{ 255.f / (averageFrameRate * fadingSpeed) };

    static float opacity{ 255.f };
    static bool fadeOut{ true };
    static bool fadeIn{ false };
    if (fadeOut)
    {
        if (opacity > 1.f) opacity -= opacityOffset;
        else
        {
            fadeOut = false;
            fadeIn = true;
            fpsValue.setTextString(newFpsValue);
        }
    }
    if (fadeIn)
    {
        if (opacity < 255.f) opacity += opacityOffset;
        else
        {
            fadeIn = false;
            fadeOut = true;
            fpsCapChanged = false;
            if (fpsValue.getTextString() == "150") window.setFramerateLimit(150);
            if (fpsValue.getTextString() == "60") window.setFramerateLimit(60);
            if (fpsValue.getTextString() == "Unlimited") window.setFramerateLimit(0);
        }
    }

    if (opacity > 255.f) opacity = 255.f;
    if (opacity < 0.f) opacity = 0.f;
    fpsValue.setTextFillColor(sf::Color(fpsValue.getTextFillColor().r, fpsValue.getTextFillColor().g, fpsValue.getTextFillColor().b, static_cast<sf::Uint8>(opacity)));
    fpsValue.setTextOutlineColor(sf::Color(fpsValue.getTextOutlineColor().r, fpsValue.getTextOutlineColor().g, fpsValue.getTextOutlineColor().b, static_cast<sf::Uint8>(opacity)));
}

// Performs the visual movement of the pieces
void MainWindow::Move()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };
    float tile_length{ width > height ? height / 8.f : width / 8.f };

    float diffX{};
    float diffY{};
    width > height ? diffX = (width - height) / 2.f : diffY = (height - width) / 2.f;

    float distanceX{ ((tile_length * movingPiece.finalTile.x + diffX) - (tile_length * movingPiece.startTile.x + diffX)) };
    float distanceY{ ((tile_length * movingPiece.finalTile.y + diffY) - (tile_length * movingPiece.startTile.y + diffY)) };

    float xSpeed{};
    float ySpeed{};

    if (pieceMovementSpeed != 0)
    {
        xSpeed = distanceX / (pieceMovementSpeed * averageFrameRate);
        ySpeed = distanceY / (pieceMovementSpeed * averageFrameRate);
    }

    float diffPosX{ (tile_length * movingPiece.finalTile.x + diffX) - (movingPiece.piece->sprite.getPosition().x) };
    float diffPosY{ (tile_length * movingPiece.finalTile.y + diffY) - (movingPiece.piece->sprite.getPosition().y) };

    if (pieceMovementSpeed == 0)
    {
        xSpeed = diffPosX;
        ySpeed = diffPosY;
    }

    if ((xSpeed > 0 && xSpeed >= diffPosX) || (xSpeed < 0 && xSpeed <= diffPosX)) xSpeed = diffPosX;
    if ((ySpeed > 0 && ySpeed >= diffPosY) || (ySpeed < 0 && ySpeed <= diffPosY)) ySpeed = diffPosY;
    movingPiece.piece->sprite.setPosition(movingPiece.piece->sprite.getPosition().x + xSpeed, movingPiece.piece->sprite.getPosition().y + ySpeed);

    if (xSpeed == 0 && ySpeed == 0)
    {
        pieceMoving = false;
        if (turn == Turn::White) turn = Turn::Black;
        else if (turn == Turn::Black) turn = Turn::White;

        SendMessage(window.getSystemHandle(), WM_MOUSEMOVE, NULL, MAKELPARAM(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
    }
}

// Switches from Windowed to Fullscreen and vice versa
void MainWindow::SwitchScreenMode()
{
    HWND hwnd = window.getSystemHandle();
    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

    if (dwStyle & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(hwnd, &m_wpPrev) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
        {
            SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP,
                mi.rcMonitor.left,
                mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(hwnd, &m_wpPrev);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }

    CalculateLayout();
}

// Called when the mouse moves within the window
void MainWindow::OnMouseMove(int xPos, int yPos)
{
    if (resume.isVisible()) resume.onHover(xPos, yPos);
    if (settings.isVisible()) settings.onHover(xPos, yPos);
    if (fullscreenOption.isVisible()) fullscreenOption.onHover(xPos, yPos);
    if (vSyncOption.isVisible()) vSyncOption.onHover(xPos, yPos);
    if (leftArrow.isVisible()) leftArrow.onHover(xPos, yPos);
    if (rightArrow.isVisible()) rightArrow.onHover(xPos, yPos);
    if (audioOption.isVisible()) audioOption.onHover(xPos, yPos);
    if (sliderArrow.isVisible()) sliderArrow.onHover(xPos, yPos);
    if (themes.isVisible()) themes.onHover(xPos, yPos);
    if (classic.isVisible()) classic.onHover(xPos, yPos);
    if (stylish.isVisible()) stylish.onHover(xPos, yPos);
    if (newGame.isVisible()) newGame.onHover(xPos, yPos);
    if (exitGame.isVisible()) exitGame.onHover(xPos, yPos);

    if (holdingSliderArrow)
    {
        float section{ slider.getSize().x / 20.f };
        float min{ abs(xPos - slider.getPosition().x) };
        static int prevSliderIndex{ 8 };
        int sliderIndex{ 10 };

        for (int i{ 0 }; i < 21; i++)
        {
            if (abs(xPos - (slider.getPosition().x + section * (i - 10))) < min)
            {
                min = abs(xPos - (slider.getPosition().x + section * (i - 10)));
                sliderIndex = i;
            }
        }
        sliderArrow.setRelativePosition(66.6f + (sliderIndex - 10) * 1.25f, 73.f);

        if (prevSliderIndex != sliderIndex && audioOn) sliderSound.play();
        prevSliderIndex = sliderIndex;

        pieceMovementSpeed = sliderIndex / 20.f;
        std::string value{ toStr(pieceMovementSpeed + 0.001f) };
        value.pop_back();
        value.append("s");
        pieceSpeedValue.setTextString(value);
    }

    if (endGame == -1 && !paused && !pieceMoving)
    {
        sf::Vector2i tile{ GetTile(xPos, yPos) };
        CurrentlyHoveredTile(tile.x, tile.y, true);
    }
}

// Called when the mouse left button is pressed 
void MainWindow::OnMouseDown(int xPos, int yPos)
{
    if (endGame == -1 && !paused)
    {
        sf::Vector2i tile{ GetTile(xPos, yPos) };

        if (tile.x != -1 && tile.y != -1)
        {
            if (chessboard[tile.x][tile.y])
            {
                if (chessboard[tile.x][tile.y]->color == (turn == MainWindow::Turn::White ? MainWindow::Color::White : MainWindow::Color::Black))
                    CurrentlyHeldTile(tile.x, tile.y, true);
            }
            if (possibleMoves[tile.x][tile.y])
            {
                if (audioOn)
                {
                    if (chessboard[tile.x][tile.y]) pieceTakenSound.play();
                    else pieceMovedSound.play();
                }

                sf::Vector2i clicked_tile{ CurrentlyClickedTile(-1,-1,false) };

                if (chessboard[tile.x][tile.y]) delete chessboard[tile.x][tile.y];
                chessboard[tile.x][tile.y] = chessboard[clicked_tile.x][clicked_tile.y];
                chessboard[clicked_tile.x][clicked_tile.y] = nullptr;

                if (chessboard[tile.x][tile.y]->type == MainWindow::Type::Pawn)
                {
                    if (tile.y == 0 || tile.y == 7)
                    {
                        chessboard[tile.x][tile.y]->type = MainWindow::Type::Queen;
                        if (chessboard[tile.x][tile.y]->color == MainWindow::Color::White) chessboard[tile.x][tile.y]->sprite.setTexture(whiteQueen);
                        else chessboard[tile.x][tile.y]->sprite.setTexture(blackQueen);
                    }
                }

                movingPiece.piece = chessboard[tile.x][tile.y];
                movingPiece.startTile = clicked_tile;
                movingPiece.finalTile = tile;
                pieceMoving = true;

                CurrentlyClickedTile(-1, -1, true);
                CheckEndgame(turn == MainWindow::Turn::White ? MainWindow::Color::Black : MainWindow::Color::White);
            }
        }
    }

    if (resume.isVisible())             { resume.onPress(); resume.onRelease(); }
    if (settings.isVisible())           { settings.onPress(); settings.onRelease(); }
    if (fullscreenOption.isVisible())   { fullscreenOption.onPress(); fullscreenOption.onRelease(); }
    if (vSyncOption.isVisible())        { vSyncOption.onPress(); vSyncOption.onRelease(); }
    if (leftArrow.isVisible())          { leftArrow.onPress(); leftArrow.onRelease(); }
    if (rightArrow.isVisible())         { rightArrow.onPress(); rightArrow.onRelease(); }
    if (audioOption.isVisible())        { audioOption.onPress(); audioOption.onRelease(); }
    if (sliderArrow.isVisible())        { sliderArrow.onPress(); }
    if (themes.isVisible())             { themes.onPress(); themes.onRelease(); }
    if (classic.isVisible())            { classic.onPress(); classic.onRelease(); }
    if (stylish.isVisible())            { stylish.onPress(); stylish.onRelease(); }
    if (newGame.isVisible())            { newGame.onPress(); newGame.onRelease(); }
    if (exitGame.isVisible())           { exitGame.onPress(); exitGame.onRelease(); }
}

// Called when the mouse left button is released
void MainWindow::OnMouseUp(int xPos, int yPos)
{
    if (!paused)
    {
        sf::Vector2i tile{ GetTile(xPos, yPos) };
        sf::Vector2i heldTile{ CurrentlyHeldTile(-1, -1, false) };

        sf::Vector2i clickedTile{ CurrentlyClickedTile(-1, -1, false) };
        CurrentlyClickedTile(-1, -1, true);

        if (tile != sf::Vector2i(-1, -1))
        {
            if (heldTile == tile)
            {
                if (clickedTile != tile)
                {
                    CurrentlyClickedTile(tile.x, tile.y, true);
                }
            }
        }

        CurrentlyHeldTile(-1, -1, true);
    }

    if (sliderArrow.isVisible()) sliderArrow.onRelease();
}

// Called when the keyboard Escape key is pressed
void MainWindow::OnEscapeDown()
{
    if (!paused)
    {
        paused = true;
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exitGame.show();
    }
    else if (fullscreen.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exitGame.show();
        fullscreen.hide();
        fullscreenOption.hide();
        vSync.hide();
        vSyncOption.hide();
        fpsCap.hide();
        fpsValue.hide();
        leftArrow.hide();
        rightArrow.hide();
        audio.hide();
        audioOption.hide();
        pieceSpeed.hide();
        slider.hide();
        sliderArrow.hide();
        pieceSpeedValue.hide();

        holdingSliderArrow = false;
        sliderArrow.onHover(0, 0);
    }
    else if (classic.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exitGame.show();
        classic.hide();
        stylish.hide();
    }
    else
    {
        paused = false;
        resume.hide();
        settings.hide();
        themes.hide();
        newGame.hide();
        exitGame.hide();
        CurrentlyHeldTile(-1, -1, true);
    }

    SendMessage(window.getSystemHandle(), WM_MOUSEMOVE, NULL, MAKELPARAM(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
}

// Updates the array of possible moves for the (x,y) positioned piece
void MainWindow::FindPossibleMoves(int x, int y)
{
    Piece* piece{ chessboard[x][y] };
    Color color{ piece->color };

    if (piece->type == Type::Queen || piece->type == Type::Rook)
    {
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7) break;
            if (chessboard[x + i][y])
            {
                if (chessboard[x + i][y]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + i, y), color);
            if (chessboard[x + i][y]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0) break;
            if (chessboard[x - i][y])
            {
                if (chessboard[x - i][y]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - i, y), color);
            if (chessboard[x - i][y]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (y + i > 7) break;
            if (chessboard[x][y + i])
            {
                if (chessboard[x][y + i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y + i), color);
            if (chessboard[x][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (y - i < 0) break;
            if (chessboard[x][y - i])
            {
                if (chessboard[x][y - i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y - i), color);
            if (chessboard[x][y - i]) break;
        }
    }

    if (piece->type == Type::Queen || piece->type == Type::Bishop)
    {
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7 || y + i > 7) break;
            if (chessboard[x + i][y + i])
            {
                if (chessboard[x + i][y + i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + i, y + i), color);
            if (chessboard[x + i][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0 || y + i > 7) break;
            if (chessboard[x - i][y + i])
            {
                if (chessboard[x - i][y + i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - i, y + i), color);
            if (chessboard[x - i][y + i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x - i < 0 || y - i < 0) break;
            if (chessboard[x - i][y - i])
            {
                if (chessboard[x - i][y - i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - i, y - i), color);
            if (chessboard[x - i][y - i]) break;
        }
        for (int i{ 1 }; i < 8; i++)
        {
            if (x + i > 7 || y - i < 0) break;
            if (chessboard[x + i][y - i])
            {
                if (chessboard[x + i][y - i]->color == color) break;
            }
            CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + i, y - i), color);
            if (chessboard[x + i][y - i]) break;
        }
    }

    if (piece->type == Type::Knight)
    {
        if (x + 1 < 8 && y - 2 > -1) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y - 2), color);
        if (x + 2 < 8 && y - 1 > -1) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 2, y - 1), color);
        if (x + 2 < 8 && y + 1 < 8) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 2, y + 1), color);
        if (x + 1 < 8 && y + 2 < 8) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y + 2), color);
        if (x - 1 > -1 && y + 2 < 8) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y + 2), color);
        if (x - 2 > -1 && y + 1 < 8) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 2, y + 1), color);
        if (x - 2 > -1 && y - 1 > -1) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 2, y - 1), color);
        if (x - 1 > -1 && y - 2 > -1) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y - 2), color);
    }

    if (piece->type == Type::King)
    {
        if (x + 1 < 8)                CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y), color);
        if (x + 1 < 8 && y + 1 < 8)   CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y + 1), color);
        if (y + 1 < 8)                CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y + 1), color);
        if (x - 1 > -1 && y + 1 < 8)  CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y + 1), color);
        if (x - 1 > -1)               CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y), color);
        if (x - 1 > -1 && y - 1 > -1) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y - 1), color);
        if (y - 1 > -1)               CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y - 1), color);
        if (x + 1 < 8 && y - 1 > -1)  CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y - 1), color);
    }

    if (piece->type == Type::Pawn)
    {
        if (playingAs == color)
        {
            if (y - 1 > -1)
            {
                if (!chessboard[x][y - 1]) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y - 1), color);
            }
            if (y == 6 && !chessboard[x][y - 1])
            {
                if (!chessboard[x][4]) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, 4), color);
            }
            if (x - 1 > -1 && y - 1 > -1)
            {
                if (chessboard[x - 1][y - 1])
                {
                    if (chessboard[x - 1][y - 1]->color != color) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y - 1), color);
                }
            }
            if (x + 1 < 8 && y - 1 > -1)
            {
                if (chessboard[x + 1][y - 1])
                {
                    if (chessboard[x + 1][y - 1]->color != color) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y - 1), color);
                }
            }
        }
        else
        {
            if (y + 1 < 8)
            {
                if (!chessboard[x][y + 1]) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, y + 1), color);
            }
            if (y == 1 && !chessboard[x][y + 1])
            {
                if (!chessboard[x][3]) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x, 3), color);
            }
            if (x - 1 > -1 && y + 1 < 8)
            {
                if (chessboard[x - 1][y + 1])
                {
                    if (chessboard[x - 1][y + 1]->color != color) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x - 1, y + 1), color);
                }
            }
            if (x + 1 < 8 && y + 1 < 8)
            {
                if (chessboard[x + 1][y + 1])
                {
                    if (chessboard[x + 1][y + 1]->color != color) CheckIfMoveExposesKing(sf::Vector2i(x, y), sf::Vector2i(x + 1, y + 1), color);
                }
            }
        }
    }
}

// Draws across the board the selected piece possible moves
void MainWindow::ShowPossibleMoves()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };
    float tile_length{ width > height ? height / 8.f : width / 8.f };

    float offsetX{};
    float offsetY{};
    width > height ? offsetX = (width - height) / 2.f : offsetY = (height - width) / 2.f;

    sf::CircleShape circle(tile_length / 8);
    sf::ConvexShape triangle(3);
    circle.setFillColor(sf::Color(80, 80, 80));
    triangle.setFillColor(themeColor);

    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (possibleMoves[i][j])
            {
                if (chessboard[i][j])
                {
                    float s_offset{ 0.3f };
                    float b_offset{ 0.7f };

                    triangle.setPoint(0, sf::Vector2f(tile_length * i + offsetX, tile_length * j + tile_length * s_offset + offsetY));
                    triangle.setPoint(1, sf::Vector2f(tile_length * i + offsetX, tile_length * j + offsetY));
                    triangle.setPoint(2, sf::Vector2f(tile_length * i + tile_length * s_offset + offsetX, tile_length * j + offsetY));
                    window.draw(triangle);

                    triangle.setPoint(0, sf::Vector2f(tile_length * i + tile_length * b_offset + offsetX, tile_length * j + offsetY));
                    triangle.setPoint(1, sf::Vector2f(tile_length * (i + 1) + offsetX, tile_length * j + offsetY));
                    triangle.setPoint(2, sf::Vector2f(tile_length * (i + 1) + offsetX, tile_length * j + tile_length * s_offset + offsetY));
                    window.draw(triangle);

                    triangle.setPoint(0, sf::Vector2f(tile_length * (i + 1) + offsetX, tile_length * j + tile_length * b_offset + offsetY));
                    triangle.setPoint(1, sf::Vector2f(tile_length * (i + 1) + offsetX, tile_length * (j + 1) + offsetY));
                    triangle.setPoint(2, sf::Vector2f(tile_length * i + tile_length * b_offset + offsetX, tile_length * (j + 1) + offsetY));
                    window.draw(triangle);

                    triangle.setPoint(0, sf::Vector2f(tile_length * i + offsetX, tile_length * j + tile_length * b_offset + offsetY));
                    triangle.setPoint(1, sf::Vector2f(tile_length * i + offsetX, tile_length * (j + 1) + offsetY));
                    triangle.setPoint(2, sf::Vector2f(tile_length * i + tile_length * s_offset + offsetX, tile_length * (j + 1) + offsetY));
                    window.draw(triangle);
                }
                else
                {
                    float offset{ tile_length / 2 - circle.getRadius() };
                    circle.setPosition(tile_length * i + offset + offsetX, tile_length * j + offset + offsetY);
                    window.draw(circle);
                }
            }
        }
    }
}

// Clears the array of possible moves
void MainWindow::ClearPossibleMoves()
{
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++) possibleMoves[i][j] = false;
    }
}

// Called by FindPossibleMoves as a last check before making the move valid
// Checks if the move results with kingColor king being checked
void MainWindow::CheckIfMoveExposesKing(sf::Vector2i before, sf::Vector2i after, Color kingColor)
{
    Piece* tempStart{ chessboard[before.x][before.y] };
    Piece* tempEnd{ chessboard[after.x][after.y] };

    chessboard[after.x][after.y] = chessboard[before.x][before.y];
    chessboard[before.x][before.y] = nullptr;

    if (CheckBoardState(kingColor)) possibleMoves[after.x][after.y] = true;

    chessboard[after.x][after.y] = tempEnd;
    chessboard[before.x][before.y] = tempStart;

    if (chessboard[after.x][after.y])
    {
        if (chessboard[after.x][after.y]->color == kingColor) possibleMoves[after.x][after.y] = false;
    }
}

// Checks the board state of the color
// Returns false if the king is threatened, true otherwise
bool MainWindow::CheckBoardState(Color color)
{
    for (int x{ 0 }; x < 8; x++)
    {
        for (int y{ 0 }; y < 8; y++)
        {
            if (chessboard[x][y])
            {
                if (chessboard[x][y]->color != color)
                {
                    if (chessboard[x][y]->type == Type::Queen || chessboard[x][y]->type == Type::Rook)
                    {
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x + i > 7) break;
                            else if (chessboard[x + i][y])
                            {
                                if (chessboard[x + i][y]->color == color && chessboard[x + i][y]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x - i < 0) break;
                            else if (chessboard[x - i][y])
                            {
                                if (chessboard[x - i][y]->color == color && chessboard[x - i][y]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (y + i > 7) break;
                            else if (chessboard[x][y + i])
                            {
                                if (chessboard[x][y + i]->color == color && chessboard[x][y + i]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (y - i < 0) break;
                            else if (chessboard[x][y - i])
                            {
                                if (chessboard[x][y - i]->color == color && chessboard[x][y - i]->type == Type::King) return false;
                                break;
                            }
                        }
                    }

                    if (chessboard[x][y]->type == Type::Queen || chessboard[x][y]->type == Type::Bishop)
                    {
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x + i > 7 || y + i > 7) break;
                            else if (chessboard[x + i][y + i])
                            {
                                if (chessboard[x + i][y + i]->color == color && chessboard[x + i][y + i]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x - i < 0 || y + i > 7) break;
                            else if (chessboard[x - i][y + i])
                            {
                                if (chessboard[x - i][y + i]->color == color && chessboard[x - i][y + i]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x - i < 0 || y - i < 0) break;
                            else if (chessboard[x - i][y - i])
                            {
                                if (chessboard[x - i][y - i]->color == color && chessboard[x - i][y - i]->type == Type::King) return false;
                                break;
                            }
                        }
                        for (int i{ 1 }; i < 8; i++)
                        {
                            if (x + i > 7 || y - i < 0) break;
                            else if (chessboard[x + i][y - i])
                            {
                                if (chessboard[x + i][y - i]->color == color && chessboard[x + i][y - i]->type == Type::King) return false;
                                break;
                            }
                        }
                    }

                    if (chessboard[x][y]->type == Type::Knight)
                    {
                        if (x + 1 < 8 && y - 2 > -1)
                        {
                            if (chessboard[x + 1][y - 2])
                            {
                                if (chessboard[x + 1][y - 2]->color == color && chessboard[x + 1][y - 2]->type == Type::King) return false;
                            }
                        }
                        if (x + 2 < 8 && y - 1 > -1)
                        {
                            if (chessboard[x + 2][y - 1])
                            {
                                if (chessboard[x + 2][y - 1]->color == color && chessboard[x + 2][y - 1]->type == Type::King) return false;
                            }
                        }
                        if (x + 2 < 8 && y + 1 < 8)
                        {
                            if (chessboard[x + 2][y + 1])
                            {
                                if (chessboard[x + 2][y + 1]->color == color && chessboard[x + 2][y + 1]->type == Type::King) return false;
                            }
                        }
                        if (x + 1 < 8 && y + 2 < 8)
                        {
                            if (chessboard[x + 1][y + 2])
                            {
                                if (chessboard[x + 1][y + 2]->color == color && chessboard[x + 1][y + 2]->type == Type::King) return false;
                            }
                        }
                        if (x - 1 > -1 && y + 2 < 8)
                        {
                            if (chessboard[x - 1][y + 2])
                            {
                                if (chessboard[x - 1][y + 2]->color == color && chessboard[x - 1][y + 2]->type == Type::King) return false;
                            }
                        }
                        if (x - 2 > -1 && y + 1 < 8)
                        {
                            if (chessboard[x - 2][y + 1])
                            {
                                if (chessboard[x - 2][y + 1]->color == color && chessboard[x - 2][y + 1]->type == Type::King) return false;
                            }
                        }
                        if (x - 2 > -1 && y - 1 > -1)
                        {
                            if (chessboard[x - 2][y - 1])
                            {
                                if (chessboard[x - 2][y - 1]->color == color && chessboard[x - 2][y - 1]->type == Type::King) return false;
                            }
                        }
                        if (x - 1 > -1 && y - 2 > -1)
                        {
                            if (chessboard[x - 1][y - 2])
                            {
                                if (chessboard[x - 1][y - 2]->color == color && chessboard[x - 1][y - 2]->type == Type::King) return false;
                            }
                        }
                    }

                    if (chessboard[x][y]->type == Type::King)
                    {
                        if (x + 1 < 8)
                        {
                            if (chessboard[x + 1][y])
                            {
                                if (chessboard[x + 1][y]->color == color && chessboard[x + 1][y]->type == Type::King) return false;
                            }
                        }
                        if (x + 1 < 8 && y + 1 < 8)
                        {
                            if (chessboard[x + 1][y + 1])
                            {
                                if (chessboard[x + 1][y + 1]->color == color && chessboard[x + 1][y + 1]->type == Type::King) return false;
                            }
                        }
                        if (y + 1 < 8)
                        {
                            if (chessboard[x][y + 1])
                            {
                                if (chessboard[x][y + 1]->color == color && chessboard[x][y + 1]->type == Type::King) return false;
                            }
                        }
                        if (x - 1 > -1 && y + 1 < 8)
                        {
                            if (chessboard[x - 1][y + 1])
                            {
                                if (chessboard[x - 1][y + 1]->color == color && chessboard[x - 1][y + 1]->type == Type::King) return false;
                            }
                        }
                        if (x - 1 > -1)
                        {
                            if (chessboard[x - 1][y])
                            {
                                if (chessboard[x - 1][y]->color == color && chessboard[x - 1][y]->type == Type::King) return false;
                            }
                        }
                        if (x - 1 > -1 && y - 1 > -1)
                        {
                            if (chessboard[x - 1][y - 1])
                            {
                                if (chessboard[x - 1][y - 1]->color == color && chessboard[x - 1][y - 1]->type == Type::King) return false;
                            }
                        }
                        if (y - 1 > -1)
                        {
                            if (chessboard[x][y - 1])
                            {
                                if (chessboard[x][y - 1]->color == color && chessboard[x][y - 1]->type == Type::King) return false;
                            }
                        }
                        if (x + 1 < 8 && y - 1 > -1)
                        {
                            if (chessboard[x + 1][y - 1])
                            {
                                if (chessboard[x + 1][y - 1]->color == color && chessboard[x + 1][y - 1]->type == Type::King) return false;
                            }
                        }
                    }

                    if (chessboard[x][y]->type == Type::Pawn)
                    {
                        if (chessboard[x][y]->color == playingAs)
                        {
                            if (x - 1 > -1 && y - 1 > -1)
                            {
                                if (chessboard[x - 1][y - 1])
                                {
                                    if (chessboard[x - 1][y - 1]->color == color && chessboard[x - 1][y - 1]->type == Type::King) return false;
                                }
                            }
                            if (x + 1 < 8 && y - 1 > -1)
                            {
                                if (chessboard[x + 1][y - 1])
                                {
                                    if (chessboard[x + 1][y - 1]->color == color && chessboard[x + 1][y - 1]->type == Type::King) return false;
                                }
                            }
                        }
                        else
                        {
                            if (x - 1 > -1 && y + 1 < 8)
                            {
                                if (chessboard[x - 1][y + 1])
                                {
                                    if (chessboard[x - 1][y + 1]->color == color && chessboard[x - 1][y + 1]->type == Type::King) return false;
                                }
                            }
                            if (x + 1 < 8 && y + 1 < 8)
                            {
                                if (chessboard[x + 1][y + 1])
                                {
                                    if (chessboard[x + 1][y + 1]->color == color && chessboard[x + 1][y + 1]->type == Type::King) return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

// Checks if (color) has any possible moves available
// If not, it ends the game by switching the (endGame) flag
void MainWindow::CheckEndgame(Color color)
{
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (chessboard[i][j])
            {
                if (chessboard[i][j]->color == color)
                {
                    FindPossibleMoves(i, j);

                    for (int k{ 0 }; k < 8; k++)
                    {
                        for (int l{ 0 }; l < 8; l++)
                        {
                            if (possibleMoves[k][l]) return;
                        }
                    }
                }
            }
        }
    }
    if (color == Color::Black) endGame = 0;
    else endGame = 1;
    turn = Turn::Neither;
}

// Returns the row and column of the board relative to window coordinates
// Returns (-1,-1) if outside the board
sf::Vector2i MainWindow::GetTile(int xPos, int yPos)
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };

    sf::Vector2i tile{ -1,-1 };
    sf::Vector2f offset{};

    if (width > height)
    {
        offset.x = (width - height) / 2.f;
        if (xPos > offset.x && xPos < width - offset.x && yPos > 0 && yPos < height)
        {
            tile.x = static_cast<int>((xPos - offset.x) / (height / 8.f));
            tile.y = static_cast<int>(yPos / (height / 8.f));
        }
    }
    else
    {
        offset.y = (height - width) / 2.f;
        if (yPos > offset.y && yPos < height - offset.y && xPos > 0 && xPos < width)
        {
            tile.x = static_cast<int>(xPos / (width / 8.f));
            tile.y = static_cast<int>((yPos - offset.y) / (width / 8.f));
        }
    }

    return tile;
}

// Returns the currently hovered piece or (-1,-1) if none hovered
// Can save the currently hovered piece as (x,y) according to the (set) flag
sf::Vector2i MainWindow::CurrentlyHoveredTile(int x, int y, bool set)
{
    static sf::Vector2i tile{ -1, -1 };

    if (set)
    {
        Color color{ turn == Turn::White ? Color::White : Color::Black };

        if (tile != sf::Vector2i(-1, -1))
        {
            if (chessboard[tile.x][tile.y])
            {
                chessboard[tile.x][tile.y]->sprite.setColor(sf::Color(255, 255, 255, 255));
            }
        }

        tile.x = x;
        tile.y = y;

        if (tile != sf::Vector2i(-1, -1))
        {
            if (chessboard[x][y])
            {
                if (chessboard[x][y]->color == color)
                {
                    chessboard[x][y]->sprite.setColor(sf::Color(255, 255, 255, 128));
                }
            }
            if (possibleMoves[x][y])
            {
                int width{ static_cast<int>(window.getSize().x) };
                int height{ static_cast<int>(window.getSize().y) };
                float tile_length{ width > height ? height / 8.f : width / 8.f };

                float offsetX{};
                float offsetY{};
                width > height ? offsetX = (width - height) / 2.f : offsetY = (height - width) / 2.f;

                hoveredPossibleMove.setSize(sf::Vector2f(tile_length, tile_length));
                hoveredPossibleMove.setPosition(tile_length * x + offsetX, tile_length * y + offsetY);

                possibleMoveXIndex = x;
                possibleMoveYIndex = y;
            }
        }
    }

    return tile;
}

// Returns the currently held piece or (-1,-1) if none are being held
// Can save the currently held piece as (x,y) according to the (set) flag
sf::Vector2i MainWindow::CurrentlyHeldTile(int x, int y, bool set)
{
    static sf::Vector2i tile{ -1, -1 };

    if (set)
    {
        if (tile != sf::Vector2i(-1, -1))
        {
            float prevLength{ chessboard[tile.x][tile.y]->sprite.getGlobalBounds().width };
            chessboard[tile.x][tile.y]->sprite.scale(1.25f, 1.25f);
            float postLength{ chessboard[tile.x][tile.y]->sprite.getGlobalBounds().width };
            
            float offset{ (prevLength - postLength) / 2.f };
            chessboard[tile.x][tile.y]->sprite.move(offset, offset);
        }

        tile.x = x;
        tile.y = y;

        if (tile != sf::Vector2i(-1, -1))
        {
            float prevLength{ chessboard[x][y]->sprite.getGlobalBounds().width };

            hoveredSquare.setSize(sf::Vector2f(prevLength, prevLength));
            hoveredSquare.setPosition(chessboard[x][y]->sprite.getGlobalBounds().left, chessboard[x][y]->sprite.getGlobalBounds().top);

            chessboard[x][y]->sprite.scale(0.8f, 0.8f);
            float postLength{ chessboard[x][y]->sprite.getGlobalBounds().width };

            float offset{ (prevLength - postLength) / 2.f };
            chessboard[x][y]->sprite.move(offset, offset);
        }
        heldPieceXIndex = x;
        heldPieceYIndex = y;
    }

    return tile;
}

// Returns the currently selected piece or (-1,-1) if none selected
// Can save the currently selected piece as (x,y) according to the (set) flag
// If updated, calls FindPossibleMoves for the newly selected piece
sf::Vector2i MainWindow::CurrentlyClickedTile(int x, int y, bool set)
{
    static sf::Vector2i tile{ -1, -1 };
    if (set)
    {
        tile.x = x;
        tile.y = y;

        for (int i{ 0 }; i < 8; i++)
        {
            for (int j{ 0 }; j < 8; j++)
            {
                possibleMoves[i][j] = false;
            }
        }

        if (x != -1 && y != -1) FindPossibleMoves(x, y);
    }
    return tile;
}

// Utility function to convert various types to string
template <typename T> std::string MainWindow::toStr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}