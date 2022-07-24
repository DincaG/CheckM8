#include "MainWindow.h"

MainWindow::MainWindow(int width, int height, const char* title)
    : networkingThread(&MainWindow::Online, this)
    , mt{ std::random_device{}() }
    , pieceSet{ "Set 1" }
    , themeColor{ 110, 159, 194 }
    , windowColor{ 245, 245, 245 }
    , menuDepth{ 0 }
    , mode{ Mode::Normal }
    , session{ nullptr }
    , sessionUpdated{ false }
    , holdingSliderArrow{ false }
    , fpsCapChanged{ false }
    , paused{ false }
    , hoveredPMSquare{ -1, -1 }
    , heldPieceSquare{ -1, -1 }
{
    window.create(sf::VideoMode(width, height), title);
    SetIcon();
    InitializeTexturesAndResources();
    InitializeUIAndSettings();
    InitializeChessboard(root, White);
    CalculateLayout();
}

void MainWindow::SetIcon()
{
    HICON largeIcon{ (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(102), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED) };
    SetClassLongPtr(window.getSystemHandle(), GCLP_HICON, reinterpret_cast<LONG_PTR>(largeIcon));

    HICON smallIcon{ (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(103), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED) };
    SetClassLongPtr(window.getSystemHandle(), GCLP_HICONSM, reinterpret_cast<LONG_PTR>(smallIcon));
}

void MainWindow::UpdateTextures()
{    
    wPawnTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White Pawn.png");
    wKnightTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White Knight.png");
    wBishopTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White Bishop.png");
    wRookTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White Rook.png");
    wQueenTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White Queen.png");
    wKingTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/White King.png");
    bPawnTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black Pawn.png");
    bKnightTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black Knight.png");
    bBishopTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black Bishop.png");
    bRookTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black Rook.png");
    bQueenTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black Queen.png");
    bKingTexture->loadFromFile(resourcesPath + "/Images/Chess Pieces/" + pieceSet + "/Black King.png");
}

