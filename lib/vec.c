#include <vec.h>

int vec_rmi(struct vector_t* v, void* item) {
	return -1;
}

int vec_rm(struct vector_t* v, size_t idx) {


	if ((idx + 1) > v->n)
		return 0;

	for (int i = idx; i < v->n; i++) {
		v->items[i] = v->items[i + 1];
	}

	krealloc(v->items, v->n - 1);
	v->n--;

	return 1;
}

void* vec_g(struct vector_t* v, size_t idx) {
	if ((idx + 1) > v->n)
		return NULL;

	return v->items[idx];
}

int vec_i(struct vector_t* v, void* item, size_t idx) {
	if ((idx + 1) > v->n)
		return 0;

	void** cpy = kmalloc(sizeof(void *) * v->n++);
	
	for (int i = 0; i < idx; i++) {
		cpy[i] = v->items[i];
	}

	cpy[idx] = item;

	for (int i = (idx + 1); i < (v->n - idx); i++) {
		cpy[i] = v->items[i];
	}

	kfree(v->items);
	v->items = cpy;

	return 1;
}

int vec_a(struct vector_t* v, void* item) {
	v->items = krealloc(v->items, v->n + 1);
	v->items[v->n] = item;
	v->n++;

	return 1;
}

int vec_n(struct vector_t* v) {
	v->items = NULL;
	v->n = 0;

	return 1;
}