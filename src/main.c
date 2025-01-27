#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "util.h"
#include "ASCII.h"

#define BASEPATH "/.config/themesw/"
#define TITLE "themesw"
#define DESC "Global theme switcher by hayattgd"
#define VER "1.0dev"
#define FULLVER TITLE " " VER

//Flags
int d = 0;
int y = 0;
int each = 0;

int m_argc = 0;

char* fullpath = "";

void printinfo()
{
	/* ASCII trick :)
	 * \033[s - Save cursor position
	 * \033[u - Restore cursor position
	 * 
	 * so it will overwrite it with FULLVER
	 */

	char* titlecolor = color(rand() % 8 + 7);
	printf("%s╭ \033[s────────────────────────────────────╮\033[u%s \n", titlecolor, FULLVER);
	printf("│  "ALT_WHITE DESC"%s  │\n", titlecolor);
	if(m_argc == 1)printf("│               "WHITE"maybe: themesw help%s   │\n", titlecolor);
	printf("╰─────────────────────────────────────╯\n" RESET);
	free(titlecolor);
}

void printhelp()
{
	printinfo();
	printf(
		"\n"
		"Usage: themesw [OPTION] (FLAGS)\n"
		"\n"
		"Options:\n"
		"  list         List all available themes\n"
		"  apply [NAME] Apply the specified theme by name\n"
		"  help         Display this help menu and exit\n"
		"  version      Display the version information and exit\n"
		"  where        Locate the themes directory\n"
		"\n"
		"Flags:\n"
		"  --debug      Enable debug mode\n"
		"  --yes        Skip confirmation\n"
		"  --each       Show confirmation for each process\n"
	);
}

void listtheme()
{
	char* themespath = concat_str(fullpath, "themes");

	printf(ALT_WHITE"List of themes:\n");
	if(d)printf("dir=%s\n", themespath);
	DIR* dir = opendir(themespath);
	if (dir == NULL)
	{
		perror("Error opening themes directory");
		free(dir);
		exit(1);
	}

	struct dirent* entries;
	while ((entries = readdir(dir)) != NULL)
	{
		if (strcmp(entries->d_name, ".") == 0 || strcmp(entries->d_name, "..") == 0)
		{
			continue;
		}
		printf(WHITE" - "ALT_WHITE"%s\n", entries->d_name);
	}
}

void applytheme(char* name)
{
	char* themespath = concat_str(fullpath, "themes/");
	char* themepath = concat_str(themespath, name);
	char* themeconfigs = concat_str(themepath, "/config/");

	if(d)printf("dir=%s\n", themeconfigs);
	DIR* dir = opendir(themeconfigs);
	if (dir == NULL)
	{
		perror("Error opening theme directory");
		free(dir);
		exit(1);
	}

	char* configpath = concat_str(getenv("HOME"), "/.config/");


	printf("~/.config is going to be replaced with ~/.config/themesw/themes/%s\n", name);
	if (!(y || each))
	{
		if (confirm())
		{
			printf("Operation cancelled.\n");
			closedir(dir);
			exit(0);
		}
	}

	struct dirent* entries;
	while ((entries = readdir(dir)) != NULL)
	{
		if (strcmp(entries->d_name, ".") == 0 || strcmp(entries->d_name, "..") == 0)
		{
			continue;
		}

		if (each)
		{
			printf("Processing %s...\n", entries->d_name);
			if (confirm())
			{
				continue;
			}
		}

		char* src = concat_str(themeconfigs, entries->d_name);
		char* dest = concat_str(configpath, entries->d_name);

		if(d)printf("src= %s\ndest=%s\n", src, dest);

		remove_directory(dest);
		copy_directory(src, dest);

		free(src);
		free(dest);
		if(d)printf("done\n");
	}
	printf("Running run.sh...\n");

	if (each)
	{
		if (confirm())
		{
			exit(0);
		}
	}

	pid_t sh = fork();
	if (sh == -1)
	{
		perror("Error forking process");
		exit(1);
	}

	if (sh == 0)
	{
		execl("/bin/sh", "sh", concat_str(themepath, "/run.sh"), (char *)NULL);
		perror("Error executing run.sh");
		exit(1);
	}

	printf("Shell script executed successfully\n");
	exit(0);
}

int main(int argc, char** argv)
{
	m_argc = argc;
	srand(time(NULL));
	fullpath = concat_str(getenv("HOME"), BASEPATH);

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--debug") == 0)
		{
			d = 1;
		}
		else if (strcmp(argv[i], "--yes") == 0)
		{
			y = 1;
		}
		else if (strcmp(argv[i], "--each") == 0)
		{
			each = 1;
		}
		
	}

	if(d)printf("%s\n", FULLVER);

	if (argc == 1)
	{
		printinfo();
		return 0;
	}
	else if (argc > 1 && strcmp(argv[1], "version") == 0)
	{
		printf(FULLVER);
		return 0;
	}
	else if(argc > 1 && strcmp(argv[1], "help") == 0)
	{
		printhelp();
		return 0;
	}
	else if (argc > 1 && strcmp(argv[1], "list") == 0)
	{
		listtheme();
	}
	else if (argc > 2 && strcmp(argv[1], "apply") == 0)
	{
		applytheme(argv[2]);
	}
	else if (argc > 1 && strcmp(argv[1], "where") == 0)
	{
		printf("%sthemes/", fullpath);
	}
	else
	{
		printhelp();
		return 1;
	}

	return 0;
}