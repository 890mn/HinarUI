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