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

[a-zA-Z_][a-zA-z_0-9]*  yylval.text = new std::string(yytext); return T_IDENTIFIER;
[0-9]+                  yylval.int_value = std::stoi(yytext); return T_INTEGER;

":="                    return T_ASSIGN;
"+"                     return T_PLUS;
"*"                     return T_STAR;
"("                     return T_OPEN;
")"                     return T_CLOSE;
";"                     return T_SEMICOL;
":"                     return T_COL;
.                       fprintf(stderr, "Unknow token: %s\n", yytext); yyterminate();

%%