#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Errors type:
//-1 can't allocate memory
#define STRING_MAX_SIZE 255
typedef struct List
{
	char* string;
	struct List* next;
}List;

List* init_List()
{
	List* list = (List*)malloc(sizeof(List));
	if(list)
	{
		list->string = NULL;
		list->next = NULL;
	}
	return list;
}
void dispose(List* list)
{
	List* current_list =list;
	while(current_list)
	{
		if(current_list->string)
		{
			free(current_list->string);
		}
		List* to_delete = current_list;
		current_list = current_list->next;
		free(to_delete);
	}
}
void print_strings(List* list)
{
	List* current_list = list;
	while(current_list)
	{
		printf("%s\n" , current_list->string);
		current_list = current_list->next;
	}
}
void append(List** list, char* string)
{
	if(*list)
	{
		*list = init_List();
		if(!*list)
		{
			perror("List didn't init\n");
			exit(-1);
		}
	}
	else
	{
		List* last_list = *list;
		while(last_list->next)
		{
			last_list=last_list->next;
		}
		List* new_item = init_List();
		new_item->string = string;
		last_list->next = new_item;
		if(!new_item)
		{
			perror("List can't add more string\n");
			print_strings(*list);
			dispose(*list);
			exit(-1);
		}
	}
}
int main()
{
	char string[STRING_MAX_SIZE];
	List* list = NULL;
	
	printf("Enter your strings. After entering data type \".\" :\n");
	while(*(fgets(string, STRING_MAX_SIZE, stdin)) != '.')
	{
		size_t string_lenght = strlen(string);
		char* temp = (char*)malloc(string_lenght);
		if(temp)
		{
			memcpy(temp, string, string_lenght);
			temp[string_lenght-1]='\0';
			append(&list, temp);
		}
	}
	print_strings(list);
	dispose(list);
	return 0;
}
