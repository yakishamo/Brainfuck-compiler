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
		fprintf(stderr, "%s: Cannot open %s\n", argv[0], argv[1]);
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
	while(cmd[j] != '\0')
	{
		switch(cmd[j]){
		case '>':
			printf(">");
			break;
		case '<':
			printf("<");
			break;
		case '+':
			printf("+");
			break;
		case '-':
			printf("-");
			break;
		case '.':
			printf(".");
			break;
		case ',':
			printf(",");
			break;
		case '[':
			printf("[");
			break;
		case ']':
			printf("]");
			break;
		default :
			break;
		}
		j++;
	}



	free(cmd);
	return 0;
}
