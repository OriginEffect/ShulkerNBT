#include "../../include/shulker/io/output/OutputAdapters.h"
#include "../../include/shulker/io/output/SnbtSerializer.h"
#include "shulker/tags/ByteTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const ByteTag& byte_tag)
{
    SnbtSerializer serializer(OutputAdapter<char>(os), os.fill());
    serializer.dump(byte_tag, false, false, 0);
    return os;
}

SHULKER_NBT_NAMESPACE_END
