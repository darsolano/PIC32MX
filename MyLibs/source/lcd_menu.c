#include <lcd_menu.h>
#include <stdint.h>
#include <GenericTypeDefs.h>
#include <pushButtons.h>
#include <lcd_hd44780.h>
#include <lcd_hd44780_special_chars.h>
#include <string.h>

#define CHAR_DOWN   0x06
#define CHAR_UP     0x07
MENU_HANDLER_t  *pMnuhldr  = &menuHdlr;
extern const char UP[];
extern const char DOWN[];


void menu_Initialize(void )
{
    pb_Init();
#if MAX_ITEMS_PER_PAGE==3
    lcd_HD44780_CreateChar((uint8_t)CHAR_DOWN, (uint8_t*)SPECIAL_CHARS+SPC_CHAR_DOWN);
    lcd_HD44780_CreateChar((uint8_t)CHAR_UP, (uint8_t*)SPECIAL_CHARS+SPC_CHAR_UP);
#endif
//    
#if MAX_ITEMS_PER_PAGE==1
    lcd_HD44780_CreateChar((uint8_t)CHAR_DOWN, (uint8_t*)SPECIAL_CHARS+SPC_CHAR_DOWN);
    lcd_HD44780_CreateChar((uint8_t)CHAR_UP, (uint8_t*)SPECIAL_CHARS+SPC_CHAR_UP);
#endif

    menu_DeleteAllMenu_And_Items();
    pMnuhldr->CURR_ITEM=0;
    pMnuhldr->CURR_MENU=0;
    pMnuhldr->CURR_MENU_AMNT_ITEMS=0;
    pMnuhldr->CURR_PAGE=0;
    pMnuhldr->STATE = MENU_INTRO_DRAW;
    
}

MenuItemID menu_AddMenuItem(MenuID menuid, const char* item_name, BOOL Is_Menu, MenuID submenuid)
{
    MenuItemID itemid=0;
    if (strlen(item_name) > MAX_ITEM_NAME_LEN) return -1;
    while (MenuArray[menuid].ITEM[itemid].ITEM_FLAGS == ITEM_ENABLE && itemid<MAX_ITEMxMENU) itemid++;
    
    if (itemid == MAX_ITEMxMENU) 
        return -1;
    else
    {
        MenuArray[menuid].ITEM[itemid].ITEM_NAME = (uint8_t*)item_name;
        MenuArray[menuid].ITEM[itemid].ITEM_ID = itemid;
        MenuArray[menuid].ITEM[itemid].IS_A_MENU = Is_Menu;
        MenuArray[menuid].ITEM[itemid].MENU_ID = submenuid;
        MenuArray[menuid].ITEM[itemid].ITEM_FLAGS = ITEM_ENABLE;
        MenuArray[menuid].ITEM_AMOUNT++;
        return itemid;
    }
}

void menu_AddCommandToItems(MenuID menuID, MenuItemID itemID, fpCommand_t command, int arg_number)
{
    MenuArray[menuID].ITEM[itemID].ITEM_CMD.cb_ItemCommand = command;
    MenuArray[menuID].ITEM[itemID].ITEM_CMD.ARG_NUMB = arg_number;
}

void menu_DeleteAllMenu_And_Items(void)
{
    int m,i = 0;
    for ( m=0;m<MAX_MENUS;m++)
    {
        MenuArray[m].FLAGS=0;
        MenuArray[m].ITEM_AMOUNT=0;
        MenuArray[m].MENU_ID=0;
        MenuArray[m].MENU_TITLE=0;
        MenuArray[m].FLAGS = MNU_UNUSED;
        for (i=0;i<MAX_ITEMxMENU;i++)
        {
            MenuArray[m].ITEM[i].IS_A_MENU=0;
            MenuArray[m].ITEM[i].ITEM_CMD.ARG_NUMB=0;
            MenuArray[m].ITEM[i].ITEM_CMD.cb_ItemCommand=NULL;
            MenuArray[m].ITEM[i].ITEM_CMD.args[0]=0;
            MenuArray[m].ITEM[i].ITEM_CMD.args[1]=0;
            MenuArray[m].ITEM[i].ITEM_CMD.args[2]=0;
            MenuArray[m].ITEM[i].ITEM_CMD.args[3]=0;
            MenuArray[m].ITEM[i].ITEM_ID=0;
            MenuArray[m].ITEM[i].ITEM_NAME=0;
            MenuArray[m].ITEM[i].MENU_ID=0;
            MenuArray[m].ITEM[i].ITEM_FLAGS = ITEM_UNUSED;
        }
        
    }
}

