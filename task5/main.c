#define _CRT_SECURE_NO_WARNINGS
#define STANDART_SIZE 10
#define FILE_NAME "file.txt"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <sys/types.h>
#include "unistd.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
typedef struct array{
	size_t* array;
	size_t capacity_;
	size_t size_;
} Struct_array;

Struct_array* init_Struct_array(size_t init_size)
{
	Struct_array* new_s_array = (Struct_array*)malloc(sizeof(Struct_array));
	if (new_s_array == NULL) {
		return NULL;
	}
	new_s_array->array = (size_t*)malloc(init_size * sizeof(size_t));
	if(new_s_array->array == NULL) {
		free(new_s_array);
		return NULL;
	}
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
	if (s_array->size_ == s_array->capacity_) {
		s_array->capacity_ *= 2;
		size_t* data = (size_t*)realloc(s_array->array, s_array->capacity_ * sizeof(size_t));
		if (data == NULL) {
			return;
		}
		s_array->array = data;
	}
	s_array->array[s_array->size_++] = element;
}

bool read_file_and_add_arrays(int file_descriptor, Struct_array* s_array)
{
	off_t position = lseek(file_descriptor, 0, SEEK_END);
	if(position == -1) {
		printf("Can't seek in file\n");
		free_Struct_array(s_array);
		close(file_descriptor);
		return false;
	}
	size_t length = (size_t)position + 1;
	lseek(file_descriptor, 0, SEEK_SET);
	char* buffer = (char*)malloc(length);
	if(buffer == NULL) {
		printf("Bad memory allocation for buffer\n");
		free_Struct_array(s_array);
		close(file_descriptor);
		return false;
	}
	read(file_descriptor, buffer, length);
	for(int i = 0;i < length; ++i) {
		if(buffer[i] == '\n') {
			add(s_array, i+1);
		}
	}
	free(buffer);
	return true;
}

void find_string(int file_descriptor, Struct_array* s_array)
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
			lseek(file_descriptor, i, SEEK_SET);
			char c;
			read(file_descriptor, &c, 1);
			printf("%c", c);
		}
	}
}

int main()
{
	int file_descriptor = open(FILE_NAME, O_RDONLY);
	if (file_descriptor == -1) {
		printf("Programm can't open/read file %s\n", FILE_NAME);
		perror("open():");
		return -1;
	}
	Struct_array* my_array = init_Struct_array(STANDART_SIZE);
	if (my_array == NULL) {
		printf("Programm can't allocate memory for array\n");
		close(file_descriptor);
		return 1;
	}
	if(read_file_and_add_arrays(file_descriptor, my_array)) {
		find_string(file_descriptor, my_array);
		free_Struct_array(my_array);
		close(file_descriptor);
		return 0;
	}
	return 2;
}

