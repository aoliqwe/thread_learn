#include<iostream>
#include<thread>
#include<stdexcept>
#include<random>
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

    do_something_in_current_thread();
    t.join();
}

int main(){
    f(10000);
    return 0;
}