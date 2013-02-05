/* Ovaj program modifikacija je programa koji sam napravio u kolovozu 2012. 
 * u okviru završnog rada na temu kriptovirologija.
 * Svrha programa bila je demonstracija rada XTEA algoritma.
 * Sam algoritam za sifriranje i desifriranje (funkcije encipher i decipher)
 * bile si preuzete sa: http://en.wikipedia.org/wiki/XTEA
 *
 * Datoteka se kriptira na način da se prilikom pokretanja programa iz komandne
 * linije kao argument navede naziv/putanja datoteke koju se želi kriptirati.
 * (Datoteka se može kriptirati i na način da se drag & drop-a na .EXE file
 * ovog programa)
 *
 * Autor: Josip Žemberi 
 * Fakultet Organizacije i Informatike, siječanj 2013.
 */
#include <iostream>
#include <fstream>
#include <stdint.h>

using namespace std;

unsigned int kljuc[4]={0xACB5,0x1344,0xEC90,0x285C};              
char red[1000];
char txt[10000];

#define BLOCK_SIZE 8

void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const kljuc[4]){
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++){
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + kljuc[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + kljuc[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

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

void crypto(char nazivDat[] ,bool naredba){
      fstream dat(nazivDat,ios::in | ios::out | ios::binary);
      if(!dat) cout << "Datoteku nije bilo moguce otvoriti" << endl;

      unsigned size;
    
      dat.seekg(0,ios::end);
      size=dat.tellg();
      dat.seekg(ios::beg);
    
      dat.clear();
    
      unsigned pos;
    
      int n_blocks=size/BLOCK_SIZE;
      if(size%BLOCK_SIZE!=0)
          ++n_blocks;
    
      for(int i=0;i<n_blocks;i++){
          unsigned char data[BLOCK_SIZE];
          pos=dat.tellg();
      
          dat.read((char*)data,BLOCK_SIZE);
      
          if(naredba) encipher(32,(uint32_t*)data,kljuc);
          else decipher(32,(uint32_t*)data,kljuc);
        
          dat.seekp(pos);
          dat.write((char*)data,BLOCK_SIZE);

          memset(data,0,BLOCK_SIZE);
      }
      dat.close();
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Koristenje: %s naziv_datoteke_za_enkripciju\n", argv[0]);
        return 1;
    }
    crypto(argv[1], true);
    //crypto(argv[1], false); //dekripcija
    cout << "Datoteka " << argv[1] << " je kriptirana!!" << endl;
   system("pause");               
   return 0;
}
