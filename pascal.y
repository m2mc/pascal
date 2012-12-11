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

    expression<int>* int_expr;
    expression<bool>* bool_expr;
    expression<void>* void_expr;

    lvalue_expression* lvalue_expr;
}

%token <str_value> T_IDENTIFIER
%token <int_value> T_INTEGER
%token T_PROGRAM T_VAR T_BEGIN T_END
%token T_ASSIGN T_PLUS T_MINUS T_STAR T_SLASH T_OPEN T_CLOSE T_SEMICOL T_COL T_COMMA T_EQ
%token T_IF T_THEN
%token T_TRUE T_FALSE

%type <str_values> id_list

%type <int_expr> int_expression prior_int_expression simple_int_expression
%type <bool_expr> bool_expression simple_bool_expression
%type <void_expr> void_expression expressions optional_expressions codeblock expression_or_block
%type <void_expr> if_expression

%type <lvalue_expr> lvalue_expression

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
    id_list T_COL T_IDENTIFIER              { for (std::string& id : *$1) global.put_variable(id, *$3); }

id_list:
    T_IDENTIFIER                            { $$ = new std::list<std::string>(); $$->push_back(*$1); }
    | id_list T_COMMA T_IDENTIFIER          { $1->push_back(*$3); }

codeblock:
    T_BEGIN optional_expressions T_END      { $$ = $2; }

optional_expressions:
    /* empty */                             { $$ = new expression_list(); }
    | expressions

expression_or_block:
//    int_expression                          { $$ = new void_wrap(*$1); }
//    | bool_expression                       { $$ = new void_wrap(*$1); }
    void_expression
    | codeblock

expressions:
    int_expression                          { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | expressions T_SEMICOL int_expression  { dynamic_cast<expression_list*>($$)->push_back(*$3); }
    | bool_expression                       { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | expressions T_SEMICOL bool_expression { dynamic_cast<expression_list*>($$)->push_back(*$3); }
    | void_expression                         { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | expressions T_SEMICOL void_expression { dynamic_cast<expression_list*>($$)->push_back(*$3); }

int_expression:
    prior_int_expression
    | int_expression T_PLUS prior_int_expression
                                            { $$ = new binary_expression<int, int>(*$1, '+', *$3); }
    | int_expression T_MINUS prior_int_expression
                                            { $$ = new binary_expression<int, int>(*$1, '-', *$3); }

prior_int_expression:
    simple_int_expression
    | prior_int_expression T_STAR simple_int_expression
                                            { $$ = new binary_expression<int, int>(*$1, '*', *$3); }
    | prior_int_expression T_SLASH simple_int_expression
                                            { $$ = new binary_expression<int, int>(*$1, '/', *$3); }
simple_int_expression:
    T_INTEGER                               { $$ = new const_expression<int>($1); }
    | T_OPEN int_expression T_CLOSE         { $$ = $2; }

bool_expression:
    simple_bool_expression
    | int_expression T_EQ int_expression    { $$ = new binary_expression<bool, int>(*$1, '=', *$3); }

simple_bool_expression:
    T_TRUE                                  { $$ = new const_expression<bool>(true); }
    | T_FALSE                               { $$ = new const_expression<bool>(false); }
    | T_OPEN bool_expression T_CLOSE        { $$ = $2; }

lvalue_expression:
    T_IDENTIFIER                            { $$ = new lvalue_expression(*$1); }

void_expression:
    lvalue_expression T_ASSIGN int_expression
                                            { $$ = new assign_expression<int>(*$1, *$3); }
    | lvalue_expression T_ASSIGN bool_expression
                                            { $$ = new assign_expression<bool>(*$1, *$3); }
    | if_expression

if_expression:
    T_IF bool_expression T_THEN expression_or_block
                                            { $$ = new if_expression<void>(*$2, *$4); } 

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