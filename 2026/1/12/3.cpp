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

void  func()
{
    for(int i=0;i<1000;i++)
    {
        std::cout<<i<<":from fucn thread_id: "<<std::this_thread::get_id()<<std::endl;
    }
    return ;
}
void other_func()
{
    for(int i=0;i<1000;i++)
    {
        std::cout<<i<<"from other_fucn thread_id: "<<std::this_thread::get_id()<<std::endl;
    }
    return ;
}

int main()
{
    std::thread t1(func);
    thread_guard rall_t1(t1);

    std::thread t2(std::move(t1));
    thread_guard rall_t2(t2);

    t1=std::thread(other_func);
    std::thread t3(std::move(t2));
    thread_guard rall_t3(t3);
    //t1=std::move(t2);
     
    return 0;

}