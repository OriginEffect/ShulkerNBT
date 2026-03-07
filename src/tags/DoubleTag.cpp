#include "../../include/shulker/io/output/OutputAdapters.h"
#include "../../include/shulker/io/output/SnbtSerializer.h"
#include "shulker/tags/DoubleTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

std::ostream& operator<<(std::ostream& os, const DoubleTag& double_tag)
{
    SnbtSerializer serializer(OutputAdapter<char>(os), os.fill());
    serializer.dump(double_tag, false, false, 0);
    return os;
}

SHULKER_NBT_NAMESPACE_END
