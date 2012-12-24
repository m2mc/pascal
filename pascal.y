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
%token <str_value> T_STR_LITERAL
%token T_PROGRAM T_VAR T_BEGIN T_END T_FUNCTION T_ARRAY T_OF
%token T_ASSIGN T_PLUS T_MINUS T_STAR T_SLASH T_OPEN T_CLOSE T_SEMICOL T_COL T_COMMA T_EQ
%token T_GREATER T_LESS T_LEFT_SQ T_RIGHT_SQ
%token T_IF T_THEN T_ELSE T_WHILE T_DO
%token T_TRUE T_FALSE T_NOT

%type <str_values> id_list

%type <expr> expression expression_30 expression_20 expression_10 expression_5 expression_3 simple_expression
%type <expr> expressions optional_expressions codeblock expression_or_block
%type <expr> comma_expressions optional_comma_expressions
%type <expr> if_expression while_expression
%type <expr> dynamic_expression explicit_function_invoke

%type <expr> vardecl varsection vars_and_code
%type <expr> type_expression
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
    T_COL type_expression
    vars_and_code                           { ctxt.get_global().declare(*$2, std::shared_ptr<type>(new invokeable_type(*dynamic_cast<expression_list*>($3), *(new var_declare_expression(*$2, *$5, ctxt)), *$6, ctxt))); }

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
    id_list T_COL type_expression           { expression_list* list = new expression_list();
                                              for (const auto& name : *$1)
                                              {
                                                  expression* decl = new var_declare_expression(name, *$3, ctxt);
                                                  list->push_back(*decl);
                                              }
                                              $$ = list;
                                            }

type_expression:
    T_IDENTIFIER                            { $$ = new primitive_type_expression(*$1); }
    | T_ARRAY T_INTEGER T_OF type_expression
                                            { $$ = new array_type_expression(*$4, $2); }

id_list:
    T_IDENTIFIER                            { $$ = new std::list<std::string>(); $$->push_back(*$1); }
    | id_list T_COMMA T_IDENTIFIER          { $1->push_back(*$3); }

codeblock:
    T_BEGIN optional_expressions T_END      { $$ = $2; }

expression_or_block:
    expression
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
    | while_expression

expression_30:
    expression_20
    | expression_30 T_ASSIGN expression_20  { $$ = new binary_expression(*$1, 'a', *$3); }
    | expression_30 T_EQ expression_20      { $$ = new binary_expression(*$1, '=', *$3); }
    | expression_30 T_GREATER expression_20 { $$ = new binary_expression(*$1, '>', *$3); }
    | expression_30 T_LESS expression_20    { $$ = new binary_expression(*$1, '<', *$3); }

expression_20:
    expression_10
    | expression_20 T_PLUS expression_10    { $$ = new binary_expression(*$1, '+', *$3); }
    | expression_20 T_MINUS expression_10   { $$ = new binary_expression(*$1, '-', *$3); }

expression_10:
    expression_5
    | expression_10 T_STAR expression_5
                                            { $$ = new binary_expression(*$1, '*', *$3); }
    | expression_10 T_SLASH expression_5
                                            { $$ = new binary_expression(*$1, '/', *$3); }
expression_5:
    expression_3
    | T_NOT expression_5                    { $$ = new unary_expression('~', *$2); }

expression_3:
    simple_expression
    | expression_3 T_LEFT_SQ expression T_RIGHT_SQ
                                            { $$ = new binary_expression(*$1, '[', *$3); }
                                            
simple_expression:
    T_INTEGER                               { $$ = new const_expression(std::shared_ptr<type>(new int_type($1))); }
    | T_TRUE                                { $$ = new const_expression(std::shared_ptr<type>(new bool_type(true))); }
    | T_FALSE                               { $$ = new const_expression(std::shared_ptr<type>(new bool_type(false))); }
    | T_STR_LITERAL                         { $$ = new const_expression(std::shared_ptr<type>(new string_type(*$1))); }
    | T_OPEN expression T_CLOSE             { $$ = $2; }
    | dynamic_expression
    | explicit_function_invoke

dynamic_expression:
    T_IDENTIFIER                            { $$ = new dynamic_expression(*$1, ctxt); }

explicit_function_invoke:
    T_IDENTIFIER T_OPEN optional_comma_expressions T_CLOSE
                                            { $$ = new function_invoke_expression(*$1, *dynamic_cast<expression_list*>($3), ctxt); }

if_expression:
    T_IF expression T_THEN codeblock     
                                            { $$ = new if_expression(*$2, *$4); }
    | T_IF expression T_THEN codeblock
    T_ELSE expression_or_block              { $$ = new if_expression(*$2, *$4, *$6); }

while_expression:
    T_WHILE expression T_DO expression_or_block 
                                            { $$ = new while_expression(*$2, *$4); }

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