	.file	"upr_1.c"
	.text
	.globl	delimiters
	.section	.rodata
	.type	delimiters, @object
	.size	delimiters, 6
delimiters:
	.string	" ,\r\n\377"
	.globl	digits
	.align 8
	.type	digits, @object
	.size	digits, 11
digits:
	.string	"0123456789"
	.globl	non_zero_digits
	.align 8
	.type	non_zero_digits, @object
	.size	non_zero_digits, 10
non_zero_digits:
	.string	"123456789"
	.globl	octal_consts
	.align 8
	.type	octal_consts, @object
	.size	octal_consts, 9
octal_consts:
	.string	"01234567"
	.globl	hex_consts
	.align 16
	.type	hex_consts, @object
	.size	hex_consts, 23
hex_consts:
	.string	"0123456789aAbBcCdDeEfF"
	.globl	nums_type_flags
	.bss
	.align 16
	.type	nums_type_flags, @object
	.size	nums_type_flags, 28
nums_type_flags:
	.zero	28
	.globl	nums_type_counters
	.align 16
	.type	nums_type_counters, @object
	.size	nums_type_counters, 28
nums_type_counters:
	.zero	28
	.text
	.globl	reset_type_count
	.type	reset_type_count, @function
reset_type_count:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	nums_type_flags(%rip), %rax
	movl	$0, (%rdx,%rax)
	addl	$1, -4(%rbp)
.L2:
	cmpl	$6, -4(%rbp)
	jle	.L3
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	reset_type_count, .-reset_type_count
	.globl	increment_type_count
	.type	increment_type_count, @function
increment_type_count:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L6
.L8:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	nums_type_flags(%rip), %rax
	movl	(%rdx,%rax), %eax
	cmpl	$1, %eax
	jne	.L7
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	nums_type_counters(%rip), %rax
	movl	(%rdx,%rax), %eax
	leal	1(%rax), %ecx
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	nums_type_counters(%rip), %rax
	movl	%ecx, (%rdx,%rax)
.L7:
	addl	$1, -4(%rbp)
.L6:
	cmpl	$6, -4(%rbp)
	jle	.L8
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	increment_type_count, .-increment_type_count
	.globl	is_delimiter
	.type	is_delimiter, @function
