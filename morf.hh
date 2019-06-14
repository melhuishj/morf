#include "operation.hh"
#include <any>
#include <memory>

#ifndef _MORF_HH_
#define _MORF_HH_

namespace morf {

  template<typename T>
  class Morf {
  public:
    Morf() {}
    Morf(std::unique_ptr<Operation> ops) {
      operations = std::move(ops);
    }
    Morf(Morf&& other) : operations(std::move(other.operations)) {}

    T get(unsigned long index) {
      return std::any_cast<T>(operations->get(index).value());
    }

    template<typename Func>
    Morf<T> generate(Func gen) {
      operations = std::make_unique<FuncGeneratorOp<T>>(gen);
      return std::move(*this);
    }

    template<typename InputContainer>
    Morf<T> source(const InputContainer * input) {
      operations = std::make_unique<SourceOp<InputContainer>>(input);
      return std::move(*this);
    }

    template<typename Func,
	     typename OutputType = typename std::invoke_result_t<Func, T>>
    Morf<OutputType> map(Func op) {
      operations = std::make_unique<NonBlockingOp<T, OutputType>>(op, std::move(operations));
      Morf<OutputType> newmorf {std::move(operations)};
      return std::move(newmorf);
    }

    Morf<T> limit(unsigned long limit) {
      operations = std::make_unique<LimitOp>(limit, std::move(operations));
      return std::move(*this);
    }

    std::vector<T> toVector() {
      std::vector<T> output {};
      std::optional<std::any> next;
      unsigned long index {0};
      next = operations->get(index++);
      while (next.has_value()) {
	output.push_back(std::any_cast<T>(*next));
	next = operations->get(index++);
      }
      return output;
    }
  private:
    std::unique_ptr<Operation> operations;
  };

  template<typename InputContainer, typename InputType = typename InputContainer::value_type>
  Morf<InputType> from(const InputContainer * input) {
    Morf<InputType> output {};
    return std::move(output.source(input));
  }
}

#endif
