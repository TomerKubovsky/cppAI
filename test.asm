section .data
    msg db 'Hello, World!', 0    ; Define a null-terminated string

section .text
    global _start                ; Declare the _start label as the entry point

_start:
    ; write(1, msg, 13)
    mov eax, 4                  ; System call number for sys_write (4)
    mov ebx, 1                  ; File descriptor 1 is stdout
    mov ecx, msg                ; Pointer to the message
    mov edx, 13                 ; Length of the message
    int 0x80                    ; Invoke system call

    ; exit(0)
    mov eax, 1                  ; System call number for sys_exit (1)
    xor ebx, ebx                ; Exit code 0
    int 0x80                    ; Invoke system call
