#include<iostream>
#include<thread>
#include<functional>
#include<mutex>
int top=9999;
std::mutex mux;
int count=0;

//观察到 pop()输出由1变成-1，接口问题导致条件竞争
bool isempty()
{
    std::lock_guard<std::mutex> lock(mux);
    if(top==-1)
    {
        return true;
    }
    return false;
}

int istop(int* arr)
{
    if(!isempty())
    {
        return arr[top];
    }
        return -1;
}

bool pop(int* arr)
{
  if(!isempty())
  {
    top--;
    return true;
  }
  return false;
}


int main()
{
    std::function<bool(int*)> fun1=pop;
    std::function<int(int*)> fun2=istop;
    int arr[10000];
    int i = 0;
    while(i < 10000) 
    {
        arr[i] = 1;
        i++;
    }
    int value=0;
    std::thread t1([&](){
      for(int i=0;i<10000;i++)
      { 
        fun1(arr);

      }
    });

    std::thread t2([&](){
      for(int i=0;i<10000;i++)
      {
       std::cout<<fun2(arr)<<": from thread "<<std::this_thread::get_id()<<std::endl;
      }
    });
    t1.join();
    t2.join();
    
    return 0;
}