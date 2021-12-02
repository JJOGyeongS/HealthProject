#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>


void menu_init();
void menu_part();
int menu_entry(int menu_part_num);
void menu_set(); // ��Ʈ�� �Է��Լ�
void menu_break_time(); //���� �ð� ���� �Լ�
int check_menu(int menu_part_num, int* menu_entry_num, int* set, int entry_count, int breaktiem); //���� Ȯ�� �Լ�
int breatime_timer(int menu_part_num, int breaktime, int* set, int entry_count, int* menu_entry_num);//Ÿ�̸� �Լ�
int health_graph(int menu_part_num, int* menu_entry_num, int* set, int entry_count); //������ ���� �׷���
void graph();
char* getNextDataPtr(char* originStr, char* nowPosStr); // ���� ���� ������ġ ���� (originStr : ���� ���ڿ� �ּ�,  nowPosStr : ����ġ����, ������� �����ϴ� ���� ã�ƿ�)
int fileCopy(const char* src, const char* dest);  //src->dest ����  (src, dest ���ϸ�).  -1���Ͻ� ���� ���� 

typedef	struct _Health {
	int menu_part_num; //� ���� ����
	int menu_entry_num[5];//� ���� ����
	int H_EVENT[7]; //����
	int set[6]; //��Ʈ ��
	//int Break_Time;
}Health;



int main()
{
	FILE* fp = fopen("health.txt", "a");

	if (fp == NULL) {
		printf("������ ������ �� �����ϴ�.\n");
		return 0;
	}
	else
		printf("������ �����Ǿ����ϴ�.\n");  //���� ����

	fclose(fp);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t); //���� ��¥ ���

	Health h1;

	int i, j;
	int menu_num; //�ʱ� �޴� ����
	int menu_part_num; // � ���� ����
	int menu_entry_num[5], entry_count = 0; //� ���� ����, � �Ұ��� ����
	int set[6]; //��Ʈ ��
	int breaktime; //���� �ð�
	int starttimer;
	int timerstop; //Ÿ�̸� ���� ����


	while (1)
	{
		menu_init();
		printf("�޴��� �����ϼ���...(1~3) ->");
		scanf("%d", &menu_num);

		switch (menu_num) {
		case 1:
		{
			FILE* fp_temp = fopen("health_temp.txt", "w"); // ���� �ӽ� ���� ����

			fprintf(fp_temp, "%d%d%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
			system("cls");

			menu_part();
			printf("� ������ �����ϼ���...(0~4) ->");
			scanf("%d", &menu_part_num);
			if (menu_part_num == 0) {
				system("cls");
				continue;
			}
			else {
				fprintf(fp_temp, " %d", menu_part_num);


				printf("��� ��� �Ͻðڽ��ϱ�?..(1~5) ");
				scanf("%d", &entry_count);
				if (entry_count == 0) {
					system("cls");
					continue;
				}


				menu_entry(menu_part_num);
				for (i = 0; i < entry_count; i++) {
					printf("%d ��° ��� ����ּ���.", i + 1);
					scanf("%d", &menu_entry_num[i]);
					fprintf(fp_temp, " %d", menu_entry_num[i]);
				}
				system("cls");

				menu_set();
				for (i = 0; i < entry_count; i++)
				{
					printf("%d ��° ��� ��Ʈ ���� �Է��ϼ���.(�ִ� 5 set)", i + 1);
					scanf("%d", &set[i]);
					fprintf(fp_temp, " %d", set[i]);
				}
				fputs("\n", fp_temp);
				fclose(fp_temp);

				//fp_temp -> fp�� ����
				int ret = fileCopy("health_temp.txt", "health.txt");
				if (ret == -1) {
					printf("�Է��� ������ ���α׷� ������ �������� �ʾҽ��ϴ�.\n");
				}

				system("cls");
				menu_break_time();
				printf("�����ѿ�� ���� �ð��� �Է��ϼ���.(�ִ� 59 sec) ");
				scanf("%d", &breaktime);

				system("cls");
				check_menu(menu_part_num, menu_entry_num, set, entry_count, breaktime);
				printf("�޴��� �Է��ϼ���.(0,1)...");
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
			printf("���α׷��� �����մϴ�...\n");
			return 0;
		}

	}

	return 0;
}

void menu_init() //�ʱ��Լ�
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("=======================================================\n");
	printf("                                                     \n");
	printf("		   ������ �Ｍ�� �Ǵ±�                 \n");
	printf("                      %d-%d-%0d                     \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                     \n");
	printf("                                                     \n");
	printf("                  1. �����                        \n");
	printf("                                                     \n");
	printf("               2. �Ϻ� � �׷���                   \n");
	printf("                                                     \n");
	printf("                   3. ����                           \n");
	printf("                                                     \n");
	printf("                                                     \n");
	printf("=======================================================\n");
}

void menu_part() //� ���� �Է�
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("=======================================================\n");
	printf("                                                     \n");
	printf("                   � ���� �Է�                    \n");
	printf("                      %d-%d-%d                       \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                     \n");
	printf("                                                     \n");
	printf("                     1. ����                         \n");
	printf("                                                     \n");
	printf("                     2. ���                         \n");
	printf("                                                     \n");
	printf("                     3. ��                           \n");
	printf("                                                     \n");
	printf("                     4. ��ü                         \n");
	printf("                                                     \n");
	printf("                     0. �ڷΰ���                     \n");
	printf("                                                     \n");
	printf("=======================================================\n");

}

