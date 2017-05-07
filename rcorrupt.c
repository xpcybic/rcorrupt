#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rcorrupt.h"

void cleanup()
{
	if (rFile)
	{
		if (fclose(rFile))
			perror("Error");
	}
}

void ParseArgs()
{
}

//Corrupt the file at the specified offset using the specified corruption function.
void CorruptByte(FILE *rFile, long rPos, byte (*cmethod)(byte, char), char amt)
{
	//go to write position
	if (fseek(rFile, rPos, SEEK_SET))
	{
		fprintf(stderr, "Error while reading file!\n");
		perror("Error");
		exit(2);
	}

	//get current val at address for cAdd
	byte val = fgetc(rFile);

	//write new value
	if (fputc(cmethod(val, amt), rFile) == EOF)
	{
		fprintf(stderr, "Error while writing to file!\n");
		perror("Error");
		exit(2);
	}
}

byte cRandom(byte val, char amt)
{
	return rand() % 0xFF;
}

byte cFixed(byte val, char amt)
{
	return amt;
}

byte cAdd(byte val, char amt)
{
	return val + amt;
}

int main(int argc, char *argv[])
{
	int rt = 0;
	char input = 'n';
	//File to be corrupted
	char *rPath = "";
	//Range of addresses to be corrupted
	long cStart = 0;
	long cEnd = EOF;
	//Amount to increment when reading file (corrupt every xth byte)
	unsigned int cIncr = 1;
	//which corruption function to use
	byte (*cMethod)(byte, char) = cRandom;
	//Approximate percentage of addresses to be corrupted
	int cPercent = 10;
	//Amount to increment bytes when using cAdd
	int cAdd = 0;
	//Total number of bytes corrupted so we can report to the user afterward
	long ccount = 0;

	//TODO: parse cmdline args here

	if (argc != 2)
	{
		fprintf(stderr, "Error: no input file specified\n");
		exit(1);
	}
	rPath = argv[1];

	srand(time(0));

	//open file
	rFile = fopen(rPath, "r+");
	if (!rFile)
	{
		fprintf(stderr, "%s :", rPath);
		perror("Error");
		exit(1);
	}

	if (cEnd <= EOF)
	{
		fseek(rFile, 0, SEEK_END);
		cEnd = ftell(rFile) - 1;
	}

	//user confirmation
	printf("Corrupting bytes %ld - %ld of file %s.\nProceed? [y/N] ", cStart, cEnd, rPath);
	input = getchar();
	if (input != 'y' && input != 'Y')
		exit(0);

	for (long addr = cStart; addr < cEnd; addr += cIncr)
	{
		if (cPercent >= 100 || rand() % 100 + 1 < cPercent)
		{
			CorruptByte(rFile, addr, cMethod, cAdd);
			ccount++;
		}
	}


	long cSize = cEnd - cStart;
	printf("--Done!--\n"
			"Range:     %8ld bytes\n"
			"Corrupted: %8ld (%.2f%%)\n",
			cSize,
			ccount, ((float)ccount / cSize) * 100);

	return rt;
}

/*Corruption value options
 *	-r	random values (default)
 *	-fx	fixed values
 *	-ax	add amount x to existing values
 *Corruption range/amount options
 *	-sx	start address (default: start of file)
 *	-ex end address (default: end of file)
 *	-px % of addresses to corrupt (default: 5)
 *	-ex	corrupt every xth byte
 */
