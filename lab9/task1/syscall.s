section .data
global blah

blah: db 8, 9, 11

section .text
global system_call
global strlen
global strt

strt: db "If you wanna shoot, shoot. Don't talk.", 39, 0
        
strlen:
	push	ebp
	mov	ebp, esp
	push	ebx
	mov	ebx, [ebp+8]
	mov	eax, 0
l:	cmp	byte [ebx], 0
	jz	end
	inc	eax
	inc	ebx
	jmp	l
end:	pop	ebx
	pop	ebp
	ret

system_call:
	push	ebp             ; Save caller state
	mov	ebp, esp
	sub	esp, 4          ; Leave space for local var on stack
	pushad                  ; Save some more caller state
	mov	eax, [ebp+8]    ; Copy function args to registers: leftmost... 
	mov	ebx, [ebp+12]   ; Next argument...
	mov	ecx, [ebp+16]   ; Next argument...
	mov	edx, [ebp+20]   ; Next argument...
	int	0x80            ; Transfer control to operating system
	mov	[ebp-4], eax    ; Save returned value...
	popad			; Restore caller state (registers)
	mov	eax, [ebp-4]    ; place returned value where caller can see it
	add	esp, 4		; Restore caller state
	pop	ebp		; Restore caller state
	ret                     ; Back to caller

