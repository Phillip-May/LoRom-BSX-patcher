/* fread example: read an entire file */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILENAME "BS1_patched.bs"

int main () {
  FILE * pFile;
  long lSize;
  int8_t * buffer;
  size_t result;
  uint32_t int_temporary;
  uint32_t hex_32bitchecksum;
  uint16_t hex_16bitchecksum;
  uint16_t hex_16bittemporary;
  //4 bytes being overwritten
  uint8_t hex_streamwrite[4];

  hex_32bitchecksum = 0;
  int_temporary = 0;
  hex_16bitchecksum = 0;

  pFile = fopen ( FILENAME , "r+b" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (int8_t*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */
  	printf("File size: 0x%08X\n",lSize);

	for (long i = 0; i < (0x00100000); i++)
	{
		if (i < 0x07FB0 || i > 0x07FDF)
		{
			int_temporary = (buffer[i] & 0x000000FF);
			hex_32bitchecksum = ( (0x000000FF & int_temporary) + hex_32bitchecksum);
			//printf("Byte (int temporary) %d: 0x%08X\n",i,int_temporary);
			//printf("Checksum (hex_32bitchecksum) %d: 0x%08X\n",i,hex_32bitchecksum);

		}
	}
	printf("32bit checksum: 0x%08X\n",hex_32bitchecksum);
	//Usefull outputs to insert into rom
	hex_16bitchecksum = (hex_32bitchecksum & 0x0000FFFF);
	hex_16bittemporary = hex_16bitchecksum << 8;
	hex_16bitchecksum = hex_16bittemporary + (hex_16bitchecksum >> 8);


	printf("16bit checksum: %4X\n",hex_16bitchecksum);
	printf("16bit inverse checksum: %4X\n",~(hex_16bitchecksum));
	printf("Size of 16bit int is: %d\n",sizeof(hex_16bitchecksum));
	hex_streamwrite[0] = ~(hex_16bitchecksum) >> 8;
	hex_streamwrite[1] = ~(hex_16bitchecksum);
	hex_streamwrite[2] = hex_16bitchecksum >> 8;
	hex_streamwrite[3] = hex_16bitchecksum;

	printf("4 bytes going to file: %1X",hex_streamwrite[0]);
	printf(" %1X",hex_streamwrite[1]);
	printf(" %1X",hex_streamwrite[2]);
	printf(" %1X\n",hex_streamwrite[3]);
	printf("Size of array in bytes: %d\n",sizeof(hex_streamwrite));
	printf("Size of array element bytes: %d\n",sizeof(int8_t));


	//Now overwrite the bits and patch the rom
	rewind (pFile);
	fseek (pFile , (0x07FDC), SEEK_SET);
	printf("Write to file error: %d",fwrite(hex_streamwrite,sizeof(int8_t),sizeof(hex_streamwrite),pFile));



  // terminate
  fclose (pFile);
  free (buffer);

  /*
  //keep window open buffer
  getchar();
  getchar();
  getchar();
  */

  return 0;
}
