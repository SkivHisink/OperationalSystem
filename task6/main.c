#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stropts.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define FILE_NAME "file.txt"
#define STANDART_SIZE 10

typedef struct {
	size_t* array;
	size_t size_;
	size_t capacity_;
} Struct_array;

Struct_array* init_Struct_array(size_t initialSize)
{
	Struct_array* new_s_array = (Struct_array*)malloc(sizeof(Struct_array));
	if (new_s_array == NULL) {
		return new_s_array;
	}
	new_s_array->array = (size_t*)malloc(initialSize * sizeof(size_t));
	if (new_s_array->array == NULL) {
		free(new_s_array);
		return NULL;
	}
	new_s_array->size_ = 0;
	new_s_array->capacity_ = initialSize;
	return new_s_array;
}

void add(Struct_array* s_array, size_t element)
{
	if (s_array->size_ == s_array->capacity_)
	{
		s_array->capacity_ *= 2;
		size_t* data = (size_t*)realloc(s_array->array, s_array->capacity_ * sizeof(size_t));
		if (data == NULL)
			return;
		s_array->array = data;
	}
	s_array->array[s_array->size_++] = element;
}

void free_Struct_array(Struct_array* s_array) {
	free(s_array->array);
	s_array->array = NULL;
	s_array->size_ = 0;
	s_array->capacity_ = 0;
}

void print_line(Struct_array* arr, int stringNumber, int file_descriptor)
{
	if (stringNumber < 0 || stringNumber >= arr->size_)
	{
		printf("Out of range\n");
		return;
	}

	size_t start;
	size_t end;
	if (stringNumber == 0)
	{
		start = 0;
		end = arr->array[0];
	}
	else
	{
		start = arr->array[stringNumber - 1];
		end = arr->array[stringNumber];
	}
	lseek(file_descriptor, start, SEEK_SET);
	char str[255];
	int num = 0;
	num = read(file_descriptor, str, end - start);
	if(num == -1 && errno != EINTR) {
		perror("read():");
		return;
	}
	str[end - start - 1] = '\0';
	printf("%s\n", str);
}

bool read_file_and_add_arrays(int file_descriptor, Struct_array* s_array)
{
	off_t position = lseek(file_descriptor, 0, SEEK_END);
	if (position == -1) {
		printf("Can't seek in file\n");
		free_Struct_array(s_array);
		close(file_descriptor);
		return false;
	}
	size_t length = (size_t)position + 1;
	lseek(file_descriptor, 0, SEEK_SET);
	char* buffer = (char*)malloc(length);
	if (buffer == NULL) {
		printf("Bad memory allocation for buffer\n");
		return false;
	}
	int num = 0;
	char additional_buffer[length];
	int indx = 0;
	while((num = read(file_descriptor, additional_buffer, length)) != 0){
		if(num == -1 && errno !=EINTR) {
			perror("read():");
			free(buffer);
			return false;
		}
		memcpy(buffer+indx, additional_buffer, num);
		indx = num;
	}
	for (int i = 0; i < length; ++i) {
		if (buffer[i] == '\n') {
			add(s_array, i + 1);
		}
	}
	free(buffer);
	return true;
}

int time_limited_entering(Struct_array* s_array, int file_descriptor)
{
	printf("You have 5 seconds to input number of string.\n");

	fd_set descriptor_set;//fd_set is used to represent file descriptor set.
	FD_ZERO(&descriptor_set);
	int fd;
	FD_SET(fd, &descriptor_set);

	struct timeval time_value;
	time_value.tv_sec = 5;
	time_value.tv_usec = 0;
	// select() allows a program to monitor multiple file descriptors,
	//waiting until one or more of the file descriptors become "ready" for
	//some class of I / O operation(e.g., input possible).
	int number_of_file_descriptors;
	number_of_file_descriptors = select(fd+1, &descriptor_set, NULL, NULL, &time_value);
	bool exit_flag = false;
	while(!exit_flag){
		if(number_of_file_descriptors == -1 && errno != EINTR)
		{
			return -1;
		}
		if (number_of_file_descriptors == 0) {
			printf("Time is over. You didn't write any string number.\n");
			for(int i=0; i<s_array->size_;++i) {
				print_line(s_array, i, file_descriptor);
			}
			return 0;
		}
		int stringNumber;
		int number_of_arguments = scanf("%d", &stringNumber);
		if (number_of_arguments != 1) {
			int character;
			while ((character = getchar()) != '\n'
				&& character != EOF) {}
			printf("Wrong input\n");
			continue;
			}
			if (stringNumber == -1) {
				exit_flag = true;
			}
			else {
				print_line(s_array, stringNumber, file_descriptor);
			}
	}
	return 0;
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
		return -2;
	}
	if (read_file_and_add_arrays(file_descriptor, my_array)) {
		int inf=time_limited_entering(my_array, file_descriptor);
		if(inf==-1){
			printf("Select() returned -1");
			free_Struct_array(my_array);
			close(file_descriptor);
			return -3;
		}
		free_Struct_array(my_array);
		close(file_descriptor);
		return 0;
	}
	free_Struct_array(my_array);
	close(file_descriptor);
	return -4;
}
