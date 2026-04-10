#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

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
        {"LongArrayTag", shulker::LongArrayTag{123456}},
        {"中文测试", "😡"}
    };

    std::size_t bytes = nbt.dumpFile("test.nbt", true, shulker::CompressionMode::Gzip);
    std::cout << "写入量：" << bytes << "\n\n";

    auto value1 = nbt["CompoundTag"]["DeepNesting"]["foo"]["x"];
    std::cout << value1 << "\n";

    auto value2 = nbt["ListTag"][2];
    std::cout << value2 << "\n";

    auto& value3 = nbt["ByteArrayTag"].as<shulker::ByteArrayTag>()[1];
    std::cout << static_cast<int>(value3) << "\n";
    value3 = 5;
    std::cout << static_cast<int>(value3) << "\n\n";

    std::cout << "operator<<()输出：\n";
    std::cout << std::setw(4) << nbt << std::endl;

    std::cout << '\n' << "dumpSnbt()输出：\n";
    std::cout << nbt.dumpSnbt(4, ' ', true, true) << std::endl;

    std::cout << "读取level.dat：\n";
    std::fstream file("./nbt/test-raw.nbt", std::ios::in | std::ios::binary);
    // file.seekg(8, std::ios::beg);
    auto result = shulker::readNbt(file, true, shulker::CompressionMode::Raw).dumpSnbt(2, ' ', true);

    std::cout << result << "\n";
    // std::ofstream out("result.snbt");
    // out << result;
    // out.close();
}
