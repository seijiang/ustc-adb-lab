#include "Replacer.h"

void LRU_k_Replacer::Update(BCB *new_bcb, BCB *old_bcb, int hit)
{
    if (!hit)
    {
        if (old_bcb)
            Delete(old_bcb);
        Insert(new_bcb);
    }
    else
    { // 命中时new_bcb和old_bcb是同一个
        new_bcb->lru_count++;
        if (new_bcb->lru_count == k)
        {
            lru_list1->Delete(old_bcb);
            lru_list2->Insert(new_bcb);
        }
        else if (new_bcb->lru_count < k)
            lru_list1->Update(new_bcb, old_bcb, hit);
        else if (new_bcb->lru_count > k)
            lru_list2->Update(new_bcb, old_bcb, hit);
    }
}

int LRU_k_Replacer::SelectVictim()
{
    if (lru_list1->GetHead())
        return lru_list1->GetHead()->frame_id;
    else
        return lru_list2->GetHead()->frame_id;
}

void LRU_k_Replacer::Insert(BCB *bcb)
{
    lru_list1->Insert(bcb);
}

void LRU_k_Replacer::Delete(BCB *bcb)
{
    if (bcb->lru_count >= k)
        lru_list2->Delete(bcb);
    else
        lru_list1->Delete(bcb);
}