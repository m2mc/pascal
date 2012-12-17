%{
    #include "expression.hpp"
    #include "pascal.hpp"

    extern "C" int yywrap() {}
%}

%%

[ \t\n]                 /* whitespace */

"program"               return T_PROGRAM;
"var"                   return T_VAR;
"begin"                 return T_BEGIN;
"end"                   return T_END;
"function"              return T_FUNCTION;

"if"                    return T_IF;
"then"                  return T_THEN;

"true"                  return T_TRUE;
"false"                 return T_FALSE;

":="                    return T_ASSIGN;
"+"                     return T_PLUS;
"-"                     return T_MINUS;
"*"                     return T_STAR;
"/"                     return T_SLASH;
"("                     return T_OPEN;
")"                     return T_CLOSE;
";"                     return T_SEMICOL;
":"                     return T_COL;
","                     return T_COMMA;
"="                     return T_EQ;

[a-zA-Z_][a-zA-z_0-9]*  yylval.str_value = new std::string(yytext); return T_IDENTIFIER;
[0-9]+                  yylval.int_value = std::stoi(yytext); return T_INTEGER;

.                       fprintf(stderr, "Unknow token: %s\n", yytext); yyterminate();

%%