is_delimiter:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L11
.L14:
	movl	-4(%rbp), %eax
	cltq
	leaq	delimiters(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	cmpb	%al, -20(%rbp)
	jne	.L12
	movl	$1, %eax
	jmp	.L13
.L12:
	addl	$1, -4(%rbp)
.L11:
	cmpl	$4, -4(%rbp)
	jle	.L14
	movl	$0, %eax
.L13:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	is_delimiter, .-is_delimiter
	.globl	is_digit
	.type	is_digit, @function
is_digit:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L16
.L19:
	movl	-4(%rbp), %eax
	cltq
	leaq	digits(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	cmpb	%al, -20(%rbp)
	jne	.L17
	movl	$1, %eax
	jmp	.L18
.L17:
	addl	$1, -4(%rbp)
.L16:
	cmpl	$9, -4(%rbp)
	jle	.L19
	movl	$0, %eax
.L18:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	is_digit, .-is_digit
	.globl	is_non_zero_digit
	.type	is_non_zero_digit, @function
is_non_zero_digit:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L21
.L24:
	movl	-4(%rbp), %eax
	cltq
	leaq	non_zero_digits(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	cmpb	%al, -20(%rbp)
	jne	.L22
	movl	$1, %eax
	jmp	.L23
.L22:
	addl	$1, -4(%rbp)
.L21:
	cmpl	$8, -4(%rbp)
	jle	.L24
	movl	$0, %eax
.L23:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	is_non_zero_digit, .-is_non_zero_digit
	.globl	is_octal_const
	.type	is_octal_const, @function
is_octal_const:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L26
.L29:
	movl	-4(%rbp), %eax
	cltq
	leaq	octal_consts(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	cmpb	%al, -20(%rbp)
	jne	.L27
	movl	$1, %eax
	jmp	.L28
.L27:
	addl	$1, -4(%rbp)
.L26:
	cmpl	$7, -4(%rbp)
	jle	.L29
	movl	$0, %eax
.L28:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	is_octal_const, .-is_octal_const
	.globl	is_hex_const
	.type	is_hex_const, @function
is_hex_const:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L31
.L34:
	movl	-4(%rbp), %eax
	cltq
	leaq	hex_consts(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	cmpb	%al, -20(%rbp)
	jne	.L32
	movl	$1, %eax
	jmp	.L33
.L32:
	addl	$1, -4(%rbp)
.L31:
	cmpl	$21, -4(%rbp)
	jle	.L34
	movl	$0, %eax
.L33:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	is_hex_const, .-is_hex_const
	.globl	is_unsigned_suffix
	.type	is_unsigned_suffix, @function
is_unsigned_suffix:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$117, -4(%rbp)
	je	.L36
	cmpb	$85, -4(%rbp)
	jne	.L37
.L36:
	movl	$1, %eax
	jmp	.L39
.L37:
	movl	$0, %eax
.L39:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	is_unsigned_suffix, .-is_unsigned_suffix
	.globl	is_long_suffix
	.type	is_long_suffix, @function
is_long_suffix:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$108, -4(%rbp)
	je	.L41
	cmpb	$76, -4(%rbp)
	jne	.L42
.L41:
	movl	$1, %eax
	jmp	.L44
.L42:
	movl	$0, %eax
.L44:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	is_long_suffix, .-is_long_suffix
	.globl	is_small_long_suffix
	.type	is_small_long_suffix, @function
is_small_long_suffix:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$108, -4(%rbp)
	sete	%al
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	is_small_long_suffix, .-is_small_long_suffix
	.globl	is_capital_long_suffix
	.type	is_capital_long_suffix, @function
is_capital_long_suffix:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$76, -4(%rbp)
	sete	%al
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	is_capital_long_suffix, .-is_capital_long_suffix
	.globl	is_float_suffix
	.type	is_float_suffix, @function
is_float_suffix:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$102, -4(%rbp)
	je	.L50
	cmpb	$70, -4(%rbp)
	jne	.L51
.L50:
	movl	$1, %eax
	jmp	.L53
.L51:
	movl	$0, %eax
.L53:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	is_float_suffix, .-is_float_suffix
	.globl	is_exponent
	.type	is_exponent, @function
is_exponent:
.LFB18:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$101, -4(%rbp)
	je	.L55
	cmpb	$69, -4(%rbp)
	jne	.L56
.L55:
	movl	$1, %eax
	jmp	.L58
.L56:
	movl	$0, %eax
.L58:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	is_exponent, .-is_exponent
	.globl	is_hex_prefix
	.type	is_hex_prefix, @function
is_hex_prefix:
.LFB19:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$120, -4(%rbp)
	je	.L60
	cmpb	$88, -4(%rbp)
	jne	.L61
.L60:
	movl	$1, %eax
	jmp	.L63
.L61:
	movl	$0, %eax
.L63:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	is_hex_prefix, .-is_hex_prefix
	.section	.rodata
.LC0:
	.string	"IS_VALID_NUM:\t\t%d\n"
.LC1:
	.string	"IS_WHOLE_NUM:\t\t%d\n"
.LC2:
	.string	"IS_REAL_NUM:\t\t%d\n"
.LC3:
	.string	"IS_U_WHOLE_NUM:\t\t%d\n"
.LC4:
	.string	"IS_OCTAL:\t\t%d\n"
.LC5:
	.string	"IS_HEXIDECIMAL:\t\t%d\n"
.LC6:
	.string	"IS_FLOAT:\t\t%d\n"
	.text
	.globl	numbers_identificator
	.type	numbers_identificator, @function
numbers_identificator:
.LFB20:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
.L261:
	movl	-12(%rbp), %eax
	addl	$1, %eax
	cmpl	$30, %eax
	ja	.L262
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L67(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L67(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L67:
	.long	.L96-.L67
	.long	.L95-.L67
	.long	.L94-.L67
	.long	.L93-.L67
	.long	.L92-.L67
	.long	.L91-.L67
	.long	.L90-.L67
	.long	.L89-.L67
	.long	.L88-.L67
	.long	.L87-.L67
	.long	.L86-.L67
	.long	.L85-.L67
	.long	.L84-.L67
	.long	.L83-.L67
	.long	.L82-.L67
	.long	.L81-.L67
	.long	.L80-.L67
	.long	.L79-.L67
	.long	.L78-.L67
	.long	.L77-.L67
	.long	.L76-.L67
	.long	.L75-.L67
	.long	.L74-.L67
	.long	.L73-.L67
	.long	.L72-.L67
	.long	.L71-.L67
	.long	.L70-.L67
	.long	.L262-.L67
	.long	.L69-.L67
	.long	.L68-.L67
	.long	.L66-.L67
	.text
.L95:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L97
	movl	$29, -12(%rbp)
	jmp	.L99
.L97:
	movl	$1, -12(%rbp)
	jmp	.L99
.L94:
	movl	$0, %eax
	call	increment_type_count
	movl	$0, %eax
	call	reset_type_count
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L100
	movl	$1, -12(%rbp)
	jmp	.L101
.L100:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$43, %al
	jne	.L102
	movl	$2, -12(%rbp)
	jmp	.L101
.L102:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$45, %al
	jne	.L103
	movl	$3, -12(%rbp)
	jmp	.L101
.L103:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$48, %al
	jne	.L104
	movl	$6, -12(%rbp)
	jmp	.L101
.L104:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L105
	movl	$7, -12(%rbp)
	jmp	.L101
.L105:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_non_zero_digit
	testl	%eax, %eax
	je	.L106
	movl	$10, -12(%rbp)
	jmp	.L101
.L106:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L107
	movl	$29, -12(%rbp)
	jmp	.L101
.L107:
	movl	$-1, -12(%rbp)
.L101:
	addl	$1, -8(%rbp)
	jmp	.L99
.L96:
	movl	$0, %eax
	call	reset_type_count
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L108
	movl	$1, -12(%rbp)
	addl	$1, -4(%rbp)
	jmp	.L109
.L108:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L110
	movl	$29, -12(%rbp)
	jmp	.L109
.L110:
	movl	$-1, -12(%rbp)
.L109:
	addl	$1, -8(%rbp)
	jmp	.L99
.L66:
	movl	$0, %eax
	call	increment_type_count
	movl	nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	4+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	8+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	12+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	16+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	20+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	24+nums_type_counters(%rip), %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L64
.L93:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$48, %al
	jne	.L112
	movl	$4, -12(%rbp)
	jmp	.L113
.L112:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_non_zero_digit
	testl	%eax, %eax
	je	.L114
	movl	$8, -12(%rbp)
	jmp	.L113
.L114:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L115
	movl	$7, -12(%rbp)
	jmp	.L113
.L115:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L116
	movl	$29, -12(%rbp)
	jmp	.L113
.L116:
	movl	$-1, -12(%rbp)
.L113:
	addl	$1, -8(%rbp)
	jmp	.L99
.L92:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$48, %al
	jne	.L117
	movl	$5, -12(%rbp)
	jmp	.L118
.L117:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_non_zero_digit
	testl	%eax, %eax
	je	.L119
	movl	$9, -12(%rbp)
	jmp	.L118
.L119:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L120
	movl	$7, -12(%rbp)
	jmp	.L118
.L120:
	movl	$-1, -12(%rbp)
.L118:
	addl	$1, -8(%rbp)
	jmp	.L99
.L91:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L121
	movl	$7, -12(%rbp)
	jmp	.L122
.L121:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L123
	movl	$1, -12(%rbp)
	jmp	.L122
.L123:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L124
	movl	$23, -12(%rbp)
	jmp	.L122
.L124:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L125
	movl	$28, -12(%rbp)
	jmp	.L122
.L125:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L126
	movl	$14, -12(%rbp)
	jmp	.L122
.L126:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L127
	movl	$17, -12(%rbp)
	jmp	.L122
.L127:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L128
	movl	$11, -12(%rbp)
	jmp	.L122
.L128:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L129
	movl	$29, -12(%rbp)
	jmp	.L122
.L129:
	movl	$-1, -12(%rbp)
.L122:
	addl	$1, -8(%rbp)
	jmp	.L99
.L90:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L130
	movl	$7, -12(%rbp)
	jmp	.L131
.L130:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L132
	movl	$1, -12(%rbp)
	jmp	.L131
.L132:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L133
	movl	$23, -12(%rbp)
	jmp	.L131
.L133:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L134
	movl	$28, -12(%rbp)
	jmp	.L131
.L134:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L135
	movl	$14, -12(%rbp)
	jmp	.L131
.L135:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L136
	movl	$17, -12(%rbp)
	jmp	.L131
.L136:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L137
	movl	$29, -12(%rbp)
	jmp	.L131
.L137:
	movl	$-1, -12(%rbp)
.L131:
	addl	$1, -8(%rbp)
	jmp	.L99
.L89:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L138
	movl	$1, -12(%rbp)
	jmp	.L139
.L138:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_hex_prefix
	testl	%eax, %eax
	je	.L140
	movl	$20, -12(%rbp)
	jmp	.L139
.L140:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_octal_const
	testl	%eax, %eax
	je	.L141
	movl	$19, -12(%rbp)
	jmp	.L139
.L141:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L142
	movl	$28, -12(%rbp)
	jmp	.L139
.L142:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L143
	movl	$11, -12(%rbp)
	jmp	.L139
.L143:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L144
	movl	$14, -12(%rbp)
	jmp	.L139
.L144:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L145
	movl	$17, -12(%rbp)
	jmp	.L139
.L145:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L146
	movl	$23, -12(%rbp)
	jmp	.L139
.L146:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L147
	movl	$7, -12(%rbp)
	jmp	.L139
.L147:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L148
	movl	$29, -12(%rbp)
	jmp	.L139
.L148:
	movl	$-1, -12(%rbp)
.L139:
	addl	$1, -8(%rbp)
	jmp	.L99
.L88:
	movl	$1, nums_type_flags(%rip)
	movl	$0, 4+nums_type_flags(%rip)
	movl	$0, 12+nums_type_flags(%rip)
	movl	$1, 8+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L149
	movl	$1, -12(%rbp)
	jmp	.L150
.L149:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L151
	movl	$21, -12(%rbp)
	jmp	.L150
.L151:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L152
	movl	$23, -12(%rbp)
	jmp	.L150
.L152:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_float_suffix
	testl	%eax, %eax
	je	.L153
	movl	$22, -12(%rbp)
	jmp	.L150
.L153:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L154
	movl	$0, %eax
	call	reset_type_count
	movl	$29, -12(%rbp)
	jmp	.L150
.L154:
	movl	$-1, -12(%rbp)
.L150:
	addl	$1, -8(%rbp)
	jmp	.L99
.L87:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L155
	movl	$1, -12(%rbp)
	jmp	.L156
.L155:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L157
	movl	$8, -12(%rbp)
	jmp	.L156
.L157:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L158
	movl	$11, -12(%rbp)
	jmp	.L156
.L158:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L159
	movl	$14, -12(%rbp)
	jmp	.L156
.L159:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L160
	movl	$17, -12(%rbp)
	jmp	.L156
.L160:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L161
	movl	$23, -12(%rbp)
	jmp	.L156
.L161:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L162
	movl	$7, -12(%rbp)
	jmp	.L156
.L162:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L163
	movl	$29, -12(%rbp)
	jmp	.L156
.L163:
	movl	$-1, -12(%rbp)
.L156:
	addl	$1, -8(%rbp)
	jmp	.L99
.L86:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L164
	movl	$1, -12(%rbp)
	jmp	.L165
.L164:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L166
	movl	$9, -12(%rbp)
	jmp	.L165
.L166:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L167
	movl	$13, -12(%rbp)
	jmp	.L165
.L167:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L168
	movl	$16, -12(%rbp)
	jmp	.L165
.L168:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L169
	movl	$23, -12(%rbp)
	jmp	.L165
.L169:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L170
	movl	$7, -12(%rbp)
	jmp	.L165
.L170:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L171
	movl	$29, -12(%rbp)
	jmp	.L165
.L171:
	movl	$-1, -12(%rbp)
.L165:
	addl	$1, -8(%rbp)
	jmp	.L99
.L85:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L172
	movl	$1, -12(%rbp)
	jmp	.L173
.L172:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L174
	movl	$10, -12(%rbp)
	jmp	.L173
.L174:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L175
	movl	$11, -12(%rbp)
	jmp	.L173
.L175:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L176
	movl	$14, -12(%rbp)
	jmp	.L173
.L176:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L177
	movl	$17, -12(%rbp)
	jmp	.L173
.L177:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L178
	movl	$23, -12(%rbp)
	jmp	.L173
.L178:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L179
	movl	$7, -12(%rbp)
	jmp	.L173
.L179:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L180
	movl	$29, -12(%rbp)
	jmp	.L173
.L180:
	movl	$-1, -12(%rbp)
.L173:
	addl	$1, -8(%rbp)
	jmp	.L99
.L84:
	movl	$1, 12+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L181
	movl	$14, -12(%rbp)
	jmp	.L182
.L181:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L183
	movl	$17, -12(%rbp)
	jmp	.L182
.L183:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L184
	movl	$1, -12(%rbp)
	jmp	.L182
.L184:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L185
	movl	$29, -12(%rbp)
	jmp	.L182
.L185:
	movl	$-1, -12(%rbp)
.L182:
	addl	$1, -8(%rbp)
	jmp	.L99
.L83:
	movl	$1, 12+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L186
	movl	$1, -12(%rbp)
	jmp	.L187
.L186:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L188
	movl	$29, -12(%rbp)
	jmp	.L187
.L188:
	movl	$-1, -12(%rbp)
.L187:
	addl	$1, -8(%rbp)
	jmp	.L99
.L82:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L189
	movl	$1, -12(%rbp)
	jmp	.L190
.L189:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L191
	movl	$29, -12(%rbp)
	jmp	.L190
.L191:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L192
	movl	$15, -12(%rbp)
	jmp	.L190
.L192:
	movl	$-1, -12(%rbp)
.L190:
	addl	$1, -8(%rbp)
	jmp	.L99
.L79:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L193
	movl	$1, -12(%rbp)
	jmp	.L194
.L193:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L195
	movl	$29, -12(%rbp)
	jmp	.L194
.L195:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L196
	movl	$18, -12(%rbp)
	jmp	.L194
.L196:
	movl	$-1, -12(%rbp)
.L194:
	addl	$1, -8(%rbp)
	jmp	.L99
.L81:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L197
	movl	$1, -12(%rbp)
	jmp	.L198
.L197:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L199
	movl	$29, -12(%rbp)
	jmp	.L198
.L199:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L200
	movl	$15, -12(%rbp)
	jmp	.L198
.L200:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L201
	movl	$12, -12(%rbp)
	jmp	.L198
.L201:
	movl	$-1, -12(%rbp)
.L198:
	addl	$1, -8(%rbp)
	jmp	.L99
.L78:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L202
	movl	$1, -12(%rbp)
	jmp	.L203
.L202:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L204
	movl	$29, -12(%rbp)
	jmp	.L203
.L204:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L205
	movl	$18, -12(%rbp)
	jmp	.L203
.L205:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L206
	movl	$12, -12(%rbp)
	jmp	.L203
.L206:
	movl	$-1, -12(%rbp)
.L203:
	addl	$1, -8(%rbp)
	jmp	.L99
.L80:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L207
	movl	$1, -12(%rbp)
	jmp	.L208
.L207:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L209
	movl	$29, -12(%rbp)
	jmp	.L208
.L209:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L210
	movl	$12, -12(%rbp)
	jmp	.L208
.L210:
	movl	$-1, -12(%rbp)
.L208:
	addl	$1, -8(%rbp)
	jmp	.L99
.L77:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L211
	movl	$1, -12(%rbp)
	jmp	.L212
.L211:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L213
	movl	$29, -12(%rbp)
	jmp	.L212
.L213:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L214
	movl	$12, -12(%rbp)
	jmp	.L212
.L214:
	movl	$-1, -12(%rbp)
.L212:
	addl	$1, -8(%rbp)
	jmp	.L99
.L76:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	$1, 12+nums_type_flags(%rip)
	movl	$1, 16+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L215
	movl	$1, -12(%rbp)
	jmp	.L216
.L215:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_octal_const
	testl	%eax, %eax
	je	.L217
	movl	$19, -12(%rbp)
	jmp	.L216
.L217:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L218
	movl	$28, -12(%rbp)
	jmp	.L216
.L218:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L219
	movl	$23, -12(%rbp)
	jmp	.L216
.L219:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L220
	movl	$14, -12(%rbp)
	jmp	.L216
.L220:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L221
	movl	$17, -12(%rbp)
	jmp	.L216
.L221:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L222
	movl	$11, -12(%rbp)
	jmp	.L216
.L222:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L223
	movl	$29, -12(%rbp)
	jmp	.L216
.L223:
	movl	$-1, -12(%rbp)
.L216:
	addl	$1, -8(%rbp)
	jmp	.L99
.L75:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 4+nums_type_flags(%rip)
	movl	$1, 12+nums_type_flags(%rip)
	movl	$1, 20+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L224
	movl	$1, -12(%rbp)
	jmp	.L225
.L224:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_hex_const
	testl	%eax, %eax
	je	.L226
	movl	$20, -12(%rbp)
	jmp	.L225
.L226:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_small_long_suffix
	testl	%eax, %eax
	je	.L227
	movl	$14, -12(%rbp)
	jmp	.L225
.L227:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_capital_long_suffix
	testl	%eax, %eax
	je	.L228
	movl	$17, -12(%rbp)
	jmp	.L225
.L228:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_unsigned_suffix
	testl	%eax, %eax
	je	.L229
	movl	$11, -12(%rbp)
	jmp	.L225
.L229:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L230
	movl	$29, -12(%rbp)
	jmp	.L225
.L230:
	movl	$-1, -12(%rbp)
.L225:
	addl	$1, -8(%rbp)
	jmp	.L99
.L74:
	movl	$1, nums_type_flags(%rip)
	movl	$1, 8+nums_type_flags(%rip)
	movl	$0, 12+nums_type_flags(%rip)
	movl	$0, 4+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L231
	movl	$1, -12(%rbp)
	jmp	.L232
.L231:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L233
	movl	$21, -12(%rbp)
	jmp	.L232
.L233:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_float_suffix
	testl	%eax, %eax
	je	.L234
	movl	$22, -12(%rbp)
	jmp	.L232
.L234:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_long_suffix
	testl	%eax, %eax
	je	.L235
	movl	$27, -12(%rbp)
	jmp	.L232
.L235:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L236
	movl	$23, -12(%rbp)
	jmp	.L232
.L236:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L237
	movl	$29, -12(%rbp)
	jmp	.L232
.L237:
	movl	$-1, -12(%rbp)
.L232:
	addl	$1, -8(%rbp)
	jmp	.L99
.L73:
	movl	$1, 8+nums_type_flags(%rip)
	movl	$1, 24+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L238
	movl	$1, -12(%rbp)
	jmp	.L239
.L238:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L240
	movl	$29, -12(%rbp)
	jmp	.L239
.L240:
	movl	$-1, -12(%rbp)
.L239:
	addl	$1, -8(%rbp)
	jmp	.L99
.L68:
	movl	$0, 16+nums_type_flags(%rip)
	movl	$0, 4+nums_type_flags(%rip)
	movl	$0, 12+nums_type_flags(%rip)
	movl	$1, 8+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L241
	movl	$1, -12(%rbp)
	movl	$0, %eax
	call	reset_type_count
	jmp	.L242
.L241:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_exponent
	testl	%eax, %eax
	je	.L243
	movl	$23, -12(%rbp)
	jmp	.L242
.L243:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movb	$46, (%rax)
	movl	$7, -12(%rbp)
.L242:
	addl	$1, -8(%rbp)
	jmp	.L99
.L72:
	movl	$0, 4+nums_type_flags(%rip)
	movl	$0, 12+nums_type_flags(%rip)
	movl	$0, 16+nums_type_flags(%rip)
	movl	$1, 8+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L244
	movl	$25, -12(%rbp)
	jmp	.L245
.L244:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$45, %al
	jne	.L246
	movl	$24, -12(%rbp)
	jmp	.L245
.L246:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$43, %al
	jne	.L247
	movl	$24, -12(%rbp)
	jmp	.L245
.L247:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L248
	movl	$29, -12(%rbp)
	movl	$0, %eax
	call	reset_type_count
	jmp	.L245
.L248:
	movl	$-1, -12(%rbp)
.L245:
	addl	$1, -8(%rbp)
	jmp	.L99
.L71:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L249
	movl	$25, -12(%rbp)
	jmp	.L250
.L249:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L251
	movl	$29, -12(%rbp)
	movl	$0, %eax
	call	reset_type_count
	jmp	.L250
.L251:
	movl	$-1, -12(%rbp)
.L250:
	addl	$1, -8(%rbp)
	jmp	.L99
.L70:
	movl	$0, 16+nums_type_flags(%rip)
	movl	$0, 4+nums_type_flags(%rip)
	movl	$0, 12+nums_type_flags(%rip)
	movl	$1, 8+nums_type_flags(%rip)
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L252
	movl	$1, -12(%rbp)
	jmp	.L253
.L252:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_digit
	testl	%eax, %eax
	je	.L254
	movl	$25, -12(%rbp)
	jmp	.L253
.L254:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_float_suffix
	testl	%eax, %eax
	je	.L255
	movl	$22, -12(%rbp)
	jmp	.L253
.L255:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_long_suffix
	testl	%eax, %eax
	je	.L256
	movl	$27, -12(%rbp)
	jmp	.L253
.L256:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L257
	movl	$29, -12(%rbp)
	jmp	.L253
.L257:
	movl	$-1, -12(%rbp)
.L253:
	addl	$1, -8(%rbp)
	jmp	.L99
.L69:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	is_delimiter
	testl	%eax, %eax
	je	.L258
	movl	$1, -12(%rbp)
	jmp	.L259
.L258:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L260
	movl	$29, -12(%rbp)
	jmp	.L259
.L260:
	movl	$-1, -12(%rbp)
.L259:
	addl	$1, -8(%rbp)
	nop
.L99:
	jmp	.L261
.L262:
	nop
.L64:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	numbers_identificator, .-numbers_identificator
	.section	.rodata
.LC7:
	.string	"lseek failed"
.LC8:
	.string	"File is empty\n"
.LC9:
	.string	"mmap failed"
	.text
	.globl	read_file
	.type	read_file, @function
read_file:
.LFB21:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	-4(%rbp), %eax
	movl	$2, %edx
	movl	$0, %esi
	movl	%eax, %edi
	call	lseek@PLT
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	cmpq	$-1, %rax
	jne	.L264
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	call	perror@PLT
	jmp	.L263
.L264:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L266
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$14, %edx
	movl	$1, %esi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	jmp	.L263
.L266:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	addq	$1, %rax
	movl	-4(%rbp), %edx
	movl	$0, %r9d
	movl	%edx, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	mmap@PLT
	movq	-16(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	cmpq	$-1, %rax
	jne	.L268
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	call	perror@PLT
	movq	-16(%rbp), %rax
	movq	$0, (%rax)
.L268:
	nop
.L263:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	read_file, .-read_file
	.section	.rodata
.LC10:
	.string	"Usage: %s <filename>\n"
.LC11:
	.string	"Failed to open file"
	.text
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$2, -52(%rbp)
	je	.L270
	movq	-64(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	exit@PLT
.L270:
	movq	-64(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -36(%rbp)
	cmpl	$-1, -36(%rbp)
	jne	.L271
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L271:
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	leaq	-32(%rbp), %rdx
	leaq	-24(%rbp), %rcx
	movl	-36(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read_file
	movq	-24(%rbp), %rax
	testq	%rax, %rax
	jne	.L272
	movl	$1, %edi
	call	exit@PLT
.L272:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	numbers_identificator
	movq	-32(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	munmap@PLT
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L274
	call	__stack_chk_fail@PLT
.L274:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