/*
 * Only called to start or reset not always
 */
void menu_Set_CurrentMenu(MenuID mnuID, MenuItemID itemid, int cursor)
{
    pMnuhldr->CURR_MENU = MenuArray[mnuID].MENU_ID;
    pMnuhldr->CURR_ITEM = MenuArray[mnuID].ITEM[itemid].ITEM_ID;
    pMnuhldr->CURSOR_LINE = cursor;
    pMnuhldr->CURR_MENU_AMNT_ITEMS = MenuArray[mnuID].ITEM_AMOUNT;
    pMnuhldr->CURSOR_LINE = 2;
    pMnuhldr->PREV_ITEM = 0;
    pMnuhldr->PREV_MENU = 0;
    pMnuhldr->PREV_CURSOR = 0;
}

/*
 * THis function will draw any Menu from scratch
 * As if were the first time it was called
 * Based on the MnuID and ItemID
 */
void menu_MenuDraw(MenuID mnuid,MenuItemID itemid, int cursor)
{
    int i = 0;
    int y_coord = 2;
    lcd_HD44780_Clear();
    lcd_HD44780_Puts(1,1,MenuArray[mnuid].MENU_TITLE);
    for (i=0;i<MAX_ITEMS_PER_PAGE;i++)
    {
        if (MenuArray[mnuid].ITEM[itemid].ITEM_FLAGS == ITEM_ENABLE && MenuArray[mnuid].ITEM_AMOUNT>itemid+i){
            lcd_HD44780_Puts(2 , y_coord+i, MenuArray[mnuid].ITEM[itemid+i].ITEM_NAME);
            if (MenuArray[mnuid].ITEM[itemid+i].IS_A_MENU) lcd_HD44780_putChar(2+strlen(MenuArray[mnuid].ITEM[itemid+i].ITEM_NAME) , y_coord+i, CHAR_RIGHT);
        }
        else
        {
            lcd_HD44780_Puts(2 , y_coord+i, "BACK");
            lcd_HD44780_putChar(7 , y_coord+i,CHAR_LEFT);
            pMnuhldr->STATE = MENU_GO_BACK;
            break;
        }
    }
}

/*
 * Cursor will only be shown between line 2 and 4
 * Once it transition to next or previous item 
 * the cursor need to be erased from the later 
 * position
 */
void menu_CursorDraw(int y_pos)
{
    // Display Prompt
    if ( y_pos < 2 || y_pos > MAX_ITEMS_PER_PAGE+1) return;
    lcd_HD44780_putChar(1, y_pos ,LCD_PROMPT);// Draw cursor next position
}
void menu_CursorErase(int y_pos)
{
    // Erase with blank Prompt
    if ( y_pos < 2 || y_pos > MAX_ITEMS_PER_PAGE+1) return;
    lcd_HD44780_putChar(1, y_pos, ' ');// Delete cursor previous position
}

/*
 * Current Menu and Current Item
 * From here we will go to next item, see that
 * only 3 item per page, if item is not in the page
 * we then fetch for the next item.
 */
