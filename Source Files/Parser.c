/*
********************************************************************************************************************
* Code version: 1.0
* Starter Files Author: Svillen Ranev & Paulo Sousa
* Author: Ahmed Mohamed & Jeel Patel
* Last Edited: Apr 16, 2022
********************************************************************************************************************
*/

/*
********************************************************************************************************************
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: startParser(), matchToken(), printError(), syncErrorHandler(), program(), dataSession(), 
*	codeSession(), optVarListDeclarations(), optionalStatements(), statements(),statementsPrime(), 
*	statement(), assignmentStatement(), assignmentExpression(),outputVariableList(), arithmeticExpression(), 
*	unaryArithmeticExpression(), additiveArithmeticExpression(), additiveArithmeticExpressionPrime(), 
*	multiplicativeArithmeticExpression(), multiplicativeArithmeticExpressionPrime(), primaryArithmeticExpression(),
*	selectionStatement(), ifPrimarySelectionStatement(), ifSelectionStatement(),elifSelectionStatement(),
*	elifPrimarySelectionStatement(), elseSelectionStatement(),elsePrimarySelectionStatement(),
*	variableTypeExpression(), numericalArithmeticExpressions(),numericalValues(),mainExpression(), 
*	mainExpressionIdentifier(),codeBlock(),floatVariableLiterals(), integerVariableLiteral(),
*	stringVariableLiteral(), logicalANDExpression(), logicalANDExpressionPrime(), logicalORExpression(), 
*	logicalORExpressionPrime()
********************************************************************************************************************
*/

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif

#ifndef PARSER_H_
#include "Header Files/Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
Null startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 
Null matchToken(Sint tokenCode, Sint tokenAttribute) {
	Sint matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */

Null syncErrorHandler(Sint syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
Null printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ***********************************************************
 */
Null program() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, "START", 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			dataSession();
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ***********************************************************
 */
Null dataSession() {
	matchToken(KW_T, MAIN); //data
	matchToken(LBR_T, NO_ATTR);
	optVarListDeclarations();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
 ***********************************************************
 */
Null optVarListDeclarations() {
	switch (lookahead.code) {
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (CODE)}.
 ***********************************************************
 */
Null codeSession() {
	matchToken(KW_T, MAIN); //CODE
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(IF),
 *				KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
Null optionalStatements() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, "System.writes", 6) == 0)) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
Null statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
Null statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, "System.writes", 6) == 0)) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF), KW_T(WHILE),
 *			KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
Null statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, "System.writes", 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> <assignment expression>
 * FIRST(<assignment statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
Null assignmentStatement() {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


/*
 ************************************************************
 * Assignment Expression
 * BNF: <assignment expression> -> <integer_variable> = <arithmetic expression>
 *						| <float_variable> = <arithmetic expression>
 *						| <string_variable>= <string expression>
 * FIRST(<assignment expression>) = { IVID_T, FVID_T, SVID_T }
 ***********************************************************
 */
Null assignmentExpression() {
	switch (lookahead.code) {
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<output statement>) = { KW_T(WRITE) }
 ***********************************************************
 */
Null outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
Null outputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "NUBI: Output list (string literal) parsed");
		break;
	default:
		printf("%s\n", "NUBI: Output list (empty) parsed");
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

//Functions added
//
/*
 ************************************************************
 * Arithmetic Expression
 * BNF: <arithmetic expression> ->
 *				<unary arithmetic expression> |
 *				<additive arithmetic expression>
 * FIRST:(<arithmetic expression>)
 ***********************************************************
 */
Null arithmeticexpression() {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.codeType == OP_ADD
			|| lookahead.attribute.codeType == OP_SUB) {
			unaryarithmeticexpression();
		}
		else {
			additivearithmeticexpression();
		}
		break;
	default:
		//printError();
		;
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}
/*
 ************************************************************
 * Unary Arithmetic Expression
 * BNF: <unary arithmetic expression> ->
 *				    OP_SUB <primary arithmetic expression> |
 *					OP_ADD <primary arithmetic expression>
 * FIRST:(<unary arithmetic expression>) =
 *					{ OP_ADD, OP_SUB }
 ***********************************************************
 */
Null unaryarithmeticexpression() {

	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.codeType == OP_ADD) {
			matchToken(ART_OP_T, OP_ADD);
			primaryarithmeticexpression();
		}
		else if (lookahead.attribute.codeType == OP_SUB) {
			matchToken(ART_OP_T, OP_SUB);
			primaryarithmeticexpression();
		}
		else {
			printError();
		}
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Unary arithmetic expression parsed");
}
/*
 ************************************************************
 * Additive Arithmetic Expression
 * BNF: <additive arithmetic expression> →
 *					<additive arithmetic expression> OP_ADD
 *					<multiplicative arithmetic expression>
|*					<additive arithmetic expression> OP_SUB
 * FIRST: (<additive arithmetic expression>) =
 *					{ OP_ADD, OP_SUB }
 ***********************************************************
 */
