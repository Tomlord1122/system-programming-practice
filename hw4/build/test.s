	.text
	.file	"test.cpp"
	.file	0 "/home/tomlord/sys/hw4/build" "../test.cpp" md5 0xd8097c9c770fcf636cff59117506a3aa
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	0 13 0                          // ../test.cpp:13:0
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	stp	x29, x30, [sp, #48]             // 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, wzr
	str	w8, [sp, #12]                   // 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	stur	w0, [x29, #-8]
	stur	x1, [x29, #-16]
	add	x0, sp, #24
	str	x0, [sp]                        // 8-byte Folded Spill
.Ltmp0:
	.loc	0 14 19 prologue_end            // ../test.cpp:14:19
	str	xzr, [sp, #24]
	bl	_ZN12FileDownloadC2Ev
	ldr	x8, [sp]                        // 8-byte Folded Reload
	.loc	0 15 17                         // ../test.cpp:15:17
	str	x8, [sp, #16]
	.loc	0 16 2                          // ../test.cpp:16:2
	ldr	x0, [sp, #16]
	.loc	0 16 5 is_stmt 0                // ../test.cpp:16:5
	ldr	x8, [x0]
	ldr	x8, [x8]
	blr	x8
	.loc	0 17 2 is_stmt 1                // ../test.cpp:17:2
	ldr	x0, [sp, #16]
	.loc	0 17 5 is_stmt 0                // ../test.cpp:17:5
	ldr	x8, [x0]
	ldr	x8, [x8, #8]
	blr	x8
	ldr	w0, [sp, #12]                   // 4-byte Folded Reload
	.cfi_def_cfa wsp, 64
	.loc	0 18 2 epilogue_begin is_stmt 1 // ../test.cpp:18:2
	ldp	x29, x30, [sp, #48]             // 16-byte Folded Reload
	add	sp, sp, #64
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	.cfi_restore w29
	ret
.Ltmp1:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN12FileDownloadC2Ev,"axG",@progbits,_ZN12FileDownloadC2Ev,comdat
	.weak	_ZN12FileDownloadC2Ev           // -- Begin function _ZN12FileDownloadC2Ev
	.p2align	2
	.type	_ZN12FileDownloadC2Ev,@function
_ZN12FileDownloadC2Ev:                  // @_ZN12FileDownloadC2Ev
.Lfunc_begin1:
	.loc	0 3 0                           // ../test.cpp:3:0
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x8, _ZTV12FileDownload
	add	x8, x8, :lo12:_ZTV12FileDownload
	add	x8, x8, #16
	str	x0, [sp, #8]
	ldr	x9, [sp, #8]
.Ltmp3:
	.loc	0 3 7 prologue_end              // ../test.cpp:3:7
	str	x8, [x9]
	.loc	0 3 7 epilogue_begin is_stmt 0  // ../test.cpp:3:7
	add	sp, sp, #16
	.cfi_def_cfa_offset 0
	ret
.Ltmp4:
.Lfunc_end1:
	.size	_ZN12FileDownloadC2Ev, .Lfunc_end1-_ZN12FileDownloadC2Ev
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN12FileDownload5checkEv,"axG",@progbits,_ZN12FileDownload5checkEv,comdat
	.weak	_ZN12FileDownload5checkEv       // -- Begin function _ZN12FileDownload5checkEv
	.p2align	2
	.type	_ZN12FileDownload5checkEv,@function
_ZN12FileDownload5checkEv:              // @_ZN12FileDownload5checkEv
.Lfunc_begin2:
	.loc	0 5 0 is_stmt 1                 // ../test.cpp:5:0
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
.Ltmp5:
	.loc	0 6 3 prologue_end              // ../test.cpp:6:3
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printf
	.cfi_def_cfa wsp, 32
	.loc	0 7 2 epilogue_begin            // ../test.cpp:7:2
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	.cfi_restore w29
	ret
.Ltmp6:
.Lfunc_end2:
	.size	_ZN12FileDownload5checkEv, .Lfunc_end2-_ZN12FileDownload5checkEv
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN12FileDownload4wgetEv,"axG",@progbits,_ZN12FileDownload4wgetEv,comdat
	.weak	_ZN12FileDownload4wgetEv        // -- Begin function _ZN12FileDownload4wgetEv
	.p2align	2
	.type	_ZN12FileDownload4wgetEv,@function
_ZN12FileDownload4wgetEv:               // @_ZN12FileDownload4wgetEv
.Lfunc_begin3:
	.loc	0 8 0                           // ../test.cpp:8:0
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
.Ltmp7:
	.loc	0 9 3 prologue_end              // ../test.cpp:9:3
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	bl	printf
	.cfi_def_cfa wsp, 32
	.loc	0 10 2 epilogue_begin           // ../test.cpp:10:2
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	.cfi_restore w29
	ret
.Ltmp8:
.Lfunc_end3:
	.size	_ZN12FileDownload4wgetEv, .Lfunc_end3-_ZN12FileDownload4wgetEv
	.cfi_endproc
                                        // -- End function
	.type	_ZTV12FileDownload,@object      // @_ZTV12FileDownload
	.section	.data.rel.ro._ZTV12FileDownload,"aGw",@progbits,_ZTV12FileDownload,comdat
	.weak	_ZTV12FileDownload
	.p2align	3, 0x0
_ZTV12FileDownload:
	.xword	0
	.xword	_ZTI12FileDownload
	.xword	_ZN12FileDownload5checkEv
	.xword	_ZN12FileDownload4wgetEv
	.size	_ZTV12FileDownload, 32

	.type	_ZTS12FileDownload,@object      // @_ZTS12FileDownload
	.section	.rodata._ZTS12FileDownload,"aG",@progbits,_ZTS12FileDownload,comdat
	.weak	_ZTS12FileDownload
_ZTS12FileDownload:
	.asciz	"12FileDownload"
	.size	_ZTS12FileDownload, 15

	.type	_ZTI12FileDownload,@object      // @_ZTI12FileDownload
	.section	.data.rel.ro._ZTI12FileDownload,"aGw",@progbits,_ZTI12FileDownload,comdat
	.weak	_ZTI12FileDownload
	.p2align	3, 0x0
_ZTI12FileDownload:
	.xword	_ZTVN10__cxxabiv117__class_type_infoE+16
	.xword	_ZTS12FileDownload
	.size	_ZTI12FileDownload, 16

	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"virtual check\n"
	.size	.L.str, 15

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"virtual wget\n"
	.size	.L.str.1, 14

	.file	1 "<stdin>"
	.section	.debug_abbrev,"",@progbits
	.byte	1                               // Abbreviation Code
	.byte	17                              // DW_TAG_compile_unit
	.byte	1                               // DW_CHILDREN_yes
	.byte	37                              // DW_AT_producer
	.byte	37                              // DW_FORM_strx1
	.byte	19                              // DW_AT_language
	.byte	5                               // DW_FORM_data2
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	114                             // DW_AT_str_offsets_base
	.byte	23                              // DW_FORM_sec_offset
	.byte	16                              // DW_AT_stmt_list
	.byte	23                              // DW_FORM_sec_offset
	.byte	27                              // DW_AT_comp_dir
	.byte	37                              // DW_FORM_strx1
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	85                              // DW_AT_ranges
	.byte	35                              // DW_FORM_rnglistx
	.byte	115                             // DW_AT_addr_base
	.byte	23                              // DW_FORM_sec_offset
	.byte	116                             // DW_AT_rnglists_base
	.byte	23                              // DW_FORM_sec_offset
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	2                               // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	3                               // Abbreviation Code
	.byte	1                               // DW_TAG_array_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	4                               // Abbreviation Code
	.byte	33                              // DW_TAG_subrange_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	55                              // DW_AT_count
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	5                               // Abbreviation Code
	.byte	38                              // DW_TAG_const_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	6                               // Abbreviation Code
	.byte	36                              // DW_TAG_base_type
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	62                              // DW_AT_encoding
	.byte	11                              // DW_FORM_data1
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	7                               // Abbreviation Code
	.byte	36                              // DW_TAG_base_type
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	62                              // DW_AT_encoding
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	8                               // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	27                              // DW_FORM_addrx
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	64                              // DW_AT_frame_base
	.byte	24                              // DW_FORM_exprloc
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	9                               // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	10                              // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	11                              // Abbreviation Code
	.byte	2                               // DW_TAG_class_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	29                              // DW_AT_containing_type
	.byte	19                              // DW_FORM_ref4
	.byte	54                              // DW_AT_calling_convention
	.byte	11                              // DW_FORM_data1
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	12                              // Abbreviation Code
	.byte	13                              // DW_TAG_member
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	56                              // DW_AT_data_member_location
	.byte	11                              // DW_FORM_data1
	.byte	52                              // DW_AT_artificial
	.byte	25                              // DW_FORM_flag_present
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	13                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	110                             // DW_AT_linkage_name
	.byte	37                              // DW_FORM_strx1
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	76                              // DW_AT_virtuality
	.byte	11                              // DW_FORM_data1
	.byte	77                              // DW_AT_vtable_elem_location
	.byte	24                              // DW_FORM_exprloc
	.byte	60                              // DW_AT_declaration
	.byte	25                              // DW_FORM_flag_present
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	50                              // DW_AT_accessibility
	.byte	11                              // DW_FORM_data1
	.byte	29                              // DW_AT_containing_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	14                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	52                              // DW_AT_artificial
	.byte	25                              // DW_FORM_flag_present
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	15                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	60                              // DW_AT_declaration
	.byte	25                              // DW_FORM_flag_present
	.byte	52                              // DW_AT_artificial
	.byte	25                              // DW_FORM_flag_present
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	50                              // DW_AT_accessibility
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	16                              // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	17                              // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	18                              // Abbreviation Code
	.byte	21                              // DW_TAG_subroutine_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	19                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	27                              // DW_FORM_addrx
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	64                              // DW_AT_frame_base
	.byte	24                              // DW_FORM_exprloc
	.byte	100                             // DW_AT_object_pointer
	.byte	19                              // DW_FORM_ref4
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	110                             // DW_AT_linkage_name
	.byte	37                              // DW_FORM_strx1
	.byte	71                              // DW_AT_specification
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	20                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	3                               // DW_AT_name
	.byte	37                              // DW_FORM_strx1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	52                              // DW_AT_artificial
	.byte	25                              // DW_FORM_flag_present
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	21                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	27                              // DW_FORM_addrx
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	64                              // DW_AT_frame_base
	.byte	24                              // DW_FORM_exprloc
	.byte	100                             // DW_AT_object_pointer
	.byte	19                              // DW_FORM_ref4
	.byte	71                              // DW_AT_specification
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	0                               // EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.word	.Ldebug_info_end0-.Ldebug_info_start0 // Length of Unit
.Ldebug_info_start0:
	.hword	5                               // DWARF version number
	.byte	1                               // DWARF Unit Type
	.byte	8                               // Address Size (in bytes)
	.word	.debug_abbrev                   // Offset Into Abbrev. Section
	.byte	1                               // Abbrev [1] 0xc:0x151 DW_TAG_compile_unit
	.byte	0                               // DW_AT_producer
	.hword	33                              // DW_AT_language
	.byte	1                               // DW_AT_name
	.word	.Lstr_offsets_base0             // DW_AT_str_offsets_base
	.word	.Lline_table_start0             // DW_AT_stmt_list
	.byte	2                               // DW_AT_comp_dir
	.xword	0                               // DW_AT_low_pc
	.byte	0                               // DW_AT_ranges
	.word	.Laddr_table_base0              // DW_AT_addr_base
	.word	.Lrnglists_table_base0          // DW_AT_rnglists_base
	.byte	2                               // Abbrev [2] 0x2b:0xa DW_TAG_variable
	.word	53                              // DW_AT_type
	.byte	0                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	2                               // DW_AT_location
	.byte	161
	.byte	0
	.byte	3                               // Abbrev [3] 0x35:0xc DW_TAG_array_type
	.word	65                              // DW_AT_type
	.byte	4                               // Abbrev [4] 0x3a:0x6 DW_TAG_subrange_type
	.word	74                              // DW_AT_type
	.byte	15                              // DW_AT_count
	.byte	0                               // End Of Children Mark
	.byte	5                               // Abbrev [5] 0x41:0x5 DW_TAG_const_type
	.word	70                              // DW_AT_type
	.byte	6                               // Abbrev [6] 0x46:0x4 DW_TAG_base_type
	.byte	3                               // DW_AT_name
	.byte	8                               // DW_AT_encoding
	.byte	1                               // DW_AT_byte_size
	.byte	7                               // Abbrev [7] 0x4a:0x4 DW_TAG_base_type
	.byte	4                               // DW_AT_name
	.byte	8                               // DW_AT_byte_size
	.byte	7                               // DW_AT_encoding
	.byte	2                               // Abbrev [2] 0x4e:0xa DW_TAG_variable
	.word	88                              // DW_AT_type
	.byte	0                               // DW_AT_decl_file
	.byte	9                               // DW_AT_decl_line
	.byte	2                               // DW_AT_location
	.byte	161
	.byte	1
	.byte	3                               // Abbrev [3] 0x58:0xc DW_TAG_array_type
	.word	65                              // DW_AT_type
	.byte	4                               // Abbrev [4] 0x5d:0x6 DW_TAG_subrange_type
	.word	74                              // DW_AT_type
	.byte	14                              // DW_AT_count
	.byte	0                               // End Of Children Mark
	.byte	8                               // Abbrev [8] 0x64:0x3c DW_TAG_subprogram
	.byte	2                               // DW_AT_low_pc
	.word	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	109
	.byte	13                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	13                              // DW_AT_decl_line
	.word	243                             // DW_AT_type
                                        // DW_AT_external
	.byte	9                               // Abbrev [9] 0x73:0xb DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	120
	.byte	15                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	13                              // DW_AT_decl_line
	.word	243                             // DW_AT_type
	.byte	9                               // Abbrev [9] 0x7e:0xb DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	112
	.byte	16                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	13                              // DW_AT_decl_line
	.word	333                             // DW_AT_type
	.byte	10                              // Abbrev [10] 0x89:0xb DW_TAG_variable
	.byte	2                               // DW_AT_location
	.byte	143
	.byte	24
	.byte	17                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	14                              // DW_AT_decl_line
	.word	160                             // DW_AT_type
	.byte	10                              // Abbrev [10] 0x94:0xb DW_TAG_variable
	.byte	2                               // DW_AT_location
	.byte	143
	.byte	16
	.byte	18                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	15                              // DW_AT_decl_line
	.word	343                             // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	11                              // Abbrev [11] 0xa0:0x43 DW_TAG_class_type
	.word	160                             // DW_AT_containing_type
	.byte	4                               // DW_AT_calling_convention
	.byte	12                              // DW_AT_name
	.byte	8                               // DW_AT_byte_size
	.byte	0                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	12                              // Abbrev [12] 0xaa:0x7 DW_TAG_member
	.byte	5                               // DW_AT_name
	.word	227                             // DW_AT_type
	.byte	0                               // DW_AT_data_member_location
                                        // DW_AT_artificial
	.byte	13                              // Abbrev [13] 0xb1:0x14 DW_TAG_subprogram
	.byte	8                               // DW_AT_linkage_name
	.byte	9                               // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	1                               // DW_AT_virtuality
	.byte	2                               // DW_AT_vtable_elem_location
	.byte	16
	.byte	0
                                        // DW_AT_declaration
                                        // DW_AT_external
	.byte	1                               // DW_AT_accessibility
                                        // DW_ACCESS_public
	.word	160                             // DW_AT_containing_type
	.byte	14                              // Abbrev [14] 0xbf:0x5 DW_TAG_formal_parameter
	.word	247                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	13                              // Abbrev [13] 0xc5:0x14 DW_TAG_subprogram
	.byte	10                              // DW_AT_linkage_name
	.byte	11                              // DW_AT_name
	.byte	0                               // DW_AT_decl_file
	.byte	8                               // DW_AT_decl_line
	.byte	1                               // DW_AT_virtuality
	.byte	2                               // DW_AT_vtable_elem_location
	.byte	16
	.byte	1
                                        // DW_AT_declaration
                                        // DW_AT_external
	.byte	1                               // DW_AT_accessibility
                                        // DW_ACCESS_public
	.word	160                             // DW_AT_containing_type
	.byte	14                              // Abbrev [14] 0xd3:0x5 DW_TAG_formal_parameter
	.word	247                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0xd9:0x9 DW_TAG_subprogram
	.byte	12                              // DW_AT_name
                                        // DW_AT_declaration
                                        // DW_AT_artificial
                                        // DW_AT_external
	.byte	1                               // DW_AT_accessibility
                                        // DW_ACCESS_public
	.byte	14                              // Abbrev [14] 0xdc:0x5 DW_TAG_formal_parameter
	.word	247                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	16                              // Abbrev [16] 0xe3:0x5 DW_TAG_pointer_type
	.word	232                             // DW_AT_type
	.byte	17                              // Abbrev [17] 0xe8:0x6 DW_TAG_pointer_type
	.word	238                             // DW_AT_type
	.byte	7                               // DW_AT_name
	.byte	18                              // Abbrev [18] 0xee:0x5 DW_TAG_subroutine_type
	.word	243                             // DW_AT_type
	.byte	6                               // Abbrev [6] 0xf3:0x4 DW_TAG_base_type
	.byte	6                               // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	16                              // Abbrev [16] 0xf7:0x5 DW_TAG_pointer_type
	.word	160                             // DW_AT_type
	.byte	19                              // Abbrev [19] 0xfc:0x1d DW_TAG_subprogram
	.byte	3                               // DW_AT_low_pc
	.word	.Lfunc_end1-.Lfunc_begin1       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	111
	.word	271                             // DW_AT_object_pointer
	.byte	0                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	14                              // DW_AT_linkage_name
	.word	217                             // DW_AT_specification
	.byte	20                              // Abbrev [20] 0x10f:0x9 DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	8
	.byte	19                              // DW_AT_name
	.word	343                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	21                              // Abbrev [21] 0x119:0x1a DW_TAG_subprogram
	.byte	4                               // DW_AT_low_pc
	.word	.Lfunc_end2-.Lfunc_begin2       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	109
	.word	297                             // DW_AT_object_pointer
	.word	177                             // DW_AT_specification
	.byte	20                              // Abbrev [20] 0x129:0x9 DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	143
	.byte	8
	.byte	19                              // DW_AT_name
	.word	343                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	21                              // Abbrev [21] 0x133:0x1a DW_TAG_subprogram
	.byte	5                               // DW_AT_low_pc
	.word	.Lfunc_end3-.Lfunc_begin3       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	109
	.word	323                             // DW_AT_object_pointer
	.word	197                             // DW_AT_specification
	.byte	20                              // Abbrev [20] 0x143:0x9 DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	143
	.byte	8
	.byte	19                              // DW_AT_name
	.word	343                             // DW_AT_type
                                        // DW_AT_artificial
	.byte	0                               // End Of Children Mark
	.byte	16                              // Abbrev [16] 0x14d:0x5 DW_TAG_pointer_type
	.word	338                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x152:0x5 DW_TAG_pointer_type
	.word	70                              // DW_AT_type
	.byte	16                              // Abbrev [16] 0x157:0x5 DW_TAG_pointer_type
	.word	160                             // DW_AT_type
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.word	.Ldebug_list_header_end0-.Ldebug_list_header_start0 // Length
.Ldebug_list_header_start0:
	.hword	5                               // Version
	.byte	8                               // Address size
	.byte	0                               // Segment selector size
	.word	1                               // Offset entry count
.Lrnglists_table_base0:
	.word	.Ldebug_ranges0-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	3                               // DW_RLE_startx_length
	.byte	2                               //   start index
	.uleb128 .Lfunc_end0-.Lfunc_begin0      //   length
	.byte	3                               // DW_RLE_startx_length
	.byte	3                               //   start index
	.uleb128 .Lfunc_end1-.Lfunc_begin1      //   length
	.byte	3                               // DW_RLE_startx_length
	.byte	4                               //   start index
	.uleb128 .Lfunc_end2-.Lfunc_begin2      //   length
	.byte	3                               // DW_RLE_startx_length
	.byte	5                               //   start index
	.uleb128 .Lfunc_end3-.Lfunc_begin3      //   length
	.byte	0                               // DW_RLE_end_of_list
.Ldebug_list_header_end0:
	.section	.debug_str_offsets,"",@progbits
	.word	84                              // Length of String Offsets Set
	.hword	5
	.hword	0
.Lstr_offsets_base0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 16.0.0 (https://github.com/llvm/llvm-project.git 08d094a0e457360ad8b94b017d2dc277e697ca76)" // string offset=0
.Linfo_string1:
	.asciz	"../test.cpp"                   // string offset=105
.Linfo_string2:
	.asciz	"/home/tomlord/sys/hw4/build"   // string offset=117
.Linfo_string3:
	.asciz	"char"                          // string offset=145
.Linfo_string4:
	.asciz	"__ARRAY_SIZE_TYPE__"           // string offset=150
.Linfo_string5:
	.asciz	"_vptr$FileDownload"            // string offset=170
.Linfo_string6:
	.asciz	"int"                           // string offset=189
.Linfo_string7:
	.asciz	"__vtbl_ptr_type"               // string offset=193
.Linfo_string8:
	.asciz	"_ZN12FileDownload5checkEv"     // string offset=209
.Linfo_string9:
	.asciz	"check"                         // string offset=235
.Linfo_string10:
	.asciz	"_ZN12FileDownload4wgetEv"      // string offset=241
.Linfo_string11:
	.asciz	"wget"                          // string offset=266
.Linfo_string12:
	.asciz	"FileDownload"                  // string offset=271
.Linfo_string13:
	.asciz	"main"                          // string offset=284
.Linfo_string14:
	.asciz	"_ZN12FileDownloadC2Ev"         // string offset=289
.Linfo_string15:
	.asciz	"argc"                          // string offset=311
.Linfo_string16:
	.asciz	"argv"                          // string offset=316
.Linfo_string17:
	.asciz	"f"                             // string offset=321
.Linfo_string18:
	.asciz	"T"                             // string offset=323
.Linfo_string19:
	.asciz	"this"                          // string offset=325
	.section	.debug_str_offsets,"",@progbits
	.word	.Linfo_string0
	.word	.Linfo_string1
	.word	.Linfo_string2
	.word	.Linfo_string3
	.word	.Linfo_string4
	.word	.Linfo_string5
	.word	.Linfo_string6
	.word	.Linfo_string7
	.word	.Linfo_string8
	.word	.Linfo_string9
	.word	.Linfo_string10
	.word	.Linfo_string11
	.word	.Linfo_string12
	.word	.Linfo_string13
	.word	.Linfo_string14
	.word	.Linfo_string15
	.word	.Linfo_string16
	.word	.Linfo_string17
	.word	.Linfo_string18
	.word	.Linfo_string19
	.section	.debug_addr,"",@progbits
	.word	.Ldebug_addr_end0-.Ldebug_addr_start0 // Length of contribution
.Ldebug_addr_start0:
	.hword	5                               // DWARF version number
	.byte	8                               // Address size
	.byte	0                               // Segment selector size
.Laddr_table_base0:
	.xword	.L.str
	.xword	.L.str.1
	.xword	.Lfunc_begin0
	.xword	.Lfunc_begin1
	.xword	.Lfunc_begin2
	.xword	.Lfunc_begin3
.Ldebug_addr_end0:
	.ident	"clang version 16.0.0 (https://github.com/llvm/llvm-project.git 08d094a0e457360ad8b94b017d2dc277e697ca76)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym printf
	.addrsig_sym _ZTVN10__cxxabiv117__class_type_infoE
	.addrsig_sym _ZTS12FileDownload
	.addrsig_sym _ZTI12FileDownload
	.section	.debug_line,"",@progbits
.Lline_table_start0:
