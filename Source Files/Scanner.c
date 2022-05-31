/*
************************************************************
* Code Version: 1.0
* Starter File Authors: Svillen Ranev - Paulo Sousa
* Author: Ahmed Mohamed || Jeel Patel
* Last Edited: Mar 12, 2022
************************************************************
*/

/*
**************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Purpose: This file contains all functionalities from Scanner.
* Function list: funcVID(), funcIL(), funcErr(), funcKey(), 
*	funcSL(), funcID(), printToken(), nextClass(), nextState(), 
*	tokenizer(), startScanner()
************************************************************
*/

/*Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Header Files/Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Header Files/Scanner.h"
#endif

/*
----------------------------------------------------------------
Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
Sint line;								/* Current line number of the source code */
extern Sint errorNumber;				/* Defined in platy_st.c - run-time error number */

extern Sint stateType[];
extern chars* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern Sint transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */

Sint startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	if (bIsEmpty(psc_buf) == NUBI_T) { return EXIT_FAILURE; }
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	chars c;	/* input symbol */
	Sint state = 0;		/* initial state of the FSM */
	Sint lexStart = 0;		/* start offset of a lexeme in the input char buffer (array) */
	Sint lexEnd = 0;		/* end offset of a lexeme in the input char buffer (array)*/
	Sint newLexState = 0;
	Sint prevState = 0;
	Sint lexLength = 0;		/* token length */
	Sint i;				/* counter */
	chars newc;			/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/
		switch (c) {

			/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

			/* Cases for symbols */
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
			/*ArithmeticOperators*/
		case '+':
			newc = bGetChar(sourceBuffer);
			if (newc == '+') {
				currentToken.code = ART_OP_T;
				currentToken.attribute.codeType = OP_INC;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_ADD;
			return currentToken;

		case '-':
			newc = bGetChar(sourceBuffer);
			if (newc == '-') {
				currentToken.code = ART_OP_T;
				currentToken.attribute.codeType = OP_DEC;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_SUB;
			return currentToken;
		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_MUL;
			return currentToken;
		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_DIV;
			return currentToken;
			/*Relational operators*/
		case '>':
			newc = bGetChar(sourceBuffer);
			if (newc == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_GTE;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_GT;

			return currentToken;
		case '<':
			newc = bGetChar(sourceBuffer);
			if (newc == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_LTE;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_LT;

			return currentToken;
		case '::'://#
			newc = bGetChar(sourceBuffer);
			if (newc == '::') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_EQ;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ASS_OP_T;
			return currentToken;

			/*Logical operators */
		case '&':
			newc = bGetChar(sourceBuffer);
			if (newc == '&') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_AND;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ERR_T;
			return currentToken;

		case '|':
			newc = bGetChar(sourceBuffer);
			if (newc == '|') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_OR;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ERR_T;
			return currentToken;

		case '='://::
			newc = bGetChar(sourceBuffer);
			if (newc == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_NE;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = LOG_OP_T;
			currentToken.attribute.codeType = OP_NOT;
			return currentToken;

			/* Comments */
		case '@':
			newc = bGetChar(sourceBuffer);
			do {
				c = bGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					bRetract(sourceBuffer);
					//return currentToken;
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '@' && c != CHARSEOF0 && c != CHARSEOF255);
			break;
			/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[prevState] == ASWR)
				bRetract(sourceBuffer);
			lexEnd = bGetReadPos(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bCreate((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			bAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			bDestroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
Sint nextState(Sint state, chars c) {
	Sint col;
	Sint next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* Adjust the logic to return next column in TT */
 /*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */
 /*L(0)  ,  D(1)  ,  U(2) ,  SQ(3) ,  DQ(4) ,  P(5) ,  CA(6) ,  C(7),	EF(8)**/
 /*[A - z], [0 - 9], _, '	  ,	   "   ,   .   ,   #	,   :: ,	SEOF*/
Sint nextClass(chars c) {
	Sint val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 6;
		break;
	case CHRCOL4:
		val = 3;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 4;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 7;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 8;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */


Token funcID(chars lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	char lastch = lexeme[length - 1];
	int isID = NUBI_F;
	switch (lastch) {
	case MNIDPREFIX:
		currentToken.code = MNID_T;
		isID = NUBI_T;
		break;
	default:
		// Test Keyword
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == NUBI_T) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(chars lexeme[]) {
	Token currentToken = { 0 };
	Sint i = 0, len = (Sint)strlen(lexeme);
	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	for (i = 1; i < len - 2; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!bAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */

Token funcKEY(chars lexeme[]) {
	Token currentToken = { 0 };
	Sint kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */

Token funcErr(chars lexeme[]) {
	Token currentToken = { 0 };
	Sint i = 0, len = (Sint)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

Null printToken(Token t) {
	extern chars* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", bGetContent(stringLiteralTable,
			(short)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case INL_T: /*funcIL*/
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
		//case CHA_T: /*funcCL*/
			//printf("CHA_T\t\t%c\n", t.attribute.chValue);
			//break;
	case ASS_OP_T:
		printf("ASS_OP_T\n");
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case ART_OP_T:
		printf("ART_OP_T\t%d\n", t.attribute.codeType);
		break;
	case REL_OP_T:
		printf("REL_OP_T\t%d\n", t.attribute.codeType);
		break;
	case LOG_OP_T:
		printf("LOG_OP_T\t%d\n", t.attribute.codeType);
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
*	Acceptance State Function IL
*	Function responsible for identifying IL( Integer Literals)
*	The Lexeme must be stored in the Integer Literal Table
*/
Token funcIL(chars lexeme[]) {
	Token currentToken = { 0 };
	Long tLong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		tLong = atol(lexeme);
		if (tLong >= 0 && tLong <= SHRT_MAX) {
			currentToken.code = INL_T;
			currentToken.attribute.intValue = (Sint)tLong;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}return currentToken;
}

/*
*	Acceptance State Function VID
*	Function responsible for identifying the Variable Identification Token
*/
Token funcVID(chars lexeme[]) {
	Token currentToken = { 0 };
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	return currentToken;
}

