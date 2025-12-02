# Font custom

This markdown conclude the way of **custom your font**.

*Font apply to Adafruit_GFX's custom Setting, HinarUI just create the frame which draw with GFX.*

## Reference

Thanks for everyone below:
[Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) | [zhihu](https://zhuanlan.zhihu.com/p/387949673) | [truetype2gfx](https://rop.nl/truetype2gfx/)

## Steps

1. Get font file **(.ttf)**
    Suggest Website: [onlinewebfonts](https://www.onlinewebfonts.com/fonts)
2. Convert to GFX Header file **(.h)**
    1. open the website: [truetype2gfx](https://rop.nl/truetype2gfx/)
    2. Choose the file and upload
    3. Choose the font size
    4. Get GFX font file
3. Copy the Header file to `./HinarUI/include/fonts`
4. Modify in `font.h`:
    ```
    #ifndef FONT_H
    #define FONT_H

    #include <Adafruit_GFX.h>
    #include "Arame_Regular5pt7b.h"
    #include "Arame_Regular6pt7b.h"
    #include "Arame_Regular8pt7b.h"
    #include "Arame_Regular10pt7b.h"

    #define SET_FONT_USMALL  display.setFont(&Arame_Regular5pt7b);
    #define SET_FONT_SMALL   display.setFont(&Arame_Regular6pt7b);
    #define SET_FONT_MEDIUM  display.setFont(&Arame_Regular8pt7b);
    #define SET_FONT_LARGE   display.setFont(&Arame_Regular10pt7b);
    #define SET_FONT_DEFAULT display.setFont();

    #endif //FONT_H
    ```
