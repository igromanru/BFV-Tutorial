#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <dwmapi.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "InternalOverlay.lib")
#include <string>

class Dxo;

typedef void(*OverlayCallback)(Dxo* dxo, int width, int height);

class Dxo
{
public:
	void DrawLine(int x0, int y0, int x1, int y1, int width, int r, int g, int b);
	void DrawFilledBox(int x, int y, int w, int h, int r, int g, int b, int a);
	void DrawBox(int x, int y, int width, int height, int px, int r, int g, int b, int a);
	void DrawString(const char* String, int x, int y, int r, int g, int b);
	void DrawStringShadow(const char* String, int x, int y, int r, int g, int b);

	static void SetupRenderer(OverlayCallback dxo, HWND window);
	static void SetupRenderer(OverlayCallback dxo);
};