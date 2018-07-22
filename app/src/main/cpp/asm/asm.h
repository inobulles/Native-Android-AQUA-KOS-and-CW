
#ifndef __AQUA__DECODER_ASM_H
#define __AQUA__DECODER_ASM_H

#include "../kos/lib/macros.h"
#include <stdlib.h>

#include "defs.h"
#include "root.h"

#define printf ALOGV

#define STACK_SIZE (1ll << 16)

#define MAX_VREGS 16 // thread macros
#define MAX_THREADS 256
#define THREAD_CYCLE_STEP 128

#define CURRENT_VERSION 1ull
#define STANDALONE 1 // is the program running off of a KOS or on its own?
#define VERBOSE 1

#define CODE_LENGTH_PADDING 64 // so that if the IP jumps too far, things have the time to exit

#if STANDALONE
	void mfree(void* pointer, unsigned_t bytes) { // `mfree` is a wrapper around `free` found in the KOS
		free(pointer);

	}
#endif

typedef struct {
	unsigned_t used;
	unsigned_t address;
	void* argument;

	unsigned_t registers[REGISTER_LAST];

} thread_t;

typedef struct {
	// enviroment

	unsigned_t registers[REGISTER_LAST + 1];
	unsigned_t* stack;

	unsigned_t reserved_count;
	void** reserved;

	// threading

	signed_t current_thread;
	unsigned_t current_thread_index;

	unsigned_t has_threads;
	thread_t threads[MAX_THREADS];

	// enviroment variables

	signed_t nest;
	unsigned_t base_rip;

	unsigned_t condition_left;
	unsigned_t condition_right;

	unsigned_t next_skip;

	// code

	unsigned_t* code;
	unsigned_t code_length;

	// misc section

	unsigned_t version;
	unsigned_t label_position_offset;
	unsigned_t base_reserved_count;
	unsigned_t main_reserved_position;

	// data section

	char** data_section;
	unsigned_t data_section_size;

	unsigned_t data_section_element_count;
	unsigned_t* data_section_element_element_count;

	// reserved positions section

	unsigned_t reserved_positions_count;
	unsigned_t* reserved_positions;

	// misc

	int error_code;

} program_t;

const int size = sizeof(unsigned_t); // just to make things easier

void program_alloc(program_t* __this) { // allocate the code in the program
	__this->code = (unsigned_t*) malloc((__this->code_length + CODE_LENGTH_PADDING) * size);

}

void program_free(program_t* __this) { // free the code in the program
	mfree(__this->code, (__this->code_length + CODE_LENGTH_PADDING) * size);

}

static void _program_free(program_t* __this) { // free all the sections and environment
	// free stack

	mfree(__this->stack, (unsigned_t) STACK_SIZE * size);

	// free data section

	unsigned_t i;
	for (i = 0; i < __this->data_section_element_count; i++) { // free the data section
		mfree(__this->data_section[i], __this->data_section_element_element_count[i] * sizeof(char));

	}

	mfree(__this->data_section_element_element_count, __this->data_section_element_count * size);
	mfree(__this->data_section, __this->data_section_element_count * sizeof(char*));

	// free reserved and reserved positions section

	mfree(__this->reserved_positions, __this->reserved_positions_count * size);
	mfree(__this->reserved, __this->reserved_count * sizeof(void*));

}

