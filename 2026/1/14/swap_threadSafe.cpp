#include<iostream>
#include<mutex>
#include <ostream>
#include<vector>
#include<thread>
#include<utility>
#include<memory>
template<typename T>
class X
{
    private:

        T data_;
        mutable std::mutex mux;

    public:

        X(T data):data_(std::move(data)){}
        X(const X&)=delete;
        X& operator=(const X&)=delete;
          
        

        friend void swap(X& T1, X& T2) noexcept
        {
            std::unique_lock k1(T1.mux,std::defer_lock);
            std::unique_lock k2(T2.mux,std::defer_lock);
            lock(k1,k2);
            
            std::swap(T1.data_,T2.data_);
        }

        friend std::ostream& operator<<(std::ostream& os,const X& x)
        { 
            std::lock_guard<std::mutex> lock (x.mux);
             os<<x.data_;
             return os;
        }
};

int main()
{
    // X x1(99.99);
    // X x2(33.33);
    // std::cout<<"交换前: x1.data="<<x1<<" x2.data="<<x2<<std::endl;
    // swap(x1,x2);
    // std::cout<<"交换前: x1.data="<<x1<<" x2.data="<<x2<<std::endl;
       
    std::vector<std::unique_ptr<X<int>>>arr;
    arr.reserve(1000);

    std::mutex cout_mx;//序列化输出显示友好

    for(int i=0;i<1000;i++)
    {
        arr.emplace_back(std::make_unique<X<int>>(i));
    }
   
    std::thread t2([&](){
        for(size_t i=0;i+1<arr.size();i++)
        {
            swap(*arr[i],*arr[i+1]);
            std::lock_guard<std::mutex> lock(cout_mx);
            std::cout << "swapped indices " << *arr[i] << " and " << *arr[i+1]
                     << " :from " << std::this_thread::get_id() << std::endl;
        }
    });
    std::thread t1([&](){
        for(auto& en:arr)
            {
                std::lock_guard<std::mutex> lock(cout_mx);
                std::cout<<*en<<" :from "<<std::this_thread::get_id()<<std::endl;
            }
        });
    //读比交换快的多所以就观察到在中间 0被读到一次，其它还是顺序
    t1.join();
    t2.join();
    for(auto& en:arr)
    {
        std::cout<<*en<<" :from "<<std::this_thread::get_id()<<std::endl;
    }
    return 0;
}