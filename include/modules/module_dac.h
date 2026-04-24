#ifndef MODULE_DAC_H
#define MODULE_DAC_H

#include <stddef.h>

static constexpr int HINARUI_DAC_MODULE_INDEX = 2;

bool DAC_Setup(void);
void module_DAC(void);
void module_DAC_ForceOff(void);

#endif // MODULE_DAC_H
