#include <stdio.h>
#include <stdlib.h>

int main() {

FILE *file = fopen("ex2.txt", "w");
if (file != NULL) {
	char a[100];
	fgets(a, sizeof(a), stdin);
	fprintf(file, "%s", a);
	fclose(file);
	printf("파일 작성 완료.\n");
}
else {
        printf("파일 작성 실패.\n");
	exit(1);
}

file = fopen("ex2.txt", "r");
if (file != NULL) {
	char buffer[100];
	fgets(buffer, sizeof(buffer), file);
	fclose(file);
	printf("파일 열기 완료.\n");
	printf("%s", buffer);
}
else {
	printf("파일 열기 실패.\n");
	exit(1);
}
return 0;
}
