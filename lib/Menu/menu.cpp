#include "Arduino.h"
#include "Menu.h"

Menu::Menu(){
    _selected = 0;
}

void Menu::UseRenderer(MenuRenderer* renderer)
{
    _renderer = renderer;
    _renderer->Init(&_items, &_selected);
}

size_t Menu::Add(String text, DisplayFn displayFunction)
{
    _isDirty = true;
    _items.push_back(MenuItem(text, displayFunction));
    size_t count = _items.size();
    if(count == 1){
        _items.at(0).IsSelected = true;
    }
    return count;
}

void Menu::Next()
{
    if(IsEntered){
        return;
    }

    _items.at(_selected).IsSelected = false;
    _selected++;
    if(_selected >= _items.size()){
        _selected = 0;
    }
    _items.at(_selected).IsSelected = true;
    _isDirty = true;
}

void Menu::Previous()
{
    if(IsEntered){
        return;
    }

    _items.at(_selected).IsSelected = false;

    if(_selected == 0){
        _selected = _items.size();
    }
    _selected--;
    
    _items.at(_selected).IsSelected = true;
    _isDirty = true;
}

uint8_t Menu::Selected()
{
    return _selected;
}

String Menu::SelectedText()
{
    return _items.at(_selected).Text;
}

void Menu::Enter()
{
    if(IsEntered){
        return;
    }
    
    _renderer->ClearScreen();
    IsEntered = true;
}

void Menu::Exit()
{
    if(!IsEntered){
        return;
    }

    IsEntered = false;
    _isDirty = true;
}

void Menu::Render()
{
    if(_isDirty){
        _renderer->Render();
        _isDirty = false;
    }
    else if (IsEntered){
        _items.at(_selected).DisplayFunction();
    }
}

void Menu::SelectedTopLeft(uint8_t* x, uint8_t* y)
{
    ItemBounds ib = _renderer->GetSelectedItemBounds();
    *x = ib.Left;
    *y = ib.Top;
}

MenuRenderer::MenuRenderer(){};