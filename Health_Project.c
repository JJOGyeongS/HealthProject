#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>


void menu_init();
void menu_part();
int menu_entry(int menu_part_num);
void menu_set(); // 세트수 입력함수
void menu_break_time(); //쉬는 시간 측정 함수
int check_menu(int menu_part_num, int* menu_entry_num, int* set, int entry_count, int breaktiem); //정보 확인 함수
int breatime_timer(int menu_part_num, int breaktime, int* set, int entry_count, int* menu_entry_num);//타이머 함수
int health_graph(int menu_part_num, int* menu_entry_num, int* set, int entry_count); //마지막 종료 그래프
void graph();
char* getNextDataPtr(char* originStr, char* nowPosStr); // 다음 글자 시작위치 리턴 (originStr : 원본 문자열 주소,  nowPosStr : 현위치기준, 공백다음 등장하는 문자 찾아옴)
int fileCopy(const char* src, const char* dest);  //src->dest 복사  (src, dest 파일명).  -1리턴시 복사 실패 

typedef	struct _Health {
	int menu_part_num; //운동 부위 선택
	int menu_entry_num[5];//운동 종목 선택
	int H_EVENT[7]; //종목
	int set[6]; //세트 수
	//int Break_Time;
}Health;



int main()
{
	FILE* fp = fopen("health.txt", "a");

	if (fp == NULL) {
		printf("파일을 생성할 수 없습니다.\n");
		return 0;
	}
	else
		printf("파일이 생성되었습니다.\n");  //파일 생성

	fclose(fp);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t); //현재 날짜 출력

	Health h1;

	int i, j;
	int menu_num; //초기 메뉴 선택
	int menu_part_num; // 운동 부위 선택
	int menu_entry_num[5], entry_count = 0; //운동 종목 선택, 몇개 할건지 선택
	int set[6]; //세트 수
	int breaktime; //쉬는 시간
	int starttimer;
	int timerstop; //타이머 중지 실행


	while (1)
	{
		menu_init();
		printf("메뉴를 선택하세요...(1~3) ->");
		scanf("%d", &menu_num);

		switch (menu_num) {
		case 1:
		{
			FILE* fp_temp = fopen("health_temp.txt", "w"); // 정보 임시 저장 파일

			fprintf(fp_temp, "%d%d%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
			system("cls");

			menu_part();
			printf("운동 부위를 선택하세요...(0~4) ->");
			scanf("%d", &menu_part_num);
			if (menu_part_num == 0) {
				system("cls");
				continue;
			}
			else {
				fprintf(fp_temp, " %d", menu_part_num);


				printf("몇개의 운동을 하시겠습니까?..(1~5) ");
				scanf("%d", &entry_count);
				if (entry_count == 0) {
					system("cls");
					continue;
				}


				menu_entry(menu_part_num);
				for (i = 0; i < entry_count; i++) {
					printf("%d 번째 운동을 골라주세요.", i + 1);
					scanf("%d", &menu_entry_num[i]);
					fprintf(fp_temp, " %d", menu_entry_num[i]);
				}
				system("cls");

				menu_set();
				for (i = 0; i < entry_count; i++)
				{
					printf("%d 번째 운동의 세트 수를 입력하세요.(최대 5 set)", i + 1);
					scanf("%d", &set[i]);
					fprintf(fp_temp, " %d", set[i]);
				}
				fputs("\n", fp_temp);
				fclose(fp_temp);

				//fp_temp -> fp로 복사
				int ret = fileCopy("health_temp.txt", "health.txt");
				if (ret == -1) {
					printf("입력한 내용이 프로그램 문제로 저장하지 않았습니다.\n");
				}

				system("cls");
				menu_break_time();
				printf("선택한운동의 쉬는 시간을 입력하세요.(최대 59 sec) ");
				scanf("%d", &breaktime);

				system("cls");
				check_menu(menu_part_num, menu_entry_num, set, entry_count, breaktime);
				printf("메뉴를 입력하세요.(0,1)...");
				scanf("%d", &starttimer);
				if (starttimer == 0)
					break;
				else {
					system("cls");
					int set_count = 0;

					breatime_timer(menu_part_num, breaktime, set, entry_count, menu_entry_num);


				}
			}
			system("cls");
			health_graph(menu_part_num, menu_entry_num, set, entry_count);

			break;
		}
		case 2:
			system("cls");
			graph();
			break;
		case 3:
			printf("프로그램을 종료합니다...\n");
			return 0;
		}

	}

	return 0;
}

