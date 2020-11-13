#define _CRT_SECURE_NO_WARNINGS
#define FILE_NAME "file.txt"
#define STANDART_SIZE 10
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <sys/types.h>
#include "unistd.h"
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
	size_t* array;
	size_t capacity_;
	size_t size_;
} Struct_array;

Struct_array* init_Struct_array(size_t init_size)
{
	Struct_array* new_s_array = (Struct_array*)malloc(sizeof(Struct_array));
	if (new_s_array == NULL) {
		return new_s_array;
	}
	new_s_array->array = (size_t*)malloc(init_size * sizeof(size_t));
	new_s_array->size_ = 0;
	new_s_array->capacity_ = init_size;
	return new_s_array;
}

void free_Struct_array(Struct_array* s_array)
{
	free(s_array->array);
	s_array->array = NULL;
	s_array->size_ = 0;
	s_array->capacity_ = 0;
}

void add(Struct_array* s_array, size_t element)
{
	if (s_array->size_ == s_array->capacity_)
	{
		s_array->capacity_ *= 2;
		size_t* data = (size_t*)realloc(s_array->array, s_array->capacity_ * sizeof(size_t));
		if (data == NULL) {
			return;
		}
		s_array->array = data;
	}
	s_array->array[s_array->size_++] = element;
}

void read_file_and_add_arrays(int file, Struct_array* s_array)
{
	size_t position = 0;
	char sym;
	int res = read(file, &sym, 1);
	if(res!=0) {
		printf("%c", sym);
	}
	while (res != 0) {
		if (sym == '\n') {
			add(s_array, position + 1);
		}
		++position;
		res = read(file, &sym, 1);
		printf("%c", sym);
	}
}
void find_string(int file, Struct_array* s_array)
{
	bool exit_flag = false;
	while (!exit_flag) {
		printf("Enter number of string which you need. Otherwise enter -1 for exit: ");
		int string_number = 0;
		scanf("%d", &string_number);
		if (string_number == -1) {
			exit_flag = true;
			continue;
		}
		if (string_number < 0 || string_number >= s_array->size_) {
			printf("String number is out of range\n");
			continue;
		}
		size_t start_pos;
		size_t end_pos;
		if (string_number == 0) {
			start_pos = 0;
			end_pos = s_array->array[0];
		}
		else {
			start_pos = s_array->array[string_number - 1];
			end_pos = s_array->array[string_number];
		}
		for (int i = start_pos; i < end_pos; ++i) {
			lseek(file, i, SEEK_SET);
			char c;
			read(file, &c, 1);
			printf("%c", c);
		}
	}
}

int main()
{
	int file = open(FILE_NAME, O_RDONLY);
	if (file == -1) {
		printf("Programm can't open/read file %s\n", FILE_NAME);
		return 1;
	}
	Struct_array* my_array = init_Struct_array(STANDART_SIZE);
	if (my_array == NULL) {
		printf("Programm can't allocate memory for array\n");
		return 1;
	}
	read_file_and_add_arrays(file, my_array);
	find_string(file, my_array);
	free_Struct_array(my_array);
	close(file);
	return 0;
}