static inline unsigned_t get_value(program_t* __this, unsigned_t type, unsigned_t data) { // get value from an argument's type and data values
	switch (type) {
		case TOKEN_REGISTER: {
			if (data <= REGISTER_LAST_GENERAL_PURPOSE) { // check if general purpose register
				unsigned_t temp = __this->registers[data & 0b0011]; // get family register value

				switch (data & 0b1100) { // mask according to the size of the register
					case REGISTER_TYPE_64: return temp; // don't do anything if 64 bit

					case REGISTER_TYPE_32: return temp & 0xFFFFFFFF;
					case REGISTER_TYPE_16: return temp & 0xFFFF;
					case REGISTER_TYPE_8: return temp & 0xFF;

					default: return temp;

				}

			} else {
				return __this->registers[data];

			}

		}

		case TOKEN_ADDRESS: return *((unsigned_t*) __this->registers[data]); // return the first `unsigned_t` value at the address of the register
		case TOKEN_NUMBER: return data;

		case TOKEN_RESERVED: return __this->reserved_positions[data - __this->label_position_offset];
		case TOKEN_PRERESERVED: return (unsigned_t) __this->reserved[data];

		default: return data;

	}

}

static inline void set_value(program_t* __this, unsigned_t type, unsigned_t data, unsigned_t set) { // set an argument to value from its type and data values
	switch (type) {
		case TOKEN_REGISTER: {
			if (data <= REGISTER_LAST_GENERAL_PURPOSE) { // check if general purpose register
				unsigned_t family = data & 0b0011;

				switch (data & 0b1100) { // set the register family and shift by (times it can fit in an unsigned_t) - 1, to keep one of that data type on the lower end
					case REGISTER_TYPE_64: __this->registers[family] = set; break; // don't do anything if 64 bit

					case REGISTER_TYPE_32: ((unsigned_32bit_t*) __this->registers)[family * 2] = (unsigned_32bit_t) set; break;
					case REGISTER_TYPE_16: ((unsigned_16bit_t*) __this->registers)[family * 4] = (unsigned_16bit_t) set; break;
					case REGISTER_TYPE_8: ((unsigned_8bit_t*) __this->registers)[family * 8] = (unsigned_8bit_t) set; break;

					default: __this->registers[family] = set; break;

				}

			} else {
				__this->registers[data] = set;

			}

			break;

		}

		case TOKEN_ADDRESS: *((unsigned_t*) __this->registers[data]) = set; break; // set the first `unsigned_t` value at the address of the register
		case TOKEN_RESERVED: __this->reserved_positions[data] = set; break; // can't really remember what __this one's for :3

		default: break;

	}

}

static program_t* current_program;

#include <stdarg.h>

static char __printf_buffer[4096];

int __print(const char* format, ...) {
	va_list args;
	va_start(args, format);

	vsprintf(__printf_buffer, format, args);
	printf("%s", __printf_buffer);

	va_end(args);
	return 0;

}

void ____sprintf(char* buffer, const char* format, ...) {
	va_list args;
	va_start(args, format);

	vsprintf(buffer, format, args);
	va_end(args);

}

static void __debug(void) {
	printf("\n=== FLAGS ===\n");

	printf("ZF = 0x%llx\n", current_program->registers[REGISTER_zf]);
	printf("OF = 0x%llx\n", current_program->registers[REGISTER_of]);
	printf("CF = 0x%llx\n", current_program->registers[REGISTER_cf]);

	printf("\n=== IMPORTANT REGISTERS ===\n");

	printf("EAX = 0x%llx\tRAX = 0x%llx\n", current_program->registers[REGISTER_eax], current_program->registers[REGISTER_rax]);
	printf("RDI = 0x%llx\tRSI = 0x%llx\tRDX = 0x%llx\tRCX = 0x%llx\n", current_program->registers[REGISTER_rdi], current_program->registers[REGISTER_rsi], current_program->registers[REGISTER_rdx], current_program->registers[REGISTER_rcx]);
	printf("RIP = 0x%llx\tRSP = 0x%llx\tRBP = 0x%llx\tADDR = 0x%llx\n", current_program->registers[REGISTER_rip], current_program->registers[REGISTER_rsp], current_program->registers[REGISTER_rbp], current_program->registers[REGISTER_addr]);

	printf("\n");

}

static void __exit(unsigned_t status) {
	ALOGV("EXIT CALLED (with status %lld)\n", status);
	current_program->error_code = (int) status;

	current_program->registers[REGISTER_rip] = current_program->code_length + 1;
	current_program->registers[REGISTER_FAMILY_a] = status;

}

