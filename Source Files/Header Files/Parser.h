/*
************************************************************
* Code version: 1.0
* Starter Files Author: Svillen Ranev & Paulo Sousa
* Author: Ahmed Mohamed & Jeel Patel
* Last Edited: Apr 16, 2022
************************************************************
*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Header Files/Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Header Files/Scanner.h"
#endif

/* Global vars */
static Token lookahead;
Sint syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern Sint line;
extern Token tokenizer();
extern chars* keywordTable[];

#define STR_LANGNAME "NUBI"

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	START,
	END,
	MAIN,
	SINT,
	FLOATING,
	CHARS,
	DO,
	WHILE,
	SCAN,
	IF,
	ELSE,
	ELSEIF,
	SYSTEM,
	WRITES,
	SIGN,
	USIGN,
	DBL,
	LONG,
	NuLL
};

/*Function definitions */
Null startParser();
Null matchToken(Sint, Sint);
Null syncErrorHandler(Sint);
Null printError();
/*
* Non-terminal function declarations
*/
Null assignmentExpression();
Null assignmentStatement();
Null codeSession();
Null dataSession();
Null optVarListDeclarations();
Null optionalStatements();
Null outputStatement();
Null outputVariableList();
Null program();
Null statement();
Null statements();
Null statementsPrime();
/*Logical expressions*/
Null logicalANDexpression();
Null logicalANDexpressionprime();
Null logicalORexpression();
Null logicalORexpressionprime();
/*Arithmetic expressions*/
Null arithmeticexpression();
Null unaryarithmeticexpression();
Null additivearithmeticexpression();
Null additivearithmeticexpressionprime();
Null multiplicativearithmeticexpression();
Null multiplicativearithmeticexpressionprime();
Null primaryarithmeticexpression();
Null numericalarithmeticexpressions();
Null primaryarithmeticexpression();
Null numericalValues();
Null mainExpression();
Null mainExpressionIdentifier();
Null varibleTypeExpression();
/*Statement expressions*/
Null selectionStatement();
Null ifprimarySelectionStatement();
Null ifSelectionStatement();
Null elifSelectionStatement();
Null elifPrimarySelectionStatement();
Null elseSelectionStatement();
Null elsePrimarySelectionStatement();
/*Literal expressions*/
Null codeBlock();
Null floatVariableLiteral();
Null integerVariableLiteral();
Null stringVariableLiteral();

#endif
