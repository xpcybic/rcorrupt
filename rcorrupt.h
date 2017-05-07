#ifndef RCORRUPT_H
#define RCORRUPT_H

#define HELPTEXT \
	"Help text will go here\n"

typedef unsigned char byte;

void cleanup();
int strNum(char *str);
int ParseArgs(int argc, char *argv[]);
void CorruptByte(FILE *rFile, long rPos, char amt);
unsigned char cAdd(unsigned char val, char amt);
unsigned char cFixed(unsigned char val, char amt);
unsigned char cRandom(unsigned char val, char amt);

//File to be corrupted
FILE *rFile = NULL;
char *rPath = "";
//Range of addresses to be corrupted
unsigned long cStart = 0;
unsigned long cEnd = EOF;
//Amount to increment when reading file (corrupt every xth byte)
unsigned int cIncr = 1;
//Which corruption function to use
byte (*cMethod)(byte, char) = cRandom;
//Approximate percentage of addresses to be corrupted
byte cPercent = 10;
//Value to set bytes to when using cFixed and increment bytes by when using cAdd
byte cCorrVal = 0;
//Whether to show results when done
char quietMode = 0;
//Whether to prompt for confirmation before corrupting
char prompt = 1;

#endif
