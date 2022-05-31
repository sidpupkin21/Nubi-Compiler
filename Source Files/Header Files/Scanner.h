/*
************************************************************
* Code version: 1.0
* Starter Files Author: Svillen Ranev & Paulo Sousa
* Author: Ahmed Mohamed & Jeel Patel
* Last Edited: Mar 12, 2022
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: transitionTable(),stateType(),startScanner(),
*	nextClass(), nextState(), finalStateTable(),keywordTable()
*************************************************************/

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */
/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/
/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/*Define Token codes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	LBR_T,		/*  5: Left brace token */
	RBR_T,		/*  6: Right brace token */
	KW_T,		/*  7: Keyword token */
	EOS_T,		/*  8: End of statement (semicolon) */
	RTE_T,		/*  9: Run-time error token */
	SEOF_T,		/* 10: Source end-of-file token */
	CHA_T,		/* 11: Char token*/
	INL_T,		/* 12: Integer Token*/
	VID_T,		/* 13: Variable identifer token*/
	ASS_OP_T,   /* Assignment  token*/
	ART_OP_T,   /* Arithmetic  token */
	REL_OP_T,   /* Relational  token*/
	LOG_OP_T   /* Logical  token */
};

/*Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_INC, OP_SUB, OP_DEC, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GTE, OP_LTE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/*Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	Sint codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	Sint intValue;						/* integer literal attribute (value) */
	Sint keywordIndex;					/* keyword index in the keyword table */
	Sint contentString;					/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	floating floatValue;				/* floating-point literal attribute (value) */
	chars idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	chars errLexeme[ERR_LEN + 1];		/* error token attribite */
	chars chValue;						/*character literal attribute (value)*/
} TokenAttribute;

/*If no symbol table is implemented */
typedef struct idAttibutes {
	Sign flags;			/* Flags information */
	union {
		Sint intValue;				/* Integer value */
		floating floatValue;		/* Float value */
		Null* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	Sint code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/* Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /*Error states and illegal state */
#define ES  11		/* Error state with no retract */
#define ER  12		/* Error state with retract */
#define IS -1		/* Illegal state */

/*State transition table definition */
#define TABLE_COLUMNS 9

/*Define lexeme FIXED classes */
/*These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '#'
#define CHRCOL4 '\"'
#define CHRCOL5 '.'
#define CHRCOL6 ';'

/*These constants will be used on VID / MID function */
#define MNIDPREFIX '&'

/*Transition table - type of states are defined in separate table */
static Sint transitionTable[][TABLE_COLUMNS] = {
	/*		 [A-z]  , [0-9]  ,  _    ,	  '	  ,	   "   ,   .   ,   #	,   :: ,	SEOF*/
	/*		  L(0)  ,  D(1)  ,  U(2) ,  SQ(3) ,  DQ(4) ,  P(5) ,  CA(6) ,  C(7),	EF(8)*/
	/*S0*/{		1,		4,		ES,		  9,		9,		1,		12,		ER,		ER}, /*S0: NOAS			*/
	/*S1*/{		1,		1,		1,		  2,		ER,		ER,		ER,		ER,		ER}, /*S1: NOAS			*/
	/*S2*/{		IS,		IS,		IS,		 IS,		IS,		IS,		IS,		IS,		IS}, /*S2: ASNR(ID)		*/
	/*S3*/{		IS,		IS,		IS,		 IS,		IS,		IS,		IS,		IS,		IS}, /*S3: ASWR(KEY)	*/
	/*S4*/{		IS,		4,		4,		 IS,		IS,		1,		IS,		IS,		IS}, /*S4: ASNR			*/
	/*S5*/{		5,		IS,		IS,		 IS,		IS,		1,		IS,		IS,		IS}, /*S5: NOAS			*/
	/*S6*/{		ES,		4,		IS,		 IS,		IS,		1,		IS,		IS,		IS}, /*S6: ASWR(IL)		*/
	/*S7*/{		8,		ES,		8,		 ER,		ER,		ER,		ER,		ER,		IS}, /*S7: NOAS			*/
	/*S8*/{		IS,		IS,		8,		 IS,		IS,		IS,		IS,		IS,		IS}, /*S8: ASWR			*/
	/*S9*/{		9,		9,		9,		  9,		10,		10,		9,		ER,		ER}, /*S9: NOAS			*/
	/*S10*/{	IS,		IS,		IS,		 11,		ER,		ER,		IS,		ER,		IS}, /*S10: ASNR(SL)	*/
	/*S11*/{	IS,		IS,		IS,		 IS,		IS,		IS,		IS,		IS,		IS}, /*S11: ASNR(ES)	*/
	/*S12*/{	IS,		IS,		IS,		 IS,		IS,		IS,		IS,		IS,		IS}  /*S12: ASWR(ER)	*/
};
/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/*List of acceptable states */
static Sint stateType[] = {
	NOAS,	/*00*/						//NOAS, /* 00 */
	NOAS,	/*01*/						//NOAS, /* 01 */
	ASWR,	/*02 ASNR(ID)*/				//ASNR, /* 02 (MID) - Methods */
	NOAS,	/*03 ASWR(KEY)*/			//ASWR, /* 03 (KEY) */
	ASNR,	/*04*/						//NOAS, /* 04 */
	NOAS,	/*05*/						//ASNR, /* 05 (SL) */
	ASWR,	/*06(IL)*/					//ASNR, /* 06 (Err1 - no retract) */
	NOAS,	/*07*/						//ASWR  /* 07 (Err2 - retract) */
	ASWR,	/*08*/
	NOAS,	/*09*/
	ASNR,	/*10(SL)*/
	ASNR,	/*11(Err1- no retract)*/
	ASWR	/*12(Err2-retract)*/

};


/* Static (local) function  prototypes */
Sint startScanner(psc_buf);
static Sint nextClass(chars c);			/* character class function */
static Sint nextState(Sint, chars);		/* state machine function */
//Sint startScanner(BufferPointer psc_buf);
/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/*Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(chars* lexeme);

/* Declare accepting states functions */
Token funcSL(chars lexeme[]);
Token funcID(chars lexeme[]);
Token funcKEY(chars lexeme[]);
Token funcErr(chars lexeme[]);
Token funcIL(chars lexeme[]); //integerLiterals
Token funcVID(chars lexeme[]);
//Token funcCL(chars lexeme[]); //character literals 


/*Final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/*-		[00] */			//NULL,		/* -    [00] */
	NULL,		/*-		[01] */			//NULL,		/* -    [01] */
	funcID,		/*-ID	[02] */			//funcID,	/* MNID	[02] */
	funcKEY,	/*-KEY	[03] */			//funcKEY,	/* KEY  [03] */
	NULL,		/*-		[04] */			//NULL,		/* -    [04] */
	funcIL,		/*-IL	[05] */			//funcSL,	/* SL   [05] */
	NULL,		/*-		[06] */			//funcErr,	/* ERR1 [06] */
	NULL,		/*-		[07] */			//funcErr	/* ERR2 [07] */
	NULL,		/*		[08] */
	funcVID,	/*-		[09] */
	funcSL,		/*-SL	[10] */
	funcErr,	/*-ERR1	[11] */
	funcErr		/*-ERR2	[12] */

};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/*Number of Keywords in table*/
#define KWT_SIZE  17 

/*List of keywords */
static chars* keywordTable[KWT_SIZE] = {
	"START",
	"main",
	"END",
	"Sint",
	"floating",
	"chars",
	"DO",
	"WHILE",
	"Scan",
	"if",
	"else",
	"System",
	"writes",
	"Sign",
	"Usign",
	"dbl",
	"Long"

};

#endif
