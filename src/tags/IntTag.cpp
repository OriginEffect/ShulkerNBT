#include "shulker/tags/IntTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& o, const IntTag& tag)
{
    const bool pretty_print = o.width() > 0;
    const auto indentation = pretty_print ? o.width() : 0;


}

SHULKER_NBT_NAMESPACE_END
