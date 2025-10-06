#ifndef YY_Y_TAB_H_INCLUDED
# define YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VALUE = 258,
    INTCONST = 259,
    DBLCONST = 260,
    HEXCONST = 261,
    VARIABLE = 262,
    OTHER = 263,
    UMINUS = 264
  };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
  int value;
  int op;
} YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

int yyparse (void *ctx);

#endif /* !YY_Y_TAB_H_INCLUDED  */