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
    lvalue_expression<int>* lvalue_int_expr;
    expression<void>* void_expr;
}

%token <str_value> T_IDENTIFIER
%token <int_value> T_INTEGER
%token  T_PROGRAM T_VAR T_BEGIN T_END
%token  T_ASSIGN T_PLUS T_STAR T_OPEN T_CLOSE T_SEMICOL T_COL T_COMMA

%type <str_values> id_list
%type <int_expr> int_expression prior_int_expression simple_int_expression
%type <lvalue_int_expr> lvalue_int_expression
%type <void_expr> void_expression

%%

program:
    header optional_varsection codeblock

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
    T_IDENTIFIER                            { $$ = new std::list<std::string>(); $$->push_front(*$1); }
    | id_list T_COMMA T_IDENTIFIER          { $1->push_front(*$3); }

codeblock:
    T_BEGIN optional_expressions T_END

optional_expressions:
    /* empty */
    | expressions

expressions:
    expression
    | expressions T_SEMICOL expression

expression:
    void_expression                         { std::cerr << "void expression" << std::endl; $1->eval(); }
    | int_expression                        { std::cerr << "int expression: " << $1->eval() << std::endl; }

int_expression:
    prior_int_expression
    | int_expression T_PLUS prior_int_expression
                                            { $$ = new binary_expression<int>(*$1, '+', *$3); }

prior_int_expression:
    simple_int_expression
    | prior_int_expression T_STAR simple_int_expression
                                            { $$ = new binary_expression<int>(*$1, '*', *$3); }
simple_int_expression:
    T_INTEGER                               { $$ = new const_expression<int>($1); }
    | lvalue_int_expression                 { $$ = $1; /* to avoid warning */ }
    | T_OPEN int_expression T_CLOSE         { $$ = $2; }

lvalue_int_expression:
    T_IDENTIFIER                          { $$ = &global.get_variable<int>(*$1); }

void_expression:
    lvalue_int_expression T_ASSIGN int_expression    { $$ = new assign_expression<int>(*$1, *$3); }

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