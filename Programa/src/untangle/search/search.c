#include "search.h"
#include "stack.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "dictionary/dictionary.h"
#include <unistd.h>

/** Operation list */
static Operation* ops;
/** Total amount of operations */
static uint8_t op_count;
/** Solution state */
static Puzzle* goal;
/** Dictionary data structure for detection of repeated states */
static Dictionary* dict;

/** Looks for a solution inside a bounded depth */
bool search_bounded(Puzzle* state, uint8_t depth, uint8_t bound, Cell* prev_cell, Operation op, Stack* stack)
{
	/* If we've exceeded the allowed depth */
	if(depth > bound) return false;

	/* Check the table for this state */
	Cell* this_cell = dictionary_get_cell(dict, state, prev_cell, op);

	/* If hash_table is implemented */
	if(this_cell)
	{
		/* First encounter with this state */
		if(this_cell -> new_cell)
		{
			/* Tag it */
			this_cell -> new_cell = false;
		}
		else
		{
			/* If found from a costier route, it's of no interest to us */
			if(depth > this_cell -> info.depth) return false;
			/* Reentrance is only allowed when bound has increased */
			if(depth == this_cell -> info.depth && bound <= this_cell -> info.bound) return false;
		}
		/* Update state info */
		this_cell -> info.depth = depth;
		this_cell -> info.bound = bound;
	}

	/* If I'm at the solution, I'm done */
	if(puzzle_equals(state, goal)) return true;

	/* Try all operations in order */
	for(int i = 0; i < op_count; i++)
	{
		/* Generate next state */
		operation_execute(state, ops[i]);

		/* Save operation that did this */
		stack_push(stack, ops[i]);

		/* Expand son */
		if(search_bounded(state, depth + 1, bound, this_cell, ops[i], stack))
		{
			return true;
		}

		/* Undo operation, go back to current state */
		operation_revert(state, stack_pop(stack));
	}

	return false;
}

/** Looks for a sequence of steps to get from state to goal */
Stack* search(Puzzle* state, Puzzle* final)
{
	/* Pre-generate all possible operations for a puzzle */
	ops = operation_generate_all(state);
	/* Amount of possible operations */
	op_count = 2 * (state -> height + state -> width);
	/* Cache the final state */
	goal = final;

	/* Initializes dictionary module */
	dict = dictionary_init(state);


	/* A stack to record each step to the solution */
	Stack* stack = stack_init();

	/* Search bounds */
	uint8_t bound = 0;

	/* Assumes puzzle is solvable */
	while(true)
	{
		/* If a solution is found */
		if(search_bounded(state, 0, bound, NULL, 0, stack))
		{
			fprintf(stderr, "Solución encontrada a profundidad %hhu\n", bound);
			/* Frees associated resources */
			free(ops);
			dictionary_free(dict);

			/* Return solution */
			return stack;
		}
		/* Expand search depth */
		else
		{
			bound++;
		}
	}
	abort();
}
