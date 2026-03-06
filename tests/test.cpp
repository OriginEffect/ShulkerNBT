#include <iomanip>
#include <iostream>

#include <shulker/Nbt.h>

int main() {
    shulker::CompoundTag nbt =
    {
        {"ByteTag",   (char)1},
        {"ShortTag",  (short)2},
        {"IntTag",    5},
        {"LongTag",   (long long)128},
        {"FloatTag",  3.14f},
        {"DoubleTag", 2.17828},
        {"StringTag", "abc"},
        {"BoolTag",   false},
        {"EmptyCompoundTag", {}},
        {"CompoundTag", {
            {"x", 1},
            {"y", 2},
            {"z", 3},
            {"DeepNesting", {
                {"foo", {
                    {"bar", {}},
                    {"x", 114514}
                }}
            }}
        }},
        {"ListTag", {
            128,
            0.25,
            "str"
        }},
        {"ByteArrayTag", shulker::ByteArrayTag{1, 2, 3}},
        {"IntArrayTag", shulker::IntArrayTag{128, 256}},
        {"LongArrayTag", shulker::LongArrayTag{123456}}
    };

    auto value1 = nbt["CompoundTag"]["DeepNesting"]["foo"]["x"];
    const auto& tag1 = value1.as<shulker::IntTag>();
    std::cout << tag1.get() << "\n";

    auto value2 = nbt["ListTag"][2];
    const auto& tag2 = value2.as<shulker::StringTag>();
    std::cout << tag2.get() << "\n";

    auto& value3 = nbt["ByteArrayTag"].as<shulker::ByteArrayTag>()[1];
    std::cout << static_cast<int>(value3) << "\n";
    value3 = 5;
    std::cout << static_cast<int>(value3) << "\n\n";

    std::cout << "operator<<()输出：\n";
    std::cout << std::setw(4) << nbt << std::endl;

    std::cout << '\n' << "dumpSnbt()输出：\n";
    std::cout << nbt.dumpSnbt(-1, ' ', true) << std::endl;
}