int menu_entry(int menu_part_num) //� ���� �Է� �Լ�
{
	system("cls");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int i;

	char CHEST[7][40] = { "��ġ ������","��Ŭ���� ��ġ", "����", "���� ������", "�嵦 �ö���", "Ǫ����","ü��Ʈ ������" };
	char shoulder[6][40] = { "���� ��� ������", "�ӽ� ��� ������", "������ ���ͷ� ������", "����Ʈ ������", "�и��͸� ������", "��Ʈ ���� ���ͷ� ������" };
	char BACK[6][40] = { "��Ǯ�ٿ�", "��Ƽ�� �ο�", "Ǯ��", "�ٺ� �ο�", "���̺� Ǯ����" ,"���� ���� �ο�" };
	char LOW[6][40] = { "����Ʈ", "���� �ͽ��ټ�", "���� ��", "���� ������", "����", "ī��������" };

	printf("%d-%d-%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                   � ���� ����                      \n");
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

void menu_set() //��Ʈ �� ���� �Լ�
{
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                � ��Ʈ �� ����                      \n");
	printf("                                                       \n");
	printf("               1. ��Ʈ ���� �Է� �ϼ���.               \n");
	printf("             (��, ���ڷθ� �Է��ϼ���. ex.5)           \n");
	printf("                                                       \n");
	printf("                                                       \n");
	printf("                  0. �ڷΰ���                          \n");
	printf("                                                       \n");
	printf("=======================================================\n");
}

void menu_break_time() //���� �ð� �Լ�
{
	printf("=======================================================\n");
	printf("                                                       \n");
	printf("                � ���� �ð� ����                    \n");
	printf("                                                       \n");
	printf("             1.1���� 59�ʱ����� �ð� ��                \n");
	printf("               ���� �ð��� �Է��ϼ���.                 \n");
	printf("                                                       \n");
	printf("          (��, ���ڷθ� �Է��ϼ���. (ex. 40)           \n");
	printf("                                                       \n");
	printf("                  0. �ڷΰ���                          \n");
	printf("                                                       \n");
	printf("=======================================================\n");
}

int check_menu(int menu_part_num, int* menu_entry_num, int* set, int entry_count, int breaktime) //���� Ȯ�� �Լ�
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i;

	printf("=======================================================\n");
	printf("                    ���� Ȯ�� â                    \n");
	printf("                      %d-%d-%d                        \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                       \n");
	switch (menu_part_num)
	{
	case 1: printf("                       ����                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. ��ġ ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. ��Ŭ���� ��ġ : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. ���� : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. ���� �ö��� : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. �嵦 �ö��� : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. Ǫ���� : %d set\n", i + 1, set[i]); break;
			case 6: printf("                 %d. ü��Ʈ ������ : %d set\n", i + 1, set[i]); break;
			}

		}
		break;
	case 2: printf("                       ���                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. ���� ��� ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. �ӽ� ��� ������ : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. ���̵� ���ͷ� ������ : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. ����Ʈ ������ : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. �и��͸� ������ : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. ��Ʈ ���� ���ͷ� ������ : %d set\n", i + 1, set[i]); break;

			}
		}
		break;
	case 3: printf("                       ��                                 \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. ��Ǯ�ٿ� : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. ��Ƽ�� �ο� : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. Ǯ�� : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. �ٺ� �ο� : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. ���̺� Ǯ ���� : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. ���� ���� �ο� : %d set\n", i + 1, set[i]); break;
			}
		}
		break;
	case 4: printf("                       ��ü                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("                 %d. ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("                 %d. ���� �ͽ��ټ� : %d set\n", i + 1, set[i]); break;
			case 2: printf("                 %d. ���� �� : %d set\n", i + 1, set[i]); break;
			case 3: printf("                 %d. ���� ������ : %d set\n", i + 1, set[i]); break;
			case 4: printf("                 %d. ���� : %d set\n", i + 1, set[i]); break;
			case 5: printf("                 %d. ī�������� : %d set\n", i + 1, set[i]); break;
			}
		}
		break;
	}
	printf("                                                       \n");
	printf("		���� �ð� : %d sec      \n", breaktime);
	printf("                                                       \n");
	printf("=======================================================\n");
	printf("         ������ ������ �Ʒ��� �޴��� �����ϼ���\n");
	printf("                                                       \n");
	printf("              1.Ÿ�̸� ����   0. �ڷΰ���              \n");
	printf("=======================================================\n");

	return 0;
}