static void ____stack_chk_fail() {
	ALOGE("WARNING Stack overflow (STACK_SIZE = %lld)\n", STACK_SIZE);
	__debug();

}

unsigned_t map(unsigned_t x, unsigned_t y) {
	return (unsigned_t) ((double) _UI64_MAX_MARGIN / ((double) y / (double) x));

}

static bool debugging_mode = false;

void break_point(const char* message) {
	printf("DEBUG Setting breakpoint (%s)\n", message);
	debugging_mode = true;

}

unsigned long long index_rom(unsigned long long index) {
	if (index < current_program->code_length) {
		return current_program->code[index];

	} else {
		printf("WARNING You are trying to access ROM that is outside authorized space (index = %lld, current_program->code_length = %lld). Returning 0 ...\n", index, current_program->code_length);
		return 0;

	}

}

signed_t noop(...) {
	printf("WARNING This is a no-operation function. If you reach this, either your current CW/KOS is incomplete, or you have a severe problem in your code (the former is most likely)\n");
	return 1;

}

signed_t __THREAD_END(signed_t address); // thread function prototypes
signed_t __THREAD_START(signed_t address, signed_t freq, void* argument);
signed_t __THREAD_FREQ(signed_t address, signed_t freq);
signed_t __THREAD_INST(signed_t address);

#include "../kos/heap.h" // include all of the KOS functions
#include "../kos/placeholders.h"
#include "../kos/video.h"
#include "../kos/events.h"
#include "../kos/misc.h"
#include "../kos/decoders/bmp.h"

#include "../kos/fs.h"
#include "../kos/socket.h"
#include "../kos/gl/font.h"

void bmp_load(bitmap_image_t* __this, unsigned long long _path);
void bmp_free(bitmap_image_t* __this);

#define NOOP (void*) noop,

static void* base_reserved[] = { // predefined functions from the KOS
	(void*) __print,
	(void*) __debug,
	(void*) __exit,

	(void*) ____stack_chk_fail,

	(void*) heap_malloc,
	(void*) heap_mfree,

	(void*) platform,
	(void*) is_device_supported,

	(void*) get_device,
	(void*) send_device,

	(void*) video_fps,

	(void*) video_clear,
	(void*) video_clear_colour,

	(void*) video_draw,
	(void*) video_flip,

	(void*) get_events,

	(void*) bmp_load,
	(void*) bmp_free,

	(void*) texture_create,
	(void*) texture_remove,

	(void*) surface_new,
	(void*) surface_set_texture,
	(void*) surface_draw,
	(void*) surface_free,

	(void*) new_rectangle,
	(void*) draw_rectangle,

	(void*) video_width,
	(void*) video_height,

	(void*) surface_set_alpha,
	(void*) surface_set_colour,
	(void*) surface_set_x,
	(void*) surface_set_y,

	(void*) free_events,
	(void*) surface_set_layer,
	NOOP //video_zoom,

	(void*) surface_set_width,
	(void*) surface_set_height,

	NOOP //socket_close,
	NOOP //socket_server,
	(void*) socket_support,
	NOOP //socket_receive,
	NOOP //socket_send,

	(void*) get_platform,
	(void*) platform_system,

	(void*) set_video_visibility,
	(void*) ____sprintf,

	(void*) fs_support,
	(void*) fs_read,
	(void*) fs_free,

	NOOP //get_predefined_texture,
	NOOP //update_predefined_texture,

	(void*) __THREAD_START,
	(void*) __THREAD_FREQ,
	(void*) __THREAD_END,
	(void*) __THREAD_INST,

	(void*) map,
	(void*) surface_scroll,

	NOOP // get_font_width,
	NOOP // get_font_height,
	(void*) new_font,
	NOOP // create_texture_from_font,
	NOOP // font_remove,

	(void*) break_point,
	(void*) index_rom,

};

