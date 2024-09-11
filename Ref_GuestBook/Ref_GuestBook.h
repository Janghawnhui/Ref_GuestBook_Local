#pragma once

#include "resource.h"
#include "Pen_Str.h"
#include "drawline.h"
#include "replay.h"
#include "button.h"
#include "erase.h"
#include "w_control.h"
#include "file_io.h"
#include "file_operations.h"
#include "c_color.h"
#include "paint_area.h"
#include "Window_Size.h"
#include "stamp.h"
#include "ColorPalette.h"
#include "Struct.h"


class Window
{
private:
	HINSTANCE hInst;
	HWND hWnd;

	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	static unique_ptr<Window> sinTonIns;
	static once_flag flag;

	unique_ptr<ColorPalette> colorPalette;

	PINFO drawPInfo;

	int penNum = 0;

	int px, py;

	bool LBState = false;


public :
	ATOM MyRegisterClass(HINSTANCE);
	static Window* GetInstance();
};


/// COMMAND ��� ��� ����

#define REPLAY 100
#define ERASE 200			// �����

#define SAVE 300
#define LOAD 400

///
/// �� ���� ���� func
/// 
#define W_UP 500
#define W_DOWN 600

///
/// �� ���� ����
///
#define COLOR_CHANGE 1001
/// ������ func
/// 
#define CHANGE_PEN 700
#define HEART_STAMP 800
#define UH_STAMP 900
#define YUHAN_STAMP 1000
#define YONGBIN_STAMP 1100
