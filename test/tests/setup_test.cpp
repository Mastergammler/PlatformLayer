int test_million_mult_sum()
{
    int val = 0;
    for (int i = 0; i < 1000000; i++)
    {
        val += i * 26;
    }

    return val != 0 ? 0 : 1;
}

int test_fail()
{
    return 1;
}
