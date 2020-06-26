
#include <grfx/tft_TextBox.h>
#include <stdint.h>
#include <string.h>
#include "grfx/ssd1289.h"

void tft32_TextBox_init(void)
{
    tft32_deleteAllTextBoxes();
}

TextBoxID_type tft32_addTextBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags)
{
    TextBoxID_type txtbcnt = 0;

    while (((textbox[txtbcnt].flags & TXT_BOX_UNUSED) == 0) && (txtbcnt < MAX_TEXTBOX))
        txtbcnt++;

    if (txtbcnt == MAX_TEXTBOX)
        return -1;
    else
    {
        textbox[txtbcnt].pos_x = x;
        textbox[txtbcnt].pos_y = y;
        textbox[txtbcnt].width = width;
        textbox[txtbcnt].height = height;
        textbox[txtbcnt].flags = flags;
        textbox[txtbcnt].label = label;
        return txtbcnt;
    }

}

/* Draw all buttons in array */
void tft32_drawTextBoxes(void)
{
    TextBoxID_type i;
    for (i = 0; i < MAX_TEXTBOX; i++)
    {
        if ((textbox[i].flags & TXT_BOX_UNUSED) == 0)
            tft32_drawTextBox(i);
    }
}

void tft32_drawTextBox(TextBoxID_type id)
{
    int text_x, text_y;
    uint8_t *_font_current = tft32_getFont();
    uint8_t *_font_current_param = tft32_getFont_param();
    uint16_t _current_color = tft32_getColor();
    uint16_t _current_back = tft32_getBackColor();

    if (!(textbox[id].flags & TXT_BOX_UNUSED))
    {
        tft32_setColor(textbox[id]._color_background);
        tft32_fillRoundRect(textbox[id].pos_x, textbox[id].pos_y,
                            textbox[id].pos_x + textbox[id].width,
                            textbox[id].pos_y + textbox[id].height);
        
        tft32_setColor(textbox[id]._color_border);
        
        if (!(textbox[id].flags & TXT_BOX_NO_BORDER))
            tft32_drawRoundRect(textbox[id].pos_x, textbox[id].pos_y, 
                                textbox[id].pos_x + textbox[id].width,
                                textbox[id].pos_y + textbox[id].height);

        if (textbox[id].flags & TXT_BOX_DISABLED)
            tft32_setColor(textbox[id]._color_text_inactive);
        else
            tft32_setColor(textbox[id]._color_text);

        tft32_setFont(textbox[id]._font_text, textbox[id]._font_param_text);
        text_x = ((textbox[id].width / 2) - ((strlen(textbox[id].label) * tft32_getFontXsize()) / 2)) + textbox[id].pos_x;
        text_y = (textbox[id].height / 2) - (tft32_getFontYsize() / 2) + textbox[id].pos_y;
        tft32_setBackColor(textbox[id]._color_background);
        tft32_print(textbox[id].label, text_x, text_y, 0);
    }
    tft32_setFont(_font_current, _font_current_param);
    tft32_setColor(_current_color);
    tft32_setBackColor(_current_back);
}

void tft32_enableTextBox(TextBoxID_type id, BOOL redraw)
{
    if (!(textbox[id].flags & TXT_BOX_UNUSED))
    {
        textbox[id].flags = textbox[id].flags & ~TXT_BOX_DISABLED;
        if (redraw)
            tft32_drawTextBox(id);
    }
}

void tft32_disableTextBox(TextBoxID_type id, BOOL redraw)
{

    if (!(textbox[id].flags & TXT_BOX_UNUSED))
    {
        textbox[id].flags = textbox[id].flags | TXT_BOX_DISABLED;
        if (redraw)
            tft32_drawTextBox(id);
    }
}

