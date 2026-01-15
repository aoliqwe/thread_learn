#include <exception>
#include<stack>
#include<mutex>
#include<memory>
#include<thread>
#include<iostream>
struct empty_stack:std::exception
{
    const char* what() const noexcept override;
};
const char* empty_stack::what() const noexcept { return "empty stack"; }
//exceptionf中what()为非纯虚函数，要提供实现。
template<typename T>
class threadSafe_stack
{
    public:
        threadSafe_stack(){};
        threadSafe_stack(const threadSafe_stack& other )
        {
            std::lock_guard<std::mutex> lock(mux);
            data=other.data;
        }
        threadSafe_stack& operator=(threadSafe_stack&)=delete;

        void push(T& new_value)
        {
            std::lock_guard<std::mutex> lock(mux);
            data.push(std::move(new_value));//移动语义提高效率直接传new_value会发生拷贝
        }

        std::shared_ptr<T> pop()
        {
            std::lock_guard<std::mutex> lock(mux);
            if(data.empty()) throw empty_stack();
            // 第一版 一次默认构造  一拷贝构造
            //std::shared_ptr<T>const res=std::make_shared<T>(data.pop());
            // 第二版  一次默认构造 一次移动构造
            std::shared_ptr<T>const res(std::make_shared<T>(std::move(data.top())));
            data.pop();
            return res;
        }

        void pop(T& value)
        {
            std::lock_guard<std::mutex> lock(mux);
            if(data.empty()) throw empty_stack();
            value =data.top();
            data.pop();
        }

        bool empty()
        {
            std::lock_guard<std::mutex> lock(mux);
            return data.empty();
        }
    private:
        std::stack<T> data;
        std::mutex mux;
};

int main()
{
    auto stc(std::make_shared<threadSafe_stack<int>>());
    for(int i=1;i<=1000000;i++) stc->push(i);  
    
    std::thread t1([&](){
         for(int i=1;i<=500000;i++)
         {
             std::cout<<stc->pop()<<" from:"<<std::this_thread::get_id()<<std::endl;
         }
    });
    std::thread t2([&](){
          for(int i=1;i<=500000;i++)
         {
            std::cout<<stc->pop()<<" from:"<<std::this_thread::get_id()<<std::endl;
         }
    });
    t1.join();
    t2.join();
    return 0;
}