#ifndef Replacer_H_
#define Replacer_H_
#include <iostream>
#include "typedef.h"

class Replacer
{
public:
    Replacer();
    std::string name;
    virtual void Insert(BCB *bcb) = 0;
    virtual void Delete(BCB *bcb) = 0;
    virtual void Update(BCB *new_bcb, BCB *old_bcb, int hit) = 0; // 删除old_bcb，插入bcb
    virtual int SelectVictim() = 0;
};

class LRU_Replacer : public Replacer
{
public:
    LRU_Replacer()
    {
        name = "LRU";
        head = nullptr;
    }
    void Insert(BCB *bcb);
    void Delete(BCB *bcb);
    BCB *GetHead() { return head; } // 给LRU-k使用
    void Update(BCB *new_bcb, BCB *old_bcb, int hit);
    virtual int SelectVictim();

private:
    BCB *head;
};

class CLOCK_Replacer : public Replacer
{
public:
    CLOCK_Replacer()
    {
        name = "CLOCK";
        current = nullptr;
    }
    void Insert(BCB *bcb);
    void Delete(BCB *bcb);
    void Update(BCB *new_bcb, BCB *old_bcb, int hit);
    virtual int SelectVictim();

private:
    BCB *current;
};

class LRU_k_Replacer : public Replacer
{
public:
    LRU_k_Replacer(int k)
    {
        this->k = k;
        name = "LRU-" + std::to_string(k);
        lru_list1 = new LRU_Replacer();
        lru_list2 = new LRU_Replacer();
    }
    void Insert(BCB *bcb);
    void Delete(BCB *bcb);
    void Update(BCB *new_bcb, BCB *old_bcb, int hit);
    virtual int SelectVictim();

private:
    LRU_Replacer *lru_list1;
    LRU_Replacer *lru_list2;
    int k;
};
#endif