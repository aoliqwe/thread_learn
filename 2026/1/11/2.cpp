#include<iostream>
#include<thread>

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

int main()
{
    int j;
    std::cin>>j;

   func myfunc(j);
   std::thread mythread(myfunc);
   
   
   mythread.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   j=-1;

   return 0;
}