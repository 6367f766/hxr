#ifndef EVAL_H
#define EVAL_H

#include "common.h"
#include "converters.h"
#include "logger.h"

enum class BinaryOperator : uint8_t {
    AND = 0,  // &
    OR = 1,   // |
    NOT = 2,  // ~
    XOR = 3,   // ^
    UNDEFINED =4
};

enum class LogicalOperator {
    AND = 0,  // &&
    OR = 1,   // ||
    NOT = 2   // !
};

template <class T>
struct Operation {
    T Item1;
    T Item2;
    BinaryOperator Operator;
    bool firstOperandSet{false};

    Operation<T>& withOperator(BinaryOperator op) {
        Operator = op;
        return *this;
    }

    Operation<T>& withOperand(T operand) {
        if (firstOperandSet) {
            Item2 = operand;
            firstOperandSet = false;
        } else {
            Item1 = operand;
            firstOperandSet = true;
        }
        return *this;
    }

    T compute() {
        T result;
        switch (Operator) {
            case BinaryOperator::AND: {
                result = Item1 & Item2;
                break;
            }
            case BinaryOperator::OR: {
                result = Item1 | Item2;
                break;
            }
            case BinaryOperator::NOT: {
                result = ~Item1;
                break;
            }
            case BinaryOperator::XOR: {
                result = Item1 ^ Item2;
                break;
            }
        }
        return result;
    }

    void showAsBin() {
        auto metadata = converters::ArgumentMetadata{4}.withShowBinary(true);
        {
            auto hxr = converters::Hexer<uint32_t>{Item1};
            hxr.setConfig(metadata);
            hxr.toBin();
            hxr.log();
        }

        switch (Operator) {
            case BinaryOperator::AND: {
                LOG_B() << "&\n";
                break;
            }
            case BinaryOperator::OR: {
                LOG_B() << "|\n";
                break;
            }
            case BinaryOperator::XOR: {
                LOG_B() << "^\n";
                break;
            }
            case BinaryOperator::NOT: {
                LOG_B() << "~\n";
                break;
            }
        }

        {
            if (Operator != BinaryOperator::NOT) {
                auto hxr = converters::Hexer<uint32_t>{Item2};
                hxr.setConfig(metadata);
                hxr.toBin();
                hxr.log();
            }
        }
        LOG_B() << "=\n";

        {
            uint32_t result = compute();
            auto hxr = converters::Hexer<uint32_t>{result};
            hxr.setConfig(metadata);
            hxr.toBin();
            hxr.log();
        }
    }
};

/// Takes a string expression and evaluates the binary operators

/**
 * @class Eval
 * @brief Eval assumes unsigned 32 bit data.
 *
 */
class Eval {
    GTEST_SEE_INTERNALS
    SizedText<100> sizedExpression_;
    uint32_t arryPos_;
    Operation<uint32_t> op_;
    bool showBin_;

    void parse();

   public:
    Eval(std::string expression, bool showBin = false)
        : sizedExpression_{expression.data()}, showBin_{showBin} {};
    uint32_t getResult();
};

#endif
