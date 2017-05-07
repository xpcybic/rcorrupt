#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "rcorrupt.h"

//TODO: fix corruption modes - additive and fixed do not do what they should

void cleanup()
{
	if (rFile)
	{
		if (fclose(rFile))
			perror("Error");
	}
}

int strNum(char *str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isdigit(str[i]))
			return 0;
	}
	return 1;
}

//Parse command line arguments.
//Return 0 if all is well; break and return 1 on error.
//TODO: set up error codes
int ParseArgs(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if (strlen(argv[i]) != 2)
				return 1;
			switch (argv[i][1])
			{
				case 'm':
					//corruption mode
					if (++i >= argc)
						return 1;
					//add
					if (argv[i][0] == '+' && strlen(argv[i]) > 1)
					{
						char *incAmtArg = argv[i] + 1;
						if (strNum(incAmtArg))
						{
							cMethod = cAdd;
							cCorrVal = atoi(incAmtArg);
						}
						else
							return 1;
					}
					//random
					else if (strcmp(argv[i], "r") == 0 ||
							strcmp(argv[i], "rand") == 0)
						cMethod = cRandom;
					//fixed
					else if (strNum(argv[i]))
					{
						cMethod = cFixed;
						cCorrVal = atoi(argv[i]);
					}
					else
						return 1;
					break;
					
				case 'r':
					//range
					//start
					if (++i >= argc)
						return 1;
					if (strNum(argv[i]))
						cStart = atol(argv[i]);
					else
						return 1;
					//end
					if (++i >= argc)
						return 1;
					if (strNum(argv[i]))
						cEnd = atol(argv[i]);
					else
						return 1;
					if (cStart >= cEnd)
						return 1;
					break;

				case 'p':
					//% corruption
					if (++i >= argc)
						return 1;
					if (strNum(argv[i]))
					{
						cPercent = atoi(argv[i]);
						if (cPercent > 100) cPercent = 100;
					}
					else
						return 1;
					break;

				case 'i':
					//step size
					if (++i >= argc)
						return 1;
					if (strNum(argv[i]))
						cIncr = atoi(argv[i]);
					else
						return 1;
					break;

				case 'f':
					prompt = 0;
					break;

				case 'q':
					quietMode = 1;
					break;

				case 'h':
					printf(HELPTEXT);
					exit(0);
					break;

				default:
					return 1;
					break;
			}

		}
		else
		{
			//must be a file path - is one already assigned?
			if (rPath[0] == '\0')
				rPath = argv[i];
			else
				return 1;
		}
	}
	return 0;
}

//Corrupt the file at the specified offset using the specified corruption function.
void CorruptByte(FILE *rFile, long rPos, char amt)
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
	if (fputc(cMethod(val, amt), rFile) == EOF)
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
	atexit(cleanup);

	int rt = 0;
	char input = 'n';
	//Total number of bytes corrupted so we can report to the user afterward
	long ccount = 0;

	if (argc < 2)
	{
		fprintf(stderr, "Error: too few arguments\n");
		exit(1);
	}
	if (ParseArgs(argc, argv))
	{
		fprintf(stderr, "Error(s) in arguments\n");
		exit(1);
	}

	srand(time(0));

	//open file
	rFile = fopen(rPath, "r+");
	if (!rFile)
	{
		fprintf(stderr, "%s :", rPath);
		perror("Error");
		exit(1);
	}

	//find eof and cap corruption range
	fseek(rFile, 0, SEEK_END);
	unsigned long fEnd = ftell(rFile) - 1;
	if (cEnd <= 0 || cEnd > fEnd)
		cEnd = fEnd;

	//user confirmation
	if (prompt)
	{
		printf("Corrupting bytes %ld - %ld of file %s.\nProceed? [y/N] ", cStart, cEnd, rPath);
		input = getchar();
	}
	else
		input = 'y';
	if (input != 'y' && input != 'Y')
		exit(0);

	//corruption loop
	for (long addr = cStart; addr < cEnd; addr += cIncr)
	{
		if (cPercent >= 100 || rand() % 100 + 1 <= cPercent)
		{
			CorruptByte(rFile, addr, cCorrVal);
			ccount++;
		}
	}

	long cSize = cEnd - cStart;
	if (!quietMode)
	{
		printf("--Done!--\n"
				"Range:     %8ld bytes\n"
				"Corrupted: %8ld (%.2f%%)\n",
				cSize,
				ccount, ((float)ccount / cSize) * 100);
	}

	return rt;
}

/*	-m	corruption mode (function) to use
 *		-m rand, -m r	random values
 *		-m <val>		set each byte to val
 *		-m +<val>		add val to each byte
 *	Corruption range/amount options
 *	-r <st> <end>		range of addresses to corrupt
 *	-p <val>			% of addresses to corrupt (default: 5)
 *	-i <val>			increment (corrupt every xth byte)
 *Other
 *	-h					display help
 *	-f					force (don't ask for confirmation)
 *	-q					quiet (don't display results when done)
 */
