#ifndef Typedef_H_
#define Typedef_H_

#define MAXPAGES 50000
#define FRAMESIZE 4096
#define PAGESIZE FRAMESIZE
#define DEFBUFSIZE 1024

struct bFrame
{
    char field[FRAMESIZE];
};

struct BCB
{
    BCB() = default;
    BCB(int page_id, int frame_id, int count, int dirty)
    {
        this->page_id = page_id;
        this->frame_id = frame_id;
        this->count = count;
        this->dirty = dirty;
        this->next = nullptr;
        this->replacer_next = nullptr;
        this->replacer_prev = nullptr;
        this->referenced = 1;
        this->lru_count = 1;
    }
    int page_id;
    int frame_id;
    // int latch; // 乐观内存锁
    int count; // 和latch一样，在并发情况中使用，本实验不会处理
    int dirty;
    BCB *next; // bucket next
    // 替换算法的双向链表
    BCB *replacer_next;
    BCB *replacer_prev;
    // CLOCK替换算法
    int referenced = 1;
    // LRU-k替换算法
    int lru_count = 1;
};
#endif