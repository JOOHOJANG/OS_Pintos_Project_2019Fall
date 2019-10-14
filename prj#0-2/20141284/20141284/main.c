#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"

struct _list Lists[10];
int Lists_idx;

struct _hash Hashes[10];
int Hashes_idx;

struct _bitmap Bitmaps[10];
int Bitmaps_idx;

int main(int argc, char* argv[]) {
	//	freopen(argv[1], "r", stdin);

	while (1) {
		char command[25];
		scanf("%s ", command);

		if (command[0] == 'c') {
			char type[10];
			scanf("%s", type);

			if (type[0] == 'b') {
				char bitmap_name[31];
				int bitmap_size;
				scanf("%s %d", bitmap_name, &bitmap_size);

				Bitmaps[Bitmaps_idx].bitmap_ = bitmap_create(bitmap_size);
				strcpy(Bitmaps[Bitmaps_idx].name, bitmap_name);

				++Bitmaps_idx;

			}
			else if (type[0] == 'h') {
				char hash_name[31];
				scanf("%s", hash_name);

				Hashes[Hashes_idx].hash_ = (struct hash*)malloc(sizeof(struct hash));
				hash_init(Hashes[Hashes_idx].hash_, h_func, h_less, NULL);
				strcpy(Hashes[Hashes_idx].name, hash_name);

				++Hashes_idx;
			}
			else if (type[0] == 'l') {
				char list_name[31];
				scanf("%s", list_name);

				Lists[Lists_idx].list_ = (struct list*)malloc(sizeof(struct list));
				list_init(Lists[Lists_idx].list_);
				strcpy(Lists[Lists_idx].name, list_name);

				++Lists_idx;
			}
		}
		else if (command[0] == 'd' && command[1] == 'u') {
			char name[31];
			scanf("%s", name);

			bool flag = false;
			for (int i = 0; i < Lists_idx; ++i) {
				if (!strcmp(Lists[i].name, name)) {
					struct list_elem* x;
					for (x = list_begin(Lists[i].list_); x != list_end(Lists[i].list_); x = list_next(x)) {
						printf("%d ", list_entry(x, struct list_item, elem)->data);
					}
					printf("\n");

					flag = true;
					break;
				}
			}

			if (flag) continue;
			for (int i = 0; i < Hashes_idx; ++i) {
				if (!strcmp(Hashes[i].name, name)) {
					struct hash_iterator* x = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));
					hash_first(x, Hashes[i].hash_);
					while (1) {
						if (!hash_next(x)) break;
						printf("%d ", hash_entry(hash_cur(x), struct hash_item, elem)->data);
					}
					printf("\n");

					flag = true;
					break;
				}
			}

			if (flag) continue;
			for (int i = 0; i < Bitmaps_idx; ++i) {
				if (!strcmp(Bitmaps[i].name, name)) {
					int len = bitmap_size(Bitmaps[i].bitmap_);
					for (int j = 0; j < len; ++j) {
						if (bitmap_test(Bitmaps[i].bitmap_, j)) printf("1");
						else printf("0");
					}
					printf("\n");

					flag = true;
					break;
				}
			}
		}
		else if (command[0] == 'd' && command[1] == 'e') {
			char name[31];
			scanf("%s", name);

			bool flag = false;
			for (int i = 0; i < Lists_idx; ++i) {
				if (!flag) {
					if (!strcmp(Lists[i].name, name)) {
						while (!list_empty(Lists[i].list_)) {
							struct list_elem* x = list_pop_front(Lists[i].list_);
							free(x);
						}

						flag = true;
					}
				}
				else {
					strcpy(Lists[i - 1].name, Lists[i].name);
					Lists[i - 1] = Lists[i];
				}
			}
			if (flag) {
				Lists[--Lists_idx].list_ = NULL;
				strcpy(Lists[Lists_idx].name, "");
				continue;
			}

			for (int i = 0; i < Hashes_idx; ++i) {
				if (!flag) {
					if (!strcmp(Hashes[i].name, name)) {
						free(Hashes[i].hash_);

						flag = true;
					}
				}
				else {
					strcpy(Hashes[i - 1].name, Hashes[i].name);
					Hashes[i - 1] = Hashes[i];
				}
			}
			if (flag) {
				Hashes[--Hashes_idx].hash_ = NULL;
				strcpy(Hashes[Hashes_idx].name, "");
				continue;
			}

			for (int i = 0; i < Bitmaps_idx; ++i) {
				if (!flag) {
					if (!strcmp(Bitmaps[i].name, name)) {
						free(Bitmaps[i].bitmap_);

						flag = true;
					}
				}
				else {
					strcpy(Bitmaps[i - 1].name, Bitmaps[i].name);
					Bitmaps[i - 1] = Bitmaps[i];
				}
			}
			if (flag) {
				Bitmaps[--Bitmaps_idx].bitmap_ = NULL;
				strcpy(Bitmaps[Bitmaps_idx].name, "");
				continue;
			}
		}
		else if (command[0] == 'q') {
			break;
		}
		else if (!strcmp(command, "list_unique")) {//list_unique list0 list1
			char name_set[65], name1[31], name2[31];
			int i, i_idx;
			fgets(name_set, 66, stdin);

			for (i = 0; name_set[i] != ' ' && name_set[i] != 0 && name_set[i] != '\n'; ++i) name1[i] = name_set[i];
			for (int i = 0; i < Lists_idx; ++i) {
				if (!strcmp(Lists[i].name, name1)) {
					i_idx = i;
					break;
				}
			}

			if (name_set[i] == ' ') {
				int j = 0, j_idx;
				for (i = i + 1; name_set[i] != ' ' && name_set[i] != 0 && name_set[i] != '\n'; ++i) name2[j++] = name_set[i];
				for (int k = 0; k < Lists_idx; ++k) {
					if (!strcmp(Lists[k].name, name2)) {
						j_idx = k;
						//break;
					}
				}

				list_unique(Lists[i_idx].list_, Lists[j_idx].list_, l_less, NULL);
			}
			else {
				struct list* temp = (struct list*)malloc(sizeof(struct list));
				list_init(temp);
				list_unique(Lists[i_idx].list_, temp, l_less, NULL);
			}
		}
		else if (command[0] == 'l') {
			int idx;
			char name[31];
			scanf("%s", name);
			for (int i = 0; i < Lists_idx; ++i) {
				if (!strcmp(Lists[i].name, name)) {
					idx = i;
					break;
				}
			}

			if (!strcmp(command, "list_front")) {//list_front list0
				struct list_elem* x = list_front(Lists[idx].list_);
				printf("%d\n", list_entry(x, struct list_item, elem)->data);
			}
			else if (!strcmp(command, "list_back")) {//list_back list0
				struct list_elem* x = list_back(Lists[idx].list_);
				printf("%d\n", list_entry(x, struct list_item, elem)->data);
			}
			else if (!strcmp(command, "list_push_front")) {//list_push_back list0 1
				int x;
				scanf("%d", &x);

				struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
				new_item->data = x;
				list_push_front(Lists[idx].list_, &new_item->elem);
			}
			else if (!strcmp(command, "list_push_back")) {//list_push_front list0 3
				int x;
				scanf("%d", &x);

				struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
				new_item->data = x;
				list_push_back(Lists[idx].list_, &new_item->elem);
			}
			else if (!strcmp(command, "list_pop_front")) {//list_pop_back list0
				list_pop_front(Lists[idx].list_);
			}
			else if (!strcmp(command, "list_pop_back")) {//list_pop_front list0
				list_pop_back(Lists[idx].list_);
			}
			else if (!strcmp(command, "list_insert")) {//list_isnert list0 1 4
				int location, x;
				scanf("%d %d", &location, &x);

				struct list_elem* before = list_begin(Lists[idx].list_);
				for (int i = 0; i < location; ++i) {
					before = list_next(before);
				}
				struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
				new_item->data = x;
				list_insert(before, &new_item->elem);
			}
			else if (!strcmp(command, "list_insert_ordered")) {//list_insert_ordered list0 5
				int x;
				scanf("%d", &x);

				struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
				new_item->data = x;
				list_insert_ordered(Lists[idx].list_, &new_item->elem, l_less, NULL);
			}
			else if (!strcmp(command, "list_remove")) {//list_remove list0 2
				int location;
				scanf("%d", &location);

				struct list_elem* delete_elem = list_begin(Lists[idx].list_);
				for (int i = 0; i < location; ++i) {
					delete_elem = list_next(delete_elem);
				}
				list_remove(delete_elem);
			}
			else if (!strcmp(command, "list_max")) {//list_max list0
				printf("%d\n", list_entry(list_max(Lists[idx].list_, l_less, NULL), struct list_item, elem)->data);
			}
			else if (!strcmp(command, "list_min")) {//list_min list0
				printf("%d\n", list_entry(list_min(Lists[idx].list_, l_less, NULL), struct list_item, elem)->data);
			}
			else if (!strcmp(command, "list_size")) {//list_size list0
				printf("%zu\n", list_size(Lists[idx].list_));
			}
			else if (!strcmp(command, "list_empty")) {//list_empty list0
				if (list_empty(Lists[idx].list_)) printf("true\n");
				else printf("false\n");
			}
			else if (!strcmp(command, "list_reverse")) {//list_reverse list0
				list_reverse(Lists[idx].list_);
			}
			else if (!strcmp(command, "list_sort")) {//list_sort list0
				list_sort(Lists[idx].list_, l_less, NULL);
			}
			else if (!strcmp(command, "list_splice")) {//list_splice list0 2 list1 1 4
				int location, src_start_idx, src_end_idx;
				char src_name[31];
				scanf("%d %s %d %d", &location, src_name, &src_start_idx, &src_end_idx);

				for (int i = 0; i < Lists_idx; ++i) {
					if (!strcmp(Lists[i].name, src_name)) {
						struct list_elem* dest, * src_start, * src_end;
						dest = list_begin(Lists[idx].list_);
						src_start = list_begin(Lists[i].list_);
						src_end = list_begin(Lists[i].list_);

						for (int i = 0; i < location; ++i) dest = list_next(dest);
						for (int i = 0; i < src_start_idx; ++i) src_start = list_next(src_start);
						for (int i = 0; i < src_end_idx; ++i) src_end = list_next(src_end);

						list_splice(dest, src_start, src_end);

						break;
					}
				}
			}
			else if (!strcmp(command, "list_swap")) {//list_swap list0 1 3
				int id1, id2;
				scanf("%d %d", &id1, &id2);

				struct list_elem* a = list_begin(Lists[idx].list_), * b = list_begin(Lists[idx].list_);
				for (int i = 0; i < id1; ++i) a = list_next(a);
				for (int i = 0; i < id2; ++i) b = list_next(b);

				list_swap(a, b);
			}
		}
		else if (command[0] == 'h') {
			int idx;
			char name[31];
			scanf("%s", name);
			for (int i = 0; i < Lists_idx; ++i) {
				if (!strcmp(Hashes[i].name, name)) {
					idx = i;
					break;
				}
			}

			if (!strcmp(command, "hash_insert")) {//hash_insert hash0 1
				int x;
				scanf("%d", &x);

				struct  hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
				new_item->data = x;
				hash_insert(Hashes[idx].hash_, &new_item->elem);
			}
			else if (!strcmp(command, "hash_apply")) {//hash_apply hash0 square(triple)
				char what[30];
				scanf("%s", what);

				if (!strcmp(what, "square")) {
					hash_apply(Hashes[idx].hash_, h_square);
				}
				else if (!strcmp(what, "triple")) {
					hash_apply(Hashes[idx].hash_, h_triple);
				}
			}
			else if (!strcmp(command, "hash_delete")) {//hash_delete hash0 10
				int x;
				scanf("%d", &x);

				struct  hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
				new_item->data = x;
				hash_delete(Hashes[idx].hash_, &new_item->elem);
			}
			else if (!strcmp(command, "hash_empty")) {//hash_empty hash0
				if (hash_empty(Hashes[idx].hash_)) printf("true\n");
				else printf("false\n");
			}
			else if (!strcmp(command, "hash_find")) {//hash_find hash0 10
				int x;
				scanf("%d", &x);

				struct  hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
				new_item->data = x;
				if (hash_find(Hashes[idx].hash_, &new_item->elem) != NULL)
					printf("%d\n", new_item->data);
			}
			else if (!strcmp(command, "hash_replace")) {//hash_replace hash0 10
				int x;
				scanf("%d", &x);

				struct  hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
				new_item->data = x;
				hash_replace(Hashes[idx].hash_, &new_item->elem);
			}
			else if (!strcmp(command, "hash_size")) {//hash_size hash0
				printf("%zu\n", hash_size(Hashes[idx].hash_));
			}
			else if (!strcmp(command, "hash_clear")) {//hash_clear hash0
				hash_clear(Hashes[idx].hash_, NULL);
			}
		}
		else if (command[0] == 'b') {
			int idx;
			char name[31];
			scanf("%s", name);
			for (int i = 0; i < Bitmaps_idx; ++i) {
				if (!strcmp(Bitmaps[i].name, name)) {
					idx = i;
					break;
				}
			}

			if (!strcmp(command, "bitmap_mark")) {//bitmap_mark bm0 0
				int location;
				scanf("%d", &location);

				bitmap_mark(Bitmaps[idx].bitmap_, location);
			}
			else if (!strcmp(command, "bitmap_all")) {//bitmap_all bm0 0 1
				int start, cnt;
				scanf("%d %d", &start, &cnt);

				if (bitmap_all(Bitmaps[idx].bitmap_, start, cnt)) printf("true\n");
				else printf("false\n");
			}
			else if (!strcmp(command, "bitmap_any")) {//bitmap_any bm0 0 1
				int start, cnt;
				scanf("%d %d", &start, &cnt);

				if (bitmap_any(Bitmaps[idx].bitmap_, start, cnt)) printf("true\n");
				else printf("false\n");
			}
			else if (!strcmp(command, "bitmap_contains")) {//bitmap_contains bm0 0 2 true
				int start, cnt;
				char val[8];
				scanf("%d %d %s", &start, &cnt, val);

				if (!strcmp(val, "true")) {
					if (bitmap_contains(Bitmaps[idx].bitmap_, start, cnt, true)) printf("true\n");
					else printf("false\n");
				}
				else {
					if (bitmap_contains(Bitmaps[idx].bitmap_, start, cnt, false)) printf("true\n");
					else printf("false\n");
				}
			}
			else if (!strcmp(command, "bitmap_count")) {//bitmap_count bm0 0 8 true
				int start, cnt;
				char val[8];
				scanf("%d %d %s", &start, &cnt, val);

				if (!strcmp(val, "true")) printf("%zu\n", bitmap_count(Bitmaps[idx].bitmap_, start, cnt, true));
				else printf("%zu\n", bitmap_count(Bitmaps[idx].bitmap_, start, cnt, false));
			}
			else if (!strcmp(command, "bitmap_dump")) {//bitmap_dump bm0
				bitmap_dump(Bitmaps[idx].bitmap_);
			}
			else if (!strcmp(command, "bitmap_expand")) {//bitmap_expand bm0 2
				int plus;
				scanf("%d", &plus);

				Bitmaps[idx].bitmap_ = bitmap_expand(Bitmaps[idx].bitmap_, plus);
			}
			else if (!strcmp(command, "bitmap_flip")) {//bitmap_flip bm0 5
				int location;
				scanf("%d", &location);

				bitmap_flip(Bitmaps[idx].bitmap_, location);
			}
			else if (!strcmp(command, "bitmap_none")) {//bitmap_none bm0 0 1
				int start, cnt;
				scanf("%d %d", &start, &cnt);

				if (bitmap_none(Bitmaps[idx].bitmap_, start, cnt)) printf("true\n");
				else printf("false\n");
			}
			else if (!strcmp(command, "bitmap_reset")) {//bitmap_reset bm0 5
				int location;
				scanf("%d", &location);

				bitmap_reset(Bitmaps[idx].bitmap_, location);
			}
			else if (!strcmp(command, "bitmap_scan_and_flip")) {//bitmap_scan_and_flip bm0 0 1 true
				int start, cnt;
				char val[8];
				scanf("%d %d %s", &start, &cnt, val);

				if (!strcmp(val, "true")) printf("%u\n", bitmap_scan_and_flip(Bitmaps[idx].bitmap_, start, cnt, true));
				else printf("%u\n", bitmap_scan_and_flip(Bitmaps[idx].bitmap_, start, cnt, false));
			}
			else if (!strcmp(command, "bitmap_scan")) {//bitmap_scan bm0 0 1 true
				int start, cnt;
				char val[8];
				scanf("%d %d %s", &start, &cnt, val);

				if (!strcmp(val, "true")) printf("%u\n", bitmap_scan(Bitmaps[idx].bitmap_, start, cnt, true));
				else printf("%u\n", bitmap_scan(Bitmaps[idx].bitmap_, start, cnt, false));
			}
			else if (!strcmp(command, "bitmap_set_all")) {//bitmap_set_all bm0 false
				char val[8];
				scanf("%s", val);

				if (!strcmp(val, "true")) bitmap_set_all(Bitmaps[idx].bitmap_, true);
				else bitmap_set_all(Bitmaps[idx].bitmap_, false);
			}
			else if (!strcmp(command, "bitmap_set")) {//bitmap_set bm0 0 true
				int location;
				char val[8];
				scanf("%d %s", &location, val);

				if (!strcmp(val, "true")) bitmap_set(Bitmaps[idx].bitmap_, location, true);
				else bitmap_set(Bitmaps[idx].bitmap_, location, false);
			}
			else if (!strcmp(command, "bitmap_set_multiple")) {//bitmap_set_multiple bm0 0 4 true
				int start, cnt;
				char val[8];
				scanf("%d %d %s", &start, &cnt, val);

				if (!strcmp(val, "true")) bitmap_set_multiple(Bitmaps[idx].bitmap_, start, cnt, true);
				else bitmap_set_multiple(Bitmaps[idx].bitmap_, start, cnt, false);
			}
			else if (!strcmp(command, "bitmap_size")) {//bitmap_size bm0
				printf("%zu\n", bitmap_size(Bitmaps[idx].bitmap_));
			}
			else if (!strcmp(command, "bitmap_test")) {//bitmap_test bm0 4
				int location;
				scanf("%d", &location);

				if (bitmap_test(Bitmaps[idx].bitmap_, location)) printf("true\n");
				else printf("false\n");
			}
		}
	}

	return 0;
}

