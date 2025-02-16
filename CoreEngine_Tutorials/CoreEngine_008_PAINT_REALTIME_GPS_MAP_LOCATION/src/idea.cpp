#pragma warning( disable : 4018 ) // warning C4018: '<' : signed/unsigned mismatch
#pragma warning( disable : 5033 ) // warning C5033: 'register' is no longer a supported storage class
#define _CRT_SECURE_NO_WARNINGS

#include "platform.h"
#if defined USE_IDEA_PACK
/*IDEA.C   v2.2
	c source code for IDEA block cipher. IDEA (International Data
	Encryption Algorithm), formerly known as IPES (Improved Proposed
	Encryption Standard). Algorithm developed by Xuejia Lai and James L.
	Massey, of ETH Zurich. This implementation modified and derived from
	original C code developed by Xuejia Lai. Zero-based indexing added,
	names changed from IPES to IDEA. CFB functions added. Random Number
	routines added. Optimized for speed 21 Oct 92 by Colin Plumb
	<colin@nsq.gts.org>  This code assumes that each pair of 8-bit bytes
	comprising a 16-bit word in the key and in the cipher block are
	externally represented with the Most Significant Byte (MSB) first,
	regardless of internal native byte order of the target cpu.
	modified for use with PC files by Colin Maroney 4/1/94*/

/*   USAGE:     pass a key made up of 8 16-bit numbers (word16) in an array
		("word16 key[8];"), an input FILE * and an output temporary
		FILE * to either encode_file() or decode_file().
		where the key comes from is up to you.
		then call swap_files_and_clean_up() with the original file's
		name as the argument, to replace the original file
		with the encoded data (stored in the temporary file).

		you can remname the tempfile to be used in idea.h
		noisy is an integer which tells encrypting/decrypting
		functions to echo a "." every 256 writes, so the user can
		see that something is happening. set it to 0 for quiet
		running.

		please note that for really good security the original file
		is overwritten before being erased if you use the w switch.
		otherwise it outputs a file "<filename>.enc"

		the main() used here as illustration reads the filename
		from the command line arguments, as well as a command
		"e" or "d" to tell it whether to encrypt or
		decrypt, and a key.  the older versions had an interface
		for when a command line was not use.  lack of editing 
		features made this buggy, so i axed it. */

#include "idea.h"

int end_of_file,noisy,overwrite = 0;   /*global vars*/

static uint16 inv(uint16 x)
{
   uint16 t0,t1;
   uint16 q,y;
   if (x<=1)
      return x;
   t1=(uint16)(0x10001l/x);
   y=(uint16)(0x10001l%x);
   if (y==1)
	return low16(1-t1);
   t0=1;
   do
   {
      q=x/y;
      x=x%y;
      t0+=q*t1;
      if (x==1)
	return t0;
      q=y/x;
      y=y%x;
      t1+=q*t0;
   } while (y!=1);
   return low16(1-t1);
}

static void en_key_idea(word16 *userkey, word16 *Z)
{
   int i,j;
   /* shifts */
   for (j=0;j<8;j++)
      Z[j]=*userkey++;
   for (i=0;j<KEYLEN;j++)
   {
      i++;
      Z[i+7]=((Z[i&7] << 9) | (Z[i+1 & 7] >> 7));
      Z+=i&8;
      i&=7;
   }
}

static void de_key_idea(IDEAkey Z,IDEAkey DK)
{
   int j;
   uint16 t1,t2,t3;
   IDEAkey T;
   word16 *p=T+KEYLEN;
   t1=inv(*Z++);
   t2=-*Z++;
   t3=-*Z++;
   *--p=inv(*Z++);
   *--p=t3;
   *--p=t2;
   *--p=t1;
   for (j=1;j<ROUNDS;j++)
   {
      t1=*Z++;
      *--p=*Z++;
      *--p=t1;
      t1=inv(*Z++);
      t2=-*Z++;
      t3=-*Z++;
      *--p=inv(*Z++);
      *--p=t2;
      *--p=t3;
      *--p=t1;
   }
   t1=*Z++;
   *--p=*Z++;
   *--p=t1;
   t1=inv(*Z++);
   t2=-*Z++;
   t3=-*Z++;
   *--p=inv(*Z++);
   *--p=t3;
   *--p=t2;
   *--p=t1;
   /*copy and destroy temp copy*/
   for(j=0,p=T;j<KEYLEN;j++)
   {
      *DK++=*p;
      *p++=0;
   }
}


uint16 mul(uint16 a, uint16 b)
{
   word32 p;

   if (a)
   {
      if (b)
      {
	 p=(word32)a*b;
	 b=(uint16)(low16(p));
	 a=(uint16)(p>>16);
	 return b-a+(b<a);
      }
      else
      {
	 return 1-a;
      }
   }
   else
      return 1-b;
}

#define MUL(x,y) (x=mul(low16(x),y))


#define CONST

static void cipher_idea(word16 in[4],word16 out[4],register CONST IDEAkey Z)
{
   register uint16 x1,x2,x3,x4,t1,t2;
   int r=ROUNDS;
   x1=*in++; x2=*in++;
   x3=*in++; x4=*in;
   do
   {
      MUL(x1,*Z++);
      x2+=*Z++;
      x3+=*Z++;
      MUL(x4,*Z++);
      t2=x1^x3;
      MUL(t2,*Z++);
      t1=t2+(x2^x4);
      MUL(t1,*Z++);
      t2=t1+t2;
      x1^=t1;
      x4^=t2;
      t2^=x2;
      x2=x3^t1;
      x3=t2;
   } while (--r);
   MUL(x1,*Z++);
   *out++=x1;
   *out++=(x3+*Z++);
   *out++=(x2+*Z++);
   MUL(x4,*Z);
   *out=x4;
}

