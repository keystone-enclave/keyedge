#include <cstdio>
#include "ocalls.h"

void simple_call(struct v val) {
	if (val.negative == -1 && val.large == 1u << 31) puts("[PASS] Simple call test succeeded.");
	return;
}

