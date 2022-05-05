#include<stdio.h>
int main(int argc,char *argv[])
{
	int i = 0;
	for(i = 0; i < argc; i++){
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	
	if(argv[1][1] == 'a'){
		int area = atoi(argv[3]) * atoi(argv[5]);
		printf("Area = %d units", area);	
	}else{
		int perimeter = atoi(argv[3]) * 2 + atoi(argv[5]) * 2;
		printf("Perimeter = %d units", perimeter);
	}
	return 0;
}
