#pragma once

#include <array>

#include "HinarUI/core/MenuConfig.h"
#include "HinarUI/core/MenuTypes.h"

class MenuModuleRegistry {
public:
    MenuModuleRegistry();

    size_t size() const { return descriptors_.size(); }

    ModuleDescriptor& at(size_t index);
    const ModuleDescriptor& at(size_t index) const;

    String& label(size_t index);
    const String& label(size_t index) const;

    MenuIcon& icon(size_t index);
    MenuIcon icon(size_t index) const;

    MenuEntry handler(size_t index) const;
    bool registerModule(size_t index, const ModuleDescriptor& descriptor);

private:
    std::array<ModuleDescriptor, MenuConfig::kMaxModules> descriptors_;
};
