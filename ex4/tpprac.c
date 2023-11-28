#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_SENTENCES 6
#define MAX_SENTENCE_LENGTH 100

const char *sentences[MAX_SENTENCES] = {
	"It's close to midnight And something evil's lurking in the dark",
	"Under the moonlight You see a sight that almost stops your heart",
	"You try to scream But terror takes the sound before you make it",
	"You start to freeze As horror looks you right between the eyes",
	"You're paralyzed Cause this is thriller, thriller night",
	"And no one's gonna save you from the beast about to strike"
};


int main() {
	srand(time(NULL));

	printf("\n");

	int totalerror = 0;
	int totalinput = 0;
	double totalpermin = 0;

	for (int i = 0; i < MAX_SENTENCES; i++) {
		printf("\n");
		printf("%s", sentences[i]);
        	printf("\n");

		time_t startTime = time(NULL);
        
		char userInput[MAX_SENTENCE_LENGTH];
		fgets(userInput, sizeof(userInput), stdin);
        
		time_t endTime = time(NULL);
		double elapsedTime = difftime(endTime, startTime);


		int typos = 0;
		for (int j = 0; j < strlen(userInput); j++) {
			if (userInput[j] != sentences[i][j]) {
			typos++;
			}
        }

		totalerror += typos;
		totalinput += strlen(userInput);
		
		totalpermin += (totalinput / elapsedTime) * 60;
		printf("\n걸린 시간: %.2f 초\n", elapsedTime);
		printf("오타수: %d\n", typos);
		printf("타자 수: %.2f 타\n", totalpermin);
	
	}

	printf("\n타자 연습 종료\n");
	printf("총 오타수: %d\n", totalerror);
	printf("평균 분당 타자 수: %.2f 타\n", totalpermin / MAX_SENTENCES);

	return 0;
}
