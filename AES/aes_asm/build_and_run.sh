#!/bin/bash

# 어셈블리 파일명을 배열로 저장
ASSEMBLY_FILES=("test.asm" "basicoperation.asm")
OUTPUT_FILES=("test" "basicoperation")

# 각 어셈블리 파일을 컴파일하고 링크한 후 실행
for i in ${!ASSEMBLY_FILES[@]}; do
    ASSEMBLY_FILE="${ASSEMBLY_FILES[$i]}"
    OUTPUT_FILE="${OUTPUT_FILES[$i]}"
    OBJECT_FILE="${OUTPUT_FILE}.o"

    # NASM을 사용하여 어셈블리 파일 컴파일
    nasm -f macho64 $ASSEMBLY_FILE

    # ld를 사용하여 오브젝트 파일 링킹
    ld -macosx_version_min 10.7.0 -o $OUTPUT_FILE $OBJECT_FILE

    # 실행 파일 실행
    ./$OUTPUT_FILE
done
