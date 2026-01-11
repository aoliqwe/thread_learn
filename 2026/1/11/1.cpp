#include<iostream>
#include<thread>

void do_something(int& m){
    for(int i=0;i<m;i++){
        std::cout<<i<<"first\n";
    }
}

void do_something_else(int& m){
    for(int i=0;i<m;i++){
        std::cout<<i<<"second\n";
    }
}

class background_task{

    public:
        background_task(int num):number(num){
            std::cout<<"构造函数，地址："<<this<<std::endl;
        }

        background_task(background_task&& f) noexcept :number(f.number){
            std::cout<<"调用移动构造函数\n";
        } 

        background_task(const background_task& f) :number (f.number){
            std::cout<<"调用复制构造函数\n";
        }
        
        void operator()(){
           do_something(number);
           do_something_else(number);
        }

        ~background_task(){
            std::cout<<"number:"<<number;
            std::cout<<"  析构完成\n";
        }
    private:
        int number;
};

int main(){
    int m;
    std::cin>>m;

    background_task f(m);
    std::thread t (std::ref (f));
    //std::thread t (f);
    //std::thread t (std::move (f));
    t.join();

    
    // std::thread t1([&](){
    //    do_something(m); 
    //    do_something_else(m);
    // });
    // t1.join();

    return 0;
}