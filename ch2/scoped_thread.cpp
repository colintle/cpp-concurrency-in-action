#include <iostream>
#include <thread>

class scoped_thread{
  std::thread t;
public:
  explicit scoped_thread(std::thread t_): t(std::move(t_)){
   if(!t.joinable()) throw std::logic_error("No thread"); 
  }
  ~scoped_thread(){
    t.join();
  }
  scoped_thread(scoped_thread const&) = delete;
  scoped_thread& operator=(scoped_thread const&) = delete;
};

struct func {
  int &i;
  func(int &i_) : i(i_) {}
  void operator()() {
    for (unsigned j = 0; j < 5; ++j) {
      std::cout << i << std::endl;
    };
  }
};

int main(){
  int some_local_state = 1;
  scoped_thread t{std::thread(func(some_local_state))};
  return 0;
}

