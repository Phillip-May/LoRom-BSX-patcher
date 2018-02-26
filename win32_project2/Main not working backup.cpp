/*****************************************************************************
*	FileName:		main.c
*	
*	Name: Lab1, BS-X checksum calculator
*
*	Description: BS-X checksum calculator
*
REVISION HISTORY
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Anthor			Date				Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Phillip May		February 25th		First session
*
*	A windows command line tool to find the checksum of a BS-X rom
*	The checksum is the 32bit integer sum of all the bits in the file ignoring the header.
*	This number is then truncated to 16 bit and should be output in raw hex.
*
******************************************************************************/
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //For string copying
#include <stdint.h>
#include <io.h>
#include <share.h>

//Symbolic constants
//GFX_SAFE keeps colour order as RED GREEN BLUE
//GFX_AUTODETECT_WINDOWED changes colour order to BLUE GREEN RED
#define		READONLY		"r"
#define		ORIGIN			0
#define		MAXREADSIZE		60000

//Globals
FILE *stm_ROM;

int filedescriptor;	
int num_position;
int8_t str_buffer[MAXREADSIZE];
unsigned int num_readbytes = MAXREADSIZE;
int		bytesread;

int32_t		hex_checksum32;

//Main
int main(void) {
	//Main variables
	printf("BS-X rom checksum calculator\n");

	#define BANKSIZE 1 /* read 1024 bytes at a time */

	hex_checksum32 = 0;
	int8_t read_buffer[BANKSIZE];
	FILE *fle_bsx;
	size_t nread;
	int64_t counter;
	int		inp_temporary;

	counter = 0;
	inp_temporary = 0;

	fle_bsx = fopen("rom.sfc", "r");
	if (fle_bsx) 
	{
		//While not error
    while ((nread = fread(read_buffer, 1, sizeof read_buffer, fle_bsx)) > 0)
	{
		//fwrite(read_buffer, 1, nread, stdout);
		//Stop tpycasting from being stupid and filling new space with binary 1's
		//I am not dealling with negatives or positives so that is unwated behavior.
		inp_temporary = (read_buffer[0] & 0x000000FF);
		hex_checksum32 = (hex_checksum32 + inp_temporary);
		printf("\nCurrent 32bit checksum: 0x%04X",hex_checksum32);
		printf("\nCurrent byte: 0x%02X",inp_temporary);

	}

    if (ferror(fle_bsx)) {
        /* deal with error */
    }
    fclose(fle_bsx);
}
	printf("\nFinal 32bit checksum: 0x%04X",hex_checksum32);

	//Visual studio debug buffer
	getchar();
	getchar();
	getchar();

}


