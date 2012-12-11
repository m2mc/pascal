%{
    #include "expression.hpp"
    #include "context.hpp"
    #include "pascal.hpp"

    #include <map>

    extern int yylex();
    void yyerror(const char *s) { std::cerr << "Grammar error: " << std::string(s) << std::endl; }

    context global;
%}

%union {
    std::string* str_value;
    std::list<std::string>* str_values;

    int int_value;

    expression* expr;
}

%token <str_value> T_IDENTIFIER
%token <int_value> T_INTEGER
%token T_PROGRAM T_VAR T_BEGIN T_END
%token T_ASSIGN T_PLUS T_MINUS T_STAR T_SLASH T_OPEN T_CLOSE T_SEMICOL T_COL T_COMMA T_EQ
%token T_IF T_THEN
%token T_TRUE T_FALSE

%type <str_values> id_list

%type <expr> expression prior_expression simple_expression
%type <expr> expressions optional_expressions codeblock expression_or_block
%type <expr> if_expression
%type <expr> dynamic_expression

%%

program:
    header optional_varsection codeblock    { $3->eval(); }

header:
    T_PROGRAM T_IDENTIFIER T_SEMICOL

optional_varsection:
    /* empty */
    | varsection

varsection:
    T_VAR T_COL vardecls

vardecls:
    vardecl
    | vardecls T_SEMICOL vardecl

vardecl:
    id_list T_COL T_IDENTIFIER              { for (std::string& id : *$1) global.put(id, *$3); }

id_list:
    T_IDENTIFIER                            { $$ = new std::list<std::string>(); $$->push_back(*$1); }
    | id_list T_COMMA T_IDENTIFIER          { $1->push_back(*$3); }

codeblock:
    T_BEGIN optional_expressions T_END      { $$ = $2; }

optional_expressions:
    /* empty */                             { $$ = new expression_list(); }
    | expressions

expression_or_block:
    expression T_SEMICOL
    | codeblock

expressions:
    expression                              { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | expressions T_SEMICOL expression      { dynamic_cast<expression_list*>($$)->push_back(*$3); }
    
expression:
    prior_expression
    | expression T_ASSIGN prior_expression  { $$ = new binary_expression(*$1, 'a', *$3); }
    | expression T_EQ prior_expression      { $$ = new binary_expression(*$1, '=', *$3); }
    | expression T_PLUS prior_expression    { $$ = new binary_expression(*$1, '+', *$3); }
    | expression T_MINUS prior_expression   { $$ = new binary_expression(*$1, '-', *$3); }
    | if_expression

prior_expression:
    simple_expression
    | prior_expression T_STAR simple_expression
                                            { $$ = new binary_expression(*$1, '*', *$3); }
    | prior_expression T_SLASH simple_expression
                                            { $$ = new binary_expression(*$1, '/', *$3); }
simple_expression:
    T_INTEGER                               { $$ = new const_expression(*(new int_type($1))); }
    | T_TRUE                                { $$ = new const_expression(*(new bool_type(true))); }
    | T_FALSE                               { $$ = new const_expression(*(new bool_type(false))); }
    | T_OPEN expression T_CLOSE             { $$ = $2; }
    | dynamic_expression

dynamic_expression:
    T_IDENTIFIER                            { $$ = new dynamic_expression(*$1, global); }

if_expression:
    T_IF expression T_THEN expression_or_block     
                                            { $$ = new if_expression(*$2, *$4); }

%%

int main(int argc, char** argv)
{
    try
    {
        yyparse();
    }
    catch(std::exception& ex)
    {
        std::cerr   << "Runtime exception:" << std::endl
                    << ex.what() << std::endl;
    }
    return 0;
}