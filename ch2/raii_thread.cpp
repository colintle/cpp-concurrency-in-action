#include <iostream>
#include <thread>

void do_something(int& i){
  std::cout << i << std::endl;
}

// Resource Acquisition Is Initialization
// thread_guard does the join() in its destructor
class thread_guard {
  std::thread &t;
public:
  explicit thread_guard(std::thread& t_): t(t_){}
  ~thread_guard(){
    if(t.joinable()){
      t.join();
    }
  }
  thread_guard(thread_guard const&)=delete;
  thread_guard& operator=(thread_guard const&)=delete;
};

struct func{
  int& i;
  func(int& i_): i(i_){}
  void operator()(){
    for (unsigned j=0; j<5;j++){do_something(i);};
  }
};

int main(){
  int some_local_state = 0;
  func my_func(some_local_state);
  std::thread t(my_func);
  thread_guard guard(t);
  return 0;
}
