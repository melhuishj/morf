#include "operation.hh"
#include "morf.hh"
#include <iostream>
#include <any>
#include <memory>
#include <vector>
#include <string>

int main() {
  std::vector<std::string> strs {"hello", "to", "the", "world"};
  auto result = morf::from(&strs).map([](auto val){return val.size();}).get(2);
  std::cout << result << std::endl;
}