void menu_NextItem(MenuID mnuID, MenuItemID itemid, int cursor)
{
    int temp_itemid,i = 0;
    if (itemid == pMnuhldr->CURR_MENU_AMNT_ITEMS) return;
    itemid++;
    pMnuhldr->CURR_ITEM = itemid;
    temp_itemid = itemid-2;
    // To PLace Arrow Down for more next items
    if (pMnuhldr->CURR_ITEM < MenuArray[mnuID].ITEM_AMOUNT)
    {
        lcd_HD44780_putChar(DEVICE_WIDTH,DEVICE_HEIGHT,CHAR_DOWN);
    }
    else{
        lcd_HD44780_putChar(DEVICE_WIDTH,DEVICE_HEIGHT,' ');        
    }

    // TO place the arrow up for more items previous
    if (pMnuhldr->CURR_ITEM > 0)
    {
        lcd_HD44780_putChar(DEVICE_WIDTH,DEVICE_HEIGHT/2,CHAR_UP);
    }
    else{
        lcd_HD44780_putChar(DEVICE_WIDTH,DEVICE_HEIGHT/2,' ');        
    }
    
    if (pMnuhldr->CURSOR_LINE >= DEVICE_HEIGHT)
    {
        pMnuhldr->CURSOR_LINE = DEVICE_HEIGHT;
        for (i=0;i<MAX_ITEMS_PER_PAGE;i++)
        {
            if (MenuArray[mnuID].ITEM[temp_itemid+i].ITEM_FLAGS != ITEM_UNUSED)
            {
                lcd_HD44780_ClearLine(MAX_ITEM_NAME_LEN+1,2+i);  // erase previous existing line
                lcd_HD44780_Puts(2, 2+i, MenuArray[mnuID].ITEM[temp_itemid+i].ITEM_NAME); // Actually print the current string for the item
                if (MenuArray[mnuID].ITEM[temp_itemid+i].IS_A_MENU) lcd_HD44780_putChar(2+(strlen(MenuArray[mnuID].ITEM[temp_itemid+i].ITEM_NAME)),2+i, CHAR_RIGHT);    // If item calls a menu it indicate it with an arrow right
                menu_CursorDraw(pMnuhldr->CURSOR_LINE);     //Print cursor at proper line
            }
            else {
                lcd_HD44780_ClearLine(7,2+i);
                lcd_HD44780_Puts(2,2+i,"BACK");
                lcd_HD44780_putChar(7, 2+i, CHAR_LEFT);
                menu_CursorDraw(pMnuhldr->CURSOR_LINE);
                pMnuhldr->STATE = MENU_GO_BACK;
                break;
            }
        }
    }
    else{
        menu_CursorErase(pMnuhldr->CURSOR_LINE++);
        menu_CursorDraw(pMnuhldr->CURSOR_LINE);
    }
}

void menu_PrevItem(MenuID mnuID, MenuItemID itemid, int cursor)
{
    int temp_itemid,i = 0;
    if (itemid == 0) return;
    itemid--;
    pMnuhldr->CURR_ITEM = itemid;
    if (itemid < pMnuhldr->CURR_MENU_AMNT_ITEMS-2) temp_itemid = itemid+2;
    else temp_itemid = itemid+1;
    
    // Arrow down for more items next
    if (pMnuhldr->CURR_ITEM < MenuArray[mnuID].ITEM_AMOUNT)
    {
        lcd_HD44780_putChar(DEVICE_WIDTH,DEVICE_HEIGHT,CHAR_DOWN);
    }
    else{
        lcd_HD44780_putChar(DEVICE_WIDTH, DEVICE_HEIGHT,' ');        
    }

    // TO place the arrow up for more items previous
    if (pMnuhldr->CURR_ITEM > 0)
    {
        lcd_HD44780_putChar(DEVICE_WIDTH, DEVICE_HEIGHT/2,CHAR_UP);
    }
    else{
        lcd_HD44780_putChar(DEVICE_WIDTH, DEVICE_HEIGHT/2,' ');        
    }
    
    if (pMnuhldr->CURSOR_LINE <= 2)
    {
        pMnuhldr->CURSOR_LINE = 2;
        for (i=0;i<MAX_ITEMS_PER_PAGE;i++)
        {
            lcd_HD44780_ClearLine(MAX_ITEM_NAME_LEN+1,4-i);  // Clear current line to be printed
            lcd_HD44780_Puts(2,4-i,MenuArray[mnuID].ITEM[temp_itemid-i].ITEM_NAME); // Actually print the command
            if (MenuArray[mnuID].ITEM[temp_itemid-i].IS_A_MENU) lcd_HD44780_putChar(2+strlen(MenuArray[mnuID].ITEM[temp_itemid-i].ITEM_NAME),4-i, CHAR_RIGHT);
            menu_CursorDraw(pMnuhldr->CURSOR_LINE); //be sure that the cursor is present in the line
        }
    }
    else{
        menu_CursorErase(pMnuhldr->CURSOR_LINE--);
        menu_CursorDraw(pMnuhldr->CURSOR_LINE);
    }
}


