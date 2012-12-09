%{
    #include "expression.hpp"
    #include "pascal.hpp"

    #include <map>

    extern int yylex();
    void yyerror(const char *s) { std::cerr << "Grammar error: " << std::string(s) << std::endl; }

    std::map<std::string, lvalue_expression<int>> int_vars;

    void put_variable(const std::string& name, const std::string& value)
    {
        int_vars.insert(std::make_pair(name, lvalue_expression<int>(0)));
    }

    lvalue_expression<int>& get_variable(const std::string& name)
    {
        return int_vars.at(name);
    }
%}

%union {
    std::string* text;
    int int_value;
    expression<int>* int_expr;
    lvalue_expression<int>* lvalue_int_expr;
    expression<void>* void_expr;
}

%token <text> T_IDENTIFIER
%token <int_value> T_INTEGER
%token  T_PROGRAM T_VAR T_BEGIN T_END
%token  T_ASSIGN T_PLUS T_STAR T_OPEN T_CLOSE T_SEMICOL T_COL

%type <int_expr> int_expression prior_int_expression simple_int_expression
%type <lvalue_int_expr> lvalue_int_expression
%type <void_expr> void_expression

%%

program:
    header varsection codeblock

header:
    T_PROGRAM T_IDENTIFIER T_SEMICOL

varsection:
    T_VAR T_COL vardecls

vardecls:
    vardecl T_SEMICOL
    | vardecls vardecl

vardecl:
    T_IDENTIFIER T_COL T_IDENTIFIER         { put_variable(*$1, *$3); }

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
    | lvalue_int_expression
    | T_OPEN int_expression T_CLOSE         { $$ = $2; }

lvalue_int_expression:
    T_IDENTIFIER                          { $$ = &get_variable(*$1); }

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