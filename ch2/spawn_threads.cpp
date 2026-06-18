#include <iostream>
#include <thread>
#include <vector>

void do_work(unsigned id){
  std::cout << id << std::endl;
}
int main(){
  std::vector<std::thread> threads;
  for (unsigned i = 0; i < 20; i++){
    // emplace_back accepts args to thread contructor
    threads.emplace_back(do_work, i);
  }
  for (std::thread& entry : threads){
    entry.join();
  }
  return 0;
}
