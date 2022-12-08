/*
Library for creating and handling a menu
*/
#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include <vector>
 
using namespace std;

typedef void (*DisplayFn)();
struct MenuItem
{
    MenuItem(String text, DisplayFn displayFunction)
    {
        Text = text;
        DisplayFunction = displayFunction;
    }
    bool IsSelected = false;
    String Text;
    DisplayFn DisplayFunction;
};

struct ItemBounds
{
    ItemBounds(uint8_t w, uint8_t h, uint8_t l, uint8_t t)
    {
        Width = w;
        Height = h;
        Left = l;
        Right = l + w;
        Top = t;
        Bottom = t + h;
    }

    uint8_t Width;
    uint8_t Height;
    uint8_t Top;
    uint8_t Bottom;
    uint8_t Left;
    uint8_t Right;
};

class MenuRenderer
{
    public:
        MenuRenderer();
        virtual void Init(vector<MenuItem>* menuItems, uint8_t* selectedIndex) = 0;
        virtual ~MenuRenderer(){};
        virtual void Render() = 0;
        virtual void ClearScreen() = 0;
        virtual ItemBounds GetSelectedItemBounds() = 0;
    protected:
        virtual ItemBounds RenderItem(uint8_t index) = 0;
        vector<MenuItem>* _menuItems;
        uint8_t* _selectedIndex;
};

class Menu 
{
    public:
        Menu();
        /// @brief class to use to render the menu to a display
        /// @param renderer Must inherit from MenuRenderer
        void UseRenderer(MenuRenderer* renderer);

        /// @brief Draws the menu or selected user screen to the display
        void Render();

        /// @brief Add a menu item
        /// @param text Text to display
        /// @param displayFunction Function that will be called when this item is selected and 'Enter'ed
        /// @return Number of menu items
        size_t Add(String text, DisplayFn displayFunction);

        /// @brief Move to the next menu item
        void Next();

        /// @brief Move to the previous menu item
        void Previous();

        /// @brief Gets the index of the selected menu item
        /// @return Index of the selected menu item
        uint8_t Selected();

        /// @brief Gets the text of the selected menu item
        /// @return Text of the selected menu item
        String SelectedText();

        /// @brief Display the selected menu screen
        void Enter();

        /// @brief Return to menu selection
        void Exit();

        /// @brief Indicates if the menu is in an 'Entered' state, and is currently
        /// displaying a user-defined screen
        bool IsEntered = false;

        void SelectedTopLeft(uint8_t* x, uint8_t* y);

    private:
        uint8_t _selected;
        vector<MenuItem> _items;
        MenuRenderer* _renderer;
        bool _isDirty;
};

#endif