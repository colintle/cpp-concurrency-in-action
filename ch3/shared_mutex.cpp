#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>

class cache {
  std::map<std::string, int> entries;
  mutable std::shared_mutex entry_mutex;

public:
  // Reader
  int find_entry(std::string const &key) const {
    std::shared_lock<std::shared_mutex> lk(entry_mutex);
    std::map<std::string, int>::const_iterator const it = entries.find(key);
    return (it == entries.end()) ? -1 : it->second;
  }
  // Writer
  void update_or_add_entry(std::string const& key, int value){
    std::lock_guard<std::shared_mutex> lk(entry_mutex);
    entries[key] = value;
  }
};

int main(){
  cache test;
  test.update_or_add_entry("test1", 1);
  std::cout << test.find_entry("test1") << std::endl;
  return 0;
}
