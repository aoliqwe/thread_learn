#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>

std::mutex mux;
int k = 0, m = 0;

void speak() {
    // 每个线程应该有自己的 unique_lock！
    std::unique_lock<std::mutex> lock(mux, std::defer_lock);
    
    while (true) {
        lock.lock();
        // 必须在锁内检查条件！
        if (k >= m) {
            std::cout << "speak线程: k >= m，退出" << std::endl;
            break;
        }
        
        // 输出信息
        for (int i = 0; i < 3; i++) {  // 减少输出
            std::cout << "speak线程 ID:" << std::this_thread::get_id() 
                      << " k=" << k << " m=" << m << std::endl;
        }
        
        k++;
        lock.unlock();  // 提前解锁
        
        // 模拟一些工作
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        
        // 循环回到开头会重新 lock()
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);  // 范围调小便于测试
    
    k = dist(gen);
    m = dist(gen);
    
    std::cout << "初始值: k=" << k << ", m=" << m << std::endl;
    
    std::thread t1([&]() {
        std::unique_lock<std::mutex> lock(mux, std::defer_lock);  // 局部锁！
        
        // 第一阶段：增加 m
        {
            std::lock_guard<std::mutex> quick_lock(mux);  // 简单情况用 lock_guard
            while (m < 20) {
                m++;
                std::cout << "t1 增加 m 到: " << m << std::endl;
            }
        }  // 自动解锁
        
        // 第二阶段：条件判断并启动新线程
        lock.lock();
        if (k < m) {
            lock.unlock();  // 解锁后再启动新线程
            
            std::thread t2(speak);
            t2.join();  // 等待 t2 完成
            
            lock.lock();  // 重新锁定
        }
        
        // 第三阶段：收尾工作
        for (int i = 0; i < 3; i++) {
            std::cout << "t1 收尾 ID:" << std::this_thread::get_id() 
                      << " 输出 " << i << std::endl;
        }
        // 自动解锁
    });
    
    t1.join();
    
    std::cout << "\n最终: k=" << k << ", m=" << m << std::endl;
    std::cout << "完美：线程输出有同步" << std::endl;
    
    return 0;
}
// std::mutex 门;

// // 1. lock_guard（只能自动解锁）
// {
//     std::lock_guard<std::mutex> 保安(门);  // 立即锁门
    
//     // 只能干等着，保安才会在结束时解锁
//     // ❌ 不能提前让他开门！
    
// } // 这里自动解锁

// // 2. unique_lock（可以提前解锁，也会自动解锁）
// {
//     std::unique_lock<std::mutex> 保安(门);  // 立即锁门
    
//     // 干点活...
    
//     保安.unlock();  // ✅ 可以让他提前开门！
//     // 门现在开着...
    
//     保安.lock();    // ✅ 可以让他再锁门！
    
//     // 干更多活...
    
// } // 如果门还锁着，这里也会自动解锁