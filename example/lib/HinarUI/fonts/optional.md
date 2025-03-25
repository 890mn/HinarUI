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
3. Copy the Header file to `./HinarUI/fonts`
4. Modify in `UI.h`:
    ```
    #ifndef __UI_H
    #define __UI_H
    ...
    #include "fonts/Arame_Regular8pt7b.h"
    ```
5. Modify by `display.setFont(&Arame_Regular8pt7b);`