void tft32_updateTextBox(TextBoxID_type id, char *label)
{
    int     text_x, text_y;
    uint8_t *_font_current          = tft32_getFont();
    uint8_t *_font_current_param    = tft32_getFont_param();
    uint16_t _current_color         = tft32_getColor();
    uint16_t _current_back          = tft32_getBackColor();

    if (!(textbox[id].flags & TXT_BOX_UNUSED))
    {
        textbox[id].label = label;
        
        if (textbox[id].flags & TXT_BOX_DISABLED)
            tft32_setColor(textbox[id]._color_text_inactive);
        else
            tft32_setColor(textbox[id]._color_text);

        tft32_setFont(textbox[id]._font_text, textbox[id]._font_param_text);
        text_x = ((textbox[id].width / 2) - ((strlen(textbox[id].label) * tft32_getFontXsize()) / 2)) + textbox[id].pos_x;
        text_y = (textbox[id].height / 2) - (tft32_getFontYsize() / 2) + textbox[id].pos_y;
        tft32_setBackColor(textbox[id]._color_background);
        tft32_print(textbox[id].label, text_x, text_y, 0);
    }
    tft32_setFont(_font_current, _font_current_param);
    tft32_setColor(_current_color);
    tft32_setBackColor(_current_back);

}

BOOL tft32_TextBoxEnabled(TextBoxID_type id)
{
    return !(textbox[id].flags & TXT_BOX_DISABLED);
}

void tft32_deleteTextBox(TextBoxID_type id)
{
    if (!(textbox[id].flags & TXT_BOX_UNUSED))
        textbox[id].flags = TXT_BOX_UNUSED;
}

void tft32_deleteAllTextBoxes(void)
{
    TextBoxID_type i;
    for (i = 0; i < MAX_TEXTBOX; i++)
    {
        textbox[i].pos_x = 0;
        textbox[i].pos_y = 0;
        textbox[i].width = 0;
        textbox[i].height = 0;
        textbox[i].flags = TXT_BOX_UNUSED;
        textbox[i].label = "";
    }

}

void tft32_setTextBoxColors(TextBoxID_type id, uint16_t atxt, uint16_t iatxt, uint16_t brd, uint16_t brdhi, uint16_t back)
{
    textbox[id]._color_text = atxt;
    textbox[id]._color_text_inactive = iatxt;
    textbox[id]._color_background = back;
    textbox[id]._color_border = brd;
    textbox[id]._color_hilite = brdhi;
}

void tft32_setTextBoxFont(TextBoxID_type id, uint8_t* font, uint8_t* font_p)
{
    textbox[id]._font_text = font;
    textbox[id]._font_param_text = font_p;
}


/******************************************************************************/
/*  Labels functions start here                                               */
/******************************************************************************/

void tft32_Label_init(void)
{
    tft32_deleteAllLabels();
}

LabelID_type tft32_addLabel(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags)
{
    LabelID_type lblbcnt = 0;

    while (((Label[lblbcnt].flags & LABEL_UNUSED) == 0) && (lblbcnt < MAX_LABEL))
        lblbcnt++;

    if (lblbcnt == MAX_LABEL)
        return -1;
    else
    {
        Label[lblbcnt].pos_x = x;
        Label[lblbcnt].pos_y = y;
        Label[lblbcnt].width = width;
        Label[lblbcnt].height = height;
        Label[lblbcnt].flags = flags;
        Label[lblbcnt].label = label;
        return lblbcnt;
    }

}

/* Draw all buttons in array */
void tft32_drawLabels(void)
{
    LabelID_type i;
    for (i = 0; i < MAX_LABEL; i++)
    {
        if ((Label[i].flags & LABEL_UNUSED) == 0)
            tft32_drawLabel(i);
    }
}

