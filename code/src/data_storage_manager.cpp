#include "data_storage_manager.h"

int DSMgr::OpenFile(std::string filename)
{
    currFile = fopen(filename.c_str(), "r+");
    if (!currFile)
    { // 没有则创建文件
        currFile = fopen(filename.c_str(), "w+");
    }
    // 获取文件大小并初始化页相关参数
    fseek(currFile, 0, SEEK_END);
    auto size = ftell(currFile);
    numPages = (size + PAGESIZE - 1) / PAGESIZE;
    if (numPages > MAXPAGES)
        throw "too large file, you need to increase MAXPAGES";
    for (int i = 0; i < numPages; i++)
    {
        pages[i] = 1;
    }
    return 0;
}

int DSMgr::CloseFile()
{
    return fclose(currFile);
}

bFrame DSMgr::ReadPage(int page_id)
{
    struct bFrame frm;
    if (fseek(currFile, page_id * PAGESIZE, SEEK_SET) == 0)
    {
        fread(&frm, 1, FRAMESIZE, currFile);
        io_count++;
        return frm;
    }
    else
        return frm;
}

int DSMgr::WritePage(int page_id, bFrame frm)
{ // 将模板中的frame_id改为page_id
    if (fseek(currFile, page_id * PAGESIZE, SEEK_SET) == 0)
    {
        io_count++;
        return fwrite(frm.field, 1, FRAMESIZE, currFile);
    }
    else
        return 0;
}

int DSMgr::Seek(int offset, int pos)
{
    return fseek(currFile, offset, pos);
}

FILE *DSMgr::GetFile()
{
    return currFile;
}

void DSMgr::IncNumPages()
{
    pages[numPages] = 1;
    char data[PAGESIZE];
    memset(data, 'a', sizeof(data));
    fseek(currFile, 0, SEEK_END);
    fwrite(data, 1, PAGESIZE, currFile);
    numPages++;
}

int DSMgr::GetNumPages()
{
    return numPages;
}

void DSMgr::SetUse(int index, int use_bit)
{
    pages[index] = use_bit;
}

int DSMgr::GetUse(int index)
{
    return pages[index];
}
