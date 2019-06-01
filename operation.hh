#include <any>
#include <functional>
#include <optional>
#include <memory>
#include <iostream>

#ifndef _OPERATION_HH_
#define _OPERATION_HH_

namespace morf {

  class Operation {
  public:
    virtual std::optional<std::any> get(unsigned long) = 0;
  };

  template<typename InputType, typename OutputType>
  class NonBlockingOp : public Operation {
  public:
    template<typename Func>
    NonBlockingOp(Func op, std::unique_ptr<Operation> prev_op) {
      operation = op;
      previous = std::move(prev_op);
    }

    std::optional<std::any> get(unsigned long index) {
      return operation(std::any_cast<InputType>(previous->get(index).value()));
    }
  private:
    std::function<OutputType(InputType)> operation;
    std::unique_ptr<Operation> previous;
  };

  template<typename OutputType>
  class FuncGeneratorOp : public Operation {
  public:
    template<typename Func>
    FuncGeneratorOp(Func gen) {
      generator = gen;
    }

    std::optional<std::any> get(unsigned long index) {
      return generator(index);
    }
  private:
    std::function<OutputType(unsigned long)> generator;
  };

  template<typename InputContainer>
  class SourceOp : public Operation {
  public:
    SourceOp(const InputContainer * input) {
      source = input;
    }

    std::optional<std::any> get(unsigned long index) {
      if (index >= source->size()) {
	return std::nullopt;
      }
      return source->at(index);
    }

  private:
    const InputContainer * source;
  };
}
#endif