void menu_init() //초기함수
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("=======================================================\n");
	printf("                                                     \n");
	printf("		   진정한 헬서가 되는길                 \n");
	printf("                      %d-%d-%0d                     \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                     \n");
	printf("                                                     \n");
	printf("                  1. 운동시작                        \n");
	printf("                                                     \n");
	printf("               2. 일별 운동 그래프                   \n");
	printf("                                                     \n");
	printf("                   3. 종료                           \n");
	printf("                                                     \n");
	printf("                                                     \n");
	printf("=======================================================\n");
}

void menu_part() //운동 부위 입력
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("=======================================================\n");
	printf("                                                     \n");
	printf("                   운동 부위 입력                    \n");
	printf("                      %d-%d-%d                       \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                     \n");
	printf("                                                     \n");
	printf("                     1. 가슴                         \n");
	printf("                                                     \n");
	printf("                     2. 어깨                         \n");
	printf("                                                     \n");
	printf("                     3. 등                           \n");
	printf("                                                     \n");
	printf("                     4. 하체                         \n");
	printf("                                                     \n");
	printf("                     0. 뒤로가기                     \n");
	printf("                                                     \n");
	printf("=======================================================\n");

}

int menu_entry(int menu_part_num) //운동 종목 입력 함수
{
	system("cls");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int i;

	char CHEST[7][40] = { "벤치 프레스","인클라인 벤치", "딥스", "덤벨 프레스", "펙덱 플라이", "푸쉬업","체스트 프레스" };
	char shoulder[6][40] = { "덤벨 숄더 프레스", "머신 숄더 프레스", "사이즈 레터럴 레이즈", "프론트 레이즈", "밀리터리 프레스", "벤트 오버 레터럴 레이즈" };
	char BACK[6][40] = { "랫풀다운", "시티드 로우", "풀업", "바벨 로우", "케이블 풀오버" ,"원암 덤벨 로우" };
	char LOW[6][40] = { "스쿼트", "레그 익스텐션", "레그 컬", "레그 프레스", "런지", "카프레이즈" };

	printf("%d-%d-%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                   운동 종목 선택                      \n");
	printf("                                                       \n");


	if (menu_part_num == 1) {
		for (i = 0; i < 7; i++) {
			printf("%d. %3s\n", i, CHEST[i]);
		}
	}
	else if (menu_part_num == 2) {
		for (i = 0; i < 6; i++) {
			printf("%d. %3s\n", i, shoulder[i]);
		}
	}
	else if (menu_part_num == 3) {
		for (i = 0; i < 6; i++) {
			printf("%d. %3s\n", i, BACK[i]);
		}
	}
	else {
		for (i = 0; i < 6; i++) {
			printf("%d. %3s\n", i, LOW[i]);
		}
	}
	printf("                                                       \n");
	printf("=======================================================\n");

	return 0;
}

void menu_set() //세트 수 선택 함수
{
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                운동 세트 수 선택                      \n");
	printf("                                                       \n");
	printf("               1. 세트 수를 입력 하세요.               \n");
	printf("             (단, 숫자로만 입력하세요. ex.5)           \n");
	printf("                                                       \n");
	printf("                                                       \n");
	printf("                  0. 뒤로가기                          \n");
	printf("                                                       \n");
	printf("=======================================================\n");
}

