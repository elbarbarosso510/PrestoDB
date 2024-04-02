#pragma once
#include <Windows.h>
#include <DWrite.h>
#include <DXGIType.h>
#include <wincodec.h>
#include <D2D1.h>
#include <cmath>
#include <string>
#include <vector>
using namespace D2D1;
using namespace std;
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"D2d1.lib")
#pragma comment(lib,"Windowscodecs.lib")
#pragma comment(lib,"Ole32.lib")
#pragma comment(lib,"D3D11.lib")
#define MULBRUSH false
static ID2D1Factory* pD2DFactory = nullptr;
static IDWriteFactory* pDWriteFactory = nullptr;
inline void CheckFactory()
{
    if (!pD2DFactory)
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
    }
}
static wchar_t* m2t(const char* _char)
{
    int iLength;
    iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
    wchar_t* tchar = new wchar_t[sizeof(wchar_t) * iLength];
    MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
    return tchar;
}
#define PI 3.1415926535897931F
static float GetLength(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
    float w = p2.x - p1.x, h = p2.y - p1.y;
    return abs(sqrt((w * w) + (h * h)));
}
static D2D1_POINT_2F AngleToPoint(D2D1_POINT_2F Cent, float Angle, float Len)
{
    if (Len == 0)return Cent;
    float x = Cent.x + (sin(Angle * PI / 180) * Len);
    float y = Cent.y - (cos(Angle * PI / 180) * Len);
    return { x, y };
}

