%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines

%define api.namespace  { paracl }
%define api.parser.class { Parser }
%define api.token.constructor
%define api.value.type variant
%define parse.error verbose
%define parse.lac full

%glr-parser
%expect-rr 0

%code requires
{
	#include <string>
	#include "frontend/Nodes.hh"
	#include "frontend/ASTBuilder.hh"

	namespace paracl
	{

	class Driver;

	}
}

%code
{
	#include <string>
	#include <iostream>
	#include "frontend/Driver.hh"
	#include "frontend/Types.hh"

	namespace paracl
	{

	inline Parser::symbol_type yylex(paracl::Driver& driver) {
		return driver.getNextToken();
	}

	}
}

%lex-param { paracl::Driver& driver }
%parse-param { paracl::Driver& driver }

%define api.token.prefix {TOKEN_}
%locations
%define api.location.file "location.hh"

%token
	BREAK       "break"
	CONTINUE    "continue"
	ELSE        "else"
	IF          "if"
	WHILE       "while"
	PRINT       "print"
	INT         "int"
	VOID        "void"
	UNDEF       "undef"
	FUNC        "func"
	RETURN      "return"

	ASSIGN      "="
	AND_OP      "&&"
	OR_OP       "||"
	LE_OP       "<="
	GE_OP       ">="
	G_OP        ">"
	L_OP        "<"
	EQ_OP       "=="
	NE_OP       "!="
	SEMICOL     ";"
	COMMA       ","
	COLON       ":"
	LSQUARE     "["
	RSQUARE     "]"
	LPAREN      "("
	RPAREN      ")"
	LCURLY      "{"
	RCURLY      "}"
	NOT_OP      "!"
	QMARK       "?"
	ADD_OP      "+"
	SUB_OP      "-"
	MUL_OP      "*"
	DIV_OP      "/"
	MOD_OP      "%"

	EOF 0       "end of file"

%token
	<int>         CONSTANT   "constant"
	<std::string> IDENTIFIER "identifier"

%type <paracl::Expr*>
	primary_expression
	postfix_expression
	unary_expression
	multiplicative_expression
	additive_expression
	relational_expression
	equality_expression
	logical_and_expression
	logical_or_expression
	conditional_expression
	assignment_expression
	expression
	// constant_expression
	initializer

%type <std::vector<Expr *>>
	argument_expression_list

%type <paracl::InitListExpr*>
	initializer_list

%type <paracl::Stmt*>
	expression_statement
	selection_statement
	iteration_statement
	jump_statement
	print_statement
	statement
	declaration
	function_definition

%type <paracl::CompoundStmt*>
	compound_statement
	statement_list
	translation_unit

%type <paracl::Type*>
	type
	direct_type
	type_specifier
	function_type

%type <std::vector<Type*>>
	type_list

%type <std::vector<paracl::ParmVarDecl*>>
	parameter_list

%type <paracl::ParmVarDecl*>
	parameter_declaration

%nonassoc IFX
%nonassoc ELSE

%start translation_unit

