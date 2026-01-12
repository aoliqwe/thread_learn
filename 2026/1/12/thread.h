#include<iostream>
#include<thread>

class thread_guard
{
    public:

        explicit thread_guard(std::thread& t):t_(t){}
        thread_guard(thread_guard const& )=delete;
        thread_guard& operator=(thread_guard const&)=delete;

        ~thread_guard()
        {
            if(t_.joinable())
            {
                t_.join();
            }
            std::cout<<"析构完成 thread_d: "<<std::this_thread::get_id()<<std::endl;
        }

    private:

        std::thread& t_;
};