static inline unsigned_t msb(unsigned_t value) { return value & (1ull << (sizeof(unsigned_t) * 8 - 1)); } // calculate most significant bit (of an `unsigned_t`)
static inline unsigned_t lsb(unsigned_t value) { return value & 1; } // calculate least significant bit

static inline unsigned_t zero_extend(unsigned_t value) { return value & 0xFFFFFFFF; } // extend zeroes into the 32 higher bits

signed_t __THREAD_END(signed_t address) {
	printf("TODO __THREAD_END\n");
	return 1;

}

#include "instructions.h"
#include "name_database.h"

static inline void copy_regs(void* __destination, void* __source) { // copy registers from one address to another
	unsigned_t* destination = (unsigned_t*) __destination;
	unsigned_t* source = (unsigned_t*) __source;

	int i;
	for (i = 0; i < REGISTER_LAST + 1; i++) {
		destination[i] = source[i];

	}

}

signed_t __THREAD_START(signed_t address, signed_t freq, void* argument) {
	printf("TODO __THREAD_START\n");
	return 1;

}

signed_t __THREAD_FREQ(signed_t address, signed_t freq) {
	printf("WARNING Deprecated function `__THREAD_FREQ`\n");
	return 1;

}

signed_t __THREAD_INST(signed_t address) {
	printf("TODO __THREAD_INST\n");
	return 1;

}

#define ip __this->registers[REGISTER_rip] // just to make things easier

#define INSTR if (!origin_skip) // __this is so that we can check if we are supposed to execute the next instruction

#define PARAMS2 { param1 = __this->code[++ip]; param2 = __this->code[++ip]; } // we need to create these macros, else the parameters will get mixed up
#define PARAMS4 { param1 = __this->code[++ip]; param2 = __this->code[++ip]; param3 = __this->code[++ip]; param4 = __this->code[++ip]; }
#define PARAMS6 { param1 = __this->code[++ip]; param2 = __this->code[++ip]; param3 = __this->code[++ip]; param4 = __this->code[++ip]; param5 = __this->code[++ip]; param6 = __this->code[++ip]; }

