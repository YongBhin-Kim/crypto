#!/bin/bash

# 현재 디렉토리에서 .o 확장자를 가진 오브젝트 파일 삭제
find . -type f -name "*.o" -exec rm {} \;

# 현재 디렉토리에서 실행 파일 삭제
# 실행 파일의 이름을 'test'로 가정
find . -type f -name "test" -exec sh -c 'if [ -x "{}" ]; then rm "{}"; fi' \;
