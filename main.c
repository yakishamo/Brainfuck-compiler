#include <stdio.h>
#include <stdlib.h>

//reallocして失敗したら終了する関数.
void *my_realloc(char *ptr, int size){
	ptr = realloc(ptr, size);
	if(ptr == NULL){
		fprintf(stderr, "realloc error.\n");
		exit(1);
	}
	return ptr;
}

int main(int argc, char **argv) {
	
	if(argc == 1){
		fprintf(stderr, "%s: No file specified.\n", argv[0]);
		exit(1);
	}

	FILE *fp;
	if((fp = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s: Cannot open \"%s\"\n", argv[0], argv[1]);
		exit(1);
	}

	char *cmd = (char*)malloc(sizeof(char)*1000);
	int cmd_len = 1000;
	int i = 0;
	char chr;
	
	//一文字ずつファイルの内容を配列に読み込む.
	while((chr = fgetc(fp)) != EOF) {
		cmd[i++] = chr;
		if(i % 1000 == 0) {		//領域が足りなくなったら拡張.
			cmd = (char*)my_realloc(cmd, sizeof(char)*1000 + cmd_len);
			cmd_len += 1000;
		}
	}

	if(i%1000 == 0) {
		cmd = (char*)my_realloc(cmd, sizeof(char) + cmd_len);
		cmd_len++;
	}
	cmd[++i] = '\0';

	int j = 0;
	int k = 0;
	int sp = 0;
	int loop_num[10000];
	int loop_num_size;
	int loop_counter = 0;
	loop_num[0] = 0;
	printf("global _start\n");
	printf("_start:\n");
	printf("\tpush rbp\n");
	printf("\tmov rbp, rsp\n");
	printf("\tsub rsp, 30000\n");

	while(cmd[j] != '\0')
	{
		switch(cmd[j]){
		case '>':
			sp--;
			break;
		case '<':
			sp++;
			break;
		case '+':
			if(sp != 0)	printf("\tadd DWORD [rbp %d], 0x1\n",sp);
			else 		printf("\tadd DWORD [rbp], 0x1\n");
			break;
		case '-':
			if(sp != 0)	printf("\tsub DWORD [rbp %d], 0x1\n",sp);
			else 		printf("\tsub DWORD [rbp], 0x1\n");;
			break;
		case '.':
			printf("\tmov rax, 1\n");
			printf("\tmov rdi, 1\n");
			printf("\tmov rdx, 1\n");
			printf("\tmov rsi, rbp\n");
			printf("\tsub rsi, %d\n",-sp);
			printf("\tsyscall\n");
			break;
		case ',':
			printf("\tmov rax, 0\n");
			printf("\tmov rdi, 0\n");
			printf("\tmov rsi, rbp\n");
			printf("\tsub rsi, %d\n", -sp);
			printf("\tmov rdx, 1\n");
			printf("\tsyscall\n");
			break;
		case '[':
			loop_counter++;
			loop_num[loop_counter]++;
			if(sp == 0)	printf("\tcmp DWORD [rbp], 0\n");
			else 		printf("\tcmp DWORD [rbp -%d], 0\n", -sp);
			printf("\tjz loop_%d_%d_end\n",loop_counter, loop_num[loop_counter]);
			printf("loop_%d_%d_start:\n",loop_counter, loop_num[loop_counter]);
			break;
		case ']':
			printf("loop_%d_%d_end:\n",loop_counter, loop_num[loop_counter]);
			if(sp == 0)	printf("\tcmp DWORD [rbp], 0\n");
			else		printf("\tcmp DWORD [rbp -%d], 0\n", -sp);
			printf("\tjnz loop_%d_%d_start\n",loop_counter, loop_num[loop_counter]);
			loop_counter--;
			break;
		default :
			break;
		}
		j++;
	}

	printf("\tmov rax, 0\n");
	printf("\tmov rsp, rbp\n");
	printf("\tpop rbp\n");
	printf("\tmov rax, 60\n");
	printf("\tmov rdi, 0\n");
	printf("\tsyscall\n");
	free(cmd);
	return 0;
}