Null additivearithmeticexpression() {
	multiplicativearithmeticexpression();
	additivearithmeticexpressionprime();
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}
/*
 ************************************************************
 * Additive Arithmetic Expression Prime
 * BNF: <additive arithmetic expression> ->
 *					 OP_ADD<additive arithmetic expression>
 *							<additive arithmetic expression prime>
 *					|OP_SUB<additive arithmetic expression>
 *							<additive arithmetic expression prime>
 *					|ϵ
 * FIRST: (<additive arithmetic expression>) =
 *					{ OP_ADD, OP_SUB, ART_OP_T }
 ***********************************************************
 */
Null additivearithmeticexpressionprime() {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.codeType == OP_ADD) {
			matchToken(ART_OP_T, OP_ADD);
			additivearithmeticexpression();
			additivearithmeticexpressionprime();
		}
		else if (lookahead.attribute.codeType == OP_SUB) {
			matchToken(ART_OP_T, OP_SUB);
			additivearithmeticexpression();
			additivearithmeticexpressionprime();
		}
		else {
			printError();
		}
		break;
	default:
		//printError();
		;
		break;
	}
}
/*
 ************************************************************
 * Multiplicative Arithmetic Expression
 * BNF: <multiplicative arithmetic expression> →
 *					multiplicative arithmetic expression>
 *							OP_MUL<primary arithmetic expression>
 *					| <multiplicative arithmetic expression> OP_DIV
 *							<primary arithmetic expression>
 *					| <primary arithmetic expression>
 * FIRST: (<multiplicative arithmetic expression>) =
 *					{ OP_MUL, OP_DIV }
 ***********************************************************
 */
Null multiplicativearithmeticexpression() {
	primaryarithmeticexpression();
	multiplicativearithmeticexpressionprime();
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmetic expression parsed");
}
/*
 ************************************************************
 Multiplicative Arithmetic Expression Prime
 * BNF: <multiplicative arithmetic expression prime> →
 *					OP_MUL<primary arithmetic expression>
 *							<multiplicative arithmetic expression prime>
 *					|OP_DIV <primary arithmetic expression>
 *							<multiplicative arithmetic expression prime>
 *					|ϵ
 * FIRST: (<multiplicative arithmetic expression prime>) =
 *					{ OP_MUL, OP_DIV }
 ***********************************************************
 */
Null multiplicativearithmeticexpressionprime() {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.codeType == OP_MUL) {
			matchToken(ART_OP_T, OP_MUL);
			primaryarithmeticexpression();
			multiplicativearithmeticexpressionprime();
		}
		else if (lookahead.attribute.codeType == OP_DIV) {
			matchToken(ART_OP_T, OP_DIV);
			primaryarithmeticexpression();
			multiplicativearithmeticexpressionprime();
		}
		else {
			printError();
		}
		break;
	default:
		;
		break;
	}
}
/*
 ************************************************************
 * Primary Arithmetic Expression
 * BNF:  <primary arithmetic expression> →
 *					<numerical_value>| <numerical arithmetic expression>
 * FIRST: (<primary arithmetic expression>) =
 *					{ LBR_T, RPR_T }
 ***********************************************************
 */
Null primaryarithmeticexpression() {
	switch (lookahead.code) {
	case LBR_T:
		matchToken(LBR_T, NO_ATTR);
		numericalarithmeticexpressions();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		numericalValues();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary Arithmetic expressions parsed");
}
//
/*
 ************************************************************
 * Selection Statement
 * BNF: <selection_statement> →
 *					IF (<conditional_expression>)
 *					ELSE IF (<conditional_expression>)
 *					ElSE { <opt_statements>
 * FIRST: (<selection_statement>) =
 *					{ IF  , ELSE IF, ELSE }
 ***********************************************************
 */
Null selectionStatement() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == IF) {
			ifprimarySelectionStatement();
			ifSelectionStatement();
		}
		else {
			printError();
		}
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Selection Statement parsed");

}

/*
 ************************************************************
 * IF Primary Selection Statement
 * BNF:<if_primary_selection_statement> ->
 *				IF (<conditional_expression>)
 * FIRST: (<if_primary_selection_statement>) =
 *					{ IF }
 ***********************************************************
 */
