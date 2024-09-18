global start           ; 'start' 레이블을 전역으로 선언
section .text          ; 코드 세그먼트 시작

start:
    mov rax, 0x02000004 ; 시스템 콜 번호 설정 (write 호출)
    mov rdi, 1          ; 첫 번째 인자: 파일 디스크립터 1 (stdout)
    mov rsi, message    ; 두 번째 인자: 메시지가 저장된 메모리 주소
    mov rdx, 13         ; 세 번째 인자: 메시지의 길이
    syscall             ; 시스템 콜 실행

    mov rax, 0x02000001 ; 시스템 콜 번호 설정 (exit 호출)
    xor rdi, rdi        ; 첫 번째 인자: 종료 코드 0 (정상 종료)
    syscall             ; 시스템 콜 실행

section .data          ; 데이터 세그먼트 시작
message:  
    db "Hello World!!!!!!!!!!\n\n\n", 20 ; 데이터 저장, 문자열과 그 길이

; nasm -f macho64 [source file name.s]
; ld -macosx_version_min 10.7.0 -o [execute file name] [object file name.o]
; ./[execute file name]