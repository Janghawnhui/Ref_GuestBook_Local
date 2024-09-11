#pragma once

#include "button.h"

class ColorPicker
{
private:
    HWND hWnd;

    // ������ x, y ��ǥ
    int x_;
    int y_;
    int width_ = 420; // 280
    int height_ = 300;

    double h_; // ����(Hue)
    double s_ = 1; // ä��(Saturation)
    double v_; // ��(Value)
    double pen_size_; // �� ũ��

    // ����
    RECT color_picker_area_;
    RECT palette_area_;
    RECT hue_slider_area_;
    RECT pen_size_slider_area_;

    // �ȷ�Ʈ
    int palette_x_;
    int palette_y_;
    int palette_width_;
    int palette_height_;

    // ���� �����̴�
    int hue_slider_x_;
    int hue_slider_y_;
    int hue_slider_width_;
    int hue_slider_height_;

    // �� ũ�� �����̴�
    int pen_size_slider_x_;
    int pen_size_slider_y_;
    int pen_size_slider_width_;
    int pen_size_slider_height_;

    // ���� �̸�����
    int color_preview_x_;
    int color_preview_y_;
    int color_preview_width_;
    int color_preview_height_;

    Gdiplus::Color current_color_;

    bool is_color_picker_open_;
    bool is_palette_click_;
    bool is_hue_slider_click_;
    bool is_pen_size_slider_click_;

    std::unique_ptr<Button> button_close_;

    void PaletteControl(POINT mouse_position);
    void HueSliderControl(POINT mouse_position);
    void PenSizeSliderControl(POINT mouse_position);
public:
    ColorPicker(HWND hWnd);
    ~ColorPicker() = default;

    bool OnOpen();

    void MouseUp();
    void MouseDown(POINT mouse_position);
    void MouseMove(POINT mouse_position);
    void Open();
    void Draw(HDC hdc);

    int GetPenSize();

    Gdiplus::Color HSVToRGB(double h, double s, double v);

    BYTE GetR();
    BYTE GetG();
    BYTE GetB();

    COLORREF GetRGB();
};
