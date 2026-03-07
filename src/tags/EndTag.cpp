#include "../../include/shulker/io/output/OutputAdapters.h"
#include "../../include/shulker/io/output/SnbtSerializer.h"
#include "shulker/tags/EndTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const EndTag& end_tag)
{
    SnbtSerializer serializer(OutputAdapter<char>(os), os.fill());
    serializer.dump(end_tag, false, false, 0);
    return os;
}

SHULKER_NBT_NAMESPACE_END
