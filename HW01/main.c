#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int plus(int a, int b) { return a + b; }
int minus(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divided(int a, int b) { return a / b; }

int (*fptr_array[])(int, int) = {plus, minus, multiply, divided};

//string array oprand[] is for strcmp() to compare "string"
char *operand[] = {"+", "-", "*", "/"};

int main(int argc, char *argv[])
{
    int i;
    int size = sizeof(fptr_array) / (sizeof(int *)); //sizeof(int *) == 8 Bytes
    
    if(argc == 4){
        for(i = 0; i < size; i++){
	    if(strcmp(argv[2], operand[i]) == 0){
	    //must transfer string (datatype of argv[]) to integer by atoi()
            int num1 = atoi(argv[1]), num2 = atoi(argv[3]);
	    printf("%d %s %d = %d\n", num1, argv[2], num2, fptr_array[i](num1, num2));
	    }
	}
    }	
}
/*
int main()
{
	int a, c;
	char b;
	printf("key a Function \nEX: 1 + 1\n");
	scanf("%i %c %i", &a, &b, &c);
	//IF ELSE IF
	if (b == '+')
		printf("%d %c %d = %d\n", a, b, c, plus(a, c));
	else if (b == '-')
		printf("%d %c %d = %d\n", a, b, c, minus(a, c));
	else if (b == '*')
		printf("%d %c %d = %d\n", a, b, c, multiply(a, c));
	else if (b == '/')
		printf("%d %c %d = %d\n", a, b, c, divided(a, c));
}
*/
