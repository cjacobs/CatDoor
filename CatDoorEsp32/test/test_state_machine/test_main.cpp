#include <unity.h>
#include <StateMachine.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_state_machine_init()
{
    // TEST_ASSERT_EQUAL(32, ...);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_state_machine_init);
    // more RUN_TESTs
    UNITY_END();

    return 0;
}
