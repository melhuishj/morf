#include "morf.hh"
#include <iostream>
#include <vector>
#include <string>

int main() {
  std::vector<std::string> strs {"hello", "to", "the", "world"};
  auto result = morf::from(&strs).map([](auto val){return val.size();}).limit(2).toVector();
  std::cout << result.size() << std::endl;
}
