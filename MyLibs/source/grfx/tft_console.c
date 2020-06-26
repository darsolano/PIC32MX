//#include <grfx/tft_console.h>
//#include <grfx/ssd1289.h>
//
//console_type *console;
//
//void tft32_ConsolePutString( char* str )
//{
//   char chr;
//   uint8_t cw;
//   uint8_t *old_font, *old_fontp;
//   uint16_t old_fcolor, old_bcolor;
//   
//   old_font = tft32_getFont();
//   old_fontp = tft32_getFont_param();
//   old_fcolor = tft32_getColor();
//   old_bcolor = tft32_getBackColor();
//   
//   tft32_setFont(console->font, console->fontp);
//   tft32_setColor(console->fore_color);
//   tft32_setBackColor(console->back_color);
//
//   while ( *str != 0 )
//   {
//      chr = *str;
//      if ( chr == '\n' )
//      {
//         console->x_pos = console->x_start;
//         str++;
//         continue;
//      }
//      
//      cw = cfont.Hrows ? cfont.Hrows[chr - gui->font.start_char] : cfont.x_size;
//      console->x_pos += cw+gui->char_h_space;
//
//      if ( console->x_pos+cw > console->x_end )
//      {
//         console->x_pos = console->x_start;
//         console->y_pos += cfont.y_size+gui->char_v_space;
//      }
//      if ( gui->console.y_pos+gui->font.char_height > gui->console.y_end )
//      {
//         console->x_pos = console->x_start;
//         console->y_pos = console->y_start;
//         UG_FillFrame(gui->console.x_start,gui->console.y_start,gui->console.x_end,gui->console.y_end,gui->console.back_color);
//      }
//
//      UG_PutChar(chr, gui->console.x_pos, gui->console.y_pos, gui->console.fore_color, gui->console.back_color);
//      str++;
//   }
//}
//
//void tft32_ConsoleSetArea( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
//{
//   gui->console.x_start = xs;
//   gui->console.y_start = ys;
//   gui->console.x_end = xe;
//   gui->console.y_end = ye;
//}
//
//void tft32_ConsoleSetForecolor( UG_COLOR c )
//{
//   gui->console.fore_color = c;
//}
//
//void tft32_ConsoleSetBackcolor( UG_COLOR c )
//{
//   gui->console.back_color = c;
//}
//
