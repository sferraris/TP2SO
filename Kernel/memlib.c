#include <memlib.h>

typedef struct n {
    int filled;
    int childs;
    struct n * leftChild;
    struct n * rightChild;
    void * pos;
}node;

node * root;
int index = 0;
node * nodes;

node * createTreeRec(int level, void * pos, uint64_t size) {
    int aux = index;
    index++;
    if (level < MAXLEVEL) {
        nodes[aux].leftChild = createTreeRec(level + 1, pos, size/2);
        nodes[aux].rightChild = createTreeRec(level + 1, pos + size/2, size/2);
    }
    nodes[aux].pos = pos;
    return &nodes[aux];
}

void createTree() {
    root = createTreeRec(0, HEAPSTART, TOTALHEAP);
}

void * lookPosRec(node * n, uint64_t nsize, uint64_t size, int level) {
    if (size > (nsize/2) || level == MAXLEVEL) {
        if (n->filled == 0 && n->childs == 0) {
            n->filled = 1;
            return n->pos;
        }
        return (void *)0;
    }
    if (n->filled == 1)
        return (void *)0;
    if (n->childs == 0)
        n->childs = 1;
    void * var = lookPosRec(n->leftChild, nsize/2, size, level + 1);
    if (var != (void *)0) {
        return var;
    }
    return lookPosRec(n->rightChild, nsize/2, size, level + 1);
}

void * malloc(uint64_t size) {
    if (size > TOTALHEAP)
        return (void *)0;
    return lookPosRec(root, TOTALHEAP, size, 0);
}

void freeRec(node * n, void * pos) {
    if (n->pos == pos) {
        if (n->childs == 1)
            freeRec(n->leftChild, pos);
        else {
            n->filled = 0;
            return;
        }
    }
    else if (pos < n->rightChild->pos) {
        freeRec(n->leftChild, pos);
    }
    else
        freeRec(n->rightChild, pos);
    if (n->leftChild->filled == 0 && n->leftChild->childs == 0 && n->rightChild->filled == 0 && n->rightChild->childs == 0)
        n->childs = 0;
}

void free(void * p) {
    if (p < HEAPSTART || p > (HEAPSTART + TOTALHEAP))
        return -1;
    freeRec(root, p);
}

void initMem() {
    nodes = HEAPSTART + TOTALHEAP;
    createTree();
}