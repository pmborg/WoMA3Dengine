/* idea.h */

#ifndef _IDEA_DOT_H
#define _IDEA_DOT_H
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <io.h>
#include <string.h>
#include <conio.h>

#define ENCODE 1
#define DECODE 2


#define IDEAKEYSIZE 16
#define IDEABLOCKSIZE 8
#define word16 unsigned short int
#define word32 unsigned int
#define ROUNDS	8
#define KEYLEN	(6*ROUNDS+4)

#define low16(x) ((x) & 0xffff)

typedef unsigned short int uint16;
typedef word16 IDEAkey[KEYLEN];

/*IDEA Algorithm functions */
void en_key_idea(word16 userkey[8],IDEAkey Z);
void de_key_idea(IDEAkey Z, IDEAkey DK);
void cipher_idea(word16 in[4],word16 out[4],IDEAkey Z);
uint16 inv(uint16 x);
uint16 mul(uint16 a,uint16 b);

/*file handling functions*/
char read_char_from_file(FILE *fp);
word16 read_word16_from_file(FILE *fp);
void write_char_to_file(char data,FILE *fp);
void write_word16_to_file(word16 data,FILE *fp);
void cipher_file(FILE *in,FILE *out,word16 *key);
void decipher_file(FILE *in,FILE *out,word16 *key);
void swap_files_and_clean_up(char *file);

#endif

extern "C" int EncodeIDEA(char* filename, int whatTOdo);