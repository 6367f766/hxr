#ifndef EVAL_H
#define EVAL_H

enum class BinaryOperator {
    AND = 0,  // &
    OR = 1,   // |
    NOT = 2,  // ~
    XOR = 3   // ^
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

    void compute() {
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
        std::cout << result;
    }
};

/// Takes a string expression and evaluates the binary operators

class Eval {
   public:  // XXX: temporarily here
    SizedText<100> sizedExpression_;

   public:
    Eval(std::string expression) : sizedExpression_{expression.data()} {};
};

#endif
