#include "buffer_manager.h"

// Interface functions
int BMgr::FixPage(int page_id, int prot)
{ // prot表示是否写,0为read,1为write
    total_count++;
    auto bcb = hash_find_BCB(page_id);
    if (bcb)
    { // 命中
        hit_count++;
        if (prot)
            bcb->dirty = 1;             // 因为没有提供实际写的数据，默认写的数据和原数据一样，不用更改frame中的值
        replacer_->Update(bcb, bcb, 1); // 命中时需要更新替换算法相关数据结构
        return bcb->frame_id;
    }
    else
    { // 不命中
        auto bcb = SelectVictim();
        int frame_id;
        if (!bcb)
        {
            frame_id = first_free_frame - 1;
        }
        else
        {
            frame_id = bcb->frame_id;
        }
        struct bFrame bf = {dsm_->ReadPage(page_id)};
        buf[frame_id] = bf; // 这里因为没有提供实际写的数据，所以就默认写的数据和原数据一样
        // 创建新的BCB加入ptof对应桶的头部
        auto new_bcb = new BCB(page_id, frame_id, 1, prot);
        update_BCB(new_bcb, bcb); // 更改两个表和替换算法相应的数据结构
        return frame_id;
    }
}

void BMgr::FixNewPage()
{
    int page_num = dsm_->GetNumPages();
    for (int i = 0; i < page_num; i++)
    {
        if (dsm_->GetUse(i) == 0)
            return;
    }
    // 找不到则分配新Page
    dsm_->IncNumPages();
}

int BMgr::UnfixPage(int page_id) // 本实验未实现并发情况，不会调用该函数
{
    auto bcb = hash_find_BCB(page_id);
    if (bcb)
    {
        if (bcb->count)
            bcb->count--;
        return bcb->frame_id;
    }
    return -1;
}

int BMgr::NumFreeFrames()
{
    int cnt = 0;
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        if (ftop[i] < 0)
            cnt++;
    }
    return cnt;
}

// Internal Functions
// 找到free的frame或者根据不同的替换算法找到替换frame
BCB *BMgr::SelectVictim()
{
    int frame_id;
    if (first_free_frame < DEFBUFSIZE)
    {
        frame_id = first_free_frame;
        first_free_frame++;
        return nullptr;
    }
    else
    {
        frame_id = replacer_->SelectVictim();
        auto bcb = hash_find_BCB(ftop[frame_id]);
        if (bcb->dirty)
        {
            dsm_->WritePage(bcb->page_id, buf[frame_id]);
            bcb->dirty = 0;
        }
        return bcb;
    }
}

int BMgr::Hash(int page_id)
{
    return page_id % DEFBUFSIZE;
}

void BMgr::SetDirty(int frame_id)
{
    int page_id = ftop[frame_id];
    if (page_id < 0)
        throw "Cannot set dirty to empty frame";
    auto bcb = hash_find_BCB(page_id);
    if (bcb)
        bcb->dirty = 1;
}

void BMgr::UnsetDirty(int frame_id)
{
    int page_id = ftop[frame_id];
    if (page_id < 0)
        throw "Cannot unset dirty to empty frame";
    auto bcb = hash_find_BCB(page_id);
    if (bcb)
        bcb->dirty = 0;
}

void BMgr::WriteDirtys()
{
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        auto bcb = ptof[i];
        while (bcb)
        {
            if (bcb->dirty)
            {
                dsm_->WritePage(bcb->page_id, buf[bcb->frame_id]);
                bcb->dirty = 0;
            }
            bcb = bcb->next;
        }
    }
}

void BMgr::PrintFrame(int frame_id)
{
    std::cout << buf[frame_id].field << std::endl;
}

std::string BMgr::GetStatistics()
{
    std::string a;
    a.append("\ttotal count: " + std::to_string(total_count) + "\n");
    a.append("\tI/O count: " + std::to_string(dsm_->io_count) + "\n");
    a.append("\thit rate: " + std::to_string((double)hit_count / total_count) + "\n");
    return a;
}
