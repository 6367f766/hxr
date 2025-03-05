#include <gtest/gtest.h>

#include "common.h"
#include "eval.h"
#include "type_list.h"

TEST(TestOne, test) { 
    ASSERT_TRUE(true); 
    ASSERT_EQ(add_two_uints(10, 30), 40);
}

TEST(TestEval, init) {
    Eval example{"this is a test"};
    example.sizedExpression_.print();

    Operation<uint32_t> op{0x1234, 0xff, BinaryOperator::AND};
    op.compute();
}

TEST(TestTypeList, init) {
    EnumToType_t<MapGet("u")> hello = 10;
    std::cout << hello << std::endl;
}
