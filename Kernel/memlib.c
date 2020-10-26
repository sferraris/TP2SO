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


#ifdef MM_BUDDY



node * createTreeRec(int level, void * pos, uint64_t size) {
    int aux = index;
    index++;
    if (level < MAXLEVEL) {
        nodes[aux].leftChild = createTreeRec(level + 1, pos, size/2);
        nodes[aux].rightChild = createTreeRec(level + 1, pos + size/2, size/2);
    }
    nodes[aux].filled = 0;
    nodes[aux].childs = 0;
    nodes[aux].pos = pos;
    return &nodes[aux];
}

void createTree() {
    root = createTreeRec(0, HEAPSTART, TOTALHEAP);
}

void * lookPosRec(node * n, uint64_t nsize, uint64_t size, int level) {
    if (size > (nsize/2) || level >= MAXLEVEL) {
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
    printString("Buddy");
    if (size > TOTALHEAP)
        return (void *)0;
    void * aux = lookPosRec(root, TOTALHEAP, size, 0);
    return aux;
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
    memset(HEAPSTART, 0, TOTALHEAP*2);
    nodes = HEAPSTART + TOTALHEAP;
    createTree();
}

#else

int bitmap[TOTALBLOCKS];

int mapLocation (int blocks);
void setBitmap (int loc, int blocks);
void unsetBitmap (int loc);

void * malloc (uint64_t size) {
    printString("Bitmap");
    uint64_t aux = size;
    int blocksCount = (size/HEAPBLOCK) + (((size%HEAPBLOCK)==0)? 0 : 1);
    int loc = mapLocation(blocksCount);
    if (loc == -1)
        return (void*) 0;
    setBitmap (loc, blocksCount);
    return (void *) (HEAPSTART+ loc*HEAPBLOCK); 
}

int mapLocation (int blocks) {
    int avail = 0;
    int i;
    for (i=0; i < TOTALBLOCKS && avail < blocks; i++) {
        if (bitmap[i] > EB)
            avail = 0;
        else
            avail++;
    }
    return (avail < blocks)? -1 : (i-blocks);
}

void setBitmap (int loc, int blocks) {
    int i;
    for (i=0; i < blocks-1; i++)
        bitmap[loc+i] = FB;
    bitmap[loc+i] = EOB; 
}

void free(void * p) {
    int aux = (int) p;
    unsetBitmap((aux - HEAPSTART)/HEAPBLOCK);
}

void unsetBitmap (int loc) {
    int i=loc;
    while (bitmap[i] != EOB)
        bitmap[i++] = EB;
    bitmap[i] = EB;
}

void initMem() {
}

#endif