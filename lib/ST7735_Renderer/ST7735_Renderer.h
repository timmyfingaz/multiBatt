
#ifndef ST7735_Renderer_h
#define ST7735_Renderer_h

#include <Menu.h>
#include <Adafruit_ST7735.h>

class ST7735_Renderer : public MenuRenderer
{
    public:
        ST7735_Renderer(Adafruit_ST7735* display);
        void Init(vector<MenuItem>* menuItems, uint8_t* selectedIndex);
        void Render();
        void ClearScreen();
        ItemBounds GetSelectedItemBounds();
        ~ST7735_Renderer(){};
    private:
        Adafruit_ST7735* _display;
        uint8_t _pageCount;
        uint8_t _currentPage;
        bool _scrollBar;
        ItemBounds GetItemBounds(uint8_t index);
        void ResetDisplay();
        void RenderScrollBar();
    protected:
        ItemBounds RenderItem(uint8_t index);
};

#endif