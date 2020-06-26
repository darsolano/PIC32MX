/*'
' Name    : xGLCD Library
'
' Author  : M. Ahmed Lazreg
'           octal@pocketmt.com
'
'           Copyright 2009 - Pocket MicroTechnics
'           http://www.pocketmt.com
'
' Version : 1.1.0
' Date    : 27/09/2009
'
' Software: Compiled Using mikroC v2.50
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


const char* xSpiGlcdSelFont;

unsigned short  xSpiGlcdX, xSpiGlcdY, xSpiGlcdSelFontHeight,
                xSpiGlcdSelFontWidth, xSpiGlcdSelFontOffset,
                xSpiGlcdSelFontNbRows;

char xSpiGLCD_Transparency;

void xSPI_Glcd_Set_Font(const char* ptrFontTbl, unsigned short font_width,
                        unsigned short font_height, unsigned int font_offset)
{
   xSpiGlcdSelFont = ptrFontTbl;
   xSpiGlcdSelFontWidth = font_width;
   xSpiGlcdSelFontHeight = font_height;
   xSpiGlcdSelFontOffset = font_offset;
   xSpiGLCD_Transparency  = FALSE;  //Transparency of Text is set to False !!!
   xSpiGlcdSelFontNbRows = xSpiGlcdSelFontHeight / 8;
   if (xSpiGlcdSelFontHeight  % 8) xSpiGlcdSelFontNbRows++;
}


void xSPI_GLCD_Write_Data(unsigned short pX, unsigned short pY, unsigned short pData)
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
    SPI_Glcd_Set_Side(pX);
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Set_Page(tmp);
    dataR = SPI_Glcd_Read_Data();
    dataR = SPI_Glcd_Read_Data();
    if (!xSpiGLCD_Transparency)
        dataR = dataR & (0xff >> (8-tmpY));
    dataR = gData | dataR;
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Write_Data(dataR);
    //Write Second group
    tmp++;
    if (tmp>7) return;
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Set_Page(tmp);
    gData = pData >> (8-tmpY);
    dataR = SPI_Glcd_Read_Data();
    dataR = SPI_Glcd_Read_Data();
    if (!xSpiGLCD_Transparency)
        dataR = dataR & (0xff << tmpY);
    dataR = gData | dataR;
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Write_Data(dataR);
  }
  else {
    SPI_Glcd_Set_Side(pX);
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Set_Page(tmp);
    if (xSpiGLCD_Transparency){
        dataR = SPI_Glcd_Read_Data();
        dataR = SPI_Glcd_Read_Data();
        dataR = pData | dataR;
    }
    else
      dataR = pData;
    SPI_Glcd_Set_X(xx);
    SPI_Glcd_Write_Data(dataR);
  }
}

unsigned short xSPI_Glcd_Write_Char(unsigned short  ch, unsigned short x, unsigned short y, unsigned short color)
{
  const char* SpiCurCharData;
  unsigned short  i, j, CharWidth, SpiCharData;
  unsigned long cOffset;

  cOffset = xSpiGlcdSelFontWidth * xSpiGlcdSelFontNbRows+1;
  cOffset = cOffset * (ch-xSpiGlcdSelFontOffset);
  SpiCurCharData = xSpiGlcdSelFont+cOffset;
  CharWidth = *SpiCurCharData;
  cOffset++;
  for (i = 0; i< CharWidth; i++)
    for (j = 0; j<xSpiGlcdSelFontNbRows; j++)
    {
        SpiCurCharData = xSpiGlcdSelFont+(i*xSpiGlcdSelFontNbRows)+j+cOffset;
        switch (color){
          case 0 : SpiCharData = 0; break;
          case 1 : SpiCharData = *SpiCurCharData; break;
          case 2 : SpiCharData =  ~(*SpiCurCharData); break;
        }
        xSPI_GLCD_Write_Data(x+i,y+j*8,SpiCharData);
    };
  return CharWidth;
}

unsigned short xSPI_Glcd_Char_Width(unsigned short ch)
{
  const char* SpiCurCharDt;
  unsigned long cOffset;
  cOffset = xSpiGlcdSelFontWidth * xSpiGlcdSelFontNbRows+1;
  cOffset = cOffset * (ch-xSpiGlcdSelFontOffset);
  SpiCurCharDt = xSpiGlcdSelFont+cOffset;
  return *SpiCurCharDt;
}

void xSPI_Glcd_Write_Text(char* text, unsigned short x, unsigned short y, unsigned short color)
{
  unsigned short i, l, posX;
  char* curChar;
  l = strlen(text);
  posX = x;
  curChar = text;
  for (i=0; i<l; i++){
      posX = posX + xSPI_Glcd_Write_Char(*curChar,posX,y, color) +  1; //add 1 blank column
      curChar++;
  }
}

unsigned short xSPI_Glcd_Text_Width(char* text)
{
  unsigned short i, l, w;
  l = strlen(text);
  w = 0;
  for (i = 0; i<l; i++)
      w = w + xSPI_Glcd_Char_Width(text[i])+1; //add 1 blank column
  return w;
}

void xSPI_GLCD_Set_Transparency(char active)
{
  xSpiGLCD_Transparency = active;
}
