	.file	"benchmark.c"
# GNU C17 (Debian 10.2.1-6) version 10.2.1 20210110 (x86_64-linux-gnu)
#	compiled by GNU C version 10.2.1 20210110, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.0, isl version isl-0.23-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu benchmark.c -mtune=generic
# -march=x86-64 -O2 -fverbose-asm -fasynchronous-unwind-tables
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -falign-functions -falign-jumps -falign-labels -falign-loops
# -fallocation-dce -fasynchronous-unwind-tables -fauto-inc-dec
# -fbranch-count-reg -fcaller-saves -fcode-hoisting
# -fcombine-stack-adjustments -fcompare-elim -fcprop-registers
# -fcrossjumping -fcse-follow-jumps -fdefer-pop
# -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively
# -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-symbols
# -feliminate-unused-debug-types -fexpensive-optimizations
# -fforward-propagate -ffp-int-builtin-inexact -ffunction-cse -fgcse
# -fgcse-lm -fgnu-unique -fguess-branch-probability -fhoist-adjacent-loads
# -fident -fif-conversion -fif-conversion2 -findirect-inlining -finline
# -finline-atomics -finline-functions -finline-functions-called-once
# -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-icf
# -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
# -fipa-ra -fipa-reference -fipa-reference-addressable -fipa-sra
# -fipa-stack-alignment -fipa-vrp -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots
# -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
# -fleading-underscore -flifetime-dse -flra-remat -fmath-errno
# -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
# -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen
# -fpartial-inlining -fpeephole -fpeephole2 -fplt -fprefetch-loop-arrays
# -free -freg-struct-return -freorder-blocks -freorder-blocks-and-partition
# -freorder-functions -frerun-cse-after-loop
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fschedule-insns2
# -fsemantic-interposition -fshow-column -fshrink-wrap
# -fshrink-wrap-separate -fsigned-zeros -fsplit-ivs-in-unroller
# -fsplit-wide-types -fssa-backprop -fssa-phiopt -fstdarg-opt
# -fstore-merging -fstrict-aliasing -fstrict-volatile-bitfields
# -fsync-libcalls -fthread-jumps -ftoplevel-reorder -ftrapping-math
# -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-ch
# -ftree-coalesce-vars -ftree-copy-prop -ftree-cselim -ftree-dce
# -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
# -ftree-loop-distribute-patterns -ftree-loop-if-convert -ftree-loop-im
# -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
# -ftree-phiprop -ftree-pre -ftree-pta -ftree-reassoc -ftree-scev-cprop
# -ftree-sink -ftree-slsr -ftree-sra -ftree-switch-conversion
# -ftree-tail-merge -ftree-ter -ftree-vrp -funit-at-a-time -funwind-tables
# -fverbose-asm -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -malign-stringops -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387 -mfxsr
# -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone
# -msse -msse2 -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
	.p2align 4
	.globl	mysecond
	.type	mysecond, @function
mysecond:
.LFB12:
	.cfi_startproc
	subq	$40, %rsp	#,
	.cfi_def_cfa_offset 48
# benchmark.c:35:   i = gettimeofday(&tp,&tzp);
	leaq	8(%rsp), %rsi	#, tmp88
	leaq	16(%rsp), %rdi	#, tmp89
	call	gettimeofday@PLT	#
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	pxor	%xmm0, %xmm0	# tmp90
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	pxor	%xmm1, %xmm1	# tmp93
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	cvtsi2sdq	24(%rsp), %xmm0	# tp.tv_usec, tmp90
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	mulsd	.LC0(%rip), %xmm0	#, tmp91
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	cvtsi2sdq	16(%rsp), %xmm1	# tp.tv_sec, tmp93
# benchmark.c:37: }
	addq	$40, %rsp	#,
	.cfi_def_cfa_offset 8
# benchmark.c:36:   return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	addsd	%xmm1, %xmm0	# tmp93, <retval>
# benchmark.c:37: }
	ret	
	.cfi_endproc
.LFE12:
	.size	mysecond, .-mysecond
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"%f"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	subq	$24, %rsp	#,
	.cfi_def_cfa_offset 32
# benchmark.c:19:   t1 = mysecond();
	xorl	%eax, %eax	#
	call	mysecond	#
# benchmark.c:22:   t2 = mysecond();
	xorl	%eax, %eax	#
# benchmark.c:19:   t1 = mysecond();
	movsd	%xmm0, 8(%rsp)	# tmp88, %sfp
# benchmark.c:22:   t2 = mysecond();
	call	mysecond	#
# benchmark.c:25:   printf("%f",(t2-t1));  
	subsd	8(%rsp), %xmm0	# %sfp, tmp86
	movl	$1, %eax	#,
	leaq	.LC1(%rip), %rdi	#,
	call	printf@PLT	#
# benchmark.c:27: }
	xorl	%eax, %eax	#
	addq	$24, %rsp	#,
	.cfi_def_cfa_offset 8
	ret	
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	-1598689907
	.long	1051772663
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