void MainWindow::InitializeTexturesAndResources()
{
    char* relativePath;
    _get_pgmptr(&relativePath);

    constexpr int backslash{ 92 };
    std::string path{ relativePath };
    path.erase(path.find_last_of(backslash));
    path += "/Resources";
    resourcesPath = path;

    pieceSet1Texture = new sf::Texture;
    pieceSet2Texture = new sf::Texture;
    pieceSet3Texture = new sf::Texture;
    board1Texture = new sf::Texture;
    board2Texture = new sf::Texture;
    board3Texture = new sf::Texture;
    arrowTexture = new sf::Texture;
    sliderArrowTexture = new sf::Texture;
    languageTexture = new sf::Texture;
    wPawnTexture = new sf::Texture;
    wKnightTexture = new sf::Texture;
    wBishopTexture = new sf::Texture;
    wRookTexture = new sf::Texture;
    wQueenTexture = new sf::Texture;
    wKingTexture = new sf::Texture;
    bPawnTexture = new sf::Texture;
    bKnightTexture = new sf::Texture;
    bBishopTexture = new sf::Texture;
    bRookTexture = new sf::Texture;
    bQueenTexture = new sf::Texture;
    bKingTexture = new sf::Texture;

    UpdateTextures();
    arrowTexture->loadFromFile(resourcesPath + "/Images/Arrow.png");
    sliderArrowTexture->loadFromFile(resourcesPath + "/Images/Slider Arrow.png");
    languageTexture->loadFromFile(resourcesPath + "/Images/language.png");
    pieceSet1Texture->loadFromFile(resourcesPath + "/Images/Chess Pieces/Set 1/Set1.png");
    pieceSet2Texture->loadFromFile(resourcesPath + "/Images/Chess Pieces/Set 2/Set2.png");
    pieceSet3Texture->loadFromFile(resourcesPath + "/Images/Chess Pieces/Set 3/Set3.png");
    board1Texture->loadFromFile(resourcesPath + "/Images/Board 1.png");
    board2Texture->loadFromFile(resourcesPath + "/Images/Board 2.jpg");
    board3Texture->loadFromFile(resourcesPath + "/Images/Board 3.jpg");

    board1Texture->setRepeated(true);
    board1Texture->setSmooth(true);
    board2Texture->setSmooth(true);
    board3Texture->setSmooth(true);
    pieceSet1Texture->setSmooth(true);
    pieceSet2Texture->setSmooth(true);
    pieceSet3Texture->setSmooth(true);
    arrowTexture->setSmooth(true);
    sliderArrowTexture->setSmooth(true);
    languageTexture->setSmooth(true);
    wPawnTexture->setSmooth(true);
    wKnightTexture->setSmooth(true);
    wBishopTexture->setSmooth(true);
    wRookTexture->setSmooth(true);
    wQueenTexture->setSmooth(true);
    wKingTexture->setSmooth(true);
    bPawnTexture->setSmooth(true);
    bKnightTexture->setSmooth(true);
    bBishopTexture->setSmooth(true);
    bRookTexture->setSmooth(true);
    bQueenTexture->setSmooth(true);
    bKingTexture->setSmooth(true);

    background.setTexture(*board1Texture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 800));
    background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);

    selectedSquareHighlight.setFillColor(themeColor);
    hoveredPMHighlight.setFillColor(themeColor);
    pausedBackground.setFillColor(sf::Color(55, 55, 55, 150));

    Youthome.loadFromFile(resourcesPath + "/Fonts/Youthome.ttf");
    AndikaNewBasic.loadFromFile(resourcesPath + "/Fonts/AndikaNewBasic-R.ttf");

    static sf::SoundBuffer pieceMovedBuffer;
    pieceMovedBuffer.loadFromFile(resourcesPath + "/Audio/Piece Moved.wav");
    pieceMovedSound = new sf::Sound;
    pieceMovedSound->setBuffer(pieceMovedBuffer);

    static sf::SoundBuffer pieceTakenBuffer;
    pieceTakenBuffer.loadFromFile(resourcesPath + "/Audio/Piece Taken.wav");
    pieceTakenSound = new sf::Sound;
    pieceTakenSound->setBuffer(pieceTakenBuffer);

    static sf::SoundBuffer mouseOverBuffer;
    mouseOverBuffer.loadFromFile(resourcesPath + "/Audio/Mouseover.wav");
    mouseOverSound = new sf::Sound;
    mouseOverSound->setBuffer(mouseOverBuffer);

    static sf::SoundBuffer sliderBuffer;
    sliderBuffer.loadFromFile(resourcesPath + "/Audio/Slider.wav");
    sliderSound = new sf::Sound;
    sliderSound->setBuffer(sliderBuffer);
}

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
                    if (audioOn) mouseOverSound->play();
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
                window->exit.hide();
                window->language.hide();
                window->CurrentlyHeldSquare(-1, -1, true);
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
                window->exit.hide();
                window->language.hide();
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
    leftArrow.setTexture(arrowTexture);
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
                    audioOn = true;
                }
                else
                {
                    button->setTextString("Off");
                    audioOn = false;
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
    sliderArrow.setTexture(sliderArrowTexture);
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
                window->exit.hide();
                window->language.hide();
                window->board1.show();
                window->board2.show();
                window->board3.show();
                window->pieceSet1.show();
                window->pieceSet2.show();
                window->pieceSet3.show();
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
                
                window->resume.hide();
                window->settings.hide();
                window->themes.hide();
                window->newGame.hide();
                window->exit.hide();
                window->language.hide();
                window->playerVsPlayer.show();
                window->playerVsComputer.show();
            }
        }
    );

    exit = resume;
    exit.setTextString("Exit");
    exit.setRelativePosition(50.f, 75.f);
    exit.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->mode = MainWindow::Mode::Normal;
                window->networkingThread.wait();
                delete window->session;
                window->session = nullptr;
                for (int i{ 0 }; i < 8; i++)
                {
                    for (int j{ 0 }; j < 8; j++)
                    {
                        delete window->root->chessboard[i][j];
                        window->root->chessboard[i][j] = nullptr;
                    }
                }
                delete window->root;
                DestroyWindow(window->window.getSystemHandle());
            }
        }
    );


    board1.setSize(sf::Vector2f(100.f, 100.f));
    board1.setCornerRadius(10.f);
    board1.setTexture(board1Texture);
    board1.setTextureRect(sf::IntRect(0, 0, 200, 200));
    board1.setFillColor(sf::Color(255, 255, 255, 255));
    board1.setRelativePosition(27.5f, 60.f);
    board1.setParent(background);
    board1.hide();
    board1.setUserData(this);
    board1.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (transition == Button::Transition::In)
            {
                button->setFillColor(sf::Color(255, 255, 255, 255));
            }
            if (transition == Button::Transition::Out)
            {
                if (button->getSize() == sf::Vector2f(80.f, 80.f))
                {
                    button->setFillColor(sf::Color(255, 255, 255, 100));
                }
            }
        }
    );
    board1.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->board1.setSize(sf::Vector2f(80.f, 80.f));
                window->board2.setSize(sf::Vector2f(80.f, 80.f));
                window->board3.setSize(sf::Vector2f(80.f, 80.f));
                window->board1.setFillColor(sf::Color(255, 255, 255, 100));
                window->board2.setFillColor(sf::Color(255, 255, 255, 100));
                window->board3.setFillColor(sf::Color(255, 255, 255, 100));
                button->setSize(sf::Vector2f(100.f, 100.f));
                button->setFillColor(sf::Color(255, 255, 255, 255));

                if (window->board1.getSize() == sf::Vector2f(100.f, 100.f))
                {
                    window->windowColor = { 245, 245, 245 };
                    window->themeColor = { 110, 159, 194 };
                    window->selectedSquareHighlight.setFillColor(window->themeColor);
                    window->hoveredPMHighlight.setFillColor(window->themeColor);
                }
                else if (window->board2.getSize() == sf::Vector2f(100.f, 100.f))
                {
                    window->windowColor = { 138, 92, 59 };
                    window->themeColor = sf::Color(0, 160, 0);
                    window->selectedSquareHighlight.setFillColor(window->themeColor * sf::Color(255, 255, 255, 150));
                    window->hoveredPMHighlight.setFillColor(window->themeColor * sf::Color(255, 255, 255, 150));
                }
                else if (window->board3.getSize() == sf::Vector2f(100.f, 100.f))
                {
                    window->windowColor = { 230, 173, 133 };
                    window->themeColor = sf::Color(0, 160, 0);
                    window->selectedSquareHighlight.setFillColor(window->themeColor * sf::Color(255, 255, 255, 150));
                    window->hoveredPMHighlight.setFillColor(window->themeColor* sf::Color(255, 255, 255, 150));
                }

                window->resume.setTextOutlineColor(window->themeColor);
                window->settings.setTextOutlineColor(window->themeColor);
                window->fullscreen.setTextOutlineColor(window->themeColor);
                window->fullscreenOption.setTextOutlineColor(window->themeColor);
                window->vSync.setTextOutlineColor(window->themeColor);
                window->vSyncOption.setTextOutlineColor(window->themeColor);
                window->fpsCap.setTextOutlineColor(window->themeColor);
                window->fpsValue.setTextOutlineColor(window->themeColor);
                window->leftArrow.setFillColor(window->themeColor);
                window->rightArrow.setFillColor(window->themeColor);
                window->audio.setTextOutlineColor(window->themeColor);
                window->audioOption.setTextOutlineColor(window->themeColor);
                window->pieceSpeed.setTextOutlineColor(window->themeColor);
                window->slider.setOutlineColor(window->themeColor);
                window->sliderArrow.setFillColor(window->themeColor);
                window->pieceSpeedValue.setTextOutlineColor(window->themeColor);
                window->themes.setTextOutlineColor(window->themeColor);
                window->newGame.setTextOutlineColor(window->themeColor);
                window->playerVsPlayer.setTextOutlineColor(window->themeColor);
                window->playerVsComputer.setTextOutlineColor(window->themeColor);
                window->online.setTextOutlineColor(window->themeColor);
                window->offline.setTextOutlineColor(window->themeColor);
                window->chooseColor.setTextOutlineColor(window->themeColor);
                window->createGame.setTextOutlineColor(window->themeColor);
                window->joinGame.setTextOutlineColor(window->themeColor);
                window->gameId.setTextOutlineColor(window->themeColor);
                window->id.setTextOutlineColor(window->themeColor);
                window->enterGameId.setTextOutlineColor(window->themeColor);
                window->inputId.setTextOutlineColor(window->themeColor);
                window->errorMessage.setTextOutlineColor(window->themeColor);
                window->exit.setTextOutlineColor(window->themeColor);
                window->language.setFillColor(window->themeColor);
                window->english.setTextOutlineColor(window->themeColor);
                window->romanian.setTextOutlineColor(window->themeColor);

                window->background.setTexture(*button->getTexture());
            }
        }
    );

    board2 = board1;
    board2.setSize(sf::Vector2f(80.f, 80.f));
    board2.setTexture(board2Texture);
    board2.setTextureRect(sf::IntRect(0, 0, 200, 200));
    board2.setFillColor(sf::Color(255, 255, 255, 100));
    board2.setRelativePosition(40.f, 40.f);

    board3 = board2;
    board3.setTexture(board3Texture);
    board3.setTextureRect(sf::IntRect(0, 0, 200, 200));
    board3.setRelativePosition(15.f, 40.f);

    pieceSet1 = board2;
    pieceSet1.setSize(sf::Vector2f(100.f, 100.f));
    pieceSet1.setTexture(pieceSet1Texture);
    pieceSet1.setTextureRect(sf::IntRect(0, 0, 400, 400));
    pieceSet1.setFillColor(sf::Color(255, 255, 255, 255));
    pieceSet1.setRelativePosition(60.f, 60.f);
    pieceSet1.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->pieceSet1.setSize(sf::Vector2f(80.f, 80.f));
                window->pieceSet2.setSize(sf::Vector2f(80.f, 80.f));
                window->pieceSet3.setSize(sf::Vector2f(80.f, 80.f));
                window->pieceSet1.setFillColor(sf::Color(255, 255, 255, 100));
                window->pieceSet2.setFillColor(sf::Color(255, 255, 255, 100));
                window->pieceSet3.setFillColor(sf::Color(255, 255, 255, 100));
                button->setSize(sf::Vector2f(100.f, 100.f));
                button->setFillColor(sf::Color(255, 255, 255, 255));

                if (window->pieceSet1.getSize() == sf::Vector2f(100.f, 100.f)) window->pieceSet = "Set 1";
                else if (window->pieceSet2.getSize() == sf::Vector2f(100.f, 100.f)) window->pieceSet = "Set 2";
                else if (window->pieceSet3.getSize() == sf::Vector2f(100.f, 100.f)) window->pieceSet = "Set 3";

                window->UpdateTextures();
            }
        }
    );

    pieceSet2 = pieceSet1;
    pieceSet2.setSize(sf::Vector2f(80.f, 80.f));
    pieceSet2.setTexture(pieceSet2Texture);
    pieceSet2.setTextureRect(sf::IntRect(0, 0, 400, 400));
    pieceSet2.setFillColor(sf::Color(255, 255, 255, 100));
    pieceSet2.setRelativePosition(85.f, 60.f);

    pieceSet3 = pieceSet2;
    pieceSet3.setTexture(pieceSet3Texture);
    pieceSet3.setTextureRect(sf::IntRect(0, 0, 400, 400));
    pieceSet3.setRelativePosition(72.5f, 40.f);

    playerVsPlayer = resume;
    playerVsPlayer.setSize(sf::Vector2f(600.f, 60.f));
    playerVsPlayer.setTextString("Player Vs Player");
    playerVsPlayer.setRelativePosition(50.f, 40.f);
    playerVsPlayer.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->playerVsPlayer.hide();
                window->playerVsComputer.hide();
                window->online.show();
                window->offline.show();
            }
        }
    );

    playerVsComputer = playerVsPlayer;
    playerVsComputer.setTextString("Player Vs Computer");
    playerVsComputer.setRelativePosition(50.f, 60.f);
    playerVsComputer.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->playerVsPlayer.hide();
                window->playerVsComputer.hide();
                window->chooseColor.show();
                window->playAsWhite.show();
                window->playAsBlack.show();
                window->menuDepth = 1;
            }
        }
    );

    online = resume;
    online.setTextString("Online");
    online.setRelativePosition(50.f, 40.f);
    online.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->online.hide();
                window->offline.hide();
                window->createGame.show();
                window->joinGame.show();
            }
        }
    );
    
    offline = resume;
    offline.setTextString("Offline");
    offline.setRelativePosition(50.f, 60.f);
    offline.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->online.hide();
                window->offline.hide();
                window->chooseColor.show();
                window->playAsWhite.show();
                window->playAsBlack.show();
                window->menuDepth = 2;
            }
        }
    );

    createGame = resume;
    createGame.setTextString("Create a game");
    createGame.setRelativePosition(50.f, 40.f);
    createGame.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->createGame.hide();
                window->joinGame.hide();
                window->chooseColor.show();
                window->playAsWhite.show();
                window->playAsBlack.show();
                window->menuDepth = 3;
            }
        }
    );

    joinGame = resume;
    joinGame.setTextString("Join a game");
    joinGame.setRelativePosition(50.f, 60.f);
    joinGame.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };
                window->createGame.hide();
                window->joinGame.hide();
                window->enterGameId.show();
                window->inputId.show();
                window->errorMessage.show();
            }
        }
    );

    playAsWhite = board2;
    playAsWhite.setSize(sf::Vector2f(150.f, 150.f));
    playAsWhite.setFillColor(sf::Color(255, 255, 255, 150));
    playAsWhite.setTexture(wKingTexture);
    playAsWhite.setTextureRect(sf::IntRect(0, 0, 200, 200));
    playAsWhite.setRelativePosition(35.f, 60.f);
    playAsWhite.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (transition == Button::Transition::In) button->setFillColor(sf::Color(255, 255, 255, 255));
            else if (transition == Button::Transition::Out) button->setFillColor(sf::Color(255, 255, 255, 100));
        }
    );
    playAsWhite.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };

                if (button->getTexture() == wKingTexture) InitializeChessboard(window->root, White);
                else InitializeChessboard(window->root, Black);
                
                window->CurrentlyHoveredSquare(-1, -1, true);
                window->CurrentlyHeldSquare(-1, -1, true);
                window->CurrentlySelectedSquare(-1, -1, true);
                window->CalculateLayout();

                window->mode = Mode::Normal;
                window->networkingThread.wait();
                delete window->session;
                window->session = nullptr;

                window->chooseColor.hide();
                window->playAsWhite.hide();
                window->playAsBlack.hide();
                window->paused = false;

                if (window->menuDepth == 1) window->mode = Mode::AI;
                else if (window->menuDepth == 3)
                {
                    std::string sessionId{};
                    std::string chars { "abcdefghijklmnopqrstuvwxyz0123456789" };
                    std::uniform_int_distribution randChar{ 0, 35 };
                    for (int i{ 0 }; i < 8; i++) sessionId.push_back(chars[randChar(window->mt)]);
                    window->id.setTextString(sessionId);
                    window->session = new GameInstance(sessionId, std::string(sessionId.rbegin(), sessionId.rend()));

                    static sf::Thread* thread{ nullptr };
                    delete thread;
                    thread = new sf::Thread
                    (
                        [window]()
                        {
                            window->session->load();
                            JSON::json data;
                            data["Playing as"] = window->root->orientation == White ? "White" : "Black";
                            window->session->save(data.dump());
                            if (window->root->orientation == Black) window->sessionUpdated = true;
                            else window->sessionUpdated = false;
                            window->networkingThread.launch();
                        }
                    );
                    thread->launch();

                    window->mode = Mode::Online;
                    window->gameId.show();
                    window->id.show();
                    window->paused = true;
                }
            }
        }
    );

    playAsBlack = playAsWhite;
    playAsBlack.setTexture(bKingTexture);
    playAsBlack.setTextureRect(sf::IntRect(0, 0, 200, 200));
    playAsBlack.setRelativePosition(65.f, 60.f);

    chooseColor = fullscreen;
    chooseColor.setSize(sf::Vector2f(600.f, 90.f));
    chooseColor.setTextOutlineThickness(4);
    chooseColor.setTextString("Choose Color");
    chooseColor.setRelativePosition(50.f, 30.f);

    gameId = chooseColor;
    gameId.setTextString("Game ID");

    id = fullscreen;
    id.setSize(sf::Vector2f(400.f, 70.f));
    id.setTextString("");
    id.setRelativePosition(50.f, 55.f);

    enterGameId = chooseColor;
    enterGameId.setTextString("Enter Game ID");
    enterGameId.setRelativePosition(50.f, 25.f);

    inputId = fullscreen;
    inputId.setSize(sf::Vector2f(400.f, 80.f));
    inputId.setCornerRadius(20.f);
    inputId.setTextSize(70.f);
    inputId.setFillColor(sf::Color(0, 0, 0, 120));
    inputId.setOutlineColor(sf::Color(0, 0, 0, 60));
    inputId.setOutlineThickness(6);
    inputId.setTextString("");
    inputId.setRelativePosition(50.f, 50.f);

    errorMessage = fullscreen;
    errorMessage.setSize(sf::Vector2f(600.f, 40.f));
    errorMessage.setTextString("");
    errorMessage.setRelativePosition(50.f, 75.f);

    language = board1;
    language.setSize(sf::Vector2f(80.f, 80.f));
    language.setTexture(languageTexture);
    language.setTextureRect(sf::IntRect(0, 0, 200, 200));
    language.setFillColor(themeColor);
    language.setRelativePosition(90.f, 10.f);
    language.setHoverBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData)
            {
                MainWindow* window{ (MainWindow*)userData };
                if (transition == Button::Transition::In)
                {
                    button->setFillColor(sf::Color(255, 255, 255, 150) * window->themeColor);
                }
                if (transition == Button::Transition::Out)
                {
                    button->setFillColor(window->themeColor);
                }
            }
        }
    );
    language.setPressBehavior
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
                window->exit.hide();
                window->language.hide();
                window->english.show();
                window->romanian.show();
            }
        }
    );

    english = resume;
    english.setTextString("English");
    english.setRelativePosition(50.f, 40.f);
    english.setTextOutlineThickness(4.f);
    english.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };

                window->resume.setTextString("Resume");
                window->settings.setTextString("Settings");
                window->pieceSpeed.setTextString("Piece Speed");
                window->themes.setTextString("Themes");
                window->newGame.setTextString("New Game");
                window->playerVsPlayer.setTextString("Player Vs Player");
                window->playerVsComputer.setTextString("Player Vs Computer");
                window->createGame.setTextString("Create a game");
                window->joinGame.setTextString("Join a game");
                window->chooseColor.setTextString("Choose Color");
                window->gameId.setTextString("Game ID");
                window->enterGameId.setTextString("Enter Game ID");
                window->enterGameId.setSize(sf::Vector2f(600.f, 90.f));
                window->exit.setTextString("Exit");
                window->english.setTextOutlineThickness(4.f);
                window->romanian.setTextOutlineThickness(2.f);

                window->english.hide();
                window->romanian.hide();
                window->resume.show();
                window->settings.show();
                window->themes.show();
                window->newGame.show();
                window->exit.show();
                window->language.show();
                window->language.onHover(0, 0);
            }
        }
    );

    romanian = resume;
    romanian.setTextString(L"Română");
    romanian.setRelativePosition(50.f, 60.f);
    romanian.setPressBehavior
    (
        [](Button* button, Button::Transition transition, void* userData)
        {
            if (userData && transition == Button::Transition::In)
            {
                MainWindow* window{ (MainWindow*)userData };

                window->resume.setTextString(L"Înapoi");
                window->settings.setTextString(L"Setări");
                window->pieceSpeed.setTextString("Viteza Piesei");
                window->themes.setTextString("Tematici");
                window->newGame.setTextString("Joc Nou");
                window->playerVsPlayer.setTextString(L"Jucător Vs Jucător");
                window->playerVsComputer.setTextString(L"Jucător Vs Calculator");
                window->createGame.setTextString(L"Creează un joc");
                window->joinGame.setTextString(L"Intră într-un joc");
                window->chooseColor.setTextString("Alege Culoarea");
                window->gameId.setTextString("Joc ID");
                window->enterGameId.setTextString("Introdu ID-ul jocului");
                window->enterGameId.setSize(sf::Vector2f(600.f, 80.f));
                window->exit.setTextString(L"Ieşire");
                window->english.setTextOutlineThickness(2.f);
                window->romanian.setTextOutlineThickness(4.f);

                window->english.hide();
                window->romanian.hide();
                window->resume.show();
                window->settings.show();
                window->themes.show();
                window->newGame.show();
                window->exit.show();
                window->language.show();
                window->language.onHover(0, 0);
            }
        }
    );
}

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
            if (root->chessboard[i][j])
            {
                root->chessboard[i][j]->sprite.setPosition((length / 8.f) * i + offsetX, (length / 8.f) * j + offsetY);
                root->chessboard[i][j]->sprite.setScale(length / 1600.f, length / 1600.f);
            }
        }
    }
    if (heldPieceSquare != sf::Vector2i(-1, -1))
    {
        if (root->chessboard[heldPieceSquare.x][heldPieceSquare.y])
        {
            root->chessboard[heldPieceSquare.x][heldPieceSquare.y]->sprite.scale(0.8f, 0.8f);
            float postLength{ root->chessboard[heldPieceSquare.x][heldPieceSquare.y]->sprite.getGlobalBounds().width };

            float offset{ (length / 8.f - postLength) / 2.f };
            root->chessboard[heldPieceSquare.x][heldPieceSquare.y]->sprite.move(offset, offset);
        }
    }

    hoveredPMHighlight.setSize(sf::Vector2f(length / 8.f, length / 8.f));
    hoveredPMHighlight.setPosition((length / 8.f) * hoveredPMSquare.x + offsetX, (length / 8.f) * hoveredPMSquare.y + offsetY);

    selectedSquareHighlight.setSize(sf::Vector2f(length / 8.f, length / 8.f));
    selectedSquareHighlight.setPosition((length / 8.f) * hoveredPMSquare.x + offsetX, (length / 8.f) * hoveredPMSquare.y + offsetY);

    pausedBackground.setSize(sf::Vector2f((float)width, (float)height));
    background.setScale(length / 800.f, length / 800.f);
    background.setPosition(width / 2.f, height / 2.f);
}

