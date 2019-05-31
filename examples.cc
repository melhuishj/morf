#include "operation.hh"
#include "morf.hh"
#include <iostream>
#include <any>
#include <memory>
#include <vector>
#include <string>

int main() {
  morf::Morf<int> morf {};
  auto morf1 = morf
    .generate([](auto val){return val*2;})
    .map([](auto val){return "hello";});
  std::cout << morf1.get(3) << std::endl;
  std::vector<std::string> strs {"hello", "to", "the", "world"};
  std::cout << &strs << std::endl;
  auto result = morf::from(&strs).map([](auto val){return val.size();}).get(2);
  std::cout << result << std::endl;
  std::cout << strs.at(2) << std::endl;
}
