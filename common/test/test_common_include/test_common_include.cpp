#include <gtest/gtest.h>

#include "common.h"
#include "converters.h"
#include "eval.h"
#include "logger.h"

TEST(TestArgumentMetadata, supportedArgs) {
    // hxr supports the following arguments:
    //
    // - text
    // - hex
    // - signed integer
    // - unsigned integer
    //
    // of which hex, and integers can be displayed as 32 or 64 bits.
    //
    auto uint32 = converters::ArgumentMetadata{4}.withShowBinary(true);
    ASSERT_EQ(uint32.size, 4);

    auto uint64 = uint32.withSize(8);
    ASSERT_EQ(uint64.size, 8);
    ASSERT_EQ(uint64.isString, false);
    ASSERT_EQ(uint64.isHex, false);
    ASSERT_EQ(uint64.showBinary, true);
    ASSERT_EQ(uint64.isSigned, false);

    auto hex = uint32.withSize(8).withIsHex(true);
    ASSERT_EQ(hex.size, 8);
    ASSERT_EQ(hex.isHex, true);

    auto text =
        converters::ArgumentMetadata{100}.withShowBinary(true).withIsString(
            true);
    ASSERT_EQ(text.size, 100);
    ASSERT_EQ(text.isString, true);
}

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
