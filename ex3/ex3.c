#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void lsR(const char *root, int depth) {
	DIR *dir = opendir(root);

	if (!dir) {
		perror("열 수 없는 파일");
		exit(1);
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		
		if (entry->d_type == DT_DIR) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				for (int i = 0; i < depth; ++i) {
					printf("  ");
				}
 				printf("%s%s:\n", (depth == 0) ? "" : "/", entry->d_name);
				char path[PATH_MAX];
				snprintf(path, sizeof(path), "%s/%s", root, entry->d_name);
				lsR(path, depth + 1);
         		}
		}
		else {
			for (int i = 0; i < depth; ++i) {
			printf("  ");
			}
			printf("%s%s\n", (depth == 0) ? "" : "/", entry->d_name);
		}
	}

	closedir(dir);
}

int main(int argc, char *argv[]) {
        const char *root = (argc < 2) ? "." : argv[1];
        printf("%s:\n", root);
        lsR(root, 0);
        
        return 0;
}
