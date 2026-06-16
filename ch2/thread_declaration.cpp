#include <iostream>
#include <thread>

class Hello{
public:
  void operator()(){
  std::cout << "Hello from Hello Class" << std::endl;
  }
};

// Need to add const to tell the complier
// message will not be modified because message
// is a short-lived copy
void print_message(const std::string& message){
  std::cout << message << std::endl;
}
int main(){
  Hello hello;
  std::thread t1(hello);
  t1.join();
  
  // Thread creates a temporary short-lived copy
  std::thread t2(print_message, "Hello from t2");
  t2.join();

  std::thread t3{print_message, "Hello from t3"};
  t3.join();
 
  std::thread t4([](){
    std::cout << "Hello from t4" << std::endl;
  });
  t4.join();
  return 0;
}
