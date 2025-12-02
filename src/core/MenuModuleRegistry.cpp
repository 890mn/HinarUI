#include "HinarUI/core/MenuModuleRegistry.h"

#include "icon.h"

void module_serial();
void module_battery();
void module_SHT30();
void module_PIN();
void module_core();
void module_ABOUT();

MenuModuleRegistry::MenuModuleRegistry() {
    descriptors_[0].label = "SERIAL";
    descriptors_[0].icon = bitmap_serial;
    descriptors_[0].handler = module_serial;

    descriptors_[1].label = "VBAT";
    descriptors_[1].icon = bitmap_battery;
    descriptors_[1].handler = module_battery;

    descriptors_[2].label = "SHT30";
    descriptors_[2].icon = bitmap_sht;
    descriptors_[2].handler = module_SHT30;

    descriptors_[3].label = "ABOUT";
    descriptors_[3].icon = bitmap_ABOUT;
    descriptors_[3].handler = module_ABOUT;
    
    descriptors_[4].label = "PINSET";
    descriptors_[4].icon = bitmap_PIN;
    descriptors_[4].handler = module_PIN;

    descriptors_[5].label = "UICORE";
    descriptors_[5].icon = bitmap_core;
    descriptors_[5].handler = module_core;
}

ModuleDescriptor& MenuModuleRegistry::at(size_t index) {
    return descriptors_.at(index);
}

const ModuleDescriptor& MenuModuleRegistry::at(size_t index) const {
    return descriptors_.at(index);
}

String& MenuModuleRegistry::label(size_t index) {
    return descriptors_.at(index).label;
}

const String& MenuModuleRegistry::label(size_t index) const {
    return descriptors_.at(index).label;
}

unsigned char*& MenuModuleRegistry::icon(size_t index) {
    return descriptors_.at(index).icon;
}

unsigned char* MenuModuleRegistry::icon(size_t index) const {
    return descriptors_.at(index).icon;
}

MenuEntry MenuModuleRegistry::handler(size_t index) const {
    return descriptors_.at(index).handler;
}

