#include<iostream>
#include<thread>
#include<mutex>
#include<functional>

//i被非法访问双倍累加了
class some_date
{
    public:
        some_date(){
            this->date=0;
        }
        void do_comething()
        {
            for(int i=0;i<100000;i++)
            {
                 date++;
            }
        }
        ~some_date()
        {
            std::cout<<"date: "<<date<<std::endl;
        }
        int date;
};

class date_wapper
{
    public:
        using Callback=std::function<void(some_date&)>;
        void do_wadate(Callback func)
        {
            std::lock_guard<std::mutex> lock(mux);
            for(int i=0;i<100000;i++)
            {
                wp_date.date++;
            }
            func(wp_date);
        }
    private:
        some_date wp_date;
        std::mutex mux;
};

some_date* unprotected;

void malicious_function(some_date& protect_date)
{
    unprotected=&protect_date;
}

date_wapper x;
void f()
{
    x.do_wadate(malicious_function);
    unprotected->do_comething();
}

int main()
{
    std::thread t1(f);
    t1.join();
    return 0;
}