char read_char_from_file(FILE *fp)
{
   char temp=0;

   if ((fread(&temp,sizeof(char),1,fp))!=1)
      end_of_file=1;

   return (temp);
}

word16 read_word16_from_file(FILE *fp)
{
   word16 temp=0;

   if ((fread(&temp,sizeof(word16),1,fp))!=1)
      end_of_file=1;

   return temp;
}

void write_char_to_file(char data,FILE *fp)
{
   if ((fwrite(&data,sizeof(char),1,fp))!=1)
   {
      printf("Fatal Error writing output file!!!\n");
      exit(-1);
   }
}

void write_word16_to_file(word16 data,FILE *fp)
{
   if ((fwrite(&data,sizeof(word16),1,fp))!=1)
   {
      printf("Fatal Error writing output file!!!\n");
      exit(-1);
   }
}

void cipher_file(FILE *in,FILE *out,word16 *key)
{
   word16 input[4],output[4];
   IDEAkey Z;
   int x,y;
   int count=0;
   long length;
   int temp;

   en_key_idea(key,Z);
   end_of_file=0;

   length=_filelength(_fileno(in));
   fwrite(&length,sizeof(long),1,out);

   while (!end_of_file)
   {
      x=0;

      while (x<4)
      {
	 input[x]=((word16)(read_char_from_file(in)<<8));
	 if (!end_of_file)
	 {
	    temp=read_char_from_file(in);
	    if (temp<0) temp+=256;
	    input[x]=input[x]|temp;
	    x++;
	 }
	 if (end_of_file)
	 {
	    while (x<4) input[x++]=0;
	    break;
	 }
      }

      cipher_idea(input,output,Z);

      for (y=0;y<x;y++)
      {
	 if (noisy) if (count++%256==0) printf(".");
	 write_word16_to_file(output[y],out);
      }
   }
}

void decipher_file(FILE *in,FILE *out,word16 *key)
{
   word16 input[4],output[4];
   int x,y;
   IDEAkey Z,DK;
   int count=0;
   long length=0;

   en_key_idea(key,Z);
   de_key_idea(Z,DK);

   end_of_file=0;

   fread(&length,sizeof(long),1,in);

   while (!end_of_file)
   {
      x=0;
      while (x<4)
      {
	 input[x]=read_word16_from_file(in);
	 if (end_of_file)
	    break;
	 x++;
      }
      cipher_idea(input,output,DK);
      for (y=0;y<x;y++)
      {
	 if (noisy) if (count++%256==0) printf(".");
	 if (length-->0)
	    write_char_to_file(((char)(output[y]>>8)),out);
	 if (length-->0)
	    write_char_to_file(((char)(output[y]&255)),out);
      }
   }
}

/*-----------------------------------------------*/
char tempfilename[100];

void get_target_name(char *file, int whatTOdo)
{
	if (whatTOdo == ENCODE) 
	{
		/*
		strcpy(tempfilename, file);	// Get Original Name
		tempfilename[0] = 'p';
		tempfilename[1] = 'a';
		tempfilename[2] = 'c';
		tempfilename[3] = 'k';
		tempfilename[4] = 'g';
		*/
		strcpy(tempfilename,file);
		file=strtok(tempfilename,"."); // Cut String until a dot
		strcat(tempfilename,".pck");

	} else 
	// DECODE:
	{
		/*
		strcpy(tempfilename, file);	// Get Original Name
		tempfilename[0] = 'u';
		tempfilename[1] = 'n';
		tempfilename[2] = 'p';
		tempfilename[3] = 'a';
		tempfilename[4] = 'c';
		tempfilename[5] = 'k';
		*/
		strcpy(tempfilename,file);
		file=strtok(tempfilename,".");	// Cut String until a dot
		strcat(tempfilename,".pack.zip");
   }
}

/*-----------------------------------------------*/
#define KBYTES 1024

void getuserkeyfromargv(word16 *key,char *arg)
{
   int x;

   for (x=0;x<strlen(arg) && x<8;x++)
   {
       if (x==0) key[x]=arg[x]<<8;
       else key[x]=((arg[x]<<8)|(key[x-1]>>8));
   }

   if (strlen(arg)>8) printf ("\nONLY first *8* characters of key used!!!\n");

   if (x<8) while (x<8) key[x++]=0;
}

#define TRUE 1
#define FALSE 0

word16 userkey[8];

int EncodeIDEA(char* filename, int whatTOdo)
{
	FILE *fp, *temp;

	getuserkeyfromargv(userkey, "13082014");		// Max 8 Chars.
	get_target_name(filename, whatTOdo);			// TEMP to TARGET

	if ((fp=fopen(filename,"r+b"))==NULL)			// READ
	{
		printf("\nError opening File %s\n",filename);
		return FALSE;
	}

	if ((temp=fopen(tempfilename,"w+b"))==NULL)		// TEMP (WRITE)
	{
		printf("\nError opening temporary file\n");
		return FALSE;
	}

	switch (whatTOdo) {
		case ENCODE:
			printf("\nEncoding file %s   \n",filename);
			cipher_file(fp,temp, userkey);	
			break;

		case DECODE:
			printf("\nDecoding file %s   \n",filename);		// De-cipher
			decipher_file(fp,temp,userkey);
			break;
	}

	fclose (fp);									// Free FILES					
	fclose(temp);

	return TRUE;
}

#endif
