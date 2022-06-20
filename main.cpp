#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include <cmath>
#include "function.h"

struct Point {
    size_t x;
    size_t y;
    size_t z;

    Point(size_t x, size_t y, size_t z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void cor(int k = 1) const {
        if (k == 1)
            std::cout << x << ' ' << y << ' ' << z << ' ';
        else
            std::cout << x << ' ' << y << ' ' << z << '\n';
    }

    bool operator<(const Point &right) const {
        if (x < right.x)
            return true;
        else if (x == right.x) {
            if (y < right.y)
                return true;
            else if (y == right.y) {
                return z < right.z;
            }
        }
        return false;
    }
};

HWND hwnd;


struct{
    float x,y,z;
    float xdeg,zdeg;

}camera={0,0,1.7,70,-40};

void Win_Resize(int x, int y){
    glViewport(0,0,x,y);
    float k = x/(float)y;
    float sz=0.1;
    glLoadIdentity();
    glFrustum(-k*sz,k*sz,-sz,sz,sz*2,80);
}

void Camera_Apply(){
    glRotatef(-camera.xdeg,1,0,0);
    glRotatef(-camera.zdeg,0,0,1);
    glTranslatef(-camera.x,-camera.y,-camera.z);
}

void Camera_Rotation(float xAngle,float zAngle){
    camera.zdeg+=zAngle;
    if(camera.zdeg<0) camera.zdeg+=360;
    if(camera.zdeg>360) camera.zdeg-=360;
    camera.xdeg+=xAngle;
    if(camera.xdeg<0) camera.xdeg=0;
    if(camera.xdeg>180) camera.xdeg=180;


}
void GameInit(){
    glEnable(GL_DEPTH_TEST);

    RECT rct;
    GetClientRect(hwnd,&rct);
    Win_Resize(rct.right,rct.bottom);
}

void Player_Move(){
    if(GetForegroundWindow()!=hwnd)return;

    float angle = -camera.zdeg / 180 * M_PI ;
    float speed =0;
    float speedz =0;
    if (GetKeyState('W')<0) speed =0.1;
    if (GetKeyState(VK_SPACE)<0) speedz =0.1;
    if (GetKeyState(VK_CONTROL)<0) speedz =-0.1;
    if (GetKeyState('S')<0) speed =-0.1;
    if (GetKeyState('A')<0) {speed =0.1;angle -=M_PI*0.5;};
    if (GetKeyState('D')<0) {speed =0.1;angle +=M_PI*0.5;};
    if (speed!=0) {
        camera.x +=sin(angle)*speed;
        camera.y +=cos(angle)*speed;
    }
    if(speedz!=0){
        camera.z+=speedz;
    }

    POINT cur;
    static POINT base ={400,300};
    GetCursorPos(&cur);
    Camera_Rotation((base.y-cur.y)/5.0,(base.x-cur.x)/5.0);
    SetCursorPos(base.x,base.y);
}

void Game_Move(){
    Player_Move();
}


void ShowWorld(float *vert,GLuint *ind, float *ppp){
    glClearColor(0.6196078431372549f, 0.9725490196078431f, 0.9333333333333333f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    Camera_Apply();

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,vert);
    glColor3f(0.203921568627451,0.5333333333333333,0.5333333333333333);
    glLineWidth(3);
    glDrawElements(GL_LINES,500,GL_UNSIGNED_INT,ind);

    glVertexPointer(3,GL_FLOAT,0,ppp);
    glColor3f(0.9490196078431373,0.2666666666666667,0.0196078431372549);
    glPointSize(10);
    glDrawArrays(GL_POINTS,0,1);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    auto v1 =std::vector<float>();
    auto v2 =std::vector<int>();
    auto p = gen(5,5,5,0.3,0.3,0.3,v1,v2);

    float vert[v1.size()];//координаты
    GLuint ind[v2.size()];// индексы
    std::cout<<v2.size();
    float ppp[]={(float)p.x,(float)p.y,(float)p.z};
    std::copy(v1.begin(),v1.end(),vert);
    std::copy(v2.begin(),v2.end(),ind);


    WNDCLASSEX wcex;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;
    /*
     *  Введение главных переменных для сообщением с Windows.
     *  bQuit - переменная отвечающая за статус окна.
     *  theta - float для подсчета тиков.
     */

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
    /*
     *  WNDCLASSEX содержит информацию о классе окна:
     *  cbSize устанавливает размер этой структуры, в байтах.
     *  style устанавливает стиль класса.
     *  lpfnWndProc указатель на оконную процедуру.
     *  cbClsExtra устанавливает число дополнительных байт, которые размещаются вслед за структурой класса окна.
     *  cbWndExtra устанавливает число дополнительных байтов, которые размещаются вслед за экземпляром окна.
     *  hInstance дескриптор экземпляра, который содержит оконную процедуру для класса.
     *  hIcon дескриптор значка класса.
     *  hCursor дескриптор курсора класса.
     *  hbrBackground дескриптор кисти фона класса.
     *  lpszMenuName указатель на символьную строку с символом конца строки.
     *  lpszClassName указатель на символьную строку с нулем в конце или атом.
     *  hIconSm дескриптор маленького значка, который связан с классом окна.
     */

    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Maze3D",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          1000,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    /*
     * Задаем параметры открывающегося окна, в том числе размемер и навание
     */

    ShowWindow(hwnd, nCmdShow);
    /*
     * Открытие окна
     */


    EnableOpenGL(hwnd, &hDC, &hRC);
    /*
     * Применяем OpenGl для Windows.
     */

    GameInit();
    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            /*
             * Реакция программы на всплявающие сообщения.
             */
        }
        else
        {

            Game_Move();
            ShowWorld(vert,ind,ppp);

            SwapBuffers(hDC);

            Sleep (1);
        }
        /*
         * Проверка на вылетающие сообщения.
         */
    }
    /*
     * Тело цикла в котором и происходит отрисовка объектов.
     */

    DisableOpenGL(hwnd, hDC, hRC);
    /*
     * Отключение OpenGl.
     */

    DestroyWindow(hwnd);
    /*
     * Закрытие окна.
     */

    return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            Win_Resize(LOWORD(lParam),HIWORD(lParam));
            break;

        case WM_SETCURSOR:
            ShowCursor(FALSE);
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
        }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}




