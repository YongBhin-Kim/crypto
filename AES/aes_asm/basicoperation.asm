section .text
global _start

_start:
    ; XOR 연산 예제
    mov rax, 0xFF00     ; RAX 레지스터에 0xFF00을 로드
    mov rbx, 0x00FF     ; RBX 레지스터에 0x00FF를 로드
    xor rax, rbx        ; RAX = RAX XOR RBX (결과: 0xFFFF)

    ; OR 연산 예제
    mov rax, 0x0F0F     ; RAX 레지스터에 0x0F0F를 다시 로드
    mov rbx, 0xF0F0     ; RBX 레지스터에 0xF0F0를 로드
    or rax, rbx         ; RAX = RAX OR RBX (결과: 0xFFFF)

    ; 프로그램 종료
    mov rax, 0x02000001 ; 시스템 콜 번호 설정 (exit 호출)
    xor rdi, rdi        ; 종료 코드를 0으로 설정
    syscall             ; 시스템 콜 실행
