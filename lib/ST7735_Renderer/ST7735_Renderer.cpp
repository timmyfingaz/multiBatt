#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <ST7735_Renderer.h>

#define ITEMS_PER_PAGE  10
#define ITEM_HEIGHT     16

ST7735_Renderer::ST7735_Renderer(Adafruit_ST7735* display)
{
    _display = display;
};

void ST7735_Renderer::Init(vector<MenuItem>* menuItems, uint8_t* selectedIndex)
{
    _menuItems = menuItems;
    _selectedIndex = selectedIndex;

    ResetDisplay();
};

void ST7735_Renderer::ResetDisplay()
{
    _display->fillScreen(ST7735_BLACK);
    _display->setTextWrap(false);
    _display->setTextSize(1);
    _display->setFont(NULL);
}

void ST7735_Renderer::Render()
{
    if(trunc(*_selectedIndex / ITEMS_PER_PAGE) != _currentPage){
        uint8_t prevPage = _currentPage;
        _currentPage = ceil(*_selectedIndex / ITEMS_PER_PAGE);
        if(prevPage != _currentPage){
            _display->fillScreen(ST7735_BLACK);
        }
    }

    _pageCount = trunc(_menuItems->size() / ITEMS_PER_PAGE);
    if(_pageCount > 0){
        _scrollBar = true;
        RenderScrollBar();
    }

    size_t itemCount = _menuItems->size();

    for(uint8_t i = 0; i < min((unsigned int)ITEMS_PER_PAGE, itemCount - (_currentPage * ITEMS_PER_PAGE)); i++){
        RenderItem(i + (_currentPage * ITEMS_PER_PAGE));
    }
};

void ST7735_Renderer::ClearScreen()
{
    _display->fillScreen(ST7735_BLACK);
};

void ST7735_Renderer::RenderScrollBar()
{
    // draw top & bottom arrows
    _display->fillTriangle(124, 0, 128, 4, 120, 4, ST7735_WHITE);
    _display->fillTriangle(124, 160, 128, 156, 120, 156, ST7735_WHITE);
    //draw scrollbar container
    _display->drawRect(120, 4, 8, 152, ST7735_WHITE);

    //get an approximate scroll block size and position
    uint8_t blockHeight = trunc(152 / (_pageCount + 1));
    uint8_t blockTop = (blockHeight * _currentPage) + 4;

    //draw the scroll block
    _display->fillRect(120, blockTop, 8, blockHeight, ST7735_WHITE);
}

ItemBounds ST7735_Renderer::GetSelectedItemBounds()
{
    return GetItemBounds(*_selectedIndex);
}

ItemBounds ST7735_Renderer::GetItemBounds(uint8_t index)
{
    uint8_t itemWidth = _scrollBar ? 118 : 128;
    uint8_t itemTop = (uint8_t)((index * 16) - (160 * _currentPage));

    return { itemWidth, 16, 0, itemTop };
}

ItemBounds ST7735_Renderer::RenderItem(uint8_t index)
{
    Serial.write("Rendering item ");
    Serial.write(String(index).c_str());
    Serial.write("\r\n");
    
    if(index >= _menuItems->size()){
        Serial.write("Cancelled.");
        return {0,0,0,0};
    }
    ItemBounds bounds = GetItemBounds(index);
    MenuItem item = _menuItems->at(index);

    _display->setTextSize(1); // 6x8

    // draw the box - white filled for selected, black with white border for not
    _display->fillRect(bounds.Left, bounds.Top, bounds.Width, bounds.Height, item.IsSelected ? ST7735_WHITE : ST7735_BLACK);
    if(!item.IsSelected){
        _display->drawRect(bounds.Left, bounds.Top, bounds.Width, bounds.Height, ST7735_WHITE);
    }

    // measure the text - if longer than the allotted space, remove characters until it fits
    String itemText = item.Text;

    int16_t measuredX = 0, measuredY = 0;
    uint16_t measuredWidth = 0, measuredHeight = 0;

    int16_t x = 4, y = 4; // should be vertically centered with 4px horizontal padding

    // bool modified = false;
    _display->getTextBounds(itemText, x, y, &measuredX, &measuredY, &measuredWidth, &measuredHeight);
    while (measuredWidth > bounds.Width - 8)
    {
        // modified = true;
        itemText = itemText.substring(0, itemText.length() - 1);
        _display->getTextBounds(itemText, x, y, &measuredX, &measuredY, &measuredWidth, &measuredHeight);
    }
    
    _display->setCursor(x, bounds.Top + y);
    _display->setTextColor(item.IsSelected ? ST7735_BLACK : ST7735_WHITE);
    _display->write(itemText.c_str());

    return bounds;
};