ACTION_STATE_t menu_Check_Navigation(void)
{
    PUSH_BUTTON_t cmd_for_menu = 0;
    
    cmd_for_menu = pb_Get_PshBtn_State();
    
    switch(cmd_for_menu)
    {
    case PSHBTN_MENU:   // Exit all menu and show main menu
        if (pMnuhldr->STATE != MENU_INTRO_DRAW) break;
        lcd_HD44780_Clear();
        menu_Set_CurrentMenu(0,0,2);
        menu_MenuDraw(pMnuhldr->CURR_MENU , pMnuhldr->CURR_ITEM,pMnuhldr->CURSOR_LINE);
        menu_CursorDraw(pMnuhldr->CURSOR_LINE);
        pMnuhldr->STATE = MENU_WAIT;
        break;
    case PSHBTN_LEFT:   // Goto exit current menu and go back to previous menu
        if (pMnuhldr->STATE != MENU_GO_BACK) break;
        menu_Set_CurrentMenu(pMnuhldr->PREV_MENU,pMnuhldr->PREV_ITEM, pMnuhldr->PREV_CURSOR);
        menu_MenuDraw(pMnuhldr->CURR_MENU , pMnuhldr->CURR_ITEM, pMnuhldr->PREV_CURSOR);
        menu_CursorDraw(pMnuhldr->CURSOR_LINE);
        pMnuhldr->STATE = MENU_WAIT;
        break;
    case PSHBTN_UP:     // Goto previous Item
        menu_PrevItem(pMnuhldr->CURR_MENU, pMnuhldr->CURR_ITEM, pMnuhldr->CURSOR_LINE);
        break;
    case PSHBTN_DOWN:   // goto next item
        menu_NextItem(pMnuhldr->CURR_MENU, pMnuhldr->CURR_ITEM, pMnuhldr->CURSOR_LINE);    // Send the current menu in context and current item
        break;
    case PSHBTN_RIGHT:  // Next Menu shown by item
        pMnuhldr->PREV_MENU = pMnuhldr->CURR_MENU;
        pMnuhldr->PREV_ITEM = pMnuhldr->CURR_ITEM;
        pMnuhldr->PREV_CURSOR = pMnuhldr->CURSOR_LINE;
        if (MenuArray[pMnuhldr->CURR_MENU].ITEM[pMnuhldr->CURR_ITEM].IS_A_MENU)
        {
            menu_Set_CurrentMenu(MenuArray[pMnuhldr->CURR_MENU].ITEM[pMnuhldr->CURR_ITEM].MENU_ID, 0,2);
            menu_MenuDraw(MenuArray[pMnuhldr->CURR_MENU].MENU_ID, pMnuhldr->CURR_ITEM,pMnuhldr->CURSOR_LINE);
            menu_CursorDraw(pMnuhldr->CURSOR_LINE);
        }
        break;
    case PSHBTN_SELECT: // Execute an item command if so...
        pMnuhldr->PREV_MENU = pMnuhldr->CURR_MENU;
        pMnuhldr->PREV_ITEM = pMnuhldr->CURR_ITEM;
        pMnuhldr->PREV_CURSOR = pMnuhldr->CURSOR_LINE;
        
        if ( MenuArray[pMnuhldr->CURR_MENU].ITEM[pMnuhldr->CURR_ITEM].IS_A_MENU == FALSE && 
             MenuArray[pMnuhldr->CURR_MENU].ITEM[pMnuhldr->CURR_ITEM].ITEM_CMD.cb_ItemCommand!= NULL)
        {
            MenuArray[pMnuhldr->CURR_MENU].ITEM[pMnuhldr->CURR_ITEM].ITEM_CMD.cb_ItemCommand();
            
            menu_Set_CurrentMenu(pMnuhldr->PREV_MENU , pMnuhldr->PREV_ITEM,pMnuhldr->PREV_CURSOR);
            menu_MenuDraw(pMnuhldr->CURR_MENU , pMnuhldr->CURR_ITEM, pMnuhldr->CURSOR_LINE);
            menu_CursorDraw(pMnuhldr->CURSOR_LINE);
        }
        break;
    default:
        break;
    }
    return cmd_for_menu;
}