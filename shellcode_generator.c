/* Izvorni autor ovoga koda je Mark Russanovich. (Tako je navedeno u README 
 * datoteci koja se nalazila uz sam kod).
 * Preuzeti kod je malo modificiran, no logika i funkcionalnost su ostale iste.
 *
 * Zaglavlje shellcode.h se generira na temelju datoteke čija je putanja
 * argument komandne linije. U datoteku shellcode.h sprema se proslijeđena 
 * datoteka na način da se bajtovi te datoteke u hex obliku dodaju polju 
 * shellcode koje je tipa unsigned char.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    FILE *f; 
    int i, c;
    char *arr_name;
  
    if (argc < 2) {
        fprintf(stderr, "Koristenje: %s naziv_kriptirane_datoteke\n", argv[0]);
        return 1;
    }
	
    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "%s: fopen(%s) nije uspio", argv[0], argv[1]);
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
