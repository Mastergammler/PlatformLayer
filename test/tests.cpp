#include "tests/matrix_alg.cpp"
#include "tests/setup_test.cpp"

#define NAMED_TEST(t) {t, #t}

struct TestInfo
{
    int (*func)();
    const char* name;
};

static TestInfo TESTS[] = {
                                        NAMED_TEST(test_million_mult_sum),
                                        // NAMED_TEST(test_fail),
                                        NAMED_TEST(row_swap_4x3),
                                        NAMED_TEST(row_swap_3x3),
                                        NAMED_TEST(row_swap_2x2),
                                        NAMED_TEST(row_swap_1x1),
                                        NAMED_TEST(row_swap_1x3),
                                        NAMED_TEST(row_swap_6x4),
                                        NAMED_TEST(row_swap_3x1),
                                        NAMED_TEST(row_swap_3x2),
                                        NAMED_TEST(row_swap_2x3),

};
