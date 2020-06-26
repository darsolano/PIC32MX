/*'
' Name    : xGLCD Library
'
' Author  : M. Ahmed Lazreg
'           octal@pocketmt.com
'
'           Copyright 2007 - Pocket MicroTechnics
'           http://www.pocketmt.com
'
' Version : 1.0.0
' Date    : 20/07/2007
'
' Software: Compiled Using mikroC v6.2.1.0
'
'
' Desc.   : xGLCD Library adds support of BIG Proportional font sizes to GLCD Lib.
'
' */


#define TRUE 1
#define FALSE 0

const unsigned short xColorClear = 0;
const unsigned short xColorSet   = 1;
const unsigned short xColorInvert= 2;


const char* xGlcdSelFont;

unsigned short  xGlcdX, xGlcdY, xGlcdSelFontHeight,
                xGlcdSelFontWidth, xGlcdSelFontOffset,
                xGlcdSelFontNbRows;

char xGLCD_Transparency;

void xGlcd_Set_Font(const char* ptrFontTbl, unsigned short font_width,
                        unsigned short font_height, unsigned int font_offset)
{
   xGlcdSelFont = ptrFontTbl;
   xGlcdSelFontWidth = font_width;
   xGlcdSelFontHeight = font_height;
   xGlcdSelFontOffset = font_offset;
   xGLCD_Transparency  = FALSE;  //Transparency of Text is set to False !!!
   xGlcdSelFontNbRows = xGlcdSelFontHeight / 8;
   if (xGlcdSelFontHeight  % 8) xGlcdSelFontNbRows++;
}


void xGLCD_Write_Data(unsigned short pX, unsigned short pY, unsigned short pData)
{
  unsigned short tmp, tmpY, gData, dataR, xx, yy;

  if ((pX>127) || (pY>63)) return;
  xx = pX % 64;
  tmp = pY / 8;
  if (tmp>7) return;
  tmpY = pY % 8;
  if (tmpY) {
    //Write first group
    gData = pData << tmpY;
    Glcd_Set_Side(pX);
    Glcd_Set_X(xx);
    Glcd_Set_Page(tmp);
    dataR = Glcd_Read_Data();
    dataR = Glcd_Read_Data();
    if (!xGLCD_Transparency)
        dataR = dataR & (0xff >> (8-tmpY));
    dataR = gData | dataR;
    Glcd_Set_X(xx);
    Glcd_Write_Data(dataR);
    //Write Second group
    tmp++;
    if (tmp>7) return;
    Glcd_Set_X(xx);
    Glcd_Set_Page(tmp);
    gData = pData >> (8-tmpY);
    dataR = Glcd_Read_Data();
    dataR = Glcd_Read_Data();
    if (!xGLCD_Transparency)
        dataR = dataR & (0xff << tmpY);
    dataR = gData | dataR;
    Glcd_Set_X(xx);
    Glcd_Write_Data(dataR);
  }
  else {
    Glcd_Set_Side(pX);
    Glcd_Set_X(xx);
    Glcd_Set_Page(tmp);
    if (xGLCD_Transparency){
        dataR = Glcd_Read_Data();
        dataR = Glcd_Read_Data();
        dataR = pData | dataR;
    }
    else
      dataR = pData;
    Glcd_Set_X(xx);
    Glcd_Write_Data(dataR);
  }
}

unsigned short xGlcd_Write_Char(unsigned short  ch, unsigned short x, unsigned short y, unsigned short color)
{
  const char* CurCharData;
  unsigned short  i, j, CharWidth, CharData;
  unsigned long cOffset;
  
  cOffset = xGlcdSelFontWidth * xGlcdSelFontNbRows+1;
  cOffset = cOffset * (ch-xGlcdSelFontOffset);
  CurCharData = xGlcdSelFont+cOffset;
  CharWidth = *CurCharData;
  cOffset++;
  for (i = 0; i< CharWidth; i++)
    for (j = 0; j<xGlcdSelFontNbRows; j++)
    {
        CurCharData = xGlcdSelFont+(i*xGlcdSelFontNbRows)+j+cOffset;
        switch (color){
          case 0 : CharData = 0; break;
          case 1 : CharData = *CurCharData; break;
          case 2 : CharData =  ~(*CurCharData); break;
        }
        xGLCD_Write_Data(x+i,y+j*8,CharData);
    };
  return CharWidth;
}

unsigned short xGlcd_Char_Width(unsigned short ch)
{
  const char* CurCharDt;
  unsigned long cOffset;
  cOffset = xGlcdSelFontWidth * xGlcdSelFontNbRows+1;
  cOffset = cOffset * (ch-xGlcdSelFontOffset);
  CurCharDt = xGlcdSelFont+cOffset;
  return *CurCharDt;
}

void xGlcd_Write_Text(char* text, unsigned short x, unsigned short y, unsigned short color)
{
  unsigned short i, l, posX;
  char* curChar;
  l = strlen(text);
  posX = x;
  curChar = text;
  for (i=0; i<l; i++){
      posX = posX + xGlcd_Write_Char(*curChar, posX,y, color) +  1; //add 1 blank column
      curChar++;
  }
}

unsigned short xGlcd_Text_Width(char* text)
{
  unsigned short i, l, w;
  l = strlen(text);
  w = 0;
  for (i = 0; i<l; i++)
      w = w + xGlcd_Char_Width(text[i])+1; //add 1 blank column
  return w;
}

void xGLCD_Set_Transparency(char active)
{
  xGLCD_Transparency = active;
}
