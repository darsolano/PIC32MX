
#ifndef _LCD_MENU_H    /* Guard against multiple inclusion */
#define _LCD_MENU_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ITEMxMENU           10
#define MAX_MENUS               5
#define MAX_ITEMS_PER_PAGE      1
#define DEVICE_WIDTH            16
#define DEVICE_HEIGHT           2
#define MAX_MENU_NAME_LEN       12
#define MAX_ITEM_NAME_LEN       12
    
#define INPUT_DEVICE_ROTARY_SW  0
#define INPUT_DEVICE_KEYPAD     0
#define INPUT_DEVICE_BUTTONS    0
    
#define LCD_PROMPT              '>'
#define CHAR_LEFT               0x7F
#define CHAR_RIGHT              0x7E

    typedef void (*fpCommand_t)(void);          // Prototype function for command in items
    typedef uint8_t MenuID;                     // Type for Menu ID
    typedef uint8_t MenuItemID;                 // Type for Item ID

    /*
     * Constant for State Menu Handler
     */
    typedef enum
    {
        MENU_WAIT = 0x80,
        MENU_INTRO,
        MENU_SHOW,
        MENU_SUB_MENU,
        MENU_PREV_ITEM,
        MENU_NEXT_ITEM,
        MENU_GO_BACK,
        MENU_CANCEL,
        MENU_IN_APP,
        MENU_EXEC_CMD,
        MENU_DRAW,
        MENU_CURSOR_DRAW,
        MENU_INTRO_DRAW
    }ACTION_STATE_t;
    
    
    /*
     * Menu Flags
     */
    typedef enum
    {
        MNU_ENABLE=90,
        MNU_UNUSED = 91,
        MNU_DISABLE =92,
        ITEM_ENABLE = 93,
        ITEM_UNUSED =94
    }MENU_FLAGS_t;
    
   
    /*
     * Data Structure for Menu handler
     */
    typedef struct MENU_s
    {
        MenuID              CURR_MENU;                  // Current Menu pointed
        MenuItemID          CURR_ITEM;                  // Current Sub MEnu item pointed
        MenuID              PREV_MENU;
        MenuItemID          PREV_ITEM;
        int                 PREV_CURSOR;
        ACTION_STATE_t      STATE;                      // Current State for menu actions
        int                 CURR_PAGE;                  // Current page displayed in the LCD
        int                 CURR_MENU_AMNT_ITEMS;       // Current Menu amount of items
        int                 CURSOR_LINE;                // Current Y pos for cursor and items
    }MENU_HANDLER_t;
    MENU_HANDLER_t menuHdlr;
    
    
    /*
     *  Data Structure for Commands
     */
    typedef struct COMMANDS_s
    {
        MenuID      MENU_ID;
        MenuItemID  ITEM_ID;
        fpCommand_t cb_ItemCommand;                // Function pointer to command function
        int         ARG_NUMB;                   // Number of arguments to take
        uint8_t     args[4];                    // Argument array
    }COMMANDS_t;

    /*
     * Items inside Menus, also Commands
     * Parent Menus when executed show child
     * menu items
     */
    typedef struct MENU_ITEMS_s
    {
        MenuID      MENU_ID;                     // Parent Menu iD
        MenuItemID  ITEM_ID;                     // Number that define the command, also can be used to fetch
        uint8_t*    ITEM_NAME;                   // string to define the actual command to be fetched
        COMMANDS_t  ITEM_CMD;                   // Each item have a Command associated
        BOOL        IS_A_MENU;                  // if true it indicates that it is a sub menu header
        MenuID      SUBMENU_ID;                 // ID of the sub menu created that must be shown when this option is selected
        MENU_FLAGS_t ITEM_FLAGS;                // Flags that sets the item status, in use or not in use
    }MENU_ITEMS_t;
    
    /*
     * Many Data Structure, as many menus as the MAX Menu indicate.
     */
    typedef struct MENUS_s
    {
        MenuID       MENU_ID;                   // ID assigned by app
        uint8_t*     MENU_TITLE;                 // Title to be displayed at the top of the device
        MENU_FLAGS_t FLAGS;                     // State Flags for Menu
        int          ITEM_AMOUNT;               // Amount of items that this menu have
        MENU_ITEMS_t ITEM[MAX_ITEMxMENU];       // Items for this Menus
    }MENUS_t;
    MENUS_t MenuArray[MAX_MENUS];
    
    
    
    /*
     * Init al variables in the menu handler structure
     * And menu, menu otems
     */
    void            menu_Initialize(void );
    MenuID          menu_CreateMenu( const uint8_t* title);
    
    /*
     *     // Name no more than 6 characters for fitting purpose
     *     // will be rejected at run time
     */
    MenuItemID      menu_AddMenuItem(MenuID menuid, const char* item_name, BOOL Is_Menu, MenuID submenuid);
    void            menu_AddCommandToItems(MenuID menuID, MenuItemID itemID, fpCommand_t command, int arg_number);
    void            menu_DeleteAllMenu_And_Items(void);
    ACTION_STATE_t  menu_Check_Navigation(void);
    void            menu_Set_CurrentMenu(MenuID mnuID, MenuItemID itemid, int cursor);
    void            menu_CheckMenuSelection(void);
    
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
