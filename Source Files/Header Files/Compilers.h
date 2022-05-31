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
* File name: compilers.h
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG						0

/* Logical constants*/
#define NUBI_T						1
#define NUBI_F						0
#define NUBI_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 

/*
------------------------------------------------------------
Data types definitions &
------------------------------------------------------------
*/
typedef char			chars;// char
typedef short			Sint;//int;
typedef float			floating; // floating
typedef void			Null; // Null;

typedef unsigned char	Usign; // boolean
typedef unsigned char	Sign; // flag/sign

typedef long			Long; // long
typedef double			dbl; // double

/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER = '1',
	PGM_SCANNER = '2',
	PGM_PARSER = '3'
};

/*
------------------------------------------------------------
Main functions signatures
------------------------------------------------------------
*/
Sint mainBuffer(Sint argc, chars** argv);
Sint mainScanner(Sint argc, chars** argv);
Sint mainParser(Sint argc, chars** argv);

#endif