static D2D1_POINT_2F Rotate(D2D1_POINT_2F pt, D2D1_POINT_2F Cent, float angle)
{
    angle = angle * PI / 180.0f;
    float s = sin(angle);
    float c = cos(angle);
    pt.x -= Cent.x;
    pt.y -= Cent.y;
    float Xnew = pt.x * c - pt.y * s;
    float Ynew = pt.x * s + pt.y * c;
    pt.x = Xnew + Cent.x;
    pt.y = Ynew + Cent.y;
    return pt;
}
static int SamePoint(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
    return *(double*)&p1 == *(double*)&p2;
}
static float PointToAngle(D2D1_POINT_2F _from, D2D1_POINT_2F _to)
{
    if (SamePoint(_from, _to))
    {
        return 0;
    }
    float wid = abs(_to.x - _from.x);
    float hei = abs(_to.y - _from.y);
    float x = _to.x - _from.x;
    float y = _to.y - _from.y;
    float len = sqrt(((wid * wid) + (hei * hei)));
    float ag1 = (asin(wid / len) / PI * 180);
    if (x >= 0 && y < 0)
    {

    }
    else if (x >= 0 && y >= 0)
    {
        ag1 = 90 + (90 - ag1);
    }
    else if (x < 0 && y >= 0)
    {
        ag1 = 180 + ag1;
    }
    else if (x < 0 && y < 0)
    {
        ag1 = 270 + (90 - ag1);
    }
    return ag1;
}
static D2D1_POINT_2F Distance(D2D1_POINT_2F fm, D2D1_POINT_2F to, float len)
{
    return AngleToPoint(fm, PointToAngle(fm, to), len);
}
class D2DGraphics
{
    int brush_get_index = 0;
    ID2D1RenderTarget* pRenderTarget = nullptr;
    static IWICImagingFactory* GetImageFactory()
    {
        static IWICImagingFactory* _pImageFactory = nullptr;
        if (!_pImageFactory)
        {
            auto hr = CoInitialize(NULL);
            hr = CoCreateInstance(
                CLSID_WICImagingFactory1,
                NULL,
                CLSCTX_INPROC_SERVER,
                IID_IWICImagingFactory,
                (LPVOID*)&_pImageFactory
            );
        }
        return _pImageFactory;
    }

private:
#if MULBRUSH
    List<ID2D1SolidColorBrush*> Brushes = List<ID2D1SolidColorBrush*>();
    inline ID2D1SolidColorBrush* GetBrush()
    {
        ID2D1SolidColorBrush* brush = Brushes[brush_get_index++];
        if (brush_get_index >= Brushes.Count)brush_get_index = 0;
        return brush;
    }
#else
    ID2D1SolidColorBrush* Default_Brush = NULL;
    inline ID2D1SolidColorBrush* GetBrush()
    {
        return Default_Brush;
    }
#endif
    int brush_color_offset = 0;
    void SetSolidColorBrushColor(ID2D1SolidColorBrush* brush, const struct _D3DCOLORVALUE* a2)
    {
        if (brush_color_offset > 0)
            *(struct _D3DCOLORVALUE*)((char*)brush + brush_color_offset) = *a2;
        else
            brush->SetColor(a2);
    }
    ID2D1SolidColorBrush* GetColorBrush(D2D1_COLOR_F& newcolor)
    {
        auto brush = this->GetBrush();
        SetSolidColorBrushColor(brush,&newcolor);
        return brush;
    }
    ID2D1SolidColorBrush* GetColorBrush(COLORREF& newcolor)
    {
        auto brush = this->GetBrush();
        D2D1_COLOR_F _newcolor = { GetRValue(newcolor) / 255.0f,GetGValue(newcolor) / 255.0f,GetBValue(newcolor) / 255.0f,1.0f };
        SetSolidColorBrushColor(brush, &_newcolor);
        return brush;
    }
    ID2D1SolidColorBrush* GetColorBrush(int r, int g, int b)
    {
        auto brush = this->GetBrush();
        D2D1_COLOR_F _newcolor = { r / 255.0f,g / 255.0f,b / 255.0f,1.0f };
        SetSolidColorBrushColor(brush, &_newcolor);
        return brush;
    }
    ID2D1SolidColorBrush* GetColorBrush(float r, float g, float b, float a)
    {
        auto brush = this->GetBrush();
        D2D1_COLOR_F _newcolor = { r,g,b,a };
        SetSolidColorBrushColor(brush, &_newcolor);
        return brush;
    }
public:
    HWND WindowHandle;
    IDWriteTextFormat* DefaultFontObject = nullptr;
    D2DGraphics(IDXGISwapChain* swap_chain_ptr)
    {
        IDXGISurface* pBackBuffer;
        swap_chain_ptr->GetBuffer(
            0,
            IID_PPV_ARGS(&pBackBuffer)
        );
        CheckFactory();
        D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_HARDWARE,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        auto err = pD2DFactory->CreateDxgiSurfaceRenderTarget(
            pBackBuffer,
            &rtDesc,
            &pRenderTarget
        );
        this->DefaultFontObject = this->CreateD2dFont(L"Arial Block", 18);
#if MULBRUSH
        for (int i = 0; i < 0x100; i++)
        {
            ID2D1SolidColorBrush* brush = nullptr;
            pRenderTarget->CreateSolidColorBrush({ 0,1,0,1.0f }, &brush);
            this->Brushes.Add(brush);
        }
#endif
        _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
        pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
        this->Default_Brush->SetColor(color);
        for (int o = 0; o < 0x80; o++)
        {
            auto ptr = ((char*)this->Default_Brush + o);
            if (memcmp(ptr, &color, sizeof(_D3DCOLORVALUE)) == 0)
            {
                brush_color_offset = o;
                break;
            }
        }
        //this->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        //this->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
    }
    D2DGraphics(IWICBitmap* bmp)
    {
        CheckFactory();
        D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_HARDWARE,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        auto err = pD2DFactory->CreateWicBitmapRenderTarget(
            bmp,
            &rtDesc,
            &pRenderTarget
        );
        this->DefaultFontObject = this->CreateD2dFont(L"Arial Block", 18);
#if MULBRUSH
        for (int i = 0; i < 0x100; i++)
        {
            ID2D1SolidColorBrush* brush = nullptr;
            pRenderTarget->CreateSolidColorBrush({ 0,1,0,1.0f }, &brush);
            this->Brushes.Add(brush);
        }
#endif
        _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
        pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
        this->Default_Brush->SetColor(color);
        for (int o = 0; o < 0x80; o++)
        {
            auto ptr = ((char*)this->Default_Brush + o);
            if (memcmp(ptr, &color, sizeof(_D3DCOLORVALUE)) == 0)
            {
                brush_color_offset = o;
                break;
            }
        }
        //this->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        //this->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
    }
    D2DGraphics(HWND hWnd)
    {
        this->WindowHandle = hWnd;
        CheckFactory();
        D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_HARDWARE,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        RECT rc = { 0,0,0,0 };
        GetClientRect(this->WindowHandle, &rc);
        auto err = pD2DFactory->CreateHwndRenderTarget(
            RenderTargetProperties(),
            HwndRenderTargetProperties(this->WindowHandle, SizeU(rc.right - rc.left, rc.bottom - rc.top)),
            (ID2D1HwndRenderTarget**)&pRenderTarget
        );
        this->DefaultFontObject = this->CreateD2dFont(L"Arial Block", 18);
#if MULBRUSH
        for (int i = 0; i < 0x100; i++)
        {
            ID2D1SolidColorBrush* brush = nullptr;
            pRenderTarget->CreateSolidColorBrush({ 0,1,0,1.0f }, &brush);
            this->Brushes.Add(brush);
        }
#endif
        _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
        pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
        this->Default_Brush->SetColor(color);
        for (int o = 0; o < 0x80; o++)
        {
            auto ptr = ((char*)this->Default_Brush + o);
            if (memcmp(ptr, &color, sizeof(_D3DCOLORVALUE)) == 0)
            {
                brush_color_offset = o;
                break;
            }
        }
        //this->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        //this->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
    }
    ~D2DGraphics()
    {

    }
    void Resize()
    {
        if (this->WindowHandle)
        {
            RECT rc = { 0,0,0,0 };
            GetClientRect(this->WindowHandle, &rc);
            ((ID2D1HwndRenderTarget*)this->pRenderTarget)->Resize(
                D2D1_SIZE_U{
                    (UINT32)rc.right - (UINT32)rc.left,
                    (UINT32)rc.bottom - (UINT32)rc.top
                }
            );
        }
    }
    void BeginRender(CONST D2D1_COLOR_F clearColor)
    {
        pRenderTarget->BeginDraw();
        this->pRenderTarget->Clear(clearColor);
    }
    void BeginRender()
    {
        pRenderTarget->BeginDraw();
    }
    void EndRender()
    {
        pRenderTarget->EndDraw();
    }
    void DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawLine(p1, p2, this->GetColorBrush(color), linewidth);
    }
    void DrawLine(float p1_x, float p1_y, float p2_x, float p2_y, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawLine({ p1_x,p1_y }, { p2_x,p2_y }, this->GetColorBrush(color), linewidth);
    }
    void DrawRect(float left, float top, float width, float height, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawRectangle({ left,top,left + width,top + height }, this->GetColorBrush(color), linewidth);
    }
    void FillRect(float left, float top, float width, float height, D2D1_COLOR_F color)
    {
        this->pRenderTarget->FillRectangle({ left,top,left + width,top + height }, this->GetColorBrush(color));
    }
    void DrawEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawEllipse({ cent ,xr,yr }, this->GetColorBrush(color), linewidth = 1.0f);
    }
    void DrawEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawEllipse(ellipse, this->GetColorBrush(color), linewidth);
    }
    void DrawEllipse(float x, float y, float xr, float yr, D2D1_COLOR_F color, float linewidth = 1.0f)
    {
        this->pRenderTarget->DrawEllipse({ {x,y} ,xr,yr }, this->GetColorBrush(color), linewidth = 1.0f);
    }
    void FillEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color)
    {
        this->pRenderTarget->FillEllipse(ellipse, this->GetColorBrush(color));
    }
    void FillEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color)
    {
        this->pRenderTarget->FillEllipse({ cent ,xr,yr }, this->GetColorBrush(color));
    }
    void FillEllipse(float cx, float cy, float xr, float yr, D2D1_COLOR_F color)
    {
        this->pRenderTarget->FillEllipse({ {cx,cy} ,xr,yr }, this->GetColorBrush(color));
    }
    void DrawGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color, float linewidth)
    {
        this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), linewidth);
    }
    void FillGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color)
    {
        this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
    }
    void FillMesh(ID2D1Mesh* mesh, D2D1_COLOR_F color)
    {   
        this->pRenderTarget->FillMesh(mesh, this->GetColorBrush(color));
    }
    void DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float w, float h, float opacity = 1.0f, D2D1_COLOR_F border = { 0,0,0,0 }, float borderwidth = 1.0f)
    {
        if (this->pRenderTarget && bmp)
        {
            this->pRenderTarget->DrawBitmap(
                bmp,
                {
                    x,y,
                    w == 0 ? bmp->GetSize().width + x : w + x,
                    h == 0 ? bmp->GetSize().height + y : h + y
                },
                opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
            if (border.a > 0.0f)
            {
                this->DrawRect(x, y, w, h, border, borderwidth);
            }
        }
    }
    void DrawBitmap(ID2D1Bitmap* bmp,
        float sx, float sy, float sw, float sh,
        float x, float y, float w, float h,
        float opacity = 1.0f,
        D2D1_COLOR_F border = { 0,0,0,0 },
        float borderwidth = 1.0f)
    {
        if (this->pRenderTarget && bmp)
        {
            D2D1_RECT_F srec = {
                    sx,sy,
                    sw + sx,
                    sh + sy
            };
            D2D1_RECT_F drec = {
                    x,y,
                    w == 0 ? bmp->GetSize().width + x : w + x,
                    h == 0 ? bmp->GetSize().height + y : h + y
            };
            this->pRenderTarget->DrawBitmap(
                bmp,
                drec,
                opacity,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                srec);

            if (border.a > 0.0f)
            {
                this->DrawRect(x, y, w, h, border, borderwidth);
            }
        }
    }
    void DrawString(const char* str, float x, float y, D2D1_COLOR_F color, IDWriteTextFormat* font = nullptr)
    {
        auto tmp = m2t(str);
        this->pRenderTarget->DrawTextW(
            tmp,
            wcslen(tmp),
            font ? font : this->DefaultFontObject,
            D2D1::RectF(x, y, x + 10000.0f, y + 10000.0f),
            this->GetColorBrush(color));
        delete tmp;
    }
    void DrawString(const wchar_t* str, float x, float y, D2D1_COLOR_F color, IDWriteTextFormat* font = nullptr)
    {
        this->pRenderTarget->DrawTextW(
            str,
            wcslen(str),
            font ? font : this->DefaultFontObject,
            D2D1::RectF(x, y, x + 10000.0f, y + 10000.0f),
            this->GetColorBrush(color));
    }
    void DrawString(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextFormat* font = nullptr)
    {
        auto tmp = m2t(str.c_str());
        this->pRenderTarget->DrawTextW(
            tmp,
            wcslen(tmp),
            font ? font : this->DefaultFontObject,
            D2D1::RectF(x, y, x + 10000.0f, y + 10000.0f),
            this->GetColorBrush(color));
        delete tmp;
    }
    void DrawString(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextFormat* font = nullptr)
    {
        this->pRenderTarget->DrawTextW(
            str.c_str(),
            str.size(),
            font ? font : this->DefaultFontObject,
            D2D1::RectF(x, y, x + 10000.0f, y + 10000.0f),
            this->GetColorBrush(color));
    }
    IDWriteTextLayout* CreateStringLayout(std::string str, IDWriteTextFormat* font = nullptr)
    {
        IDWriteTextLayout* textLayout = 0;
        auto tmp = m2t(str.c_str());
        HRESULT hr = pDWriteFactory->CreateTextLayout(
            tmp,
            wcslen(tmp),
            font ? font : this->DefaultFontObject,
            10000000,
            10000000,
            &textLayout);
        return textLayout;
    }
    IDWriteTextLayout* CreateStringLayout(std::wstring str, IDWriteTextFormat* font = nullptr)
    {
        IDWriteTextLayout* textLayout = 0;
        HRESULT hr = pDWriteFactory->CreateTextLayout(
            str.c_str(),
            str.size(),
            font ? font : this->DefaultFontObject,
            10000000,
            10000000,
            &textLayout);
        return textLayout;
    }
    void DrawStringLayout(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color)
    {
        this->pRenderTarget->DrawTextLayout(
            { x,y },
            layout,
            this->GetColorBrush(color));
    }
    D2D1_SIZE_F DrawStringLayoutCent(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f })
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = layout->GetMetrics(&metrics);
        auto tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));

        D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
        return tsize;
    }
    D2D1_SIZE_F DrawStringCent(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = {0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        auto tmp = m2t(str.c_str());
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(
                    loc,
                    textLayout,
                    this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        delete tmp;
        return tsize;
    }
    D2D1_SIZE_F DrawStringCent(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = {0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        return tsize;
    }
    D2D1_SIZE_F DrawStringLayoutCentHorizontal(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f })
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = layout->GetMetrics(&metrics);
        auto tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));

        D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y};
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
        return tsize;
    }
    D2D1_SIZE_F DrawStringCentHorizontal(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        auto tmp = m2t(str.c_str());
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(
                    loc,
                    textLayout,
                    this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        delete tmp;
        return tsize;
    }
    D2D1_SIZE_F DrawStringCentHorizontal(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        return tsize;
    }
    D2D1_SIZE_F DrawStringLayoutCentVertical(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f })
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = layout->GetMetrics(&metrics);
        auto tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));

        D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
            this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
        return tsize;
    }
    D2D1_SIZE_F DrawStringCentVertical(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        auto tmp = m2t(str.c_str());
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(
                    loc,
                    textLayout,
                    this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        delete tmp;
        return tsize;
    }
    D2D1_SIZE_F DrawStringCentVertical(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = { 0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F tsize = { 0,0 };
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            DWRITE_TEXT_METRICS metrics;
            auto hr = textLayout->GetMetrics(&metrics);
            if (SUCCEEDED(hr))
            {
                tsize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
                if (back.a > 0.0f)
                {
                    auto brush = this->GetColorBrush(back);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                    this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
                }
                this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
            }
            if (_recodeLayout) *_recodeLayout = textLayout;
            else textLayout->Release();
        }
        return tsize;
    }
    void DrawStringLayout(std::string str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = {0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        auto tmp = m2t(str.c_str());
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ x - 1,y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x + 1,y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x + 1,y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x - 1,y + 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
            textLayout->Release();
        }
        delete tmp;
    }
    void DrawStringLayout(std::wstring str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = {0.0f,0.0f,0.0f,1.0f }, IDWriteTextFormat* font = nullptr)
    {
        IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
        if (textLayout)
        {
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ x - 1,y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x + 1,y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x + 1,y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ x - 1,y + 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
            textLayout->Release();
        }
    }
    void FillTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            ID2D1GeometrySink* tmp = NULL;
            if (SUCCEEDED(geo->Open(&tmp)))
            {
                tmp->BeginFigure(triangle.point1, D2D1_FIGURE_BEGIN_FILLED);
                tmp->AddLine(triangle.point2);
                tmp->AddLine(triangle.point3);
                tmp->AddLine(triangle.point1);
                tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
                tmp->Close();
                this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
                tmp->Release();
            }
            geo->Release();
        }
    }
    void DrawTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color, float width)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            ID2D1GeometrySink* tmp = NULL;
            if (SUCCEEDED(geo->Open(&tmp)))
            {
                tmp->BeginFigure(triangle.point1, D2D1_FIGURE_BEGIN_FILLED);
                tmp->AddLine(triangle.point2);
                tmp->AddLine(triangle.point3);
                tmp->AddLine(triangle.point1);
                tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
                tmp->Close();
                this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                tmp->Release();
            }
            geo->Release();
        }
    }
    void FillPolygon(initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            ID2D1GeometrySink* tmp = NULL;
            if (SUCCEEDED(geo->Open(&tmp)))
            {
                if (points.size() > 2)
                {
                    tmp->BeginFigure(points.begin()[0], D2D1_FIGURE_BEGIN_FILLED);
                    for (int i = 1; i < points.size(); i++)
                    {

                        tmp->AddLine(points.begin()[i]);
                    }
                    tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
                    tmp->Close();
                    this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
                    tmp->Release();
                }
            }
            geo->Release();
        }
    }
    void DrawPolygon(initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color, float width)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            ID2D1GeometrySink* tmp = NULL;
            if (SUCCEEDED(geo->Open(&tmp)))
            {
                if (points.size() > 2)
                {
                    tmp->BeginFigure(points.begin()[0], D2D1_FIGURE_BEGIN_FILLED);
                    for (int i = 1; i < points.size(); i++)
                    {
                        tmp->AddLine(points.begin()[i]);
                    }
                    tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
                    tmp->Close();
                    this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                    tmp->Release();
                }
            }
            geo->Release();
        }
    }
    void DrawArc(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            float ts = sa;
            float te = ea;
            if (te < ts) te += 360.0f;
            D2D1_ARC_SIZE sweep = (te - ts < 180.0f) ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE;
            ID2D1GeometrySink* pSink = NULL;
            if (SUCCEEDED(geo->Open(&pSink)))
            {
                D2D1_SWEEP_DIRECTION sw;
                auto start = AngleToPoint(center, sa, size);
                auto end = AngleToPoint(center, ea, size);
                pSink->BeginFigure(start, D2D1_FIGURE_BEGIN_FILLED);
                pSink->AddArc(
                    D2D1::ArcSegment
                    (
                        end,
                        D2D1::SizeF(size, size),
                        0.0f,
                        D2D1_SWEEP_DIRECTION_CLOCKWISE,
                        sweep
                    )
                );
                pSink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
                pSink->Close();
                this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                pSink->Release();
            }
            geo->Release();
        }
    }
    void DrawArcCounter(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width)
    {
        ID2D1PathGeometry* geo = nullptr;
        if (SUCCEEDED(pD2DFactory->CreatePathGeometry(&geo)))
        {
            float ts = sa;
            float te = ea;
            if (te < ts) te += 360.0f;
            D2D1_ARC_SIZE sweep = (te - ts < 180.0f) ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE;
            ID2D1GeometrySink* pSink = NULL;
            if (SUCCEEDED(geo->Open(&pSink)))
            {
                D2D1_SWEEP_DIRECTION sw;
                auto start = AngleToPoint(center, sa, size);
                auto end = AngleToPoint(center, ea, size);
                pSink->BeginFigure(start, D2D1_FIGURE_BEGIN_FILLED);
                pSink->AddArc(
                    D2D1::ArcSegment
                    (
                        end,
                        D2D1::SizeF(size, size),
                        0.0f,
                        D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                        sweep
                    )
                );
                pSink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
                pSink->Close();
                this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                pSink->Release();
            }
            geo->Release();
        }
    }
    D2D1_SIZE_F GetTextSize(std::string str, IDWriteTextFormat* font = nullptr)
    {
        auto tmp = m2t(str.c_str());
        IDWriteTextLayout* textLayout = 0;
        HRESULT hr = pDWriteFactory->CreateTextLayout(
            tmp,
            wcslen(tmp),
            font ? font : this->DefaultFontObject,
            10000000,
            10000000,
            &textLayout);

        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_METRICS metrics;
            hr = textLayout->GetMetrics(&metrics);
            textLayout->Release();
            if (SUCCEEDED(hr))
            {
                D2D1_SIZE_F minSize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                delete tmp;
                return minSize;
            }
        }
        delete tmp;
        return { 0,0 };
    }
    D2D1_SIZE_F GetTextSize(std::wstring str, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F minSize = { 0,0 };
        IDWriteTextLayout* textLayout = 0;
        HRESULT hr = pDWriteFactory->CreateTextLayout(
            str.c_str(),
            str.size(),
            font ? font : this->DefaultFontObject,
            10000000,
            10000000,
            &textLayout);

        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_METRICS metrics;
            hr = textLayout->GetMetrics(&metrics);
            textLayout->Release();
            if (SUCCEEDED(hr))
            {
                minSize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                return minSize;
            }
        }
        return { 0,0 };
    }
    D2D1_SIZE_F GetTextSize(wchar_t c, IDWriteTextFormat* font = nullptr)
    {
        D2D1_SIZE_F minSize = { 0,0 };
        IDWriteTextLayout* textLayout = 0;
        HRESULT hr = pDWriteFactory->CreateTextLayout(
            &c,
            1,
            font ? font : this->DefaultFontObject,
            10000000,
            10000000,
            &textLayout);

        if (SUCCEEDED(hr))
        {
            DWRITE_TEXT_METRICS metrics;
            hr = textLayout->GetMetrics(&metrics);
            textLayout->Release();
            if (SUCCEEDED(hr))
            {
                minSize = D2D1::Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
                return minSize;
            }
        }
        return { 0,0 };
    }
    void SetRenderRange(float left, float top, float width, float height)
    {
        this->pRenderTarget->PushAxisAlignedClip(
            {
                left,
                top,
                left + width,
                top + height
            },
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }
    void ClearRenderRange()
    {
        this->pRenderTarget->PopAxisAlignedClip();
    }
    void SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
    {
        pRenderTarget->SetAntialiasMode(antialiasMode);
    }
    void SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE antialiasMode)
    {
        pRenderTarget->SetTextAntialiasMode(antialiasMode);
    }
    ID2D1Bitmap* CreateBitmap(const char* path)
    {
        ID2D1Bitmap* bmp = nullptr;
        IWICBitmapDecoder* bitmapdecoder = NULL;
        GetImageFactory()->CreateDecoderFromFilename(m2t(path), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);//
        if (bitmapdecoder)
        {
            IWICBitmapFrameDecode* pframe = NULL;
            bitmapdecoder->GetFrame(0, &pframe);
            IWICFormatConverter* fmtcovter = NULL;
            GetImageFactory()->CreateFormatConverter(&fmtcovter);
            fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
            this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
            pframe->Release();
            fmtcovter->Release();
            bitmapdecoder->Release();
        }
        return bmp;
    }
    ID2D1Bitmap* CreateBitmap(unsigned char* data, int size)
    {
        ID2D1Bitmap* bmp = nullptr;
        IWICBitmapDecoder* bitmapdecoder = NULL;
        IWICStream* pStream;
        auto errmsg = GetImageFactory()->CreateStream(&pStream);
        if (!SUCCEEDED(errmsg))
        {
            return nullptr;
        }
        errmsg = pStream->InitializeFromMemory(data, size);
        if (!SUCCEEDED(errmsg))
        {
            return nullptr;
        }
        errmsg = GetImageFactory()->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
        if (!SUCCEEDED(errmsg))
        {
            return nullptr;
        }
        if (bitmapdecoder)
        {
            IWICBitmapFrameDecode* pframe = NULL;
            bitmapdecoder->GetFrame(0, &pframe);
            IWICFormatConverter* fmtcovter = NULL;
            GetImageFactory()->CreateFormatConverter(&fmtcovter);
            fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
            this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
            pframe->Release();
            fmtcovter->Release();
            bitmapdecoder->Release();
        }
        return bmp;
    }
    ID2D1Bitmap* CreateBitmap(HBITMAP hb)
    {
        IWICBitmap* wb = nullptr;
        ID2D1Bitmap* bmp;
        GetImageFactory()->CreateBitmapFromHBITMAP(hb, 0, WICBitmapUsePremultipliedAlpha, &wb);
        auto hr = this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
        wb->Release();
        return bmp;
    }
    ID2D1Bitmap* CreateBitmap(int width, int height)
    {
        IWICBitmap* wb = nullptr;
        ID2D1Bitmap* bmp;
        GetImageFactory()->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &wb);
        auto hr = this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
        wb->Release();
        return bmp;
    }
    static IWICBitmap* CreateWICBitmap(int width, int height)
    {
        IWICBitmap* wb = nullptr;
        GetImageFactory()->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &wb);
        return wb;
    }
    ID2D1Bitmap* CreateBitmap(HICON hb)
    {
        IWICBitmap* wb = nullptr;
        ID2D1Bitmap* bmp = nullptr;
        GetImageFactory()->CreateBitmapFromHICON(hb, &wb);
        this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
        wb->Release();
        return bmp;
    }
    ID2D1Bitmap* CreateBitmap(IWICBitmap* wb)
    {
        ID2D1Bitmap* bmp = nullptr;
        this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
        return bmp;
    }
    vector<ID2D1Bitmap*> CreateBitmapFromGif(const char* path)
    {
        vector<ID2D1Bitmap*> result = vector<ID2D1Bitmap*>();
        ID2D1Bitmap* bmp = nullptr;
        IWICBitmapDecoder* bitmapdecoder = NULL;
        GetImageFactory()->CreateDecoderFromFilename(m2t(path), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapdecoder);
        if (bitmapdecoder)
        {
            UINT count = 0;
            bitmapdecoder->GetFrameCount(&count);
            for (int i = 0; i < count; i++)
            {
                IWICBitmapFrameDecode* pframe = NULL;
                bitmapdecoder->GetFrame(i, &pframe);
                IWICFormatConverter* fmtcovter = NULL;
                GetImageFactory()->CreateFormatConverter(&fmtcovter);
                fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
                this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
                pframe->Release();
                fmtcovter->Release();
                result.push_back(bmp);
            }
            bitmapdecoder->Release();
        }
        return result;
    }
    vector<ID2D1Bitmap*> CreateBitmapFromGifBuffer(unsigned char* data, int size)
    {
        vector<ID2D1Bitmap*> result = vector<ID2D1Bitmap*>();
        ID2D1Bitmap* bmp = nullptr;
        IWICBitmapDecoder* bitmapdecoder = NULL;
        IWICStream* pStream;
        auto errmsg = GetImageFactory()->CreateStream(&pStream);
        if (!SUCCEEDED(errmsg))
        {
            return result;
        }
        errmsg = pStream->InitializeFromMemory(data, size);
        if (!SUCCEEDED(errmsg))
        {
            return result;
        }
        errmsg = GetImageFactory()->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
        if (!SUCCEEDED(errmsg))
        {
            return result;
        }
        if (bitmapdecoder)
        {
            UINT count = 0;
            bitmapdecoder->GetFrameCount(&count);

            for (int i = 0; i < count; i++)
            {
                IWICBitmapFrameDecode* pframe = NULL;
                bitmapdecoder->GetFrame(i, &pframe);
                IWICFormatConverter* fmtcovter = NULL;
                GetImageFactory()->CreateFormatConverter(&fmtcovter);
                fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
                this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
                pframe->Release();
                fmtcovter->Release();
                result.push_back(bmp);
            }
            bitmapdecoder->Release();
        }
        return result;
    }
    static ID2D1PathGeometry* CreateGeomtry()
    {
        CheckFactory();
        ID2D1PathGeometry* geo = nullptr;
        HRESULT hr = pD2DFactory->CreatePathGeometry(&geo);
        if (SUCCEEDED(hr))
            return geo;
        else
            return nullptr;
    }
    static IDWriteTextFormat* CreateD2dFont(WCHAR const* fontFamilyName, float size)
    {
        CheckFactory();
        IDWriteTextFormat* result;
        pDWriteFactory->CreateTextFormat(fontFamilyName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"", &result);
        return result;
    }
    static HBITMAP GetHBITMAPFromImageFile(const wchar_t* path)
    {
        HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        return hBitmap;
    }
    static HBITMAP CopyFromScreen(int x, int y, int width, int height)
    {
        HDC hScreen = GetDC(NULL);
        HDC hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
        HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
        BOOL bRet = BitBlt(hDC, x, y, width, height, hScreen, 0, 0, SRCCOPY);
        SelectObject(hDC, old_obj);
        DeleteDC(hDC);
        ReleaseDC(NULL, hScreen);
        return hBitmap;
    }
    static HBITMAP CopyFromWidnow(HWND handle, int x, int y, int width, int height)
    {
        HDC hScreen = GetDC(handle);
        HDC hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
        HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
        BOOL bRet = BitBlt(hDC, x, y, width, height, hScreen, 0, 0, SRCCOPY);
        SelectObject(hDC, old_obj);
        DeleteDC(hDC);
        ReleaseDC(NULL, hScreen);
        return hBitmap;
    }
    static SIZE GetScreenSize(int index = 0)
    {
        vector<SIZE> sizes = vector<SIZE>();
        auto callback = [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
        {
            vector<SIZE>* tmp = (vector<SIZE>*)dwData;
            MONITORINFO info;
            info.cbSize = sizeof(info);
            if (GetMonitorInfoW(hMonitor, &info))
            {
                tmp->push_back({ info.rcMonitor.right - info.rcMonitor.left,info.rcMonitor.bottom - info.rcMonitor.top });
            }
            return TRUE;
        };
        EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)callback, (LPARAM)&sizes);
        if (sizes.size() > index)
        {
            return sizes[index];
        }
        return { GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
    }
    static vector<std::wstring> GetFonts()
    {
        CheckFactory();
        vector<std::wstring> result = vector<std::wstring>();
        IDWriteFontCollection* pFontCollection = NULL;
        auto hr = pDWriteFactory->GetSystemFontCollection(&pFontCollection);
        UINT32 familyCount = 0;
        if (SUCCEEDED(hr))
        {
            familyCount = pFontCollection->GetFontFamilyCount();
            for (UINT32 i = 0; i < familyCount; ++i)
            {
                IDWriteFontFamily* pFontFamily = NULL;
                if (SUCCEEDED(hr))
                {
                    hr = pFontCollection->GetFontFamily(i, &pFontFamily);
                    IDWriteLocalizedStrings* pFamilyNames = NULL;
                    if (SUCCEEDED(hr))
                    {
                        hr = pFontFamily->GetFamilyNames(&pFamilyNames);
                        UINT32 index = 0;
                        BOOL exists = false;

                        wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

                        if (SUCCEEDED(hr))
                        {
                            int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
                            if (defaultLocaleSuccess)
                            {
                                hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);
                            }
                            if (SUCCEEDED(hr) && !exists)
                            {
                                hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
                            }
                        }
                        if (!exists)
                            index = 0;
                        UINT32 length = 0;
                        if (SUCCEEDED(hr))
                        {
                            hr = pFamilyNames->GetStringLength(index, &length);
                        }
                        wchar_t* name = new wchar_t[length + 1];
                        if (name == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                        }
                        if (SUCCEEDED(hr))
                        {
                            hr = pFamilyNames->GetString(index, name, length + 1);
                            result.push_back(std::wstring(name));
                        }
                    }
                }
            }
        }
        return result;
    }
};