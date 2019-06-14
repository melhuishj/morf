#include <any>
#include <functional>
#include <optional>
#include <memory>

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
      std::optional<std::any> val = previous->get(index);
      if (!val) {
	return std::nullopt;
      }
      return operation(std::any_cast<InputType>(val.value()));
    }
  private:
    std::function<OutputType(InputType)> operation;
    std::unique_ptr<Operation> previous;
  };

  class LimitOp : public Operation {
  public:
    LimitOp(unsigned long lim, std::unique_ptr<Operation> prev_op) {
      limit = lim;
      previous = std::move(prev_op);
    }

    std::optional<std::any> get(unsigned long index) {
      if (index >= limit) {
	return std::nullopt;
      }
      return previous->get(index);
    }
  private:
    unsigned long limit;
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
