#include "../../include/shulker/io/output/OutputAdapters.h"
#include "../../include/shulker/io/output/SnbtSerializer.h"
#include "shulker/tags/LongArrayTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

auto LongArrayTag::operator[](std::size_t index)
    -> decltype(std::declval<std::vector<LongTag::LongType>&>()[index])
{
    return m_value[index];
}

std::ostream& operator<<(std::ostream& os, const LongArrayTag& long_array_tag)
{
    const bool pretty_print = os.width() > 0;
    const auto indentation = pretty_print ? os.width() : 0;

    os.width(0);

    SnbtSerializer serializer(OutputAdapter<char>(os), os.fill());
    serializer.dump(long_array_tag, pretty_print, false, static_cast<unsigned int>(indentation));
    return os;
}

SHULKER_NBT_NAMESPACE_END
