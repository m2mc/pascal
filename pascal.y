%{
    #include "expression.hpp"
    #include "context.hpp"
    #include "pascal.hpp"

    #include <map>

    extern int yylex();
    void yyerror(const char *s) { std::cerr << "Grammar error: " << std::string(s) << std::endl; }

    context_manager ctxt;
%}

%union {
    std::string* str_value;
    std::list<std::string>* str_values;
    std::pair<std::list<std::string>, std::string>* declar;

    int int_value;

    expression* expr;
}

%token <str_value> T_IDENTIFIER
%token <int_value> T_INTEGER
%token T_PROGRAM T_VAR T_BEGIN T_END T_FUNCTION
%token T_ASSIGN T_PLUS T_MINUS T_STAR T_SLASH T_OPEN T_CLOSE T_SEMICOL T_COL T_COMMA T_EQ
%token T_IF T_THEN T_ELSE
%token T_TRUE T_FALSE

%type <str_values> id_list

%type <expr> expression expression_30 expression_20 expression_10 simple_expression
%type <expr> expressions optional_expressions codeblock expression_or_block
%type <expr> comma_expressions optional_comma_expressions
%type <expr> if_expression
%type <expr> dynamic_expression explicit_function_invoke

%type <expr> vardecl varsection vars_and_code
%type <expr> signature_entries optional_signature_entries

%%

program:
    header function_decls vars_and_code
                                            { ctxt.put_local();
                                              $3->eval(); }

header:
    T_PROGRAM T_IDENTIFIER T_SEMICOL

function_decls:
    /* empty */
    | function_decls function_decl T_SEMICOL

function_decl:
    T_FUNCTION T_IDENTIFIER
    optional_signature_entries
    T_COL T_IDENTIFIER
    vars_and_code                           { ctxt.get_global().declare(*$2, *(new invokeable_type(*dynamic_cast<expression_list*>($3), *$6, ctxt))); }

vars_and_code:
    varsection codeblock                    { $$ = $1;
                                              dynamic_cast<expression_list*>($$)->push_all(*dynamic_cast<expression_list*>($2)); }
                        
optional_signature_entries:
    /* empty */                             { $$ = new expression_list(); }
    | T_OPEN signature_entries T_CLOSE      { $$ = $2; }

signature_entries:
    vardecl                                 { expression_list* list = new expression_list();
                                              list->push_back(*$1);
                                              $$ = list;
                                            }
    | signature_entries T_COMMA vardecl     { dynamic_cast<expression_list*>($$)->push_back(*$3); }

varsection:
    /* empty */                             { $$ = new expression_list(); }
    | varsection T_VAR vardecl T_SEMICOL    { dynamic_cast<expression_list*>($$)->push_back(*$3); }     

vardecl:
    id_list T_COL T_IDENTIFIER              { expression_list* list = new expression_list();
                                              for (const auto& name : *$1)
                                              {
                                                  expression* decl = new var_declare_expression(name, *$3, ctxt);
                                                  list->push_back(*decl);
                                              }
                                              $$ = list;
                                            }

id_list:
    T_IDENTIFIER                            { $$ = new std::list<std::string>(); $$->push_back(*$1); }
    | id_list T_COMMA T_IDENTIFIER          { $1->push_back(*$3); }

codeblock:
    T_BEGIN optional_expressions T_END      { $$ = $2; }

expression_or_block:
    expression T_SEMICOL
    | codeblock

optional_expressions:
    /* empty */                             { $$ = new expression_list(); }
    | expressions

expressions:
    expression                              { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | expressions T_SEMICOL expression      { dynamic_cast<expression_list*>($$)->push_back(*$3); }

optional_comma_expressions:
    /* empty */                             { $$ = new expression_list(); }
    | comma_expressions                     

comma_expressions:
    expression                              { $$ = new expression_list();
                                              dynamic_cast<expression_list*>($$)->push_back(*$1); }
    | comma_expressions T_COMMA expression  { dynamic_cast<expression_list*>($$)->push_back(*$3); }

expression:
    expression_30
    | if_expression

expression_30:
    expression_20
    | expression_30 T_ASSIGN expression_20  { $$ = new binary_expression(*$1, 'a', *$3); }
    | expression_30 T_EQ expression_20      { $$ = new binary_expression(*$1, '=', *$3); }

expression_20:
    expression_10
    | expression_20 T_PLUS expression_10    { $$ = new binary_expression(*$1, '+', *$3); }
    | expression_20 T_MINUS expression_10   { $$ = new binary_expression(*$1, '-', *$3); }

expression_10:
    simple_expression
    | expression_10 T_STAR simple_expression
                                            { $$ = new binary_expression(*$1, '*', *$3); }
    | expression_10 T_SLASH simple_expression
                                            { $$ = new binary_expression(*$1, '/', *$3); }
simple_expression:
    T_INTEGER                               { $$ = new const_expression(*(new int_type($1))); }
    | T_TRUE                                { $$ = new const_expression(*(new bool_type(true))); }
    | T_FALSE                               { $$ = new const_expression(*(new bool_type(false))); }
    | T_OPEN expression T_CLOSE             { $$ = $2; }
    | dynamic_expression
    | explicit_function_invoke

dynamic_expression:
    T_IDENTIFIER                            { $$ = new dynamic_expression(*$1, ctxt); }

explicit_function_invoke:
    T_IDENTIFIER T_OPEN optional_comma_expressions T_CLOSE
                                            { $$ = new function_invoke_expression(*$1, *dynamic_cast<expression_list*>($3), ctxt); }

if_expression:
    T_IF expression T_THEN expression_or_block     
                                            { $$ = new if_expression(*$2, *$4); }
    | T_IF expression T_THEN expression_or_block
    T_ELSE expression_or_block              { $$ = new if_expression(*$2, *$4, *$6); }
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