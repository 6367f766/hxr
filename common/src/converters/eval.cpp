#include "eval.h"

void Eval::parse() {
    std::string numberAsString = "";
    uint32_t number{0};
    BinaryOperator binOperator = BinaryOperator::UNDEFINED;

    for (uint32_t i = 0; i < sizedExpression_.getSize(); i++) {
        char ch = sizedExpression_.get().at(i);

        if (ch >= 0x30 && ch <= 0x39) {
            numberAsString.push_back(ch);
        }
        else {
            switch (ch) {
                case 0x26: {
                    binOperator = BinaryOperator::AND;
                    break;
                }
                case 0x7e: {
                    binOperator = BinaryOperator::NOT;
                    break;
                }
                case 0x5e: {
                    binOperator = BinaryOperator::XOR;
                    break;
                }
                case 0x7c: {
                    binOperator = BinaryOperator::OR;
                    break;
                }
            }

            if (numberAsString.empty()) {
                // prevents `std::stoi` calculation when `numberAsString` is
                // empty (which causes a crash)
                continue;
            }

            number = std::stoi(numberAsString);
            op_ = op_.withOperand(number);
            number = 0;
            numberAsString = "";
        }
    }
    number = std::stoi(numberAsString);
    op_ = op_.withOperand(number);
    op_ = op_.withOperator(binOperator);
}

uint32_t Eval::getResult() {
    parse();
    if (showBin_) {
        op_.showAsBin();
    }
    return op_.compute();
}

#ifdef UNIT_TEST
#include <gtest/gtest.h>

TEST(Operation, OR) {
    Operation<uint8_t> op{0b00000001, 0b00000010, BinaryOperator::OR};
    auto result = op.compute();
    ASSERT_EQ(result, 3U);
}

TEST(Operation, AND) {
    Operation<uint8_t> op{0b00000011, 0b00001111, BinaryOperator::AND};
    auto result = op.compute();
    ASSERT_EQ(result, 3U);
}

TEST(Operation, NOT) {
    Operation<uint8_t> op{0b00000000, 0b00001111, BinaryOperator::NOT};
    auto result = op.compute();
    ASSERT_EQ(result, 255U);
}

TEST(Operation, XOR) {
    Operation<uint8_t> op{0b00000000, 0b00001111, BinaryOperator::XOR};
    auto result = op.compute();
    ASSERT_EQ(result, 15U);
}

TEST(Eval, parse) {
    auto evaluation = Eval{"100 & 300"};
    evaluation.parse();
    ASSERT_EQ(evaluation.op_.Item1, 100U);
    ASSERT_EQ(evaluation.op_.Item2, 300U);
    ASSERT_EQ(evaluation.op_.Operator, BinaryOperator::AND);
}

TEST(Eval, getResult) {
    auto evaluation = Eval{"100 & 300"};
    auto result = evaluation.getResult();
    ASSERT_EQ(result, 36);
}

#endif
