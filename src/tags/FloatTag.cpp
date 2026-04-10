#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
#include "shulker/tags/FloatTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const FloatTag& float_tag)
{
    detail::SnbtSerializer serializer(detail::OutputAdapter<char>(os), os.fill());
    serializer.dump(float_tag, false, false, false, 0);
    return os;
}

SHULKER_NBT_NAMESPACE_END
