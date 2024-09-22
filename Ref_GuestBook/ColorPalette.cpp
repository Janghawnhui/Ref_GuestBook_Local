/**
@file ColorPalette.cpp
@brief �� ���󺯰� �ȷ�Ʈ �ڵ�
*/
#include "ColorPalette.h" 


/// ���� ���� ��ȭ���� ���� ���õ� ������ �����ϴ� �Լ�
/*
@fn ColorPalette:colorSelect(HWND hWnd, int penNum)
@brief �ȷ�Ʈ ���� ���� ���� �����ϴ� �Լ�
*/
void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
	ZeroMemory(&cc, sizeof(cc)); /// cc(ColorChooser) ����ü�� �ʱ�ȭ�Ͽ� ��� ���� 0���� ����
	cc.lStructSize = sizeof(cc); /// ����ü ũ�� ���� (ChooseColor API���� ���)
	cc.hwndOwner = hWnd; /// ��ȭ ���ڸ� ������ â�� �ڵ��� ����
	cc.lpCustColors = (LPDWORD)acrCustClr; /// ����� ���� ���� �迭�� �ּ� ����
	cc.rgbResult = RGB(0, 0, 0); /// �ʱ� ���� ���� (���������� �⺻�� ����)
	cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// ���� ��ȭ ���� ���� (��ü ���� �ȷ�Ʈ�� ǥ���ϰ� �ʱ� ������ ����)

	/// ����ڰ� ������ ������ ��� (��ȭ ���ڿ��� OK�� ������ ��)
	if (ChooseColor(&cc)) {
		currentcolor = cc.rgbResult;
		/// ������ ��� ���� �귯�ð� �ִٸ� ����
		if (hBrush) {
			DeleteObject(hBrush); /// �޸� ������ �����ϱ� ���� ���� �귯�� ��ü�� ����
		}
		/// ���� ���õ� �������� �ܻ� �귯�� ����
		hBrush = CreateSolidBrush(cc.rgbResult);
		/// ���õ� ������ colorArr �迭�� ���� (penNum ��° ���� ���� ����)
		colorArr[penNum] = cc.rgbResult;
		/// â�� �ٽ� �׸����� ��û (ȭ�� ������Ʈ)
		InvalidateRect(hWnd, NULL, TRUE);
	}
}


/// ���� �ȷ�Ʈ ���� ���ҽ��� �����ϴ� �Լ�
void ColorPalette::destroy()
{
	/// �귯�ð� �����Ǿ��� ��� �޸𸮿��� �����Ͽ� ���ҽ� ���� ����
	if (hBrush)
	{
		DeleteObject(hBrush); /// �귯�� ��ü ����
	}
}

/// ������ �� ��ȣ�� ���� ������ ��ȯ�ϴ� �Լ�
COLORREF ColorPalette::getColor(int penNum) {
	/// colorArr �迭���� penNum ��° ������ ��ȯ
	return colorArr[penNum];
}
