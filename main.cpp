#include "MainWindow.h"
#include <windowsx.h>
#include <curl/curl.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* win = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
        case WM_CLOSE:
        {
            win->mode = MainWindow::Mode::Normal;
            win->networkingThread.wait();
            delete win->session;
            for (int i{ 0 }; i < 8; i++)
            {
                for (int j{ 0 }; j < 8; j++)
                {
                    delete win->root->chessboard[i][j];
                    win->root->chessboard[i][j] = nullptr;
                }
            }
            delete win->root;
            DestroyWindow(win->window.getSystemHandle());
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZING:
        {
            win->edge = wParam;
            return 0;
        }

        // Resize the window such that its dimensions are divisible by 8
        case WM_EXITSIZEMOVE:
        {
            int width{ static_cast<int>(win->window.getSize().x) };
            int height{ static_cast<int>(win->window.getSize().y) };

            RECT rect{};
            GetWindowRect(hwnd, &rect);

            int offset{ 0 };
            if (width % 8 < 5) offset = (width % 8) * -1;
            else offset = 8 - width % 8;

            if (win->edge == WMSZ_TOPLEFT || win->edge == WMSZ_LEFT || win->edge == WMSZ_BOTTOMLEFT) rect.left -= offset;
            if (win->edge == WMSZ_TOPRIGHT || win->edge == WMSZ_RIGHT || win->edge == WMSZ_BOTTOMRIGHT) rect.right += offset;

            if (height % 8 < 5) offset = (height % 8) * -1;
            else offset = 8 - height % 8;

            if (win->edge == WMSZ_TOPLEFT || win->edge == WMSZ_TOP || win->edge == WMSZ_TOPRIGHT) rect.top -= offset;
            if (win->edge == WMSZ_BOTTOMLEFT || win->edge == WMSZ_BOTTOM || win->edge == WMSZ_BOTTOMRIGHT) rect.bottom += offset;

            SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL);
            return 0;
        }

        case WM_SIZE:
        {
            int width = (int)(short)LOWORD(lParam);
            int height = (int)(short)HIWORD(lParam);

            win->window.setSize(sf::Vector2u(width, height));
            sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(width), static_cast<float>(height));
            win->window.setView(sf::View(visibleArea));

            win->CalculateLayout();
            win->Paint();

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            win->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        
        case WM_LBUTTONDOWN:
        {
            SetCapture(hwnd);
            win->OnMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            SendMessage(hwnd, WM_MOUSEMOVE, NULL, lParam);
            return 0;
        }
        
        case WM_LBUTTONUP:
        {
            ReleaseCapture();
            win->OnMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE) win->OnEscapeDown();
            return 0;
        }

        case WM_CHAR:
        {
            win->TextEntered(static_cast<unsigned int>(wParam));
            return 0;
        }

        case WM_NCMOUSEMOVE:
        {
            PostMessage(hwnd, WM_MOUSEMOVE, MAKELPARAM(static_cast<int>(win->window.getSize().x) + 1, static_cast<int>(win->window.getSize().y) + 1), NULL);
            return 0;
        }

        case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT)
            {
                HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
                SetCursor(hCursor);
                return TRUE;
            }
            else return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        // Restrict minimum window dimensions to 400x400
        case WM_GETMINMAXINFO:
        {
            ((MINMAXINFO*)(lParam))->ptMinTrackSize.x = 400;
            ((MINMAXINFO*)(lParam))->ptMinTrackSize.y = 400 + GetSystemMetrics(SM_CYCAPTION);
            return 0;
        }

        case WM_PAINT:
        { 
            PAINTSTRUCT ps{};
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    MainWindow* win{ new MainWindow(1000, 800, "CheckM8") };
    SetWindowLongPtr(win->window.getSystemHandle(), GWL_WNDPROC, (LONG_PTR)&WndProc);
    SetWindowLongPtr(win->window.getSystemHandle(), GWLP_USERDATA, (LONG_PTR)win);

    MSG msg = { };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        win->GameLoop();
    }

    delete win;
    curl_global_cleanup();
    return 0;
}