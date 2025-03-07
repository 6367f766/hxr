#include <gtest/gtest.h>

#include "common.h"
#include "eval.h"
#include "logger.h"

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

TEST(TestLogger, init) {
    common::Logger& logger = common::Logger::getLogger();
    logger.log(common::Severity::VERBOSE)
        << "More stuff here" << " and here: " << 5;

    logger.log(common::Severity::VERBOSE)
        << "Another one " << " and here: " << 5;
    //
}
