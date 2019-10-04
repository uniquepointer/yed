#ifndef __ARRAY_H__
#define __ARRAY_H__

/* #include "internal.h" */

#define ARRAY_DEFAULT_CAP (8)

typedef struct {
    void *data;
    int   elem_size;
    int   used;
    int   capacity;
} array_t;

static array_t _array_make(int elem_size);
static void * _array_push(array_t *array, void *elem);
static void * _array_next_elem(array_t *array);
static void _array_delete(array_t *array, int idx);
static void _array_zero_term(array_t *array);

#define array_make(T) \
    (_array_make(sizeof(T)))

#define array_free(array) \
    (_array_free(&(array)))

#define array_len(array) \
    ((array).used)

#define array_next_elem(array) \
    (_array_next_elem(&(array)))

#define array_push(array, elem) \
    (_array_push(&(array), &(elem)))

#define array_insert(array, idx, elem) \
    (_array_insert(&(array), idx, &(elem)))

#define array_delete(array, idx) \
    (_array_delete(&(array), idx))

#define array_pop(array) \
    (_array_delete(&(array), (array).used - 1))

#define array_clear(array) \
    ((array).used = 0)

#define array_item(array, idx) \
    ((array).data + ((array).elem_size * (idx)))

#define array_last(array) \
    ((array).used ? ((array).data + ((array).elem_size * ((array).used - 1))) : NULL)


#define array_traverse(array, it)                                                \
    for (it = (array).data;                                                      \
         it < (__typeof(it))((array).data + ((array).used * (array).elem_size)); \
         it += 1)

#define array_traverse_from(array, it, starting_idx)                             \
    for (it = (array).data + ((starting_idx) * (array).elem_size);               \
         it < (__typeof(it))((array).data + ((array).used * (array).elem_size)); \
         it += 1)

#define array_data(array) ((array).data)

#define array_zero_term(array) (_array_zero_term(&(array)))

#endif
