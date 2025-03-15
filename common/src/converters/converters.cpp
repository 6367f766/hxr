#include "converters.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>

TEST(Arg, init) {
    auto mtdt = converters::ArgumentMetadata{4};
    ASSERT_FALSE(mtdt.isString);
    ASSERT_FALSE(mtdt.isHex);
    ASSERT_FALSE(mtdt.showBinary);
    ASSERT_FALSE(mtdt.isFloating);
    ASSERT_EQ(mtdt.size, 4U);
}
#endif
