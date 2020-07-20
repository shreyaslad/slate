#include <stdint.h>
#include <io.h>
#include <sys/trace.h>

struct ubsan_type_t {
	uint16_t type;
	uint16_t info;
	char name[];
};

struct ubsan_src_loc_t {
	char* filename;
	uint32_t line;
	uint32_t col;
};

struct ubsan_overflow_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_type_t* type;
};

struct ubsan_shift_out_of_bounds_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_type_t* lhs;
	struct ubsan_type_t* rhs;
};

struct ubsan_out_of_bounds_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_type_t* array;
	struct ubsan_type_t* index;
};

struct ubsan_type_mismatch_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_type_t* type;
	uint8_t log_align;
	uint8_t type_check_kind;
};

struct ubsan_unreachable_t {
	struct ubsan_src_loc_t loc;
};

struct ubsan_invalid_builtin_t {
	struct ubsan_src_loc_t loc;
};

struct ubsan_alignment_assumption_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_src_loc_t assumption_loc;
	struct ubsan_type_t* type;
};

struct ubsan_invalid_val_t {
	struct ubsan_src_loc_t loc;
	struct ubsan_type_t* type;
};

const char* g_type_check_names[] = {
	"load of",
	"store to",
	"reference binding to",
	"member access within",
	"member call on",
	"constructor call on",
	"downcast of",
	"downcast of",
	"upcast of",
	"cast to virtual base of",
	"_Nonnull binding to"
};

void __ubsan_handle_add_overflow(struct ubsan_overflow_t* data, size_t lhs, size_t rhs) {
	printf(KPRN_WARN, "ubsan: Addition overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_sub_overflow(struct ubsan_overflow_t* data, size_t lhs, size_t rhs) {
	printf(KPRN_WARN, "ubsan: Subtraction overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_mul_overflow(struct ubsan_overflow_t* data, size_t lhs, size_t rhs) {
	printf(KPRN_WARN, "ubsan: Multiplication overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_divrem_overflow(struct ubsan_overflow_t* data, size_t lhs, size_t rhs) {
	printf(KPRN_WARN, "ubsan: Division overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_pointer_overflow(struct ubsan_overflow_t* data, size_t base, size_t result) {
	printf(KPRN_WARN, "ubsan: Pointer overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_negate_overflow(struct ubsan_overflow_t* data, size_t old) {
	printf(KPRN_WARN, "ubsan: Negation overflow! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_shift_out_of_bounds(struct ubsan_shift_out_of_bounds_t* data, size_t idx) {
	printf(KPRN_WARN, "ubsan: Out of bounds shift! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_out_of_bounds(struct ubsan_out_of_bounds_t* data, size_t idx) {
	printf(KPRN_WARN, "ubsan: Index %lu out of bounds for %s\n",
						idx,
						data->array->name);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_type_mismatch_v1(struct ubsan_type_mismatch_t* data, size_t ptr) {
	printf(KPRN_WARN, "ubsan: Type mismatch! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_builtin_unreachable(struct ubsan_unreachable_t* data) {
	printf(KPRN_WARN, "ubsan: Unreachable point! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_invalid_builtin(struct ubsan_invalid_builtin_t* data) {
	printf(KPRN_WARN, "ubsan: Invalid Builtin! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_alignment_assumption(struct ubsan_alignment_assumption_t* data,
										size_t pointer,
										size_t alignment,
										size_t offset) {
	printf(KPRN_WARN, "ubsan: Alignment Assumption! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}

void __ubsan_handle_load_invalid_value(struct ubsan_invalid_val_t* data, size_t value) {
	printf(KPRN_WARN, "ubsan: Load of invalid value! (%s:%u)\n",
						data->loc.filename,
						data->loc.line);
	stacktrace(__builtin_frame_address(1));
}