void MainWindow::GameLoop()
{
    lock.lock();
    if (!movingPieces.empty()) MoveTransition();
    if (fpsCapChanged) FpsTransition();
    if (mode == Mode::AI && root->turn != root->orientation) AI(root);
    UpdateFrameRate();

    Paint();
    lock.unlock();
}

void MainWindow::Paint()
{
    window.clear(windowColor);
    window.draw(background);

    if (CurrentlySelectedSquare(-1, -1, false) != sf::Vector2i(-1, -1)) ShowPossibleMoves();
    if (CurrentlyHeldSquare(-1, -1, false) != sf::Vector2i(-1, -1)) window.draw(selectedSquareHighlight);
    if (CurrentlyHoveredSquare(-1, -1, false) != sf::Vector2i(-1, -1))
    {
        if (std::find(possibleMoves.begin(), possibleMoves.end(), CurrentlyHoveredSquare(-1, -1, false)) != possibleMoves.end())
        {
            window.draw(hoveredPMHighlight);
        }
    }
    for (int i{ 0 }; i < 8; i++)
    {
        for (int j{ 0 }; j < 8; j++)
        {
            if (root->chessboard[i][j])
            {
                window.draw(root->chessboard[i][j]->sprite);
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

        if (endGame == 0)
        {
            if (english.getTextOutlineThickness() == 4.f) text.setString("White Wins");
            else
            {
                text.setCharacterSize(window.getSize().x > window.getSize().y ? window.getSize().y / 7 : window.getSize().x / 7);
                text.setString("Alb Castiga");
            }
        }
        else if (endGame == 1)
        {
            if (english.getTextOutlineThickness() == 4.f) text.setString("Black Wins");
            else
            {
                text.setCharacterSize(window.getSize().x > window.getSize().y ? window.getSize().y / 7 : window.getSize().x / 7);
                text.setString("Negru Castiga");
            }
        }
        else if (endGame == 2)
        {
            if (english.getTextOutlineThickness() == 4.f) text.setString(std::string(3, ' ') + "Draw\nStalemate");
            else text.setString("Egalitate\nStalemate");
        }
        else
        {
            text.setCharacterSize(window.getSize().x > window.getSize().y ? window.getSize().y / 10 : window.getSize().x / 10);
            if (english.getTextOutlineThickness() == 4.f) text.setString(std::string(11, ' ') + "Draw\nInsufficient Material");
            else text.setString(std::string(9, ' ') + "Egalitate\nMaterial Insuficient");
        }

        text.setPosition((window.getSize().x - text.getGlobalBounds().width) / 2.f, (window.getSize().y - text.getGlobalBounds().height) / 2.f - text.getLocalBounds().top);
        window.draw(text);
    }

    if (paused) window.draw(pausedBackground);
    if (resume.isVisible()) window.draw(resume);
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
    if (settings.isVisible()) window.draw(settings);

    if (board1.isVisible()) window.draw(board1);
    if (board2.isVisible()) window.draw(board2);
    if (board3.isVisible()) window.draw(board3);
    if (pieceSet1.isVisible()) window.draw(pieceSet1);
    if (pieceSet2.isVisible()) window.draw(pieceSet2);
    if (pieceSet3.isVisible()) window.draw(pieceSet3);
    if (themes.isVisible()) window.draw(themes);

    if (errorMessage.isVisible()) window.draw(errorMessage);
    if (inputId.isVisible()) window.draw(inputId);
    if (enterGameId.isVisible()) window.draw(enterGameId);
    if (gameId.isVisible()) window.draw(gameId);
    if (id.isVisible()) window.draw(id);
    if (playAsWhite.isVisible()) window.draw(playAsWhite);
    if (playAsBlack.isVisible()) window.draw(playAsBlack);
    if (createGame.isVisible()) window.draw(createGame);
    if (joinGame.isVisible()) window.draw(joinGame);
    if (online.isVisible()) window.draw(online);
    if (chooseColor.isVisible()) window.draw(chooseColor);
    if (offline.isVisible()) window.draw(offline);
    if (playerVsPlayer.isVisible()) window.draw(playerVsPlayer);
    if (playerVsComputer.isVisible()) window.draw(playerVsComputer);
    if (newGame.isVisible()) window.draw(newGame);

    if (exit.isVisible()) window.draw(exit);
    if (english.isVisible()) window.draw(english);
    if (romanian.isVisible()) window.draw(romanian);
    if (language.isVisible()) window.draw(language);

    window.display();
}

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

void MainWindow::MoveTransition()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };
    float squareLength{ width > height ? height / 8.f : width / 8.f };

    float diffX{};
    float diffY{};
    width > height ? diffX = (width - height) / 2.f : diffY = (height - width) / 2.f;

    for (MovingPiece movingPiece : movingPieces)
    {
        float distanceX{ ((squareLength * movingPiece.move.after.x + diffX) - (squareLength * movingPiece.move.before.x + diffX)) };
        float distanceY{ ((squareLength * movingPiece.move.after.y + diffY) - (squareLength * movingPiece.move.before.y + diffY)) };

        float xSpeed{};
        float ySpeed{};

        if (pieceMovementSpeed != 0)
        {
            xSpeed = distanceX / (pieceMovementSpeed * averageFrameRate);
            ySpeed = distanceY / (pieceMovementSpeed * averageFrameRate);
        }

        float diffPosX{ (squareLength * movingPiece.move.after.x + diffX) - (movingPiece.piece->sprite.getPosition().x) };
        float diffPosY{ (squareLength * movingPiece.move.after.y + diffY) - (movingPiece.piece->sprite.getPosition().y) };

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
            root->turn = movingPiece.piece->color == White ? Black : White;
            endGame = CheckEndgame(root);
            movingPieces.clear();

            SendMessage(window.getSystemHandle(), WM_MOUSEMOVE, NULL, MAKELPARAM(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
            return;
        }
    }
}

void MainWindow::SwitchScreenMode()
{
    HWND hwnd = window.getSystemHandle();
    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

    if (dwStyle & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(hwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
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
        SetWindowPlacement(hwnd, &wpPrev);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }

    CalculateLayout();
}

void MainWindow::OnMouseMove(int xPos, int yPos)
{
    lock.lock();
    if (resume.isVisible()) resume.onHover(xPos, yPos);
    if (fullscreenOption.isVisible()) fullscreenOption.onHover(xPos, yPos);
    if (vSyncOption.isVisible()) vSyncOption.onHover(xPos, yPos);
    if (leftArrow.isVisible()) leftArrow.onHover(xPos, yPos);
    if (rightArrow.isVisible()) rightArrow.onHover(xPos, yPos);
    if (audioOption.isVisible()) audioOption.onHover(xPos, yPos);
    if (sliderArrow.isVisible()) sliderArrow.onHover(xPos, yPos);
    if (settings.isVisible()) settings.onHover(xPos, yPos);
    if (board1.isVisible()) board1.onHover(xPos, yPos);
    if (board2.isVisible()) board2.onHover(xPos, yPos);
    if (board3.isVisible()) board3.onHover(xPos, yPos);
    if (pieceSet1.isVisible()) pieceSet1.onHover(xPos, yPos);
    if (pieceSet2.isVisible()) pieceSet2.onHover(xPos, yPos);
    if (pieceSet3.isVisible()) pieceSet3.onHover(xPos, yPos);
    if (themes.isVisible()) themes.onHover(xPos, yPos);
    if (playAsWhite.isVisible()) playAsWhite.onHover(xPos, yPos);
    if (playAsBlack.isVisible()) playAsBlack.onHover(xPos, yPos);
    if (createGame.isVisible()) createGame.onHover(xPos, yPos);
    if (joinGame.isVisible()) joinGame.onHover(xPos, yPos);
    if (online.isVisible()) online.onHover(xPos, yPos);
    if (offline.isVisible()) offline.onHover(xPos, yPos);
    if (playerVsPlayer.isVisible()) playerVsPlayer.onHover(xPos, yPos);
    if (playerVsComputer.isVisible()) playerVsComputer.onHover(xPos, yPos);
    if (newGame.isVisible()) newGame.onHover(xPos, yPos);
    if (exit.isVisible()) exit.onHover(xPos, yPos);
    if (english.isVisible()) english.onHover(xPos, yPos);
    if (romanian.isVisible()) romanian.onHover(xPos, yPos);
    if (language.isVisible()) language.onHover(xPos, yPos);
    lock.unlock();

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

        if (prevSliderIndex != sliderIndex && audioOn) sliderSound->play();
        prevSliderIndex = sliderIndex;

        pieceMovementSpeed = sliderIndex / 20.f;
        pieceSpeedValue.setTextString(std::to_string(pieceMovementSpeed).substr(0, 4).append("s"));
    }

    if (endGame == -1 && !paused && movingPieces.empty())
    {
        if ((mode == Mode::Normal || (root->turn == root->orientation && (mode == Mode::AI || mode == Mode::Online && !sessionUpdated))))
        {
            sf::Vector2i square{ GetSquare(xPos, yPos) };
            CurrentlyHoveredSquare(square.x, square.y, true);
        }
    }
}

void MainWindow::OnMouseDown(int xPos, int yPos)
{
    lock.lock();
    if (resume.isVisible())             { resume.onPress(); resume.onRelease(); }
    if (fullscreenOption.isVisible())   { fullscreenOption.onPress(); fullscreenOption.onRelease(); }
    if (vSyncOption.isVisible())        { vSyncOption.onPress(); vSyncOption.onRelease(); }
    if (leftArrow.isVisible())          { leftArrow.onPress(); leftArrow.onRelease(); }
    if (rightArrow.isVisible())         { rightArrow.onPress(); rightArrow.onRelease(); }
    if (audioOption.isVisible())        { audioOption.onPress(); audioOption.onRelease(); }
    if (sliderArrow.isVisible())        { sliderArrow.onPress(); }
    if (settings.isVisible())           { settings.onPress(); settings.onRelease(); }
    if (board1.isVisible())             { board1.onPress(); board1.onRelease(); }
    if (board2.isVisible())             { board2.onPress(); board2.onRelease(); }
    if (board3.isVisible())             { board3.onPress(); board3.onRelease(); }
    if (pieceSet1.isVisible())          { pieceSet1.onPress(); pieceSet1.onRelease(); }
    if (pieceSet2.isVisible())          { pieceSet2.onPress(); pieceSet2.onRelease(); }
    if (pieceSet3.isVisible())          { pieceSet3.onPress(); pieceSet3.onRelease(); }
    if (themes.isVisible())             { themes.onPress(); themes.onRelease(); }
    if (playAsWhite.isVisible())        { playAsWhite.onPress(); playAsWhite.onRelease(); }
    if (playAsBlack.isVisible())        { playAsBlack.onPress(); playAsBlack.onRelease(); }
    if (createGame.isVisible())         { createGame.onPress(); createGame.onRelease(); }
    if (joinGame.isVisible())           { joinGame.onPress(); joinGame.onRelease(); }
    if (online.isVisible())             { online.onPress(); online.onRelease(); }
    if (offline.isVisible())            { offline.onPress(); offline.onRelease(); }
    if (playerVsPlayer.isVisible())     { playerVsPlayer.onPress(); playerVsPlayer.onRelease(); }
    if (playerVsComputer.isVisible())   { playerVsComputer.onPress(); playerVsComputer.onRelease(); }
    if (newGame.isVisible())            { newGame.onPress(); newGame.onRelease(); }
    if (exit.isVisible())               { exit.onPress(); newGame.onRelease(); }
    if (english.isVisible())            { english.onPress(); english.onRelease(); }
    if (romanian.isVisible())           { romanian.onPress(); romanian.onRelease(); }
    if (language.isVisible())           { language.onPress(); language.onRelease(); }
    lock.unlock();

    if (endGame == -1 && !paused && movingPieces.empty())
    {
        if ((mode == Mode::Normal || (root->turn == root->orientation && (mode == Mode::AI || mode == Mode::Online && !sessionUpdated))))
        {
            sf::Vector2i square{ GetSquare(xPos, yPos) };

            if (square != sf::Vector2i(-1, -1))
            {
                if (root->chessboard[square.x][square.y])
                {
                    if (root->chessboard[square.x][square.y]->color == root->turn)
                    {
                        CurrentlyHeldSquare(square.x, square.y, true);
                    }
                }
                if (std::find(possibleMoves.begin(), possibleMoves.end(), square) != possibleMoves.end())
                {
                    sf::Vector2i selectedSquare{ CurrentlySelectedSquare(-1, -1, false) };
                    MakeMove(root, Move(selectedSquare, square), true);

                    if (mode == Mode::Online)
                    {
                        JSON::json data{ JSON::json::parse(session->view()) };
                        JSON::json move;
                        move["Turn"] = root->turn == White ? "White" : "Black";
                        move["Before"]["x"] = selectedSquare.x;
                        move["Before"]["y"] = selectedSquare.y;
                        move["After"]["x"] = square.x;
                        move["After"]["y"] = square.y;
                        data["Moves"].push_back(move);

                        static sf::Thread* thread{ nullptr };
                        delete thread;
                        thread = new sf::Thread
                        (
                            [=]()
                            {
                                session->save(data.dump());
                                sessionUpdated = true;
                            }
                        );
                        thread->launch();
                    }
                    CurrentlySelectedSquare(-1, -1, true);
                }
            }
        }
    }
}

void MainWindow::OnMouseUp(int xPos, int yPos)
{
    if (!paused)
    {
        sf::Vector2i square{ GetSquare(xPos, yPos) };
        sf::Vector2i heldSquare{ CurrentlyHeldSquare(-1, -1, false) };

        sf::Vector2i selectedSquare{ CurrentlySelectedSquare(-1, -1, false) };
        CurrentlySelectedSquare(-1, -1, true);

        if (square != sf::Vector2i(-1, -1))
        {
            if (heldSquare == square)
            {
                if (selectedSquare != square)
                {
                    CurrentlySelectedSquare(square.x, square.y, true);
                }
            }
        }

        CurrentlyHeldSquare(-1, -1, true);
    }

    if (sliderArrow.isVisible()) sliderArrow.onRelease();
}

void MainWindow::OnEscapeDown()
{
    if (!paused)
    {
        paused = true;
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exit.show();
        language.show();
    }
    else if (english.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exit.show();
        language.show();
        english.hide();
        romanian.hide();
    }
    else if (fullscreen.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exit.show();
        language.show();
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
    else if (board1.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exit.show();
        language.show();
        board1.hide();
        board2.hide();
        board3.hide();
        pieceSet1.hide();
        pieceSet2.hide();
        pieceSet3.hide();
    }
    else if (gameId.isVisible())
    {
        gameId.hide();
        id.hide();
        chooseColor.show();
        playAsBlack.show();
        playAsWhite.show();
    }
    else if (enterGameId.isVisible())
    {
        enterGameId.hide();
        inputId.hide();
        inputId.setTextString("");
        errorMessage.hide();
        errorMessage.setTextString("");
        createGame.show();
        joinGame.show();
    }
    else if (chooseColor.isVisible() && menuDepth == 3)
    {
        createGame.show();
        joinGame.show();
        chooseColor.hide();
        playAsBlack.hide();
        playAsWhite.hide();
    }
    else if (createGame.isVisible())
    {
        online.show();
        offline.show();
        createGame.hide();
        joinGame.hide();
    }
    else if (chooseColor.isVisible() && menuDepth == 2)
    {
        online.show();
        offline.show();
        chooseColor.hide();
        playAsBlack.hide();
        playAsWhite.hide();
    }
    else if (online.isVisible())
    {
        playerVsPlayer.show();
        playerVsComputer.show();
        online.hide();
        offline.hide();
    }
    else if (chooseColor.isVisible() && menuDepth == 1)
    {
        playerVsPlayer.show();
        playerVsComputer.show();
        chooseColor.hide();
        playAsBlack.hide();
        playAsWhite.hide();
    }
    else if (playerVsPlayer.isVisible())
    {
        resume.show();
        settings.show();
        themes.show();
        newGame.show();
        exit.show();
        language.show();
        playerVsPlayer.hide();
        playerVsComputer.hide();
    }
    else
    {
        paused = false;
        resume.hide();
        settings.hide();
        themes.hide();
        newGame.hide();
        exit.hide();
        language.hide();
        CurrentlyHeldSquare(-1, -1, true);
    }

    SendMessage(window.getSystemHandle(), WM_MOUSEMOVE, NULL, MAKELPARAM(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
}

void MainWindow::TextEntered(unsigned int character)
{
    if (character < 128 && inputId.isVisible())
    {
        std::string id{ inputId.getTextString() };
        if (character == '\b')
        {
            if (id.size() == 0) return;
            id.pop_back();
            inputId.setTextString(id);
            errorMessage.setTextString("");
        }
        else if (character == '\r')
        {
            if (id.size() < 8)
            {
                if (english.getTextOutlineThickness() == 4.f) errorMessage.setTextString("The ID must be 8 characters long");
                else errorMessage.setTextString(L"ID-ul trebuie să aibe 8 caractere");
            }
            else
            {
                mode = Mode::Normal;
                networkingThread.wait();
                delete session;

                session = new GameInstance(id, std::string(id.rbegin(), id.rend()));
                if (session->load())
                {
                    JSON::json data{ JSON::json::parse(session->view(), nullptr, false) };
                    if (data.is_discarded())
                    {
                        if (english.getTextOutlineThickness() == 4.f) errorMessage.setTextString("Couldn't find match with the given ID");
                        else errorMessage.setTextString(L"Nu s-a gasit joc cu ID-ul respectiv");
                        delete session;
                        session = nullptr;
                    }
                    else if (data["Playing as"].empty())
                    {
                        if (english.getTextOutlineThickness() == 4.f) errorMessage.setTextString("Couldn't find match with the given ID");
                        else errorMessage.setTextString(L"Nu s-a găsit joc cu ID-ul respectiv");
                        delete session;
                        session = nullptr;
                    }
                    else
                    {
                        if (data["Playing as"] == "White")
                        {
                            InitializeChessboard(root, Black);
                            sessionUpdated = true;
                        }
                        else
                        {
                            InitializeChessboard(root, White);
                            sessionUpdated = false;
                        }

                        CurrentlyHoveredSquare(-1, -1, true);
                        CurrentlyHeldSquare(-1, -1, true);
                        CurrentlySelectedSquare(-1, -1, true);
                        CalculateLayout();

                        enterGameId.hide();
                        inputId.hide();
                        errorMessage.hide();

                        paused = false;
                        mode = Mode::Online;
                        networkingThread.launch();
                    }
                }
                else
                {
                    if (english.getTextOutlineThickness() == 4.f) errorMessage.setTextString("Couldn't find match with the given ID");
                    else errorMessage.setTextString(L"Nu s-a gasit joc cu ID-ul respectiv");
                    delete session;
                    session = nullptr;
                }
            }
        }
        else if (id.size() < 8)
        {
            inputId.setTextString(id + static_cast<char>(character));
            errorMessage.setTextString("");
        }
    }
}

void MainWindow::ShowPossibleMoves()
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };
    float tileLength{ width > height ? height / 8.f : width / 8.f };

    float offsetX{};
    float offsetY{};
    width > height ? offsetX = (width - height) / 2.f : offsetY = (height - width) / 2.f;

    sf::CircleShape circle(tileLength / 8);
    sf::ConvexShape triangle(3);
    if (board1.getSize() == sf::Vector2f(100.f, 100.f))
    {
        circle.setFillColor(sf::Color(80, 80, 80));
        triangle.setFillColor(themeColor);
    }
    else if (board2.getSize() == sf::Vector2f(100.f, 100.f))
    {
        circle.setFillColor(sf::Color(200, 200, 200) * themeColor);
        triangle.setFillColor(themeColor);
    }
    else if (board3.getSize() == sf::Vector2f(100.f, 100.f))
    {
        circle.setFillColor(sf::Color(150, 150, 150) * themeColor);
        triangle.setFillColor(sf::Color(150, 150, 150) * themeColor);
    }

    for (sf::Vector2i move : possibleMoves)
    {
        if (root->chessboard[move.x][move.y])
        {
            float sOffset{ 0.3f }; // small offset
            float bOffset{ 0.7f }; // big offset

            triangle.setPoint(0, sf::Vector2f(tileLength * move.x + offsetX, tileLength * move.y + tileLength * sOffset + offsetY));
            triangle.setPoint(1, sf::Vector2f(tileLength * move.x + offsetX, tileLength * move.y + offsetY));
            triangle.setPoint(2, sf::Vector2f(tileLength * move.x + tileLength * sOffset + offsetX, tileLength * move.y + offsetY));
            window.draw(triangle);

            triangle.setPoint(0, sf::Vector2f(tileLength * move.x + tileLength * bOffset + offsetX, tileLength * move.y + offsetY));
            triangle.setPoint(1, sf::Vector2f(tileLength * (move.x + 1) + offsetX, tileLength * move.y + offsetY));
            triangle.setPoint(2, sf::Vector2f(tileLength * (move.x + 1) + offsetX, tileLength * move.y + tileLength * sOffset + offsetY));
            window.draw(triangle);

            triangle.setPoint(0, sf::Vector2f(tileLength * (move.x + 1) + offsetX, tileLength * move.y + tileLength * bOffset + offsetY));
            triangle.setPoint(1, sf::Vector2f(tileLength * (move.x + 1) + offsetX, tileLength * (move.y + 1) + offsetY));
            triangle.setPoint(2, sf::Vector2f(tileLength * move.x + tileLength * bOffset + offsetX, tileLength * (move.y + 1) + offsetY));
            window.draw(triangle);

            triangle.setPoint(0, sf::Vector2f(tileLength * move.x + offsetX, tileLength * move.y + tileLength * bOffset + offsetY));
            triangle.setPoint(1, sf::Vector2f(tileLength * move.x + offsetX, tileLength * (move.y + 1) + offsetY));
            triangle.setPoint(2, sf::Vector2f(tileLength * move.x + tileLength * sOffset + offsetX, tileLength * (move.y + 1) + offsetY));
            window.draw(triangle);
        }
        else
        {
            float offset{ tileLength / 2 - circle.getRadius() };
            circle.setPosition(tileLength * move.x + offset + offsetX, tileLength * move.y + offset + offsetY);
            window.draw(circle);
        }
    }
}

void MainWindow::Online()
{
    while (mode == Mode::Online)
    {
        if (root->turn != root->orientation && sessionUpdated && !paused)
        {
            session->load();
            JSON::json data{ JSON::json::parse(session->view(),nullptr, false) };
            if (data.is_discarded()) continue;
            if (data["Moves"].empty()) continue;

            if (data["Moves"].back()["Turn"] == (root->turn == White ? "White" : "Black"))
            {
                lock.lock();
                sf::Vector2i before{ 7 - data["Moves"].back()["Before"]["x"], 7 - data["Moves"].back()["Before"]["y"] };
                sf::Vector2i after{ 7 - data["Moves"].back()["After"]["x"], 7 - data["Moves"].back()["After"]["y"] };

                Piece* piece{ root->chessboard[after.x][after.y] };
                MakeMove(root, Move(before, after), true);
                delete piece;
                lock.unlock();

                sessionUpdated = false;
                possibleMoves.clear();
            }
        }
    }
}

sf::Vector2i MainWindow::GetSquare(int xPos, int yPos)
{
    int width{ static_cast<int>(window.getSize().x) };
    int height{ static_cast<int>(window.getSize().y) };

    sf::Vector2i square{ -1,-1 };
    sf::Vector2f offset{};

    if (width > height)
    {
        offset.x = (width - height) / 2.f;
        if (xPos > offset.x && xPos < width - offset.x && yPos > 0 && yPos < height)
        {
            square.x = static_cast<int>((xPos - offset.x) / (height / 8.f));
            square.y = static_cast<int>(yPos / (height / 8.f));
        }
    }
    else
    {
        offset.y = (height - width) / 2.f;
        if (yPos > offset.y && yPos < height - offset.y && xPos > 0 && xPos < width)
        {
            square.x = static_cast<int>(xPos / (width / 8.f));
            square.y = static_cast<int>((yPos - offset.y) / (width / 8.f));
        }
    }

    return square;
}

sf::Vector2i MainWindow::CurrentlyHoveredSquare(int x, int y, bool set)
{
    static sf::Vector2i square{ -1, -1 };

    if (set)
    {
        if (square != sf::Vector2i(-1, -1))
        {
            if (root->chessboard[square.x][square.y])
            {
                root->chessboard[square.x][square.y]->sprite.setColor(sf::Color(255, 255, 255, 255));
            }
        }

        square.x = x;
        square.y = y;

        if (square != sf::Vector2i(-1, -1))
        {
            if (root->chessboard[x][y])
            {
                if (root->chessboard[x][y]->color == root->turn)
                {
                    root->chessboard[x][y]->sprite.setColor(sf::Color(255, 255, 255, 128));
                }
            }

            if (std::find(possibleMoves.begin(), possibleMoves.end(), square) != possibleMoves.end())
            {
                int width{ static_cast<int>(window.getSize().x) };
                int height{ static_cast<int>(window.getSize().y) };
                float squareLength{ width > height ? height / 8.f : width / 8.f };

                float offsetX{ 0 };
                float offsetY{ 0 };
                width > height ? offsetX = (width - height) / 2.f : offsetY = (height - width) / 2.f;

                hoveredPMHighlight.setSize(sf::Vector2f(squareLength, squareLength));
                hoveredPMHighlight.setPosition(squareLength * x + offsetX, squareLength * y + offsetY);

                hoveredPMSquare = { x, y };
            }
        }
    }

    return square;
}

sf::Vector2i MainWindow::CurrentlyHeldSquare(int x, int y, bool set)
{
    static sf::Vector2i square{ -1, -1 };

    if (set)
    {
        if (square != sf::Vector2i(-1, -1))
        {
            if (root->chessboard[square.x][square.y])
            {
                float prevLength{ root->chessboard[square.x][square.y]->sprite.getGlobalBounds().width };
                root->chessboard[square.x][square.y]->sprite.scale(1.25f, 1.25f);
                float postLength{ root->chessboard[square.x][square.y]->sprite.getGlobalBounds().width };

                float offset{ (prevLength - postLength) / 2.f };
                root->chessboard[square.x][square.y]->sprite.move(offset, offset);
            }
        }

        square.x = x;
        square.y = y;

        if (square != sf::Vector2i(-1, -1))
        {
            if (root->chessboard[x][y])
            {
                float prevLength{ root->chessboard[x][y]->sprite.getGlobalBounds().width };

                selectedSquareHighlight.setSize(sf::Vector2f(prevLength, prevLength));
                selectedSquareHighlight.setPosition(root->chessboard[x][y]->sprite.getGlobalBounds().left, root->chessboard[x][y]->sprite.getGlobalBounds().top);

                root->chessboard[x][y]->sprite.scale(0.8f, 0.8f);
                float postLength{ root->chessboard[x][y]->sprite.getGlobalBounds().width };

                float offset{ (prevLength - postLength) / 2.f };
                root->chessboard[x][y]->sprite.move(offset, offset);
            }
        }
        heldPieceSquare = square;
    }

    return square;
}

sf::Vector2i MainWindow::CurrentlySelectedSquare(int x, int y, bool set)
{
    static sf::Vector2i square{ -1, -1 };
    if (set)
    {
        square.x = x;
        square.y = y;

        possibleMoves.clear();
        if (x != -1 && y != -1) FindPossibleMoves(root, false, x, y, &possibleMoves);
    }
    return square;
}