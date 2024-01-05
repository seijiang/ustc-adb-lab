#ifndef DSMgr_H_
#define DSMgr_H_
#include <iostream>
#include <cstring>
#include "typedef.h"

class DSMgr
{
public:
    DSMgr(std::string filename)
    {
        OpenFile(filename);
    }
    ~DSMgr()
    {
        CloseFile();
    }

    int io_count = 0;

    int OpenFile(std::string filename);

    int CloseFile();

    bFrame ReadPage(int page_id);

    int WritePage(int page_id, bFrame frm); // 将模板中的frame_id改为page_id

    int Seek(int offset, int pos);

    FILE *GetFile();

    void IncNumPages();

    int GetNumPages();

    void SetUse(int index, int use_bit);

    int GetUse(int index);

private:
    FILE *currFile;
    int numPages;
    int pages[MAXPAGES]; // 该实验中用不到，默认所有页use_bit均为1
};
#endif