void menu_break_time() //쉬는 시간 함수
{
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                운동 쉬는 시간 선택                    \n");
	printf("                                                       \n");
	printf("             1.1부터 59초까지의 시간 중                \n");
	printf("               쉬는 시간을 입력하세요.                 \n");
	printf("                                                       \n");
	printf("          (단, 숫자로만 입력하세요. (ex. 40)           \n");
	printf("                                                       \n");
	printf("                  0. 뒤로가기                          \n");
	printf("                                                       \n");
	printf("=======================================================\n");
}

int check_menu(int menu_part_num, int* menu_entry_num, int* set, int entry_count, int breaktime) //정보 확인 함수
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i;

	printf("=======================================================\n");
	printf("                    정보 확인 창                    \n");
	printf("                      %d-%d-%d                        \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                       \n");
	switch (menu_part_num)
	{
	case 1: printf("                       가슴                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. 벤치 프레스 : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. 인클라인 벤치 : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. 딥스 : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. 덤벨 플라이 : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. 펙덱 플라이 : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. 푸쉬업 : %d set\n", i + 1, set[i]); break;
			case 6: printf("                 %d. 체스트 프레스 : %d set\n", i + 1, set[i]); break;
			}

		}
		break;
	case 2: printf("                       어깨                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. 덤벨 숄더 프레스 : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. 머신 숄더 프레스 : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. 사이드 레터럴 레이즈 : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. 프론트 레이즈 : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. 밀리터리 프레스 : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. 벤트 오버 레터럴 레이즈 : %d set\n", i + 1, set[i]); break;

			}
		}
		break;
	case 3: printf("                       등                                 \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. 렛풀다운 : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. 시티드 로우 : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. 풀업 : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. 바벨 로우 : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. 케이블 풀 오버 : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. 원암 덤벨 로우 : %d set\n", i + 1, set[i]); break;
			}
		}
		break;
	case 4: printf("                       하체                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. 스쿼드 : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. 레그 익스텐션 : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. 레그 컬 : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. 레그 프레스 : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. 런지 : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. 카프레이즈 : %d set\n", i + 1, set[i]); break;
			}
		}
		break;
	}
	printf("                                                       \n");
	printf("		쉬는 시간 : %d sec      \n", breaktime);
	printf("                                                       \n");
	printf("=======================================================\n");
	printf("         정보가 맞으면 아래의 메뉴를 선택하세요\n");
	printf("                                                       \n");
	printf("              1.타이머 시작   0. 뒤로가기              \n");
	printf("=======================================================\n");

	return 0;
}

