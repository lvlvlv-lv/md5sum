#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "md5.h"

bool IsFile(const char *fileOrStr)
{
  struct stat fileStat;
  int ret = stat(fileOrStr, &fileStat);
  if (ret) {
    return false;
  }
  
  return S_ISREG(fileStat.st_mode);
}


int main(int argc, char *argv[]) {
	int i, n;
	bool isfile = false;
	unsigned char decrypt[16];
	MD5_CTX md5;

    if (argc == 1) {
      fprintf(stderr, "Usage:\n    %s [file|string] ...\n", argv[0]);
      exit(1);
    }

    for (int i = 1; i < argc; i++) {
      if(IsFile(argv[i])) {
		FILE *fp;
        fp = fopen(argv[i], "rb");
        if(fp == NULL) {
          fprintf(stderr, "File %s not exists, errno = %d, error = %s\n", argv[i], errno, strerror(errno));
          continue;
        }
			
        MD5Init(&md5);
        do {
          unsigned char encrypt[1024];
          while(!feof(fp)) {
            MD5Update(&md5, encrypt, fread(encrypt, 1, sizeof(encrypt), fp));
          }
          fclose(fp);
        } while(0);
			
        MD5Final(&md5, decrypt);

        for(int j = 0; j < 16; j++) {
          printf("%02x", decrypt[j]);
        }
			
        printf("  %s\n", argv[i]);
      } else {
        MD5Init(&md5);
        MD5Update(&md5, (unsigned char *)argv[i], strlen(argv[i]));
        MD5Final(&md5, decrypt);

        for(int j=0; j < 16; j++) {
          printf("%02x", decrypt[j]);
        }
			
        printf("  %s\n", argv[i]);

      }

    }
	return 0;
}  

