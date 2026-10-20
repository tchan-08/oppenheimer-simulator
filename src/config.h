#ifndef _CONFIG_H
#define _CONFIG_H

#include <time.h>

struct ConfigFile {
	char imageDir[1024]; //the directory with all the images in it
	int chanceNormal;
	int chanceZoom;
	int chanceEpilepsy;
	int chanceFlash;
	time_t interval;
};

/*
int main() {
	struct ConfigFile config;
	parseConfig(&config);
	//config has now got all its values
}
*/
int parseConfig(struct ConfigFile *config);

#endif