void tft32_drawLabel(LabelID_type id)
{
    int text_x, text_y;
    uint8_t *_font_current = tft32_getFont();
    uint8_t *_font_current_param = tft32_getFont_param();
    uint16_t _current_color = tft32_getColor();
    uint16_t _current_back = tft32_getBackColor();

    if (!(Label[id].flags & LABEL_UNUSED))
    {
        tft32_setColor(Label[id]._color_background);
        tft32_fillRoundRect(Label[id].pos_x, Label[id].pos_y,
                            Label[id].pos_x + Label[id].width,
                            Label[id].pos_y + Label[id].height);
        
        
        if (!(Label[id].flags & LABEL_NO_BORDER))
        {
            tft32_setColor(Label[id]._color_border);
            tft32_drawRoundRect(Label[id].pos_x, Label[id].pos_y, 
                                Label[id].pos_x + Label[id].width,
                                Label[id].pos_y + Label[id].height);
        }

        if (Label[id].flags & LABEL_DISABLED)
            tft32_setColor(Label[id]._color_text_inactive);
        else
            tft32_setColor(Label[id]._color_text);

        tft32_setFont(Label[id]._font_text, Label[id]._font_param_text);
        text_x = ((Label[id].width / 2) - ((strlen(Label[id].label) * tft32_getFontXsize()) / 2)) + Label[id].pos_x;
        text_y = (Label[id].height / 2) - (tft32_getFontYsize() / 2) + Label[id].pos_y;
        tft32_setBackColor(Label[id]._color_background);
        tft32_print(Label[id].label, text_x, text_y, 0);
    }
    tft32_setFont(_font_current, _font_current_param);
    tft32_setColor(_current_color);
    tft32_setBackColor(_current_back);
}

void tft32_enableLabel(LabelID_type id, BOOL redraw)
{
    if (!(Label[id].flags & LABEL_UNUSED))
    {
        Label[id].flags &=  ~LABEL_DISABLED;
        if (redraw)
            tft32_drawLabel(id);
    }
}

void tft32_disableLabel(LabelID_type id, BOOL redraw)
{

    if (!(Label[id].flags & LABEL_UNUSED))
    {
        Label[id].flags = Label[id].flags | LABEL_DISABLED;
        if (redraw)
            tft32_drawLabel(id);
    }
}

void tft32_updateLabel(LabelID_type id, char *label)
{
    int     text_x, text_y;
    uint8_t *_font_current          = tft32_getFont();
    uint8_t *_font_current_param    = tft32_getFont_param();
    uint16_t _current_color         = tft32_getColor();
    uint16_t _current_back          = tft32_getBackColor();

    if (!(Label[id].flags & LABEL_UNUSED))
    {
        Label[id].label = label;
        
        if (Label[id].flags & LABEL_DISABLED)
            tft32_setColor(Label[id]._color_text_inactive);
        else
            tft32_setColor(Label[id]._color_text);

        tft32_setFont(Label[id]._font_text, Label[id]._font_param_text);
        text_x = ((Label[id].width / 2) - ((strlen(Label[id].label) * tft32_getFontXsize()) / 2)) + Label[id].pos_x;
        text_y = (Label[id].height / 2) - (tft32_getFontYsize() / 2) + Label[id].pos_y;
        tft32_setBackColor(Label[id]._color_background);
        tft32_print(Label[id].label, text_x, text_y, 0);
    }
    tft32_setFont(_font_current, _font_current_param);
    tft32_setColor(_current_color);
    tft32_setBackColor(_current_back);

}

BOOL tft32_LabelEnabled(LabelID_type id)
{
    return !(Label[id].flags & LABEL_DISABLED);
}

void tft32_deleteLabel(LabelID_type id)
{
    if (!(Label[id].flags & LABEL_UNUSED))
        Label[id].flags = LABEL_UNUSED;
}

void tft32_deleteAllLabels(void)
{
    LabelID_type i;
    for (i = 0; i < MAX_TEXTBOX; i++)
    {
        Label[i].pos_x = 0;
        Label[i].pos_y = 0;
        Label[i].width = 0;
        Label[i].height = 0;
        Label[i].flags = LABEL_UNUSED;
        Label[i].label = "";
    }

}

void tft32_setLabelColors(LabelID_type id, uint16_t atxt, uint16_t iatxt, uint16_t brd, uint16_t brdhi, uint16_t back)
{
    Label[id]._color_text = atxt;
    Label[id]._color_text_inactive = iatxt;
    Label[id]._color_background = back;
    Label[id]._color_border = brd;
    Label[id]._color_hilite = brdhi;
}

void tft32_setLabelFont(LabelID_type id, uint8_t* font, uint8_t* font_p)
{
    Label[id]._font_text = font;
    Label[id]._font_param_text = font_p;
}
