#include <cstdio>

int test_two_dimensional_array(int arr[2][3]) {
	int cnt = 0;
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			if (arr[i][j] != cnt++) {
				puts("[FAIL] Two dimensional array test failed.");
				return 1;
			}
	puts("[PASS] Two dimensional array test succeeded.");
	return 0;
}