%%

	primary_expression
		: IDENTIFIER { $$ = driver.createNode<DeclRefExpr>(@1, $1); }
		| UNDEF { $$ = driver.createNode<UndefLiteral>(@1); }
		| CONSTANT { $$ = driver.createNode<IntegerLiteral>(@1, $1); }
		| QMARK {
			auto expr = driver.createNode<DeclRefExpr>(@1, "scan");
			$$ = driver.createNode<CallExpr>(@1, expr);
		}
		| LPAREN expression RPAREN  { $$ = $2; }

	postfix_expression
		: primary_expression { $$ = $1; }
		| postfix_expression LSQUARE expression RSQUARE {
			$$ = driver.createNode<ArraySubscriptExpr>(@2, $1, $3);
		}
		| postfix_expression LPAREN RPAREN {
			$$ = driver.createNode<CallExpr>(@1, $1);
		}
		| postfix_expression LPAREN argument_expression_list RPAREN {
			auto call = driver.createNode<CallExpr>(@1, $1);
			for (auto&& param : $3)
				call->addParam(param);
			$$ = call;
		}

	argument_expression_list
		: assignment_expression {
			auto exprs = std::vector<Expr *>{};
			exprs.push_back($1);
			$$ = exprs;
		}
		| argument_expression_list COMMA assignment_expression {
			$1.push_back($3);
			$$ = $1;
		}

	unary_expression
		: postfix_expression        { $$ = $1; }
		| ADD_OP unary_expression   { $$ = driver.createNode<UnaryOperator>(@$, UnaryOpcode::UN_ADD, $2); }
		| SUB_OP unary_expression   { $$ = driver.createNode<UnaryOperator>(@$, UnaryOpcode::UN_SUB, $2); }
		| NOT_OP unary_expression   { $$ = driver.createNode<UnaryOperator>(@$, UnaryOpcode::UN_NOT, $2); }

	multiplicative_expression
		: unary_expression                                  { $$ = $1; }
		| multiplicative_expression MUL_OP unary_expression { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_MUL, $1, $3); }
		| multiplicative_expression DIV_OP unary_expression { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_DIV, $1, $3); }
		| multiplicative_expression MOD_OP unary_expression { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_MOD, $1, $3); }

	additive_expression
		: multiplicative_expression                             { $$ = $1; }
		| additive_expression ADD_OP multiplicative_expression  { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_ADD, $1, $3); }
		| additive_expression SUB_OP multiplicative_expression  { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_SUB, $1, $3); }

	relational_expression
		: additive_expression                               { $$ = $1; }
		| relational_expression L_OP additive_expression    { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_L, $1, $3); }
		| relational_expression G_OP additive_expression    { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_G, $1, $3); }
		| relational_expression LE_OP additive_expression   { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_LE, $1, $3); }
		| relational_expression GE_OP additive_expression   { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_GE, $1, $3); }

	equality_expression
		: relational_expression                             { $$ = $1; }
		| equality_expression EQ_OP relational_expression   { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_EQ, $1, $3); }
		| equality_expression NE_OP relational_expression   { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_NE, $1, $3); }

	logical_and_expression
		: equality_expression                               { $$ = $1; }
		| logical_and_expression AND_OP equality_expression { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_AND, $1, $3); }

	logical_or_expression
		: logical_and_expression                                { $$ = $1; }
		| logical_or_expression OR_OP logical_and_expression    { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_OR, $1, $3); }

	conditional_expression
		: logical_or_expression                                                 { $$ = $1; }
		| logical_or_expression QMARK expression COLON conditional_expression   { $$ = driver.createNode<ConditionalOperator>(@$, $1, $3, $5); }

	assignment_expression
		: conditional_expression                        { $$ = $1; }
		| unary_expression ASSIGN assignment_expression { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_ASSIGN, $1, $3); }

	expression
		: assignment_expression                     { $$ = $1; }
		| expression COMMA assignment_expression    { $$ = driver.createNode<BinaryOperator>(@$, BinaryOpcode::BIN_COMMA, $1, $3); }

	// constant_expression
		// : conditional_expression { $$ = $1; }

	declaration
		: type IDENTIFIER ASSIGN initializer SEMICOL {
			auto decl = driver.createNode<VarDecl>(@$, $2);
			decl->setType($1);
			decl->setExpr($4);
			auto stmt = driver.createNode<DeclStmt>(@$);
			stmt->addDecl(decl);
			$$ = stmt;
		}

	type
		: direct_type {
			$$ = $1;
		} | function_type {
			$$ = $1;
		}

	direct_type
		: type_specifier {
			$$ = $1;
		} | direct_type LSQUARE CONSTANT RSQUARE {
			$$ = driver.createArrayType($1, $3);
		}

	type_specifier
		: INT   { $$ = driver.createIntType(); }
		| VOID  { $$ = driver.createVoidType(); }

	function_type
		: direct_type FUNC LPAREN RPAREN {
			auto paramTypes = driver.createTupleType({});
			$$ = driver.createFunctionType($1, paramTypes);
		} | direct_type FUNC LPAREN type_list RPAREN {
			auto tuple = driver.createTupleType(std::move($4));
			$$ = driver.createFunctionType($1, tuple);
		}

	type_list
		: type { auto types = std::vector<Type*>{}; types.push_back($1); $$ = types; }
		| type_list COMMA type { $1.push_back($3); $$ = $1; }

	function_definition
		:  direct_type FUNC LPAREN RPAREN COLON IDENTIFIER compound_statement {
			auto paramTypes = driver.createTupleType({});
			auto type = driver.createFunctionType($1, paramTypes);
			auto func = driver.createNode<FunctionDecl>(@$, $6);
			func->setType(type);
			func->setBody($7);
			auto stmt = driver.createNode<DeclStmt>(@$);
			stmt->addDecl(func);
			$$ = stmt;
		} | direct_type FUNC LPAREN parameter_list RPAREN COLON IDENTIFIER compound_statement {
			auto func = driver.createNode<FunctionDecl>(@$, $7);
			std::vector<paracl::Type*> types{};
			std::transform($4.begin(), $4.end(), std::back_inserter(types), [=](auto&& param) {
				func->addParam(param);
				return param->getType();
			});
			auto paramTypes = driver.createTupleType(std::move(types));
			auto type = driver.createFunctionType($1, paramTypes);
			func->setType(type);
			func->setBody($8);
			auto stmt = driver.createNode<DeclStmt>(@$);
			stmt->addDecl(func);
			$$ = stmt;
		}

	parameter_list
		: parameter_declaration { auto parms = std::vector<ParmVarDecl*>{}; parms.push_back($1); $$ = parms; }
		| parameter_list parameter_declaration { $1.push_back($2); $$ = $1; }

	parameter_declaration
		: type IDENTIFIER { auto parm = driver.createNode<ParmVarDecl>(@$, $2); parm->setType($1); $$ = parm; }

	initializer
		: assignment_expression             { $$ = $1; }
		| LCURLY initializer_list RCURLY    { $$ = $2; }

	initializer_list
		: initializer                         { auto list = driver.createNode<InitListExpr>(@$); list->addExpr($1); $$ = list; }
		| initializer_list COMMA initializer  { $1->addExpr($3); $$ = $1; }

	statement
		: compound_statement        { $$ = $1; }
		| expression_statement      { $$ = $1; }
		| selection_statement       { $$ = $1; }
		| iteration_statement       { $$ = $1; }
		| jump_statement            { $$ = $1; }
		| declaration 				{ $$ = $1; }
		| function_definition 		{ $$ = $1; }
		| print_statement			{ $$ = $1; }

	compound_statement
		:// LCURLY RCURLY { $$ = driver.createNode<CompoundStmt>(@$); }
		//|
		{
			auto stmt = driver.createNode<CompoundStmt>(@$);
			driver.pushCompoundStmt(stmt);
		} LCURLY statement_list RCURLY {
			driver.popCompoundStmt();
			$$ = $3;
		}

	statement_list
		: statement                     { auto stmt = driver.getCompoundStmt(); stmt->addStmt($1); $$ = stmt; }
		| statement_list statement      { $1->addStmt($2); $$ = $1; }
		| statement_list error SEMICOL  { $$ = $1; yyerrok; }

	expression_statement
		: SEMICOL {}
		| expression SEMICOL { $$ = driver.createNode<ValueStmt>(@$, $1); }

	selection_statement
		: IF LPAREN expression RPAREN statement %prec IFX       { $$ = driver.createNode<IfStmt>(@$, $3, $5); }
		| IF LPAREN expression RPAREN statement ELSE statement  { $$ = driver.createNode<IfStmt>(@$, $3, $5, $7); }

	iteration_statement
		: WHILE LPAREN expression RPAREN statement  { $$ = driver.createNode<WhileStmt>(@$, $3, $5); }

	jump_statement
		: CONTINUE SEMICOL  { $$ = driver.createNode<ContinueStmt>(@$); }
		| BREAK SEMICOL     { $$ = driver.createNode<BreakStmt>(@$); }
		| RETURN expression SEMICOL { $$ = driver.createNode<ReturnStmt>(@$, $2); }

	print_statement
		: PRINT expression SEMICOL	{ $$ = driver.createNode<OutputStmt>(@$, $2); }

	translation_unit
		: {
			auto stmt = driver.createNode<CompoundStmt>(@$);
			driver.pushCompoundStmt(stmt);
		} statement_list {
			driver.popCompoundStmt();
			driver.setRoot($2);
			$$ = $2;
		}
%%

void paracl::Parser::error(const location& loc, const std::string& msg) {
	driver.reportError<Syntax>(loc, msg);
}
