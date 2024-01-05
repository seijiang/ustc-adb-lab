#include "Replacer.h"

Replacer::Replacer() {}
// 是否hit都是删除旧的，插入新的，只是hit的情况旧的和新的是相同的bcb
void LRU_Replacer::Update(BCB *new_bcb, BCB *old_bcb, int hit)
{
    if (old_bcb)
        Delete(old_bcb);
    Insert(new_bcb);
}

int LRU_Replacer::SelectVictim()
{
    return head->frame_id;
}

void LRU_Replacer::Insert(BCB *bcb)
{
    if (!head)
    {
        head = bcb;
        bcb->replacer_next = bcb;
        bcb->replacer_prev = bcb;
    }
    else
    { // 插入到LRU的最末端
        auto tail = head->replacer_prev;
        head->replacer_prev = bcb;
        bcb->replacer_next = head;
        tail->replacer_next = bcb;
        bcb->replacer_prev = tail;
    }
}

void LRU_Replacer::Delete(BCB *bcb)
{
    auto prev = bcb->replacer_prev;
    auto next = bcb->replacer_next;
    if (bcb == head)
    {
        head = next;
        if (head->replacer_next == head)
        { // 只有一个head节点
            head = nullptr;
            return;
        }
    }
    prev->replacer_next = next;
    next->replacer_prev = prev;
}