int breatime_timer(int menu_part_num, int breaktime, int* set, int entry_count, int* menu_entry_num) //타이머 함수
{


	int endTime = (unsigned)time(NULL);
	endTime += breaktime;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i = 0;
	int set_count = 1;


	while (1)
	{
		int startTime = (unsigned)time(NULL);
		printf("=======================================================\n");
		printf("                      %d-%d-%d                       \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
		printf("                                                       \n");
		switch (menu_part_num)
		{
		case 1: printf("                       가슴                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 벤치 프레스 : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. 인클라인 벤치 : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. 딥스 : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. 덤벨 플라이 : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. 펙덱 플라이 : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. 푸쉬업 : %d set\n", i + 1, set[i]); break;
			case 6: printf("               %d. 체스트 프레스 : %d set\n", i + 1, set[i]); break;
			}

			break;
		case 2: printf("                       어깨                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 덤벨 숄더 프레스 : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. 머신 숄더 프레스 : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. 사이드 레터럴 레이즈 : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. 프론트 레이즈 : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. 밀리터리 프레스 : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. 벤트 오버 레터럴 레이즈 : %d set\n", i + 1, set[i]); break;

			}

			break;
		case 3: printf("                       등                                 \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 렛풀다운 : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. 시티드 로우 : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. 풀업 : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. 바벨 로우 : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. 케이블 풀 오버 : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. 원암 덤벨 로우 : %d set\n", i + 1, set[i]); break;

			}
			break;
		case 4: printf("                       하체                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 스쿼드 : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. 레그 익스텐션 : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. 레그 컬 : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. 레그 프레스 : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. 런지 : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. 카프레이즈 : %d set\n", i + 1, set[i]); break;
			}
			break;
		}
		printf("\n");
		printf("            ===========================                \n");
		printf("                 00      :      %d                     \n", endTime - startTime);
		printf("            ===========================                \n");
		printf("                                                       \n");
		printf("               전체 세트 / 현재 세트                   \n");

		printf("                    %d    /   %d                         \n", set[i], set_count);
		printf("=======================================================\n");
		Sleep(500);
		system("cls");


		if (endTime - startTime == 0)
		{
			if (i == entry_count - 1 && set[i] == set_count) {
				printf("=======================================================\n");
				printf("\n");
				printf("        타이머 종료!! 운동끝  고생하셨습니다!!!!!!!\n");
				printf("\n");
				printf("=======================================================\n");
				return 0;
			}

			if (set[i] == set_count)
			{
				printf("=======================================================\n");
				printf("\n");
				printf("        타이머 종료!! 다음 **운동**을 시작하세요!!!!!!!\n");
				printf("\n");
				printf("             계속하시려면 메뉴를 눌러주세요.\n");
				printf("\n");
				printf("           다음 운동 시작 --> 1\n");
				printf("           타이머 중지 --> 0\n");
				printf("\n");
				printf("=======================================================\n");
				i++;
				set_count = 1;
			}
			else {
				printf("=======================================================\n");
				printf("\n");
				printf("        타이머 종료!! 다음 세트를 시작하세요!!!!!!!\n");
				printf("\n");
				printf("             계속하시려면 메뉴를 눌러주세요.\n");
				printf("\n");
				printf("           다음 운동 시작 --> 1\n");
				printf("           타이머 중지 --> 0\n");
				printf("\n");
				printf("=======================================================\n");

				set_count++;
			}
			int timerstop;
			scanf("%d", &timerstop);
			if (timerstop == 0) return 0;
			endTime = (unsigned)time(NULL);
			endTime += breaktime;

		}
	}
}
int health_graph(int menu_part_num, int* menu_entry_num, int* set, int entry_count)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i, j;

	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                %d-%d-%d  오늘의 운동                      \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                       \n");
	switch (menu_part_num)
	{
	case 1: printf("                       가슴                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 벤치 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. 인클라인 벤치 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. 딥스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. 덤벨 플라이 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. 펙덱 플라이 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. 푸쉬업 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 6: printf("               %d. 체스트 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}

		}
		break;
	case 2: printf("                       어깨                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 덤벨 숄더 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");

				}
				printf("\n");
				break;
			case 1: printf("               %d. 머신 숄더 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. 사이드 레터럴 레이즈 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. 프론트 레이즈 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. 밀리터리 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. 벤트 오버 레터럴 레이즈 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;

			}
		}
		break;
	case 3: printf("                       등                                 \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 렛풀다운 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. 시티드 로우 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. 풀업 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. 바벨 로우 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. 케이블 풀 오버 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. 원암 덤벨 로우 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}
		}
		break;
	case 4: printf("                       하체                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. 스쿼드 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. 레그 익스텐션 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. 레그 컬 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. 레그 프레스 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. 런지 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. 카프레이즈 : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}
		}
		break;
	}
	printf("\n               고생하셨습니다!!!!\n");
	printf("\n");
	printf("=======================================================\n");


	return 0;
}

