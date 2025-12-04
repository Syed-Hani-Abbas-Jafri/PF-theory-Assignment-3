#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int pop;
    int time;
} Bk;

typedef struct {
    Bk *arr;
    int cap;
    int cnt;
    int clock;
} Lib;

void init(Lib *l, int c);
int find(Lib *l, int id);
int get_lru(Lib *l);
void add(Lib *l, int id, int p);
void acc(Lib *l, int id);

int main() {
    int c, q;
    if (scanf("%d %d", &c, &q) != 2) return 1;

    Lib l;
    init(&l, c);

    for (int i = 0; i < q; i++) {
        char op[10];
        scanf("%s", op);

        if (strcmp(op, "ADD") == 0) {
            int id, p;
            scanf("%d %d", &id, &p);
            add(&l, id, p);
        } 
        else if (strcmp(op, "ACCESS") == 0) {
            int id;
            scanf("%d", &id);
            acc(&l, id);
        }
    }

    free(l.arr);
    return 0;
}

void add(Lib *l, int id, int p) {
    l->clock++;
    int idx = find(l, id);

    if (idx != -1) {
        l->arr[idx].pop = p;
        l->arr[idx].time = l->clock;
    } else {
        if (l->cnt < l->cap) {
            idx = l->cnt;
            l->cnt++;
        } else {
            idx = get_lru(l);
        }
        l->arr[idx].id = id;
        l->arr[idx].pop = p;
        l->arr[idx].time = l->clock;
    }
}

void acc(Lib *l, int id) {
    l->clock++;
    int idx = find(l, id);

    if (idx != -1) {
        printf("%d\n", l->arr[idx].pop);
        l->arr[idx].time = l->clock;
    } else {
        printf("-1\n");
    }
}

void init(Lib *l, int c) {
    l->cap = c;
    l->cnt = 0;
    l->clock = 0;
    l->arr = (Bk *)malloc(sizeof(Bk) * c);
    for(int i = 0; i < c; i++) {
        l->arr[i].id = -1;
    }
}

int find(Lib *l, int id) {
    for (int i = 0; i < l->cnt; i++) {
        if (l->arr[i].id == id) {
            return i;
        }
    }
    return -1;
}

int get_lru(Lib *l) {
    if (l->cnt == 0) return -1;
    int idx = 0;
    int min = l->arr[0].time;
    for (int i = 1; i < l->cnt; i++) {
        if (l->arr[i].time < min) {
            min = l->arr[i].time;
            idx = i;
        }
    }
    return idx;
}
