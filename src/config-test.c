#include "config.h"
#include <assert.h>
#include <string.h>

int main(void) {
	struct ConfigFile config;
	parseConfig(&config);
	assert(strcmp(config.imageDir,"images") == 0);
	assert(config.interval == 5);
}
