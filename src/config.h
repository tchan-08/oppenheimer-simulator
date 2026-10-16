#ifndef _CONFIG_H
#define _CONFIG_H

struct ConfigFile {
	char imageDir[1024];
	time_t interval;
}

int parseConfig(struct ConfigFile *config);

#endif
