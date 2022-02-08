#include <stdio.h>

int main(int argc, char **argv) {
	char *str;
	if(argc == 1) {
		char s[256];
		scanf("%s", s);
		str = s;
	} else if(argc == 2) {
		str = argv[1];
	}

	int i, j;
	i = 0;
	while(str[i] != '\0') {
		for(j = 0; j < str[i]; j++) {
			printf("+");
		}
		printf(">");
		i++;
	}

	for(j = 0; j < i; j++) {
		printf("<");
	}
	for(j = 0; j < i; j++) {
		printf(".>");
	}
	
	return 0;
}

