#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define _CRT_SECURE_NO_WARNINGS
#define SIZE 50 

struct task{
	char name[SIZE];
	int date;
	struct task* next;
};
typedef struct task* task_ptr;

// task_ptr* head_ptr -> 이중 포인터 : head 값이 바껴야 하므로 Call by Reference
void read_file(task_ptr* head_ptr);
int menu();
int count_tasks(const task_ptr head);
void print_all(task_ptr head);
void print_task(task_ptr head);
void add_task(task_ptr* head_ptr); 
void delete_task(task_ptr* head_ptr);
void delete_all(task_ptr* head_ptr);
void write_file(task_ptr head);
void search_task(task_ptr head);

int main(){
	task_ptr head = NULL;    
	read_file(&head);

	while (1){
		printf("\n");

		int i = menu();

		switch (i){
		case 1:
			print_all(head);
			break;
		case 2:
			print_task(head);
			break;
		case 3:
			add_task(&head);
			break;
		case 4:
			delete_task(&head);
			break;
		case 5:
			delete_all(&head);
			break;
		case 6:
			write_file(head);
			break;
		case 7:
			search_task(head);
			break;
		case 8:
			printf("Good Bye\n");
			write_file(head);
			exit(0);
		default:
			printf("%d is not included. \n", i);
		}
	}
}

void read_file(task_ptr* head_ptr){
	char filename[SIZE] = { 0, };

	printf("Please input filename to read.\n");

	if (scanf("%[^\n]", filename) != 1){
		printf("Wrong input.\n");
		exit(1);
	}

	FILE* fp = fopen(filename, "r");

	if (fp == NULL){
		printf("Error: Cannot open file. \n");
		exit(1);
	}

	int num;
	if (fscanf(fp, "%d%*c", &num) != 1){
		printf("Error: Wrong file format. \n");
		exit(1);
	}

	task_ptr prev = *head_ptr;

	for (int n = 0; n < num; ++n){
		task_ptr new_task = (task_ptr)malloc(sizeof(struct task));
		if (new_task == NULL){
			printf("Malloc failed.\n");
			exit(1);
		}

		new_task->next = NULL;

		if (fscanf(fp, "%[^\n]%*c", &new_task->name) != 1 ||
			fscanf(fp, "%d%*c", &new_task->date) != 1){
			printf("Error: Wrong content format.\n");
			exit(1);
		}

		if (prev == NULL){
			*head_ptr = new_task;     // head가 new_task
			prev = new_task;        // prev가 new_task
		}
		else{
			prev->next = new_task;   // prev 다음인 next가 new
			prev = new_task;		// prev가 new
		}
	}

	fclose(fp);

	printf("%d tasks.\n", num);
}

int input_int(){
	int input;

	while (1){
		printf(">> ");
		if (scanf("%d%*c", &input) == 1) return input;
		else{
			printf("Try again.\n");
			while (getchar() != '\n') continue;
		}
	}
}

int menu(){
	while (1){
		printf("Please select an option.\n");
		printf("1. Print all tasks\n");
		printf("2. Print an task\n");
		printf("3. Add an task\n");
		printf("4. Delete an task\n");
		printf("5. Delete all tasks\n");
		printf("6. Save file\n");
		printf("7. Search by name\n");
		printf("8. Quit\n");

		int input = input_int();

		if (input >= 0 && input <= 10) return input;
		else printf("Try again.\n");
	}
}

int count_tasks(const task_ptr head){
	int count = 0;
	task_ptr node = head;

	while (node != NULL){
		count += 1;
		node = node->next;
	}

	return count;
}

void print_all(task_ptr head){
	task_ptr node = head;

	int count = 0;
	while (node != NULL){
		printf("%d : %s %d\n", count, node->name, node->date);
		node = node->next;
		count++;
	}
}

void print_task(task_ptr head){
	printf("Input the index of task to print.\n");
	int index = input_int();

	task_ptr node = head;

	int count = 0;

	while (node != NULL){
		if (count == index) break;

		node = node->next;
		count++;
	}

	if (node != NULL)
		printf("%d : %s %d\n", count, node->name, node->date);
	else
		printf("Invalid task.\n");
}

void add_task(task_ptr* head_ptr) {
	printf("Input name.\n");
	printf(">> ");

	task_ptr new_task = (task_ptr)malloc(sizeof(struct task));
	if (new_task == NULL){
		printf("Malloc failed\n");
		exit(1);
	}

	new_task->next = NULL;

	int i = scanf("%[^\n]%*c", &new_task->name);
	printf("Input date.\n");
	printf(">> ");
	i = scanf("%d%*c", &new_task->date);

	int count = 0;
	task_ptr node = *head_ptr;

	if (node == NULL)
		*head_ptr = new_task;
	else{
		while (node->next != NULL){
			node = node->next;
			count++;
		}
		node->next = new_task;
		count++;
	}

	printf("%d : %s %d\n", count, new_task->name, new_task->date);
}

void delete_task(task_ptr* head_ptr){
	printf("Input the index of task to delete.\n");
	int index = input_int();

	task_ptr node = *head_ptr;
	task_ptr prev = NULL;

	int count = 0;
	while (node != NULL){
		if (count == index) break;

		prev = node;
		node = node->next;
		count++;
	}

	if (node == NULL){
		printf("Wrong index\n");
		return;
	}

	if (prev == NULL)
		*head_ptr = node->next;
	else
		prev->next = node->next;
	free(node);
}

void delete_all(task_ptr* head_ptr){
	if (*head_ptr == NULL){
		printf("Nothing to delete.\n");
		return;
	}

	task_ptr search = *head_ptr;
	task_ptr tmp = NULL;

	int count = 0;
	while (search != NULL){
		printf("%s is deleted.\n", search->name);
		tmp = search->next;
		free(search);
		search = tmp;
		count++;
	}

	*head_ptr = NULL;
	printf("%d tasks deleted.\n", count);
}

void write_file(task_ptr head){
	char filename[SIZE] = { 0, };

	printf("Please input filename to read and press Enter.\n");
	printf(">> ");

	if (scanf("%[^\n]%*c", filename) != 1){
		printf("Wrong input.\n");
		exit(1);
	}

	FILE* file = fopen(filename, "w");

	if (file == NULL){
		printf("Error: Cannot open file. \n");
		exit(1);
	}

	int count = 0;

	fprintf(file, "%d\n", (int)count_tasks(head));

	task_ptr node = head;

	while (node != NULL){
		fprintf(file, "%s\n", node->name);
		fprintf(file, "%d\n", node->date);

		node = node->next;

		count++;
	}

	fclose(file);

	assert(count == (int)count_tasks(head));

	printf("%d tasks have been saved to the file.\n", count);
}

void search_task(task_ptr head){
	printf("Please input name to search.\n");
	printf(">> ");

	char name[SIZE] = { 0, };
	if (scanf("%[^\n]%*c", name) != 1){
		printf("Wrong input.\n");
		return;
	}

	task_ptr node = head;

	int count = 0;
	while (node != NULL){
		if (strcmp(node->name, name) == 0) break;
		node = node->next;
		count++;
	}

	if (node == NULL){
		printf("No task found : %s\n", name);
		return;
	}

	printf("%d : %s %d\n", count, node->name, node->date);
}