/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _TFT_TEXTBOX_H    /* Guard against multiple inclusion */
#define _TFT_TEXTBOX_H


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
#define MAX_TEXTBOX	20	// Maximum number of text boxes available at one time
#define MAX_LABEL   20  // Max number of labels availabel at one time
  
// Define presets for text box status
#define TXT_BOX_DISABLED		0x0001
#define TXT_BOX_NO_BORDER		0x0010
#define TXT_BOX_UNUSED			0x8000

    
// Define presets for labels status
#define LABEL_DISABLED		0x0001
#define LABEL_NO_BORDER		0x0010
#define LABEL_UNUSED		0x8000

/* Text box data structure*/
typedef struct
{
  uint16_t			pos_x, pos_y, width, height;
  uint16_t			flags;
  char				*label;
  uint16_t          _color_text, 
                    _color_text_inactive, 
                    _color_background, 
                    _color_border, 
                    _color_hilite;
  uint8_t           *_font_text,
                    *_font_param_text;
} TextBox_type;
TextBox_type	textbox[MAX_TEXTBOX];
typedef   uint8_t TextBoxID_type;

/* Label Data Structure */
typedef struct
{
  uint16_t			pos_x, pos_y, width, height;
  uint16_t			flags;
  char				*label;
  uint16_t          _color_text, 
                    _color_text_inactive, 
                    _color_background, 
                    _color_border, 
                    _color_hilite;
  uint8_t           *_font_text,
                    *_font_param_text;
} Label_type;
Label_type	Label[MAX_TEXTBOX];
typedef   uint8_t LabelID_type;

/* Functions prototype for text boxes */
void                tft32_TextBox_init(void);
TextBoxID_type		tft32_addTextBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags);
void                tft32_drawTextBoxes(void);
void                tft32_drawTextBox(TextBoxID_type id);
void                tft32_enableTextBox(TextBoxID_type id, BOOL redraw);
void                tft32_disableTextBox(TextBoxID_type id, BOOL redraw);
void                tft32_updateTextBox(TextBoxID_type id, char *label);
BOOL                tft32_TextBoxEnabled(TextBoxID_type id);
void                tft32_deleteTextBox(TextBoxID_type id);
void                tft32_deleteAllTextBoxes(void);
void                tft32_setTextBoxFont(TextBoxID_type id,uint8_t* font,uint8_t* font_p);
void                tft32_setTextBoxColors(TextBoxID_type id,uint16_t atxt, uint16_t iatxt, uint16_t brd, uint16_t brdhi, uint16_t back);


/* Functions prototype for Labels */
void                tft32_Label_init(void);
LabelID_type		tft32_addLabel(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags);
void                tft32_drawLabels(void);
void                tft32_drawLabel(LabelID_type id);
void                tft32_enableLabel(LabelID_type id, BOOL redraw);
void                tft32_disableLabel(LabelID_type id, BOOL redraw);
void                tft32_updateLabel(LabelID_type id, char *label);
BOOL                tft32_LabelEnabled(LabelID_type id);
void                tft32_deleteLabel(LabelID_type id);
void                tft32_deleteAllLabels(void);
void                tft32_setLabelFont(LabelID_type id,uint8_t* font,uint8_t* font_p);
void                tft32_setLabelColors(LabelID_type id,uint16_t atxt, uint16_t iatxt, uint16_t brd, uint16_t brdhi, uint16_t back);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
