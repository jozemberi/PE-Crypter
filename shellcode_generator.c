
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    FILE *f;
    int i, c;
    char *arr_name;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [filepath]\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "%s: fopen(%s) Failed. Could'nt open file.", argv[0], argv[1]);
        return 1;
    }


	freopen ("shellcode.h","w",stdout);

    arr_name = "shellcode";

    printf("unsigned char %s[] = {", arr_name);

    for (i=0;;i++) {
        if ((c = fgetc(f)) == EOF) break;
        if (i != 0) printf(",");
        if ((i % 12) == 0) printf("\n\t");
        printf("0x%.2X", (unsigned char)c);
    }

    printf("\n\t};\n");

	printf("unsigned int size = %i;\n", i);

    fclose(f);
	fclose(stdout);

    return 0;
}
