#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <string.h>

#define import_stack(type, container) \
\
	typedef struct _##container \
	{ \
		size_t size; \
		type *buffer; \
		const char *exptr; \
		char exception; \
		size_t (*stack_size)(struct _##container *stack); \
		void (*stack_push)(struct _##container* stack, type value); \
		type (*stack_peek)(struct _##container* stack); \
		type (*stack_pop)(struct _##container* stack); \
		void (*stack_clear)(struct _##container* stack); \
		void (*stack_free)(struct _##container* stack); \
	} _##container; \
	\
	typedef _##container *container; \
	\
	static void container##_exception(container stack) \
	{ \
		stack->exception = *stack->exptr; \
	} \
	static size_t container##_size(container stack) \
	{ \
		return stack->size; \
	} \
	\
	static void container##_push(container stack, type value) \
	{ \
		if (!stack) \
		{ \
			container##_exception(stack); \
		} \
		type *newbuff = (type *)realloc(stack->buffer, (stack->size + 1) * sizeof(type)); \
		if (!newbuff) \
		{ \
			container##_exception(stack); \
		} \
		stack->buffer = newbuff; \
		stack->buffer[stack->size] = value; \
		stack->size++; \
	} \
	\
	static type container##_peek(container stack) \
	{ \
		if (!stack || !stack->buffer || !stack->size) \
		{ \
			container##_exception(stack); \
		} \
		return stack->buffer[stack->size - 1]; \
	} \
	\
	static type container##_pop(container stack) \
	{ \
		type result = container##_peek(stack); \
		stack->size--; \
		type *newbuff = (type *)realloc(stack->buffer, stack->size * sizeof(type)); \
		if (!newbuff) \
		{ \
			container##_exception(stack); \
		} \
		stack->buffer = newbuff; \
		return result; \
	} \
	\
	static void container##_clear(container stack) \
	{ \
		if (!stack) \
		{ \
			container##_exception(stack); \
		} \
		free(stack->buffer); \
		stack->size = 0; \
		stack->buffer = NULL; \
	} \
	\
	static void container##_free(container stack) \
	{ \
		if (stack) \
		{ \
			container##_clear(stack); \
			free(stack); \
		} \
	} \
	\
	static container new_##container() \
	{ \
		container result = (container)malloc(sizeof(_##container)); \
		if (result) \
		{ \
			result->buffer = NULL; \
			result->size = 0; \
			result->exptr = NULL; \
			result->exception = 0; \
			result->stack_size = &container##_size; \
			result->stack_push = &container##_push; \
			result->stack_peek = &container##_peek; \
			result->stack_pop = &container##_pop; \
			result->stack_clear = &container##_clear; \
			result->stack_free = &container##_free; \
		} \
		return result; \
	}

#define new(container) new_##container()

#define stack_size(stack) stack->stack_size(stack)
#define stack_push(stack, value) stack->stack_push(stack, value)
#define stack_peek(stack) stack->stack_peek(stack)
#define stack_pop(stack) stack->stack_pop(stack)
#define stack_clear(stack) stack->stack_clear(stack)
#define stack_free(stack) stack->stack_free(stack)

#endif /* STACK_H */
