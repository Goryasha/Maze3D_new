// Список всех используемых функций
#include "gL/gl.h"
#include <map>
#include <vector>

void Win_Resize(int x, int y);
LRESULT CALLBACK WindowProc(HWND HWND, UINT UNIT, WPARAM WPARAM, LPARAM LPARAM);
void EnableOpenGL(HWND hwnd, HDC* HDC, HGLRC* HGLRC);
void DisableOpenGL(HWND HWND, HDC HDC, HGLRC HGLRC);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// configs
//auto x_size = 5;
//auto y_size = 5;
//auto z_size = 5;
//auto x_weight = float(0.3);
//auto y_weight = float(0.3);
//auto z_weight = float(0.3);
//

template<typename my_type>
auto m_print(std::vector<std::vector<std::vector<my_type>>> &maze) -> void;

struct Point;

auto comp(std::pair<char, int> &a, std::pair<char, int> &b) -> bool;

auto gen(size_t side_x, size_t side_y, size_t side_z, float weightx, float weighty, float weightz,
         std::vector<float> &vertex, std::vector<int> &index) -> Point;

auto cost(Point p0, std::map<Point, std::vector<Point>> &maze, std::map<Point, std::pair<int, int>> &vert_ind) -> void;
/*
 * WindowProc - отправление сообщений в Windows
 * EnableOpenGL и DisableOpenGL - открытие и закрытие отрисовки GL
 * WinMain - Cоглашение о вызовах. Соглашение о вызовах определяет, как функция получает параметры от вызывающего абонента.
 *           Например, он определяет порядок отображения параметров в стеке. Включае в себя тело отрисовки GL.
 *           Возвращает номер ошибки.
 */
#ifndef MAZE3D_NEW_FUNCTION_H
#define MAZE3D_NEW_FUNCTION_H



#endif //MAZE3D_NEW_FUNCTION_H