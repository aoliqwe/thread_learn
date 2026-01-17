#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
//各拿一把锁，产生死锁
//死锁产生的四个必要条件
//1、互斥条件
//2、请求和保持条件
//3、不可剥夺条件
//4、环路等待

//解决方案
//1、顺序加锁
//2.使用scoped_lock一次锁住多个锁
int main()
{
    std::mutex mux1;
    std::mutex mux2;

    std::thread t1([&](){

        std::lock_guard lock1(mux1);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        std::lock_guard lock2(mux2);
        //Solution 2
        //std::scoped_lock(mux1,mux2);

        for(int i=0;i<10000;i++)
        {
            std::cout<<i<<" from:"<<std::this_thread::get_id()<<std::endl;
        }

    });
    std::thread t2([&](){

        std::lock_guard lock2(mux2);
        std::lock_guard lock1(mux1);
        //Solution 1
        //std::lock_guard lock2(mmux2);
        //Solution 2
        //std::scoped_lock(mux1,mux2);
        for(int i=0;i<10000;i++)
        {
            std::cout<<i<<" from:"<<std::this_thread::get_id()<<std::endl;
        }
        
    });
    t1.join();
    t2.join();
    return 0;
}
// 单mutex：std::lock_guard（最轻量）

// 多mutex：std::scoped_lock（性能相当，更安全）

//Solution 3
//std::lock(mux1,mux2,....);
//std::lock_guard<std::mutex> lock(mux1,std::adopt_lock);转移所有权，不调用模板构造函数
//std::lock_guard<std::mutex> lock(mux2,std::adopt_lock); 
//此处省略