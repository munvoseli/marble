	segment .data
	global _start
	extern printf
	extern SDL_CreateWindow, SDL_Init, SDL_Quit
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

	call SDL_Quit

	mov rdi, _hello
	mov ax, 0
	call printf

;; exit
	mov eax, 0x3c
	mov edi, 0
	syscall


	segment .bss
