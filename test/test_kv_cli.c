#include "test_kv_cli.h"

extern void setUp(void) {
  // set stuff up here
}

extern void tearDown(void) {
  // clean stuff up here
}

extern int main() {
  UNITY_BEGIN();
  printf("Hello from kv_cli tests\n");
  TEST_ASSERT_EQUAL(12, 13);
  return UNITY_END();
}