void graph()
{
	FILE* fp = fopen("health.txt", "rb");

	int i;

	char str[200] = { 0 }, date[12];
	int menu_part, menu_entry[5], * set[5];



	while (!feof(fp)) {
		fgets(str, sizeof(str), fp);

		//날짜받고
		sscanf(str, "%s", date);
		//운동부위 받음
		char* next = str;
		next = getNextDataPtr(str, next);
		if (next == NULL) continue;

		sscanf(next, "%d", &menu_part);

		//운동종목, 세트파악
		int menu_entryCnt = 0;
		char* temp = next;
		while (temp != NULL) {
			temp = getNextDataPtr(str, temp);
			menu_entryCnt++;
		}
		menu_entryCnt /= 2;


		Health data = { 0 }; //초기화

		//구조체에 데이터 대입
		data.menu_part_num = menu_part;
		for (int i = 0; i < menu_entryCnt; i++) {
			next = getNextDataPtr(str, next);
			sscanf(next, "%d", &data.menu_entry_num[i]);
		}

		for (int i = 0; i < menu_entryCnt; i++) {
			next = getNextDataPtr(str, next);
			sscanf(next, "%d", &data.set[i]);
		}


		printf("%-6s %-10s", "DATE(y.m.d) : ", date);
		if (menu_part == 1) {
			printf("%10s %-12s\n", "부위 : ", "가슴");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("벤치 프레스 : "); break;
				case 1: printf("인클라인 벤치 : "); break;
				case 2: printf("딥스 : "); break;
				case 3: printf("덤벨 프레스 : "); break;
				case 4: printf("펙덱 플라이 : "); break;
				case 5: printf("푸쉬업 : "); break;
				case 6: printf("체스트 프레스 : "); break;
				}

				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}

			printf("\n\n");
		}
		else if (menu_part == 2) {
			printf("%10s %-12s\n\n", "부위 : ", "어깨");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("덤벨 숄더 프레스 : "); break;
				case 1: printf("머신 숄더 프레스 : "); break;
				case 2: printf("사이즈 레터럴 레이즈 : "); break;
				case 3: printf("프론트 레이즈"); break;
				case 4: printf("밀리터리 프레스"); break;
				case 5: printf("벤트 오버 레터럴 레이즈"); break;
				}
				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}
			printf("\n\n");
		}
		else if (menu_part == 3) {
			printf("%10s %-12s\n\n", "부위 : ", "등");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("랫풀다운 : "); continue;
				case 1: printf("시티드 로우 : "); continue;
				case 2: printf("풀업 : "); continue;
				case 3: printf("바벨 로우 : "); continue;
				case 4: printf("케이블 풀오버 : "); continue;
				case 5: printf("원암 덤벨 로우 : "); continue;
				}

				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}

			printf("\n\n");
		}
		else {
			printf("%10s %-12s\n\n", "부위 : ", "하체");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("스쿼트 : "); break;
				case 1: printf("레그 익스텐션 : "); break;
				case 2: printf("레그 컬 : "); break;
				case 3: printf("레그 프레스 : "); break;
				case 4: printf("런지 : "); break;
				case 5: printf("카프레이즈 : "); break;
				}
				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}
			printf("\n\n");
		}
	}
	fclose(fp);

}

char* getNextDataPtr(char* originStr, char* nowPosStr) {
	int len = 0;
	//originStr의 널문자 혹은 엔터키 등장하기 전까지의 길이 잼
	while (originStr[len] != 0 && originStr[len] != '\r' && originStr[len] != '\n') { len++; }

	//nowPosStr이 originStr 외 영역을 가리키면 NULL리턴 
	if (nowPosStr < originStr || nowPosStr - originStr >= len) return NULL;

	int pos = 0;
	while (nowPosStr[pos] != ' ' && nowPosStr[pos] != '\r' && nowPosStr[pos] != '\n' && nowPosStr[pos] != 0) {
		pos++;
	}

	if (nowPosStr - originStr + pos >= len) return NULL;
	return nowPosStr + pos + 1; // +1 한 이유는 다음데이터 위치를 가리킬거여서  데이터 공백 데이터 공백 데이터 공백  형태로 메모장에 저장돼있으니  공백 다음 데이터 주소 가리키려고 +1함.    
}


int fileCopy(const char* src, const char* dest) {
	FILE* fp_src = fopen(src, "r");
	FILE* fp_dest = fopen(dest, "a");

	if (fp_src == NULL) return -1;
	if (fp_dest == NULL) return -1;

	char readData;
	while (!feof(fp_src)) {
		fread(&readData, sizeof(readData), 1, fp_src);
		fwrite(&readData, sizeof(readData), 1, fp_dest);
	}
	fclose(fp_src);
	fclose(fp_dest);
	return 0;
}