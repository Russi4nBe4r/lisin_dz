// graphic.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "graphic.h"
#include "math.h"
#include<string>
#include<ctime>

BOOL DLine(HDC hdc, double x1, double y1, double x2, double y2);
#define MAX_LOADSTRING 100

char szProgName[] = "Имя программы";

int  xView, yView, test = -5, ff = 0, check = 0;;
double i, r;
double y, x;
char Buf[100];
#define pi 3.14
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GRAPHIC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHIC));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHIC));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GRAPHIC);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 900, 900, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
HPEN hPen;
int dog = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //создаём контекст устройства
	PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода
	//HPEN hPen; //создаём перо
	//Цикл обработки сообщений
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case ID_SCALE:
			dog = 1;
			SendMessage(hWnd, WM_PAINT, WPARAM(hWnd), NULL);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_NORMALSCALE:
			dog = 0;
			SendMessage(hWnd, WM_PAINT, WPARAM(hWnd), NULL);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_SIZE:
		xView = LOWORD(lParam);
		yView = HIWORD(lParam);

		break;


		//сообщение рисования

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ISOTROPIC); //логические единицы отображаем, как физические
		SetWindowExtEx(hdc, 900, 900, NULL); //Длина осей
		SetViewportExtEx(hdc, xView, -yView, NULL); //Определяем облась вывода
		SetViewportOrgEx(hdc, xView / 2, yView / 2, NULL); //Начало координат

		//Рисуем оси координат
		DLine(hdc, 0, -450, 0, 450);//ось У
		DLine(hdc, -450, 0, 450, 0);//ось Х
		if (ff == 0) {
			MoveToEx(hdc, 0, 0, NULL);
		}//перемещаемся в начало координат

		//Создание красного пера
		hPen = CreatePen(1, 4, RGB(255, 25, 0));
		SelectObject(hdc, hPen);
				for (i = -450; i <= 450; i++) {
					r = i / 90;
						hPen = CreatePen(1, 4, RGB(rand() % 255, 0, 0));
						SelectObject(hdc, hPen);
						y = (1 + 2 * sin(15 * r)) * pow((1 + 1 * cos(35 * r)), (1 + 5 * cos(6 * r)));
						x = r;

						x = y*cos(r);
						y *= sin(r);

						if ((ff == 0)) {
							MoveToEx(hdc, x, y, NULL);
							ff = 1;
						}
						LineTo(hdc, x, y);

				}
		//InvalidateRect(hWnd, NULL, TRUE);
		//ValidateRect(hWnd, NULL); //Обновляем экран
		EndPaint(hWnd, &ps);
		ff = 0;
		check = 0;
		test = -5;
		break;

		//сообщение выхода - разрушение окна
	case WM_DESTROY:
		DeleteObject(hPen); //не забываем уничтожать перья
		PostQuitMessage(0); //Посылаем сообщение выхода с кодом 0 - нормальное завершение
		break;

	default:
		return(DefWindowProc(hWnd, message, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
	}
	return 0;
}
BOOL DLine(HDC hdc, double x1, double y1, double x2, double y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2); //нарисовать линию
}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}