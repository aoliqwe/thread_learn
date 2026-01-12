#include <exception>
#include<iostream>
#include<thread>
#include<stdexcept>
#include<random>

class thread_guard
{

    public:
        explicit thread_guard(std::thread& t):t_(t){}

        thread_guard( thread_guard const& other)=delete;
        thread_guard& operator=(thread_guard const& other)=delete;

        ~thread_guard()
        {
            
            if(t_.joinable())
            {
                t_.join();
            }
            std::cout<<"调用析构函数\n";
        }
    
    private:
    
        std::thread& t_;
};


struct func
{
    int &i_;
    func(int& i):i_(i)
    {
       std::cout<<"调用构造函数\n";
    }

    void operator()()
    {
        for(int j=0;j<i_;j++)
        {
          std::cout<<j<<" \n";
          
        }
        std::cout<<i_<<" sdfdsfdsjkgf  \n";
    }
};

void do_something_in_current_thread() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 1);

    if (dist(gen) == 1) {
        throw std::runtime_error("Oops! Something went wrong!");
    }

    std::cout << "Main thread working normally..." << std::endl;
}
void f(int m){
    func myfunc(m);
    std::thread t(myfunc);
    thread_guard t1(t);
    
    do_something_in_current_thread();
}
int main()
{
    int m;
    std::cin>>m;
    try
    {
        f(m);
    }catch(const std::exception& e)
    {
       std::cout<<"main()捕获到异常: "<<e.what()<<std::endl;
    }
    

    return 0;
}