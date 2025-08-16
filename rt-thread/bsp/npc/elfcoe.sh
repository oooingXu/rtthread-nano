#!/bin/bash

# 用法检查
if [ $# -ne 1 ]; then
    echo "用法: $0 <input.elf>"
    exit 1
fi

ELF="$1"
BASENAME=$(basename "$ELF" .elf)

# 工具
OBJCOPY=riscv64-unknown-elf-objcopy
READELF=riscv64-unknown-elf-readelf

# 输出文件
#INST_BIN="./build/${BASENAME}_irom.bin"
#FULL_BIN="./build/${BASENAME}_dram.bin"
#INST_COE="./build/${BASENAME}_irom.coe"
#FULL_COE="./build/${BASENAME}_dram.coe"
INST_BIN="./build/irom.bin"
FULL_BIN="./build/dram.bin"
INST_COE="./build/irom.coe"
FULL_COE="./build/dram.coe"

# 提取 .text 段为 inst_only.bin
$OBJCOPY -O binary -j .text "$ELF" "$INST_BIN"

# 提取整个映像（含 .text, .data, .rodata）为 full_data.bin
$OBJCOPY -O binary "$ELF" "$FULL_BIN"

# 用 Python 内联脚本转为 .coe 文件（32位对齐）
bin_to_coe() {
    BIN="$1"
    COE="$2"
    python3 - <<EOF
import sys

bin_file = "$BIN"
coe_file = "$COE"
word_size = 4  # 32-bit words

with open(bin_file, "rb") as f:
    data = f.read()

# Pad to multiple of word_size
while len(data) % word_size != 0:
    data += b'\x00'

words = [
    int.from_bytes(data[i:i+word_size], byteorder='little')
    for i in range(0, len(data), word_size)
]

with open(coe_file, "w") as f:
    f.write("memory_initialization_radix=16;\n")
    f.write("memory_initialization_vector=\n")
    for i, word in enumerate(words):
        sep = "," if i < len(words) - 1 else ";"
        f.write(f"{word:08X}{sep}\n")
EOF
}

# 转换为 COE 文件
echo "生成 $INST_COE"
bin_to_coe "$INST_BIN" "$INST_COE"

echo "生成 $FULL_COE"
bin_to_coe "$FULL_BIN" "$FULL_COE"

# 可选：清理中间 .bin 文件
rm "$INST_BIN" "$FULL_BIN"

echo "完成 "