void program_run_setup_phase(program_t* __this) {
	current_program = __this;
	debugging_mode = false;

	// initialize / clear the heap

#if VERBOSE
	printf("Initializing / clearing the heap `init_heap()` ...");
#endif

	init_heap();

	// setup the program

	__this->stack = (unsigned_t*) malloc((size_t) (STACK_SIZE * size));
	__this->registers[REGISTER_rsp] = (unsigned_t) __this->stack + size * (STACK_SIZE / 2); // set the stack pointer to the middle of the stack
	*((unsigned_t*) (__this->registers[REGISTER_rsp] -= size)) = __this->code_length; // push the code length to the stack

	__this->next_skip = 0;
	__this->nest = 0;
	unsigned_t i = 0;

	// setup threads

	for (i = 1; i < MAX_THREADS; i++) {
		__this->threads[i].used = 0;

	}

	__this->current_thread = 0;
	__this->current_thread_index = 0;

	__this->threads[0].used = 1; // main thread is always used, even if threading is not enabled
	__this->has_threads = 0;

	i = 0;

	// parse the misc section

	if (__this->code[i++] != __this->code_length) {
		printf("WARNING The extracted code_length (%lld) is not the same as the predefined one (%lld) ...\n", __this->code[i - 1], __this->code_length);

	}

	__this->version = __this->code[i++];
	__this->base_reserved_count = __this->code[i++];
	__this->label_position_offset = __this->code[i++];
	__this->main_reserved_position = __this->code[i++];

	if (__this->version > CURRENT_VERSION) {
		printf("WARNING The program's version (%lld) is higher than the current version (%lld)\n", __this->version, CURRENT_VERSION);

	}

	// parse the data section

	unsigned_t base_i = i;
	__this->data_section_element_count = 0;

	for (; __this->code[i] != TOKEN_DATA_SECTION_END; i++) { // get the size of the data section
		__this->data_section_element_count += __this->code[i] == TOKEN_DATA_END;

	}

	__this->data_section_size = i;

	__this->data_section_element_element_count = (unsigned_t*) malloc(__this->data_section_element_count * size);
	__this->data_section = (char**) malloc(__this->data_section_element_count * sizeof(char*));

	unsigned_t j = 1;
	unsigned_t k = 0;

	for (i = base_i; __this->code[i] != TOKEN_DATA_SECTION_END; i++) { // get the size of each element in the data section
		if (__this->code[i] == TOKEN_DATA_END) {
			__this->data_section_element_element_count[k++] = j - 1;
			j = 0;

		}

		j++;

	}

#if VERBOSE
	printf("__this->data_section_element_count = %lld\n", __this->data_section_element_count);
#endif

	k = base_i;
	for (i = 0; i < __this->data_section_element_count; i++) { // allocate the data section
		__this->data_section[i] = (char*) malloc(__this->data_section_element_element_count[i] * sizeof(char));

		for (j = 0; j < __this->data_section_element_element_count[i]; j++) { // fill each element of the data section
			unsigned_t l = __this->code[k++];

			if (l == TOKEN_DATA_END) __this->data_section[i][j] = (char) __this->code[k++];
			else __this->data_section[i][j] = (char) l;

		}

	}

	// parse the reserved positions section

	__this->reserved_positions_count = 0;

	unsigned_t old_k = k;
	for (; __this->code[k] != TOKEN_RESERVED_POSITIONS_END; k++) { // get the size of the reserved positions section
		__this->reserved_positions_count++;

	}

#if VERBOSE
	printf("__this->reserved_positions_count = %lld\n", __this->reserved_positions_count);
#endif

	__this->reserved_positions = (unsigned_t*) malloc(__this->reserved_positions_count * size);

	i = 0;
	for (k = old_k + 2 /* offset the last two data section tokens */; __this->code[k] != TOKEN_RESERVED_POSITIONS_END; k++) { // fill each element of the reserved positions section
		__this->reserved_positions[i++] = __this->code[k];

	}

	// create the reserved list (from base_reserved)

	__this->reserved_count = __this->base_reserved_count + __this->data_section_element_count;
	__this->reserved = (void**) malloc(__this->reserved_count * sizeof(void*));

	for (i = 0; i < __this->base_reserved_count; i++) {
		__this->reserved[i] = base_reserved[i];

	}

#if VERBOSE
	printf("__this->reserved_count = %lld\n", __this->reserved_count);
#endif

	unsigned_t base = i;
	for (; i < __this->reserved_count; i++) {
		__this->reserved[i] = __this->data_section[i - base];

	}

	// parse the text section

	__this->base_rip = k + 1;

#if VERBOSE
	printf("__this->base_rip = %lld\n", __this->base_rip);
#endif

	// jump to `main` label

	ip = ++k;
	ip += __this->main_reserved_position;

	*((unsigned_t*) (__this->registers[REGISTER_rsp] -= size)) = ip; // push the `main` label to the stack

#if VERBOSE
	printf("Pushed `main` label to stack at address 0x%llx and IP %lld\n", __this->registers[REGISTER_rsp], ip);
#endif

}

static unsigned_t thread_count = 0;
static unsigned_t origin_skip;

static unsigned_t param1;
static unsigned_t param2;

static unsigned_t param3;
static unsigned_t param4;

static unsigned_t param5;
static unsigned_t param6;

