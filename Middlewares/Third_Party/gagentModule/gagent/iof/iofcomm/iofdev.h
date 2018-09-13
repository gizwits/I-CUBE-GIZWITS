#ifndef IOF_DEV_H_
#define IOF_DEV_H_
void devOtaUpgrade( unsigned int fileSize );
void devRestart();
int dev_free_heap_size();

unsigned long int htonl(unsigned long int h);
unsigned long int ntohl(unsigned long int n);
unsigned short int htons(unsigned short int h);
unsigned short int ntohs(unsigned short int n);
#endif