int breatime_timer(int menu_part_num, int breaktime, int* set, int entry_count, int* menu_entry_num) //Ÿ�̸� �Լ�
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
		case 1: printf("                       ����                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ��ġ ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. ��Ŭ���� ��ġ : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. ���� : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. ���� �ö��� : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. �嵦 �ö��� : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. Ǫ���� : %d set\n", i + 1, set[i]); break;
			case 6: printf("               %d. ü��Ʈ ������ : %d set\n", i + 1, set[i]); break;
			}

			break;
		case 2: printf("                       ���                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ���� ��� ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. �ӽ� ��� ������ : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. ���̵� ���ͷ� ������ : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. ����Ʈ ������ : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. �и��͸� ������ : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. ��Ʈ ���� ���ͷ� ������ : %d set\n", i + 1, set[i]); break;

			}

			break;
		case 3: printf("                       ��                                 \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ��Ǯ�ٿ� : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. ��Ƽ�� �ο� : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. Ǯ�� : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. �ٺ� �ο� : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. ���̺� Ǯ ���� : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. ���� ���� �ο� : %d set\n", i + 1, set[i]); break;

			}
			break;
		case 4: printf("                       ��ü                               \n");
			printf("                                                          \n");
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ������ : %d set\n", i + 1, set[i]); break;
			case 1: printf("               %d. ���� �ͽ��ټ� : %d set\n", i + 1, set[i]); break;
			case 2: printf("               %d. ���� �� : %d set\n", i + 1, set[i]); break;
			case 3: printf("               %d. ���� ������ : %d set\n", i + 1, set[i]); break;
			case 4: printf("               %d. ���� : %d set\n", i + 1, set[i]); break;
			case 5: printf("               %d. ī�������� : %d set\n", i + 1, set[i]); break;
			}
			break;
		}
		printf("\n");
		printf("            ===========================                \n");
		printf("                 00      :      %d                     \n", endTime - startTime);
		printf("            ===========================                \n");
		printf("                                                       \n");
		printf("               ��ü ��Ʈ / ���� ��Ʈ                   \n");

		printf("                    %d    /   %d                         \n", set[i], set_count);
		printf("=======================================================\n");
		Sleep(500);
		system("cls");


		if (endTime - startTime == 0)
		{
			if (i == entry_count - 1 && set[i] == set_count) {
				printf("=======================================================\n");
				printf("\n");
				printf("        Ÿ�̸� ����!! ���  ����ϼ̽��ϴ�!!!!!!!\n");
				printf("\n");
				printf("=======================================================\n");
				return 0;
			}

			if (set[i] == set_count)
			{
				printf("=======================================================\n");
				printf("\n");
				printf("        Ÿ�̸� ����!! ���� **�**�� �����ϼ���!!!!!!!\n");
				printf("\n");
				printf("             ����Ͻ÷��� �޴��� �����ּ���.\n");
				printf("\n");
				printf("           ���� � ���� --> 1\n");
				printf("           Ÿ�̸� ���� --> 0\n");
				printf("\n");
				printf("=======================================================\n");
				i++;
				set_count = 1;
			}
			else {
				printf("=======================================================\n");
				printf("\n");
				printf("        Ÿ�̸� ����!! ���� ��Ʈ�� �����ϼ���!!!!!!!\n");
				printf("\n");
				printf("             ����Ͻ÷��� �޴��� �����ּ���.\n");
				printf("\n");
				printf("           ���� � ���� --> 1\n");
				printf("           Ÿ�̸� ���� --> 0\n");
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
	printf("                %d-%d-%d  ������ �                      \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	printf("                                                       \n");
	switch (menu_part_num)
	{
	case 1: printf("                       ����                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ��ġ ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. ��Ŭ���� ��ġ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. ���� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. ���� �ö��� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. �嵦 �ö��� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. Ǫ���� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 6: printf("               %d. ü��Ʈ ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}

		}
		break;
	case 2: printf("                       ���                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ���� ��� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");

				}
				printf("\n");
				break;
			case 1: printf("               %d. �ӽ� ��� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. ���̵� ���ͷ� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. ����Ʈ ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. �и��͸� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. ��Ʈ ���� ���ͷ� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;

			}
		}
		break;
	case 3: printf("                       ��                                 \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ��Ǯ�ٿ� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. ��Ƽ�� �ο� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. Ǯ�� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. �ٺ� �ο� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. ���̺� Ǯ ���� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. ���� ���� �ο� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}
		}
		break;
	case 4: printf("                       ��ü                               \n");
		printf("                                                          \n");
		for (i = 0; i < entry_count; i++) {
			switch (menu_entry_num[i]) {
			case 0: printf("               %d. ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 1: printf("               %d. ���� �ͽ��ټ� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 2: printf("               %d. ���� �� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 3: printf("               %d. ���� ������ : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 4: printf("               %d. ���� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			case 5: printf("               %d. ī�������� : ", i + 1);
				for (j = 0; j < set[i]; j++) {
					printf("*  ");
				}
				printf("\n");
				break;
			}
		}
		break;
	}
	printf("\n               ����ϼ̽��ϴ�!!!!\n");
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

		//��¥�ް�
		sscanf(str, "%s", date);
		//����� ����
		char* next = str;
		next = getNextDataPtr(str, next);
		if (next == NULL) continue;

		sscanf(next, "%d", &menu_part);

		//�����, ��Ʈ�ľ�
		int menu_entryCnt = 0;
		char* temp = next;
		while (temp != NULL) {
			temp = getNextDataPtr(str, temp);
			menu_entryCnt++;
		}
		menu_entryCnt /= 2;


		Health data = { 0 }; //�ʱ�ȭ

		//����ü�� ������ ����
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
			printf("%10s %-12s\n", "���� : ", "����");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("��ġ ������ : "); break;
				case 1: printf("��Ŭ���� ��ġ : "); break;
				case 2: printf("���� : "); break;
				case 3: printf("���� ������ : "); break;
				case 4: printf("�嵦 �ö��� : "); break;
				case 5: printf("Ǫ���� : "); break;
				case 6: printf("ü��Ʈ ������ : "); break;
				}

				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}

			printf("\n\n");
		}
		else if (menu_part == 2) {
			printf("%10s %-12s\n\n", "���� : ", "���");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("���� ��� ������ : "); break;
				case 1: printf("�ӽ� ��� ������ : "); break;
				case 2: printf("������ ���ͷ� ������ : "); break;
				case 3: printf("����Ʈ ������"); break;
				case 4: printf("�и��͸� ������"); break;
				case 5: printf("��Ʈ ���� ���ͷ� ������"); break;
				}
				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}
			printf("\n\n");
		}
		else if (menu_part == 3) {
			printf("%10s %-12s\n\n", "���� : ", "��");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("��Ǯ�ٿ� : "); continue;
				case 1: printf("��Ƽ�� �ο� : "); continue;
				case 2: printf("Ǯ�� : "); continue;
				case 3: printf("�ٺ� �ο� : "); continue;
				case 4: printf("���̺� Ǯ���� : "); continue;
				case 5: printf("���� ���� �ο� : "); continue;
				}

				for (int j = 0; j < data.set[i]; j++) {
					printf(" *");
				}
				printf("\n");
			}

			printf("\n\n");
		}
		else {
			printf("%10s %-12s\n\n", "���� : ", "��ü");
			printf("=========================================\n");
			for (int i = 0; i < menu_entryCnt; i++) {
				switch (data.menu_entry_num[i]) {
				case 0: printf("����Ʈ : "); break;
				case 1: printf("���� �ͽ��ټ� : "); break;
				case 2: printf("���� �� : "); break;
				case 3: printf("���� ������ : "); break;
				case 4: printf("���� : "); break;
				case 5: printf("ī�������� : "); break;
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
	//originStr�� �ι��� Ȥ�� ����Ű �����ϱ� �������� ���� ��
	while (originStr[len] != 0 && originStr[len] != '\r' && originStr[len] != '\n') { len++; }

	//nowPosStr�� originStr �� ������ ����Ű�� NULL���� 
	if (nowPosStr < originStr || nowPosStr - originStr >= len) return NULL;

	int pos = 0;
	while (nowPosStr[pos] != ' ' && nowPosStr[pos] != '\r' && nowPosStr[pos] != '\n' && nowPosStr[pos] != 0) {
		pos++;
	}

	if (nowPosStr - originStr + pos >= len) return NULL;
	return nowPosStr + pos + 1; // +1 �� ������ ���������� ��ġ�� ����ų�ſ���  ������ ���� ������ ���� ������ ����  ���·� �޸��忡 �����������  ���� ���� ������ �ּ� ����Ű���� +1��.    
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