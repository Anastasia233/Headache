/*
 ============================================================================
 Name        : calculator_6.c
 Author      : Anastasia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

//очередь для хранения всех чисел и символов ( напр.: 3 4 + 3 * )
typedef struct queue {
	int size;
	char *element;
	struct queue *next; // ссылка на следующий элемент (ячейку)
} queue;

//cтек для хранения всех результатов
typedef struct stack {
	float result;
	struct stack *next;
} stack;

//cписок для хранения каждой цифры числа
typedef struct number {
	char value;
	struct number *next;
} number;

//ф-я добавления последнего элемента в очередь
queue* put_element(FILE* file, char c) { //klast nazad
	number* head_element = (number*) malloc(sizeof(number));
	number* last_element = (number*) malloc(sizeof(number));
	last_element = head_element;
	queue* element_n = (queue*) malloc(sizeof(queue));
	int size = 0;
	while (c != ' ') {
		size++;
		if (size == 1) {
			//создаём начало списка с числами или операциями
			head_element->value = c;
			head_element->next = NULL;
			last_element = head_element;
		} else {
			//иначе это число и продолжаем заполнять список цифрами числа
			element_n->next = NULL;
			number* element_n_number = (number*) malloc(sizeof(number));
			element_n_number->value = c;
			element_n_number->next = NULL;
			last_element->next = element_n_number;
			last_element = last_element->next;
		}
		//если конец файла, то выходим из цикла
		if (feof(file) != 0) {
			break;
		} else {
			//а если не конец, значит не выходим и читаем символы дальше
			if (c != '$')
				fscanf(file, "%c", &c);
			else
				break;
		}
	}
	//возвращаемся в начало списка с операциями или операндами
	last_element = head_element;
	element_n->size = size;
	element_n->element = malloc(size * sizeof(char));
	//заполняем список с массивами числом или операцией
	for (int i = 0; i < size; i++) {
		element_n->element[i] = last_element->value;
		if (last_element->next)
			last_element = last_element->next;
	}
	return element_n;
}
stack* add_elements(char *number, stack* head_stack) {
	stack* element_n = (stack*) malloc(sizeof(stack));
	element_n->next = head_stack;
	element_n->result = atof(number);
	return element_n;
}
//функция, которая выполняет операции
stack* disaster(char oper, stack* element) {
	switch (oper) {
	case '+':
		element->next->result = element->next->result + element->result;
		break;

	case '-':
		element->next->result = element->next->result - element->result;
		break;

	case '*':
		element->next->result = element->next->result * element->result;
		break;

	case '/':
		element->next->result = element->next->result / element->result;
		break;
	}
	free(element);
	return element->next;
}
int main(void) {
	FILE *file, *writefile;
	file = fopen("file.txt", "r");
	char c;
	queue* head_queue = (queue*) malloc(sizeof(queue));
	fscanf(file, "%c", &c);
	head_queue = put_element(file, c); //создаем 1й элемент  очереди
	queue* current = (queue*) malloc(sizeof(queue));
	current = head_queue;
	while (!feof(file)) {
		fscanf(file, "%c", &c);
		current->next = put_element(file, c);
		current = current->next;
	}
	fclose(file);
	current = head_queue;
	writefile = fopen("result.txt", "w");
	stack* head_stack = (stack*) malloc(sizeof(stack*));
	head_stack = add_elements(current->element, head_stack);
	current = current->next;

	while (current->next) {
		if (current->element[0] != '$') {
			if (current->element[0] == '+'
					|| (current->element[0] == '-' && current->size == 1)
					|| current->element[0] == '*'
					|| current->element[0] == '/') {
				head_stack = disaster(current->element[0], head_stack);
			} else
				head_stack = add_elements(current->element, head_stack);
			current = current->next;
		} else {
			fprintf(writefile, "%.0f\n", head_stack->result);
			head_stack = head_stack->next;
			current = current->next;
		}
	}
	fclose(writefile);
	return EXIT_SUCCESS;
}