Null ifprimarySelectionStatement() {
	matchToken(KW_T, IF);
	matchToken(LBR_T, NO_ATTR);
	matchToken(RBR_T, NO_ATTR);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Primary Selection Statement parsed");
}

/*
 ************************************************************
 *  IF Selection Statement
 * BNF:<if_selection_statement> ->
 *				IF (<conditional_expression>)
 * FIRST: (<if_selection_statement>) =
 *					{ IF }
 ***********************************************************
 */
Null ifSelectionStatement() {
	matchToken(KW_T, IF);
	matchToken(LBR_T, NO_ATTR);
	matchToken(RBR_T, NO_ATTR);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": IF Selection Statement parsed");
}

/*
 ************************************************************
 * ELSE IF Selection Statement
 * BNF:<elseif_selection_statement> ->
 *				ELSE IF (<conditional_expression>)
 * FIRST: (<elseif_selection_statement>) =
 *					{ ELSE IF }
 ***********************************************************
 */
Null elifSelectionStatement() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == ELSEIF) {
			elifPrimarySelectionStatement();
		}
		else {
			printError();
		}
		break;
	default:
		;
		break;
	}
}

/*
 ************************************************************
 * ELSE IF Primary Selection Statement
 * BNF:<elseif_primary_selection_statement> ->
 *				ELSE IF (<conditional_expression>)
 * FIRST: (<elseif__primary_selection_statement>) =
 *					{ ELSE IF }
 ***********************************************************
 */
Null elifPrimarySelectionStatement() {
	matchToken(KW_T, ELSEIF);
	matchToken(LBR_T, NO_ATTR);
	matchToken(RBR_T, NO_ATTR);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": ELSE IF Selection Statement parsed");
}

/*
 ************************************************************
 * ELSE Selection Statement
 * BNF:<else_selection_statement> ->
 *				ELSE (<conditional_expression>)
 * FIRST: (<else_selection_statement>) =
 *					{ ELSE }
 ***********************************************************
 */
Null elseSelectionStatement() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == ELSEIF) {
			elsePrimarySelectionStatement();
		}
		else {
			printError();
		}
		break;
	default:
		;
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": ELSE Selection Statement parsed");
}

/*
 ************************************************************
 * ELSE Primary Selection Statement
 * BNF:<else_primary_selection_statement> ->
 *				ELSE (<conditional_expression>)
 * FIRST: (<else__primary_selection_statement>) =
 *					{ ELSE }
 ***********************************************************
 */
Null elsePrimarySelectionStatement() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == ELSEIF) {
			elseSelectionStatement();
		}
		else {
			printError();
		}
		break;
	default:
		;
		break;
	}
}

/*
 ************************************************************
 * Variable Type Expressions
 * BNF: <Variable type expressions>
 *					<integer_identifier> → KY_W{Sint} VID_T
 *					<float_identifier> → KY_W{floating} VID_T
 *					<character_identifier> → KY_W{chars} VID_T
 *					<double_identifier> → KY_W{dbl} VID_T
 *					<string_identifier> → KY_W{Str} VID_T
 *					<Long_identifier> → KY_W{Long} VID_T
 * FIRST: (<Variable type expressions>) =
 *					{ SINT, FLOATING, CHARS, DBL, STR, LONG }
 ***********************************************************
 */
Null varibleTypeExpression() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == SINT) {
			matchToken(KW_T, SINT);
		}
		if (lookahead.attribute.codeType == CHARS) {
			matchToken(KW_T, CHARS);
		}
		if (lookahead.attribute.codeType == SIGN) {
			matchToken(KW_T, SIGN);
		}
		if (lookahead.attribute.codeType == USIGN) {
			matchToken(KW_T, USIGN);
		}
		if (lookahead.attribute.codeType == DBL) {
			matchToken(KW_T, DBL);
		}
		if (lookahead.attribute.codeType == LONG) {
			matchToken(KW_T, LONG);
		}
		if (lookahead.attribute.codeType == FLOATING) {
			matchToken(KW_T, FLOATING);
		}
		if (lookahead.attribute.codeType == NuLL) {
			matchToken(KW_T, NuLL);
		}
		else {
			printError();
		}
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Variable Type Expression parsed");

}

/*
 ************************************************************
 * Numerical Arithmetic Expression
 ***********************************************************
 */
