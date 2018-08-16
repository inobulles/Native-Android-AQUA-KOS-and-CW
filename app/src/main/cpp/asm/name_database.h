//
// Created by obiwac on 18.07.18.
//

#ifndef ANDROID_NAME_DATABASE_H
#define ANDROID_NAME_DATABASE_H

#include "defs.h"

const char* get_name(unsigned long long type, unsigned long long data) {
	if (type == TOKEN_REGISTER) {
		if (data == REGISTER_rax) return "rax";
		else if (data == REGISTER_rcx) return "rcx";
		else if (data == REGISTER_rdx) return "rdx";
		else if (data == REGISTER_rbx) return "rbx";

		else if (data == REGISTER_eax) return "eax";
		else if (data == REGISTER_ecx) return "ecx";
		else if (data == REGISTER_edx) return "edx";
		else if (data == REGISTER_ebx) return "ebx";

		else if (data == REGISTER_ax) return "ax";
		else if (data == REGISTER_cx) return "cx";
		else if (data == REGISTER_dx) return "dx";
		else if (data == REGISTER_bx) return "bx";

		else if (data == REGISTER_al) return "al";
		else if (data == REGISTER_cl) return "cl";
		else if (data == REGISTER_dl) return "dl";
		else if (data == REGISTER_bl) return "bl";

		else if (data == REGISTER_sp) return "sp";
		else if (data == REGISTER_bp) return "bp";
		else if (data == REGISTER_si) return "si";
		else if (data == REGISTER_di) return "di";
		else if (data == REGISTER_esi) return "esi";
		else if (data == REGISTER_edi) return "edi";
		else if (data == REGISTER_ebp) return "ebp";
		else if (data == REGISTER_esp) return "esp";
		else if (data == REGISTER_rsp) return "rsp";
		else if (data == REGISTER_rbp) return "rbp";
		else if (data == REGISTER_rsi) return "rsi";
		else if (data == REGISTER_rdi) return "rdi";
		else if (data == REGISTER_ss) return "ss";
		else if (data == REGISTER_cs) return "cs";
		else if (data == REGISTER_ds) return "ds";
		else if (data == REGISTER_es) return "es";
		else if (data == REGISTER_fs) return "fs";
		else if (data == REGISTER_gs) return "gs";
		else if (data == REGISTER_eip) return "eip";
		else if (data == REGISTER_rip) return "rip";
		else if (data == REGISTER_r8d) return "r8d";
		else if (data == REGISTER_r8) return "r8";
		else if (data == REGISTER_addr) return "addr";
		else if (data == REGISTER_r12) return "r12";
		else if (data == REGISTER_r13) return "r13";
		else if (data == REGISTER_r14) return "r14";
		else if (data == REGISTER_r15) return "r15";
		else if (data == REGISTER_r9) return "r9";
		else if (data == REGISTER_r10) return "r10";
		else if (data == REGISTER_r11) return "r11";

		else if (data == REGISTER_cf) return "cf";
		else if (data == REGISTER_of) return "of";
		else if (data == REGISTER_zf) return "zf";
		else if (data == REGISTER_sf) return "sf";
		else if (data == REGISTER_pf) return "pf";
		else if (data == REGISTER_af) return "af";
			
		else return "<register>";

	} else if (type == TOKEN_KEYWORD) {
		if (data == TOKEN_mov) return "mov";
		else if (data == TOKEN_ret) return "ret";
		else if (data == TOKEN_jmp) return "jmp";
		else if (data == TOKEN_call) return "call";
		else if (data == TOKEN_push) return "push";
		else if (data == TOKEN_pop) return "pop";
		else if (data == TOKEN_add) return "add";
		else if (data == TOKEN_sub) return "sub";
		else if (data == TOKEN_mul) return "mul";
		else if (data == TOKEN_div) return "div";
		else if (data == TOKEN_mod) return "mod";
		else if (data == TOKEN_and) return "and";
		else if (data == TOKEN_or) return "or";
		else if (data == TOKEN_xor) return "xor";
		else if (data == TOKEN_inc) return "inc";
		else if (data == TOKEN_dec) return "dec";
		else if (data == TOKEN_lea) return "lea";
		else if (data == TOKEN_shl) return "shl";
		else if (data == TOKEN_shr) return "shr";
		else if (data == TOKEN_cmp) return "cmp";
		else if (data == TOKEN_imul) return "imul";
		else if (data == TOKEN_idiv) return "idiv";
		else if (data == TOKEN_movsx) return "movsx";
		else if (data == TOKEN_cdq) return "cdq";
		else if (data == TOKEN_test) return "test";
		else if (data == TOKEN_sal) return "sal";
		else if (data == TOKEN_sar) return "sar";
		else if (data == TOKEN_neg) return "neg";
		else if (data == TOKEN_cla) return "cla";
		else if (data == TOKEN_mul3) return "mul2";
		else if (data == TOKEN_div3) return "div2";
		else if (data == TOKEN_mul1) return "mul1";
		else if (data == TOKEN_div1) return "div1";
		else if (data == TOKEN_cqo) return "cqo";
		else if (data == TOKEN_not) return "not";
		else if (data == TOKEN_rep) return "rep";
		else if (data == TOKEN_movsq) return "movsq";
		else if (data == TOKEN_cnd) return "cnd";
		else if (data == TOKEN_movzx) return "movzx";
		else if (data == TOKEN_set) return "set";

		else return "<instruction>";

	} else if (type == TOKEN_NUMBER) {
		if (data == CONDITION_Z) return "z";
		else if (data == CONDITION_NZ) return "nz";
		else if (data == CONDITION_S) return "s";
		else if (data == CONDITION_NS) return "ns";
		else if (data == CONDITION_E) return "e";
		else if (data == CONDITION_NE) return "ne";
		else if (data == CONDITION_LT) return "lt";
		else if (data == CONDITION_GT) return "gt";
		else if (data == CONDITION_LE) return "le";
		else if (data == CONDITION_GE) return "ge";
		else if (data == CONDITION_C) return "c";
		else if (data == CONDITION_NC) return "nc";

		else return "<number>";

	}

	else if (type == TOKEN_RESERVED) return "<reserved>";
	else if (type == TOKEN_PRERESERVED) return "<pre>";
	else if (type == TOKEN_QTYPE) return "<qtype>";
	else if (type == TOKEN_ADDRESS) return "<address>";

	else if (type == TOKEN_END) return "|||";

	else return "<unknown>";

}

#endif
