#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
#include "shulker/tags/StringTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const StringTag& string_tag)
{
    detail::SnbtSerializer serializer(detail::OutputAdapter<char>(os), os.fill());
    serializer.dump(string_tag, false, false, false, 0);
    return os;
}


SHULKER_NBT_NAMESPACE_END
