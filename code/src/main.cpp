#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include "buffer_manager.h"

int main(int argc, char *argv[])
{
    char opt;
    const char *optstring = "r:k:d:h"; // r表示替换算法，k表示LRU-k的k，d表示trace_file路径，h表示参数说明

    std::string replacer_name, trace_path;
    int k = 2;
    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        if (opt == 'r')
        {
            if (strcmp(optarg, "lru") && strcmp(optarg, "clock") && strcmp(optarg, "lru-k"))
            {
                std::cout << "-r need lru or clock or lru-k" << std::endl;
                exit(1);
            }
            replacer_name = optarg;
        }
        else if (opt == 'k')
        {
            k = atoi(optarg);
            if (k < 2)
            {
                std::cout << "The value of k in lru-k is a positive integer larger than 1" << std::endl;
                exit(1);
            }
        }
        else if (opt == 'd')
        {
            trace_path = optarg;
        }
        else if (opt == 'h')
        {
            std::cout << "optional parameters are '-r','-k','-d' " << std::endl;
            std::cout << "-r represents the type of replacement algorithm, which can be either lru, clock or lru-k, the default is lru" << std::endl;
            std::cout << "-k represents the value of k in lru-k, which is only needed when the value of the '-r' is lru-k, the default is 2" << std::endl;
            std::cout << "-d represents the path of trace file, the default is '../data/data-5w-50w-zipf.txt'" << std::endl;
            exit(1);
        }
    }

    if (replacer_name.size() == 0)
        replacer_name = "lru";
    Replacer *replacer;
    if (replacer_name == "lru")
        replacer = new LRU_Replacer();
    else if (replacer_name == "clock")
        replacer = new CLOCK_Replacer();
    else if (replacer_name == "lru-k")
        replacer = new LRU_k_Replacer(k);

    DSMgr *dsm = new DSMgr("data.dbf");
    BMgr *bm = new BMgr(dsm, replacer);
    if (dsm->GetNumPages() == 0)
    {
        for (int i = 0; i < 50000; i++)
            bm->FixNewPage();
        // bm->UnfixPage(); // 串行情况无需调用
    }

    if (trace_path.size() == 0)
        trace_path = "../data/data-5w-50w-zipf.txt";
    auto trace_file = fopen(trace_path.c_str(), "r");
    if (!trace_file)
    {
        std::cout << "trace file cannot be opened" << std::endl;
        exit(1);
    }

    auto beforeTime = std::chrono::steady_clock::now();

    int r_or_w, page_num;
    while (fscanf(trace_file, "%d,%d", &r_or_w, &page_num) == 2)
    {
        bm->FixPage(page_num, r_or_w); // 0为read,1为write
    }
    bm->WriteDirtys();

    auto afterTime = std::chrono::steady_clock::now();
    double duration_second = std::chrono::duration<double>(afterTime - beforeTime).count();
    std::string statistics = "\ttime: " + std::to_string(duration_second) + "\n";
    statistics.append(bm->GetStatistics());
    std::cout << "replacement algorithm: " << replacer->name << std::endl;
    std::cout << statistics;
}