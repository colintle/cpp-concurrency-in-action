#include <iostream>
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception
{
  const char* what() const throw(){
    return "empty_stack";
  };
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
  threadsafe_stack(){}
  threadsafe_stack (const threadsafe_stack& other){
    std::lock_guard<std::mutex> l(other.m);
    data = other.data;
  }
  threadsafe_stack& operator=(const threadsafe_stack&)=delete;
  void push(T new_value){
    std::lock_guard<std::mutex> l(m);
    data.push(std::move(new_value));
  }
  std::shared_ptr<T> pop(){
    std::lock_guard<std::mutex> l(m);
    if (data.empty()) throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
  }
  void pop(T& value){
    std::lock_guard<std::mutex> l(m);
    if(data.empty()) throw empty_stack();
    value = data.top();
    data.pop();
  }
  bool empty() const{
    std::lock_guard<std::mutex> l(m);
    return data.empty();
  } 
};

threadsafe_stack<int> stack;
int main(){
  stack.push(1);
  stack.push(2);

  std::shared_ptr<int> res = stack.pop();
  std::cout << *res << std::endl;
  return 0;
}