int program_run_loop_phase(program_t* __this) {
	/*if (__this->has_threads && thread_count++ > THREAD_CYCLE_STEP) { /// TODO (also in ret_instruction)
		__thread_swap(__this);
		thread_count = 0;

	}*/

	switch (__this->code[ip]) { // switch though the type
		case TOKEN_KEYWORD: { // fail-safe, I guess?
			if (debugging_mode) { // TODO commands and stuff here
				printf("%lld: %s %s %s %s %s\n", ip,
					get_name(TOKEN_KEYWORD, __this->code[ip + 1]),
					get_name(__this->code[ip + 2], __this->code[ip + 3]),
					get_name(__this->code[ip + 4], __this->code[ip + 5]),
					get_name(__this->code[ip + 6], __this->code[ip + 7]),
					get_name(__this->code[ip + 8], __this->code[ip + 9]));

			}

			origin_skip = __this->next_skip;
			__this->next_skip = 0; // reset the skip

			switch (__this->code[++ip]) { // switch through the data value (instruction)
				case TOKEN_cla: PARAMS6; INSTR cla_instruction(__this, param1, param2, param3, param4, param5, param6); break;
				case TOKEN_mov: PARAMS4; INSTR mov_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_jmp: PARAMS2; INSTR jmp_instruction(__this, param1, param2); break;
				case TOKEN_cnd: PARAMS2; INSTR cnd_instruction(__this, param1, param2); break;
				case TOKEN_cmp: PARAMS4; INSTR cmp_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_sar: PARAMS4; INSTR sar_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_call: PARAMS2; INSTR call_instruction(__this, param1, param2); break;

				case TOKEN_add: PARAMS4; INSTR add_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_sub: PARAMS4; INSTR sub_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_lea: PARAMS4; INSTR lea_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_push: PARAMS2; INSTR push_instruction(__this, param1, param2); break;
				case TOKEN_pop: PARAMS2; INSTR pop_instruction(__this, param1, param2); break;

				case TOKEN_ret: INSTR ret_instruction(__this); break;

				case TOKEN_and: PARAMS4; INSTR and_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_or: PARAMS4; INSTR or_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_mul: PARAMS4; INSTR mul_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_div: PARAMS4; INSTR div_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_mul3: PARAMS6; INSTR mul3_instruction(__this, param1, param2, param3, param4, param5, param6); break;
				case TOKEN_div3: PARAMS6; INSTR div3_instruction(__this, param1, param2, param3, param4, param5, param6); break;

				case TOKEN_mul1: PARAMS2; INSTR mul1_instruction(__this, param1, param2); break;
				case TOKEN_div1: PARAMS2; INSTR div1_instruction(__this, param1, param2); break;

				case TOKEN_shr: PARAMS4; INSTR rsh_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_shl: PARAMS4; INSTR lsh_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_test: PARAMS4; INSTR test_instruction(__this, param1, param2, param3, param4); break;

				case TOKEN_neg: PARAMS2; INSTR neg_instruction(__this, param1, param2); break;
				case TOKEN_not: PARAMS2; INSTR not_instruction(__this, param1, param2); break;

				case TOKEN_cdq: INSTR cdq_instruction(__this); break;
				case TOKEN_cqo: INSTR cqo_instruction(__this); break;

				case TOKEN_rep: PARAMS2; INSTR rep_instruction(__this, param1, param2); break;

				case TOKEN_movzx: PARAMS4; INSTR movzx_instruction(__this, param1, param2, param3, param4); break;
				case TOKEN_set: PARAMS2; mov_instruction(__this, param1, param2, TOKEN_NUMBER, (unsigned_t) !origin_skip); break;

				default: INSTR printf("WARNING Unknown instruction %lld\n", __this->code[ip]);

			}

			ip++; // pass the ending token so we don't have to process it later

		} default: {
			break;

		}

	}

	ip++;

	if (ip >= __this->code_length) {
		printf("IP has surpassed __this->code_length (%lld). Initiating exit sequence ...\n", __this->code_length);

		__this->error_code = (int) __this->registers[REGISTER_FAMILY_a];
		_program_free(__this);

		return 1;

	} else {
		return 0;

	}

}

#endif
