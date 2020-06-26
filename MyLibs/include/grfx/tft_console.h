
#ifndef _TFT_CONSOLE_H    /* Guard against multiple inclusion */
#define _TFT_CONSOLE_H

#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

typedef struct {
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;
    uint16_t fore_color;
    uint16_t back_color;
    uint8_t* font;
    uint8_t* fontp;
} console_type;



void tft32_ConsolePutString(char* str);
void tft32_ConsoleSetArea(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
void tft32_ConsoleSetForecolor(uint16_t c);
void tft32_ConsoleSetBackcolor(uint16_t c);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
