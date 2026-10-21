#include "config.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int parseConfig(struct ConfigFile *config) {
	struct ConfigFile baseConfig = {
		.chanceNormal = 45,
		.chanceZoom = 20,
		.chanceEpilepsy = 10,
		.chanceFlash = 25,
	};
	memcpy(config,&baseConfig,sizeof(struct ConfigFile));
	//open file
	int configFile = open("../configfile",O_RDONLY);
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
	char *fileContents = malloc(fileSize + 1);

	if (fileContents == NULL) {
		perror("malloc");
		close(configFile);
		return 1;
	}

	ssize_t bytesRead = read(configFile, fileContents, fileSize);
	if (bytesRead < 0) {
		perror("read");
		free(fileContents);
		close(configFile);
		return 1;
	}

	fileContents[bytesRead] = '\0';
	close(configFile);

	char *line = fileContents;
	while(*line != '\0') {
		//split by lines
		char *newline = strchr(line,'\n');
		if (newline != NULL) {
			newline++;
			newline[-1] = '\0';
		}else {
			newline = line + strlen(line);
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
		if (strcmp(key,"interval") == 0) {
			config->interval = strtol(value,NULL,10);
		}
		if (strcmp(key,"normal_chance") == 0) {
			char *endptr = NULL;
			long chance = strtol(value,&endptr,10);
			if (*endptr == '\0') config->chanceNormal = chance;
		}
		if (strcmp(key,"flash_chance") == 0) {
			char *endptr = NULL;
			long chance = strtol(value,&endptr,10);
			if (*endptr == '\0') config->chanceFlash = chance;
		}
		if (strcmp(key,"zoom_chance") == 0) {
			char *endptr = NULL;
			long chance = strtol(value,&endptr,10);
			if (*endptr == '\0') config->chanceZoom = chance;
		}
		if (strcmp(key,"epilepsy_chance") == 0) {
			char *endptr = NULL;
			long chance = strtol(value,&endptr,10);
			if (*endptr == '\0') config->chanceEpilepsy = chance;
		}
		//next iteration
		line = newline;
	}
	//cleanup and return
	close(configFile);
	free(fileContents);
	return 0;
}
