#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<vector>
#include<Windows.h>

using namespace std;


class Pen {
    RECT  area;
};
/**
 * @brief       �� ���� ������ ����ü
 * (��ǥ, �� ����, ����, �ð�, ����(LBUTTON))
*/
typedef struct Pen_Info
{
    LPARAM      penCoordinate;     // ��ǥ
    int         penWidth;          // �� ����
    COLORREF    penColor;          // �� ����
    DWORD       penTime;           // �ð�
    UINT        penState;          // ���� (LBUTTON)
        
} PEN_INFO;




/**
 * @brief       Pen_Info ����ü�� ���� �ڷᱸ���� �����ϱ� ���� ���� ����ü
 * ���÷��� ������� ���
*/
extern vector<PEN_INFO> penMemory;

extern PEN_INFO g_Pen_Info;     // �� ����ü �������� ���� (extern)
extern COLORREF pen_Color;      // �� ���� �������� ���� (extern)
extern int pen_Width;           // �� ���� �������� ���� (ectern)

#define C_RED		 7000
#define C_ORANGE	 8000
#define C_YELLOW	 9000
#define C_GREEN		10000
#define C_BLUE		11000
#define C_NAVY		12000
#define C_PURPLE	13000
#define C_BLACK		14000

void Change_Color(int color);