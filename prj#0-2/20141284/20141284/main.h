#include "list.h"
#include "hash.h"
#include "bitmap.h"

struct list_item {
	struct list_elem elem;
	int data;
}list_item;
struct _list {
	struct list* list_;
	char name[31];
}_list;

struct hash_item {
	struct hash_elem elem;
	int data;
}hash_item;
struct _hash {
	struct hash* hash_;
	char name[31];
}_hash;

struct _bitmap {
	struct bitmap* bitmap_;
	char name[31];
};

bool l_less(const struct list_elem* a, const struct list_elem* b, void* aux);
unsigned h_func(const struct hash_elem* e, void* aux);
bool h_less(const struct hash_elem* a, const struct hash_elem* b, void* aux);
void h_square(struct hash_elem* e, void* aux);
void h_triple(struct hash_elem* e, void* aux);

void list_swap(struct list_elem* a, struct list_elem* b);
void list_shuffle(struct list* list);
unsigned hash_int_2(int i);
struct bitmap* bitmap_expand(struct bitmap* bitmap, int size);