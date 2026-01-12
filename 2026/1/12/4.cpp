#include<iostream>
#include<thread>
#include<vector>
#include"thread.h"

void do_work(int i)
{
    for(int j=0;j<10;j++)
    {
        std::cout<<i<<"from thread_id:"<<std::this_thread::get_id()<<std::endl;
    }
}

int main()
{
    int m;
    std::cin>>m;

    std::vector<std::thread>thread_sum;
    std::vector<thread_guard> rall_thread_sum;
    for(int i=0;i<m;i++)
    {
        thread_sum.push_back(std::thread(do_work,i));
        //rall_thread_sum.emplace_back(thread_sum[i]);
    }

    for(auto& tn:thread_sum)
    {
        if(tn.joinable())
        {
            tn.join();
        }
    }
    return 0;
}
