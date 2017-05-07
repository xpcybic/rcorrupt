#ifndef RCORRUPT_H
#define RCORRUPT_H

typedef unsigned char byte;

FILE *rFile = NULL;

void cleanup();
void ParseArgs();
void CorruptByte(FILE *rFile, long rPos, byte (*cmethod)(byte, char), char amt);
unsigned char cAdd(unsigned char val, char amt);
unsigned char cFixed(unsigned char val, char amt);
unsigned char cRandom(unsigned char val, char amt);

#endif
