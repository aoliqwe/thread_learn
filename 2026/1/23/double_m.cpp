#include <cstddef>
#include<iostream>
#include <thread>
#include<vector>
#include<mutex>
#include<memory>

std::mutex mux;
std::shared_ptr<int> ptr;
//static std::shared_ptr<int> ptr;//静态变量线程安全类型
void some_thing()
{
    for(int i=*ptr;i<300;++i)
    {
        std::cout<<i<<"from: "<<std::this_thread::get_id()<<std::endl;
    }
}
void fun()
{
    if(!ptr)
    {
        std::lock_guard<std::mutex> lock(mux);
         if(!ptr)
        {
            ptr=std::make_shared<int>(42);
        }
        
    }
    some_thing();//多个线程可能同时读取未完全初始化的 `ptr`。
}
//对应Solution 2
// void fun()
// {
//          if(!ptr)
//         {
//             ptr=std::make_shared<int>(42);
//         }
//     some_thing();
// }

int main()
{
    std::vector<std::thread> arr_th(4);
    for(size_t i=0;i<arr_th.size();i++)
    {
        arr_th[i]=std::thread(fun);

    }

    for(size_t i=0;i<arr_th.size();++i)
    {
        arr_th[i].join();
    }
    return 0;
}