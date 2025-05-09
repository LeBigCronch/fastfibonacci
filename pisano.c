#include <stdio.h>
#include <stdlib.h>

struct Pair {
	long a;
	long b;
};

struct TreeNode {
	struct Pair *val;
	struct TreeNode *left;
	struct TreeNode *right;
};

long pisano_period(long);

void update(struct Pair *, int);
int compare(struct Pair *, struct Pair *);
struct TreeNode *insert(struct TreeNode *, struct Pair *);
struct TreeNode *get(struct TreeNode *, struct Pair *);
void destroy(struct TreeNode *);
long fib_remainder_base(long, long);

/* Get the peroid that the fibonacci numbers
 * repeat in (the pisano period) remainder base */
long pisano_period(long base) {
	struct TreeNode *root = NULL;

	long period = 0;

	struct Pair *pair = (struct Pair *) malloc(sizeof(struct Pair));
	pair->a = 1;
	pair->b = 0;

	while (get(root, pair) == NULL) {
		root = insert(root, pair);
		update(pair, base);
		++period;
	}

	destroy(root);
	return period;
}

/* Update a pair to the next
 * fibonacci number remainder base */
void update(struct Pair *pair, int base) {
	long t = pair->a;
	pair->a = (pair->a + pair->b) % base;
	pair->b = t;
}

/* Compare two pairs by their first number. If
 * those are equal compare by the second. */
int compare(struct Pair *fst, struct Pair *sec) {
	return (fst->a-sec->a)?fst->a-sec->a:fst->b-sec->b;
}

/* Insert a pair into a search tree */
struct TreeNode *insert(struct TreeNode *p, struct Pair *pair) {
	if (p == NULL) {
		p = (struct TreeNode *) malloc(sizeof(struct TreeNode));
		p->left = NULL;
		p->right = NULL;
		struct Pair *copy = (struct Pair *) malloc(sizeof(struct Pair));
		copy->a = pair->a;
		copy->b = pair->b;
		p->val = copy;
		return p;
	}
	int c;
	if (!(c = compare(pair, p->val))) {
		return p;
	}
	if (c > 0) {
		p->left = insert(p->left, pair);
	} else {
		p->right = insert(p->right, pair);
	}
	return p;
}

/* Get a pointer to an equal pair
 * from a search tree, NULL if none match */
struct TreeNode *get(struct TreeNode *p, struct Pair *pair) {
	if (!p) { return NULL; }
	int c;
	if (!(c = compare(pair, p->val))) { return p; }
	if (c > 0) {
		return get(p->left, pair);
	} else {
		return get(p->right, pair);
	}
}

/* Free the memory used by
 * a search tree */
void destroy(struct TreeNode * p) {
	if (!p) { return; }
	destroy(p->left);
	destroy(p->right);
	free(p->val);
	free(p);
}

/* Calculate the remainder base b
 * of the nth fibonacci number. */
long fib_remainder_base(long index, long base) {
	index %= pisano_period(base);
	struct Pair pair;
	pair.a = 1;
	pair.b = 0;

	while (index--) {
		update(&pair, base);
	}

	return pair.b;
}
