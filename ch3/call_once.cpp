#include <mutex>
#include <memory>

std::shared_ptr<int> shared_int;
std::once_flag int_flag;

void init(){
  std::call_once(int_flag, [](){
    std::make_shared<int>(100);
  });
}

int main(){
  init();
  return 0;
}
