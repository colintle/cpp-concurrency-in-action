#include <iostream>
#include <thread>

void do_something(int &i) { std::cout << i << std::endl; }

struct func {
  int &i;
  func(int &i_) : i(i_) {}
  void operator()() {
    for (unsigned j = 0; j < 5; ++j) {
      do_something(i);
    };
  }
};

int main(){
  int some_local_state = 0;
  func my_func(some_local_state);
  std::thread my_thread(my_func);
  my_thread.detach();
  // The main thread will not wait for my_thread to finish
  // If the main thread finishes execution, and my_thread
  // accesses some_local_state (some_local_state does not exist anymore), this
  // is undefined behavior
  //
  return 0;
}
