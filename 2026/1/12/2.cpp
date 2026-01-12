#include <fstream>
#include<iostream>
#include<thread>


//清空2.2.txt，运行此文件
class thread_guard
{
    public:
        explicit thread_guard(std::thread& t):t_(t){}

        thread_guard(thread_guard const&)=delete;
        thread_guard& operator=(thread_guard const& t)=delete;
        
        ~thread_guard()
        {
            if(t_.joinable())
            {
                t_.join();
            }
        }
    private:
        std::thread& t_;
};
void readf(std::ifstream&& outfile ,std::ofstream&& infile)
{
    if(!outfile)
    {
        std::cerr<<"读文件打开失败\n";
        return ;
    }
    if(!infile)
    {
        std::cout<<"写文件打开失败\n";
    }
    std::string line;
    while(std::getline(outfile,line))
    {
    
     std::cout<<"写入:"<<line<<std::endl;  
     infile<<line<<"\n";

    }
    return ;
}

int main()
{
  std::ifstream re("2.1.txt");
  std::ofstream we("2.2.txt");

  std::thread thread_re(readf,std::move(re),std::move(we));
  thread_guard t1(thread_re);

  std::cout<<"主线程运行结束\n";
  return 0;
}