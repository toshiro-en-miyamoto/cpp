#include <gtest/gtest.h>
#include "stage2ut/main/hello-greet.h"

TEST(HelloTest, GetGreet) {
    EXPECT_EQ(get_greet("Bazel"), "Hello Bazel");
}
