
#include<stdio.h>
int main(int argc,char *argv[])
{
	FILE *f = fopen("Datalog.txt", "w");
	if (f == NULL){
		fprintf(stderr, "Error opening file!\n");
		exit(1);
	}
	const char *text = "Write this to the file";
	char name[100];
	char id[10];
	char add[1];
	int count = 0;

	while(count < atoi(argv[2])){
		count += 1;
		printf("Enter Name: \n");
		scanf("%s",name);
		printf("Enter ID: \n");
		scanf("%s",id);
		printf("Do you wish to add mote [Y/n] \n");
		scanf("%s", add);
		fprintf(f, "Name: %s\n ID: %s\n\n", &name,&id);
		if (add[0] == 'n') break;
	}
	fclose(f);
	return 0;
}
