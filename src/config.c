#include "config.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int parseConfig(struct ConfigFile *config) {
	memset(config,0,sizeof(struct ConfigFile));
	//open file
	int configFile = open("configfile",O_RDONLY);
	if (configFile < 0) {
		perror("open");
		return 1;
	}
	//get filesize
	struct stat statbuf = {0};
	if (fstat(configFile, &statbuf) < 0) {
		perror("fstat");
		close(configFile);
		return 1;
	}
	//read the whole file
	size_t fileSize = statbuf.st_size;
	char *fileContents = malloc(fileSize);
	if (read(configFile, fileContents, fileSize) < 0) {
		perror("read");
		close(configFile);
	}
	char *line = fileContents;
	while(*line != '\0') {
		//split by lines
		char *newline = strchr(line,'\n');
		if (newline != NULL) {
			newline++;
			newline[-1] = '\0';
		}else {
			newline += strlen(newline);
		}
		//split by =
		char *equals = strchr(line,'=');
		*equals = '\0';
		char *value = equals + 1;
		char *key = line;
		//fill in struct
		if (strcmp(key,"image_dir") == 0) {
			snprintf(config->imageDir,sizeof(config->imageDir),"%s",value);
		}
		//next iteration
		line = newline;
	}
	//cleanup and return
	close(configFile);
	return 0;
}
