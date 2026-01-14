#include<iostream>
#include<thread>
#include<vector>
#include<mutex>

std::mutex mux;
int count=0;
void f(int j)
{
    
    for(int i=1;i<=100000;i++)
    {
       // std::lock_guard<std::mutex> lock(mux);
        count++;
        std::cout<<j<<"from thrad_id: "<<std::this_thread::get_id()<<std::endl;
    }
    return ;

}

int main()
{

    int n;
    std::cin>>n;
    std::vector<std::thread> thread_sum;
    for(int i=1;i<=n;i++)
    {
           thread_sum.emplace_back(f,i);
    }

    for(auto& en:thread_sum)
    {
        en.join();
    }
    
    std::cout<<"count 计数："<<count<<std::endl;
    return 0 ;

}

