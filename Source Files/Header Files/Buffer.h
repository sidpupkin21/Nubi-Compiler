/*
************************************************************
* Code version: 1.0
* Starter Files Author: Svillen Ranev & Paulo Sousa
* Author: Ahmed Mohamed & Jeel Patel
* Last Edited: Feb 5, 2022
************************************************************
*/

/*
************************************************************
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) */
		/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/
		/* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>				/* standard input/output */
#include <malloc.h>				/* for dynamic memory allocation*/
#include <limits.h>				/* implementation-defined data type ranges and limits */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

#define BUFFER_ERROR				(-1)		/* General error message */
#define BUFFER_EOF					'\0'		/* General EOF */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor */

/* BITMASK Definitions */
#define NUBI_DEFAULT_FLAG			0x0F 		/* (0000.1111)_2 = (015)_10 */
/* Relocation */
#define SET_REL_FLAG				0x80
#define RST_REL_FLAG				0x7F
#define CHK_REL_FLAG				0x80
/* EndOfBuffer */
#define SET_EOB_FLAG				0x40
#define RST_EOB_FLAG				0x3F
#define	CHK_EOB_FLAG				0x40
/* Full */
#define SET_FUL_FLAG				0x20
#define RST_FUL_FLAG				0x5F
#define CHK_FUL_FLAG				0x20
/* Empty */
#define SET_EMP_FLAG				0x10
#define RST_EMP_FLAG				0x6F
#define	CHK_EMP_FLAG				0x10

/* Offset declaration */
typedef struct position {
	Sint	    markPos;			/* the offset (in chars) to the mark location */
	Sint		readPos;			/* the offset (in chars) to the get-character location */
	Sint		writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	chars*		string;				/* pointer to the beginning of character array (character buffer) */
	Sint		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	Sint		increment;			/* character array increment factor */
	Sint		mode;				/* operational mode indicator */
	Sign		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;			/* Offset / position field */
} Buffer, * BufferPointer;
/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(Sint, Sint, Sint);
BufferPointer	bAddChar(BufferPointer const, chars);
Usign			bRetract(BufferPointer const);
Usign			bRestore(BufferPointer const);
Usign			bRecover(BufferPointer const);
Usign			bClear(BufferPointer const);
Usign			bDestroy(BufferPointer const);
Usign			bSetMark(BufferPointer const, Sint);
Usign			bIsEmpty(BufferPointer const);
Usign			bIsFull(BufferPointer const);
Usign			bIsRealocated(BufferPointer const);
Sint			bPrint(BufferPointer const);
Sint			bLoad(BufferPointer const, FILE* const);
/* Getters */
Sint			bGetSize(BufferPointer const);
Sint			bGetWritePos(BufferPointer const);
Sint			bGetMarkPos(BufferPointer const);
Sint			bGetReadPos(BufferPointer const);
Sint			bGetIncrement(BufferPointer const);
Sint			bGetMode(BufferPointer const);
chars			bGetChar(BufferPointer const);
chars*			bGetContent(BufferPointer const, Sint);
Sign			bGetFlags(BufferPointer const);

#endif