bool l_less(const struct list_elem* a, const struct list_elem* b, void* aux) {
	int a_data = list_entry(a, struct list_item, elem)->data,
		b_data = list_entry(b, struct list_item, elem)->data;

	if (a_data < b_data) return true;
	else return false;
}

unsigned h_func(const struct hash_elem* e, void* aux) {
	return hash_int(hash_entry(e, struct hash_item, elem)->data);
}

bool h_less(const struct hash_elem* a, const struct hash_elem* b, void* aux) {
	int a_data = hash_entry(a, struct hash_item, elem)->data,
		b_data = hash_entry(b, struct hash_item, elem)->data;

	if (a_data < b_data) return true;
	else return false;
}

void h_square(struct hash_elem* e, void* aux) {
	int x = hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data = x * x;
}
void h_triple(struct hash_elem* e, void* aux) {
	int x = hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data = x * x * x;
}


void list_swap(struct list_elem* a, struct list_elem* b) {
	int t = list_entry(a, struct list_item, elem)->data;
	list_entry(a, struct list_item, elem)->data = list_entry(b, struct list_item, elem)->data;
	list_entry(b, struct list_item, elem)->data = t;
}

void list_shuffle(struct list* list) {
	srand((unsigned int)time(NULL));

	int size = list_size(list);
	struct list_elem* a, * b;

	for (int i = 0; i < size; ++i) {
		int times = rand() % size;
		a = list_begin(list);
		for (int j = 0; j < times; ++j) {
			a = list_next(a);
		}

		times = rand() % size;
		b = list_begin(list);
		for (int j = 0; j < times; ++j) {
			b = list_next(b);
		}

		list_swap(a, b);
	}
}

unsigned hash_int_2(int i) {
	return i / 10;
}

struct bitmap* bitmap_expand(struct bitmap* bitmap, int size) {
	struct bitmap* res;
	int new_size = size + bitmap_size(bitmap);

	res = bitmap_create(new_size);

	for (int i = 0; i < bitmap_size(bitmap); ++i) {
		if (bitmap_test(bitmap, i)) bitmap_mark(res, i);
	}

	return res;
}