Null numericalarithmeticexpressions() {
	switch (lookahead.code) {
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		numericalValues();
		arithmeticexpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		numericalValues();
		arithmeticexpression();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Numerical Arithmetic expressions parsed");
}

/*
 ************************************************************
 * Numerical Values
 ***********************************************************
 */
Null numericalValues() {
	switch (lookahead.code) {
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case MNID_T:
		matchToken(MNID_T, NO_ATTR);
		break;
	default:
		;
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Numerical Values parsed");
}

/*
 ************************************************************
 * Main Expression
 * BNF: <NUBI_main>
 *			<main_function_identifier>
 *			<opt_parameter_list><code_block>
 * FIRST: (<NUBI_main>)
 ***********************************************************
 */
Null mainExpression() {
	mainExpressionIdentifier();
	optVarListDeclarations();
	codeBlock();
	printf("%s%s\n", STR_LANGNAME, ": Main Expression Identifer parsed ");
}

/*
 ************************************************************
 * Main Expression Identifier
 * BNF: <main_function_identifier>->
 *					 KW_T { function } <return_type> KW_T { main }
 * FIRST: (<main_function_identifer>)
 ***********************************************************
 */
Null mainExpressionIdentifier() {
	switch (lookahead.code) {
	case KW_T:
		matchToken(KW_T, START);
		matchToken(KW_T, END);
		varibleTypeExpression();
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Main Function parsed ");

}

/*
 ************************************************************
 * CODE BLOCK
 * BNF: code_block> → <opt_statements>
 * FIRST: (code_block>)
 ***********************************************************
 */
Null codeBlock() {
	matchToken(RBR_T, NO_ATTR);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Code Block parsed");

}

/*
 ************************************************************
 * Floating Variable Literal
 * BNF: <float_literal> → KY_W{floating} VID_T
 * FIRST: (<float_literal>)
 ***********************************************************
 */
Null floatVariableLiteral() {
	matchToken(KW_T, FLOATING);
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Floating Variable Literal parsed");
}

/*
 ************************************************************
 * Integer Variable Literal
 * BNF: <integer_literal> → INL_L
 * FIRST: (<integer_literal>)
 ***********************************************************
 */
Null integerVariableLiteral() {
	matchToken(KW_T, SINT);
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer Variable Literal parsed");

}

/*
 ************************************************************
 * String Variable Literal
 * BNF: <string_literal> → STR_T
 * FIRST: (<string_literal>)
 ***********************************************************
 */
Null stringVariableLiteral() {
	matchToken(KW_T, CHARS);
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": String Variable Literal parsed");
}

/*
 ************************************************************
 * Logical AND Expression
 * BNF: <logical AND expression> → <logical NOT expression>
 *								|<logical AND expression>
 *								OP_AND<logical NOT expression>
 * FIRST: (<logical AND expression> ) =
 *								{ OP_AND }
 ***********************************************************
 */
Null logicalANDexpression() {
	//relationExpression();
	matchToken(OP_ADD, NO_ATTR);
	logicalANDexpressionprime();
}

/*
 ************************************************************
 * Logical AND Expression Prime
 * BNF: <logical AND expression> →
 *							<logical NOT expression>|
 *							<logical AND expression prime>
 *							<logical AND expression prime>
 *							OP_AND<logical NOT expression>|
 *							<logical AND expression prime>|ϵ
 * FIRST: (<logical AND expression>) =
 *							{ OP_AND }
 ***********************************************************
 */
Null logicalANDexpressionprime()
{

	switch (lookahead.code)
	{
	case LOG_OP_T:
		switch (lookahead.code)
		{
		case OP_AND:
			matchToken(LOG_OP_T, SINT);
			matchToken(OP_AND, NO_ATTR);
			//relationExpression();
			logicalANDexpressionprime();
			printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");
			break;
		default:
			/*empty*/
			break;
		}

	}
}


/*
 ************************************************************
 * Logical OR Expression
 * BNF: <logical OR expression> →
 *								<logical AND expression>
 *								<logical OR expression> OP_OR
 *								<logical AND expression>
 * FIRST: (<logical OR expression>) =
 *								{ OP_OR }
 ***********************************************************
 */
Null logicalORexpression()
{
	logicalANDexpression();
	logicalORexpressionprime();
}

/*
 ************************************************************
 * Logical OR Expression Prime
 * BNF: <logical OR expression_prime> →
 *								<logical AND expression>|
 *								<logical OR expression>
 *								<logical OR expression prime>OP_OR
 *								<logical AND expression>|
 *								<logical OR expression prime>|ϵ
 * FIRST: (<logical OR expression>) =
 *								{ OP_OR }
 ***********************************************************
 */
Null logicalORexpressionprime()
{
	switch (lookahead.code)
	{
	case LOG_OP_T:

		switch (lookahead.code)
		{
		case OP_OR:
			matchToken(LOG_OP_T, SINT);
			logicalANDexpression();
			logicalORexpressionprime();
			printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");
			break;
		default:
			/*empty*/;
			break;
		}
	}
}

