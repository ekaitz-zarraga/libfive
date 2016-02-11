#include <catch/catch.hpp>

#include "ao/kernel/tree/store.hpp"
#include "ao/kernel/tree/tree.hpp"
#include "ao/kernel/eval/evaluator.hpp"
#include "ao/kernel/eval/result.hpp"

#ifdef __AVX__

TEST_CASE("Vectorized performance")
{
    const float N = 1000;

    Store s;
    Tree t(&s, s.operation(OP_ADD, s.X(), s.Y()));
    Evaluator e(&t);

    for (int i=0; i < 256; ++i)
    {
        e.set(i, 2*i, 0, i);
    }

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    const float* slow;
    for (int i=0; i < N; ++i)
    {
        slow = e.values(256, false);
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> ft = end - start;

    start = std::chrono::system_clock::now();
    const float* fast;
    for (int i=0; i < N; ++i)
    {
        fast = e.values(256, true);
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> mt = end - start;

    REQUIRE(mt.count() < ft.count());

    bool matched = true;
    for (int i=0; i < 256; ++i)
    {
        if (fast[i] != slow[i])
        {
            CAPTURE(i);
            CAPTURE(fast[i]);
            CAPTURE(slow[i]);
            matched = false;
        }
    }
    REQUIRE(matched);
}

#endif