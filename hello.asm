	segment .data
	global _start
	extern printf
	extern SDL_CreateWindow, SDL_Init, SDL_Quit, SDL_Delay
_hello:
	db 'hello',10,0
_pftext:
	db 'Gerald',10,0
_winname:
	db 'apioform',0

	segment .text
_start:
	mov rax, 0x01		; write
	mov rdi, 1		; stdout
	mov rsi, _hello
	mov rdx, 6
	syscall
	
	mov rdi, _pftext
	mov ax, 0
	call printf

	mov rdi, 0x4030
	call SDL_Init
	
	mov rdi, _winname
	mov rsi, 100
	mov rdx, 100
	mov rcx, 100
	mov r8, 100
	mov r9, 0
	call SDL_CreateWindow

;;	mov rdi, 5000
;;	call SDL_Delay

	call SDL_Quit

	mov rdi, _hello
	mov ax, 0
	call printf

	mov rax, 2
	mov [_har], rax
	mov rax, 3
	mov [_har+8], rax
	sub sp, 16
	mov rax, [_har]
	mov rbx, [_har+8]
	imul rax, rbx
	add al, 0x30
	mov [_har+16], al
;;	mov [_har+16], dword 0x35

	mov rax, 0x01
	mov rdi, 1
	mov rsi, _har+16
	mov rdx, 1
	syscall

;; exit
	mov eax, 0x3c
	mov edi, 0
	syscall


	segment .bss
_har:
	resb 64
