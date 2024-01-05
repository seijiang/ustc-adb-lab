#include "Replacer.h"

// 命中时将对应bcb的referenced置为1
void CLOCK_Replacer::Update(BCB *new_bcb, BCB *old_bcb, int hit)
{
    if (!hit)
    {
        if (old_bcb)
            Delete(old_bcb);
        Insert(new_bcb);
    }
    else
        new_bcb->referenced = 1;
}

int CLOCK_Replacer::SelectVictim()
{
    if (!current)
        return -1;
    while (true)
    {
        if (current->referenced)
        {
            current->referenced = 0;
            current = current->replacer_next;
        }
        else
        {
            int frame_id = current->frame_id;
            current = current->replacer_next;
            return frame_id;
        }
    }
    return -1;
}

// 更新时由于SelectVictim算法的保证，如果是需要替换的情况，current一定是old_bcb->next
// 所以插入时插入到current的prev
void CLOCK_Replacer::Insert(BCB *bcb)
{
    if (!current)
    {
        current = bcb;
        current->replacer_next = bcb;
        current->replacer_prev = bcb;
    }
    else
    { // 插入到current前面
        auto prev = current->replacer_prev;
        current->replacer_prev = bcb;
        bcb->replacer_next = current;
        prev->replacer_next = bcb;
        bcb->replacer_prev = prev;
    }
}

void CLOCK_Replacer::Delete(BCB *bcb)
{
    auto prev = bcb->replacer_prev;
    auto next = bcb->replacer_next;
    prev->replacer_next = next;
    next->replacer_prev = prev;
}
