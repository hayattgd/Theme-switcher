#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "ASCII.h"

char* concat_str(const char* str1, const char* str2)
{
	char* result = malloc(strlen(str1) + strlen(str2) + 1);
	if (!result)
	{
		return NULL; // Memory allocation failed
	}

	strcpy(result, str1);
	strcat(result, str2);

	return result;
}

char* color(int code)
{
	char* result;
	if (code == -1)
	{
		result = strdup("\033[0m");
	}
	else
	{
		result = malloc(12); // Enough to hold "\033[38;05;%im" and the code
		if (result)
		{
			sprintf(result, "\033[38;05;%im", code);
		}
	}
	return result;
}

void remove_directory(const char* path)
{
	DIR* dir = opendir(path);
	if (dir == NULL)
	{
		perror("Error opening directory");
		return;
	}

	struct dirent* entry;
	char filepath[1024];
	struct stat statbuf;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}

		snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
		if (stat(filepath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			remove_directory(filepath);
		}
		else
		{
			if (remove(filepath) != 0)
			{
				perror("Error removing file");
			}
		}
	}

	closedir(dir);
	if (rmdir(path) != 0)
	{
		perror("Error removing directory");
	}
}

void copy_file(const char* src, const char* dest)
{
	FILE* src_file = fopen(src, "rb");
	if (src_file == NULL)
	{
		perror("Error opening source file");
		return;
	}

	FILE* dest_file = fopen(dest, "wb");
	if (dest_file == NULL)
	{
		perror("Error opening destination file");
		fclose(src_file);
		return;
	}

	char buffer[1024];
	size_t bytes;
	while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0)
	{
		fwrite(buffer, 1, bytes, dest_file);
	}

	fclose(src_file);
	fclose(dest_file);
}

void copy_directory(const char* src, const char* dest)
{
	DIR* dir = opendir(src);
	if (dir == NULL)
	{
		return;
	}

	struct stat st = {0};
	if (stat(dest, &st) == -1)
	{
		mkdir(dest, 0700);
	}

	struct dirent* entry;
	char src_path[1024];
	char dest_path[1024];
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}

		snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
		snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

		struct stat statbuf;
		if (stat(src_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			copy_directory(src_path, dest_path);
		}
		else
		{
			copy_file(src_path, dest_path);
		}
	}

	closedir(dir);
}

int confirm()
{
	char response;
	printf(ALT_WHITE"Do you wanna proceed? "ALT_BLUE"[Y/N]: "RESET);
	scanf(" %c", &response);
	return (!(response == 'Y' || response == 'y'));
}
