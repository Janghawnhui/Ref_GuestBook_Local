/**
@file ColorPalette.cpp
@brief 펜 색상변경 팔레트 코드
*/
#include "ColorPalette.h" 


/// 색상 선택 대화상자 열고 선택된 색상을 설정하는 함수
/*
@fn ColorPalette:colorSelect(HWND hWnd, int penNum)
@brief 팔레트 상자 열고 색상 선택하는 함수
*/
void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
	ZeroMemory(&cc, sizeof(cc)); /// cc(ColorChooser) 구조체를 초기화하여 모든 값을 0으로 설정
	cc.lStructSize = sizeof(cc); /// 구조체 크기 설정 (ChooseColor API에서 사용)
	cc.hwndOwner = hWnd; /// 대화 상자를 소유할 창의 핸들을 설정
	cc.lpCustColors = (LPDWORD)acrCustClr; /// 사용자 정의 색상 배열의 주소 설정
	cc.rgbResult = RGB(0, 0, 0); /// 초기 색상 설정 (검정색으로 기본값 설정)
	cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// 색상 대화 상자 설정 (전체 색상 팔레트를 표시하고 초기 색상을 설정)

	/// 사용자가 색상을 선택한 경우 (대화 상자에서 OK를 눌렀을 때)
	if (ChooseColor(&cc)) {
		currentcolor = cc.rgbResult;
		/// 기존에 사용 중인 브러시가 있다면 삭제
		if (hBrush) {
			DeleteObject(hBrush); /// 메모리 누수를 방지하기 위해 이전 브러시 객체를 삭제
		}
		/// 새로 선택된 색상으로 단색 브러시 생성
		hBrush = CreateSolidBrush(cc.rgbResult);
		/// 선택된 색상을 colorArr 배열에 저장 (penNum 번째 펜의 색상 설정)
		colorArr[penNum] = cc.rgbResult;
		/// 창을 다시 그리도록 요청 (화면 업데이트)
		InvalidateRect(hWnd, NULL, TRUE);
	}
}


/// 색상 팔레트 관련 리소스를 정리하는 함수
void ColorPalette::destroy()
{
	/// 브러시가 생성되었을 경우 메모리에서 제거하여 리소스 누수 방지
	if (hBrush)
	{
		DeleteObject(hBrush); /// 브러시 객체 삭제
	}
}

/// 지정된 펜 번호에 대한 색상을 반환하는 함수
COLORREF ColorPalette::getColor(int penNum) {
	/// colorArr 배열에서 penNum 번째 색상을 반환
	return colorArr[penNum];
}
