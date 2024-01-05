#ifndef BMgr_H_
#define BMgr_H_
#include <cstring>
#include "data_storage_manager.h"
#include "Replacer.h"

class BMgr
{
public:
    BMgr(DSMgr *dsm, Replacer *replacer)
    {
        memset(ftop, -1, sizeof(ftop));
        dsm_ = dsm;
        replacer_ = replacer;
    }
    // Interface functions
    int FixPage(int page_id, int prot);

    void FixNewPage();

    int UnfixPage(int page_id);

    int NumFreeFrames();

    // Internal Functions
    BCB *SelectVictim(); // 有free frame返回空指针，否则返回要被替换的frame对应的BCB

    int Hash(int page_id);

    void RemoveBCB(BCB *ptr, int page_id);

    void RemoveLRUEle(int frid);

    void SetDirty(int frame_id);

    void UnsetDirty(int frame_id);

    void WriteDirtys();

    void PrintFrame(int frame_id);

    std::string GetStatistics();

private:
    // Tables
    int ftop[DEFBUFSIZE];
    BCB *ptof[DEFBUFSIZE];
    // buffer
    bFrame buf[DEFBUFSIZE];
    // DSMgr
    DSMgr *dsm_;

    Replacer *replacer_;
    int total_count = 0;
    int hit_count = 0;
    // 本实验frame按顺序占用，且中途不会释放frame，所以可以只用一个first_free_frame表示
    // first_free_frame之前的全都不free，之后的全都free
    int first_free_frame = 0;

    BCB *hash_find_BCB(int page_id)
    { // 根据page_id找到对应的BCB
        auto bcb = ptof[Hash(page_id)];
        while (bcb)
        {
            if (bcb->page_id == page_id)
            {
                break;
            }
            else
                bcb = bcb->next;
        }
        return bcb;
    }

    void hash_insert_BCB(BCB *bcb)
    {
        int page_id = bcb->page_id;
        ftop[bcb->frame_id] = page_id;
        auto old_bucket_head = ptof[Hash(page_id)];
        ptof[Hash(page_id)] = bcb;
        bcb->next = old_bucket_head;
    }

    void hash_delete_BCB(int frame_id)
    {
        int page_id = ftop[frame_id];
        auto bcb = ptof[Hash(page_id)];
        ftop[frame_id] = -1;
        auto prev = bcb;
        if (!bcb)
            return;
        // 该page在桶的头部
        if (bcb->page_id == page_id)
        {
            ptof[Hash(page_id)] = bcb->next;
        }
        // 不在桶的头部，则遍历该桶查找
        else
        {
            prev = bcb;
            bcb = bcb->next;
            while (bcb)
            {
                if (bcb->page_id == page_id)
                    break;
                else
                {
                    prev = bcb;
                    bcb = bcb->next;
                }
            }
            if (!bcb)
                return;
            prev->next = bcb->next;
        }
    }

    // old_bcb不为空则删除old_bcb再插入new_bcb；否则直接插入new_bcb
    void update_BCB(BCB *new_bcb, BCB *old_bcb)
    {
        if (old_bcb)
        {
            hash_delete_BCB(old_bcb->frame_id);
        }
        hash_insert_BCB(new_bcb);
        replacer_->Update(new_bcb, old_bcb, 0);
        delete old_bcb;
    }
};

#endif