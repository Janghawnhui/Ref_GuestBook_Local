#include "ColorPalette.h" /// colorpalette.h ��� ������ �����Ѵ�

/// ���� ���� ��ȭ���� ���� ���õ� ������ �����ϴ� �Լ�
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

/// ȭ�鿡 ���õ� ������ ĥ�ϴ� �Լ�
void ColorPalette::paint(PAINTSTRUCT ps, HDC hdc)
{
	/// �귯�ð� �����Ǿ��� ��, �귯�� �������� ȭ�� ������ ä��
	if (hBrush)
	{
		/// ps.rcPaint�� WM_PAINT �޽����� ���� �׷��� ������ ��Ÿ��, �� ������ �귯�÷� ä��
		FillRect(hdc, &ps.rcPaint, hBrush);
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
