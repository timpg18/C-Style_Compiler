/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/grammar.y"

int yylex();
void yyerror(const char *s);
#include <stdio.h>

extern int line_no;
extern void print_errors();
extern void print_symtab();
extern int error_count;
extern void update_symtab(char* ttoken);
extern int symcount;
extern char* yytext;

typedef struct{
	char* token;
	char* token_type;
}token_table;

#define MAX_TOKENS 20000

int parser_errror=0;

token_table tokenTable[MAX_TOKENS];
int token_count=0;
/* Global variable to store the current elementary type */
char *currentType = NULL;

void add_to_token_table(const char *token, const char *token_type) {
    if (token_count < MAX_TOKENS) {
        tokenTable[token_count].token = strdup(token);
        tokenTable[token_count].token_type = strdup(token_type);
		if (strstr(token_type, "typedef") != NULL){
			update_symtab(tokenTable[token_count].token);
		  }
        token_count++;
    }
}

void print_token_table() {
    printf("\n=================================================================\n");
    printf("                          TOKEN TABLE\n");
    printf("=================================================================\n");
    printf("| %-29s | %-29s |\n", "Token", "Token_Type");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < token_count; i++) {
        printf("| %-29s | %-29s |\n", 
               tokenTable[i].token, tokenTable[i].token_type);
    }
    printf("=================================================================\n");
}

#line 123 "y.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "y.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_I_CONSTANT = 4,                 /* I_CONSTANT  */
  YYSYMBOL_F_CONSTANT = 5,                 /* F_CONSTANT  */
  YYSYMBOL_STRING_LIT = 6,                 /* STRING_LIT  */
  YYSYMBOL_CHAR_LIT = 7,                   /* CHAR_LIT  */
  YYSYMBOL_PTR_OP = 8,                     /* PTR_OP  */
  YYSYMBOL_INC_OP = 9,                     /* INC_OP  */
  YYSYMBOL_DEC_OP = 10,                    /* DEC_OP  */
  YYSYMBOL_LEFT_OP = 11,                   /* LEFT_OP  */
  YYSYMBOL_RIGHT_OP = 12,                  /* RIGHT_OP  */
  YYSYMBOL_LE_OP = 13,                     /* LE_OP  */
  YYSYMBOL_GE_OP = 14,                     /* GE_OP  */
  YYSYMBOL_EQ_OP = 15,                     /* EQ_OP  */
  YYSYMBOL_NE_OP = 16,                     /* NE_OP  */
  YYSYMBOL_AND_OP = 17,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 18,                     /* OR_OP  */
  YYSYMBOL_MUL_ASSIGN = 19,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 20,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 21,                /* MOD_ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 22,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 23,                /* SUB_ASSIGN  */
  YYSYMBOL_LEFT_ASSIGN = 24,               /* LEFT_ASSIGN  */
  YYSYMBOL_RIGHT_ASSIGN = 25,              /* RIGHT_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 26,                /* AND_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 27,                /* XOR_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 28,                 /* OR_ASSIGN  */
  YYSYMBOL_TYPEDEF_NAME = 29,              /* TYPEDEF_NAME  */
  YYSYMBOL_TYPEDEF = 30,                   /* TYPEDEF  */
  YYSYMBOL_EXTERN = 31,                    /* EXTERN  */
  YYSYMBOL_STATIC = 32,                    /* STATIC  */
  YYSYMBOL_AUTO = 33,                      /* AUTO  */
  YYSYMBOL_REGISTER = 34,                  /* REGISTER  */
  YYSYMBOL_SIZEOF = 35,                    /* SIZEOF  */
  YYSYMBOL_CHAR = 36,                      /* CHAR  */
  YYSYMBOL_SHORT = 37,                     /* SHORT  */
  YYSYMBOL_INT = 38,                       /* INT  */
  YYSYMBOL_LONG = 39,                      /* LONG  */
  YYSYMBOL_SIGNED = 40,                    /* SIGNED  */
  YYSYMBOL_UNSIGNED = 41,                  /* UNSIGNED  */
  YYSYMBOL_FLOAT = 42,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 43,                    /* DOUBLE  */
  YYSYMBOL_CONST = 44,                     /* CONST  */
  YYSYMBOL_VOLATILE = 45,                  /* VOLATILE  */
  YYSYMBOL_VOID = 46,                      /* VOID  */
  YYSYMBOL_BOOL = 47,                      /* BOOL  */
  YYSYMBOL_STRUCT = 48,                    /* STRUCT  */
  YYSYMBOL_UNION = 49,                     /* UNION  */
  YYSYMBOL_ENUM = 50,                      /* ENUM  */
  YYSYMBOL_CASE = 51,                      /* CASE  */
  YYSYMBOL_DEFAULT = 52,                   /* DEFAULT  */
  YYSYMBOL_IF = 53,                        /* IF  */
  YYSYMBOL_ELSE = 54,                      /* ELSE  */
  YYSYMBOL_SWITCH = 55,                    /* SWITCH  */
  YYSYMBOL_WHILE = 56,                     /* WHILE  */
  YYSYMBOL_DO = 57,                        /* DO  */
  YYSYMBOL_FOR = 58,                       /* FOR  */
  YYSYMBOL_GOTO = 59,                      /* GOTO  */
  YYSYMBOL_CONTINUE = 60,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 61,                     /* BREAK  */
  YYSYMBOL_RETURN = 62,                    /* RETURN  */
  YYSYMBOL_UNTIL = 63,                     /* UNTIL  */
  YYSYMBOL_FUNC_NAME = 64,                 /* FUNC_NAME  */
  YYSYMBOL_ENUMERATION_CONSTANT = 65,      /* ENUMERATION_CONSTANT  */
  YYSYMBOL_CLASS = 66,                     /* CLASS  */
  YYSYMBOL_PUBLIC = 67,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 68,                   /* PRIVATE  */
  YYSYMBOL_PROTECTED = 69,                 /* PROTECTED  */
  YYSYMBOL_70_ = 70,                       /* '('  */
  YYSYMBOL_71_ = 71,                       /* ')'  */
  YYSYMBOL_72_ = 72,                       /* '['  */
  YYSYMBOL_73_ = 73,                       /* ']'  */
  YYSYMBOL_74_ = 74,                       /* '.'  */
  YYSYMBOL_75_ = 75,                       /* '{'  */
  YYSYMBOL_76_ = 76,                       /* '}'  */
  YYSYMBOL_77_ = 77,                       /* ','  */
  YYSYMBOL_78_ = 78,                       /* '&'  */
  YYSYMBOL_79_ = 79,                       /* '*'  */
  YYSYMBOL_80_ = 80,                       /* '+'  */
  YYSYMBOL_81_ = 81,                       /* '-'  */
  YYSYMBOL_82_ = 82,                       /* '~'  */
  YYSYMBOL_83_ = 83,                       /* '!'  */
  YYSYMBOL_84_ = 84,                       /* '/'  */
  YYSYMBOL_85_ = 85,                       /* '%'  */
  YYSYMBOL_86_ = 86,                       /* '<'  */
  YYSYMBOL_87_ = 87,                       /* '>'  */
  YYSYMBOL_88_ = 88,                       /* '^'  */
  YYSYMBOL_89_ = 89,                       /* '|'  */
  YYSYMBOL_90_ = 90,                       /* '?'  */
  YYSYMBOL_91_ = 91,                       /* ':'  */
  YYSYMBOL_92_ = 92,                       /* '='  */
  YYSYMBOL_93_ = 93,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 94,                  /* $accept  */
  YYSYMBOL_primary_expression = 95,        /* primary_expression  */
  YYSYMBOL_constant = 96,                  /* constant  */
  YYSYMBOL_enumeration_constant = 97,      /* enumeration_constant  */
  YYSYMBOL_string = 98,                    /* string  */
  YYSYMBOL_postfix_expression = 99,        /* postfix_expression  */
  YYSYMBOL_argument_expression_list = 100, /* argument_expression_list  */
  YYSYMBOL_unary_expression = 101,         /* unary_expression  */
  YYSYMBOL_unary_operator = 102,           /* unary_operator  */
  YYSYMBOL_cast_expression = 103,          /* cast_expression  */
  YYSYMBOL_multiplicative_expression = 104, /* multiplicative_expression  */
  YYSYMBOL_additive_expression = 105,      /* additive_expression  */
  YYSYMBOL_shift_expression = 106,         /* shift_expression  */
  YYSYMBOL_relational_expression = 107,    /* relational_expression  */
  YYSYMBOL_equality_expression = 108,      /* equality_expression  */
  YYSYMBOL_and_expression = 109,           /* and_expression  */
  YYSYMBOL_exclusive_or_expression = 110,  /* exclusive_or_expression  */
  YYSYMBOL_inclusive_or_expression = 111,  /* inclusive_or_expression  */
  YYSYMBOL_logical_and_expression = 112,   /* logical_and_expression  */
  YYSYMBOL_logical_or_expression = 113,    /* logical_or_expression  */
  YYSYMBOL_conditional_expression = 114,   /* conditional_expression  */
  YYSYMBOL_assignment_expression = 115,    /* assignment_expression  */
  YYSYMBOL_assignment_operator = 116,      /* assignment_operator  */
  YYSYMBOL_expression = 117,               /* expression  */
  YYSYMBOL_constant_expression = 118,      /* constant_expression  */
  YYSYMBOL_declaration = 119,              /* declaration  */
  YYSYMBOL_declaration_specifiers = 120,   /* declaration_specifiers  */
  YYSYMBOL_init_declarator_list = 121,     /* init_declarator_list  */
  YYSYMBOL_init_declarator = 122,          /* init_declarator  */
  YYSYMBOL_storage_class_specifier = 123,  /* storage_class_specifier  */
  YYSYMBOL_type_specifier = 124,           /* type_specifier  */
  YYSYMBOL_class_specifier = 125,          /* class_specifier  */
  YYSYMBOL_class_member_list = 126,        /* class_member_list  */
  YYSYMBOL_class_member = 127,             /* class_member  */
  YYSYMBOL_access_specifier = 128,         /* access_specifier  */
  YYSYMBOL_base_clause_opt = 129,          /* base_clause_opt  */
  YYSYMBOL_base_specifier_list = 130,      /* base_specifier_list  */
  YYSYMBOL_base_specifier = 131,           /* base_specifier  */
  YYSYMBOL_access_specifier_opt = 132,     /* access_specifier_opt  */
  YYSYMBOL_struct_or_union_specifier = 133, /* struct_or_union_specifier  */
  YYSYMBOL_struct_or_union = 134,          /* struct_or_union  */
  YYSYMBOL_struct_declaration_list = 135,  /* struct_declaration_list  */
  YYSYMBOL_struct_declaration = 136,       /* struct_declaration  */
  YYSYMBOL_specifier_qualifier_list = 137, /* specifier_qualifier_list  */
  YYSYMBOL_struct_declarator_list = 138,   /* struct_declarator_list  */
  YYSYMBOL_struct_declarator = 139,        /* struct_declarator  */
  YYSYMBOL_enum_specifier = 140,           /* enum_specifier  */
  YYSYMBOL_enumerator_list = 141,          /* enumerator_list  */
  YYSYMBOL_enumerator = 142,               /* enumerator  */
  YYSYMBOL_type_qualifier = 143,           /* type_qualifier  */
  YYSYMBOL_declarator = 144,               /* declarator  */
  YYSYMBOL_direct_declarator = 145,        /* direct_declarator  */
  YYSYMBOL_pointer = 146,                  /* pointer  */
  YYSYMBOL_type_qualifier_list = 147,      /* type_qualifier_list  */
  YYSYMBOL_parameter_type_list = 148,      /* parameter_type_list  */
  YYSYMBOL_parameter_list = 149,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 150,    /* parameter_declaration  */
  YYSYMBOL_identifier_list = 151,          /* identifier_list  */
  YYSYMBOL_type_name = 152,                /* type_name  */
  YYSYMBOL_abstract_declarator = 153,      /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 154, /* direct_abstract_declarator  */
  YYSYMBOL_initializer = 155,              /* initializer  */
  YYSYMBOL_initializer_list = 156,         /* initializer_list  */
  YYSYMBOL_designation = 157,              /* designation  */
  YYSYMBOL_designator_list = 158,          /* designator_list  */
  YYSYMBOL_designator = 159,               /* designator  */
  YYSYMBOL_statement = 160,                /* statement  */
  YYSYMBOL_labeled_statement = 161,        /* labeled_statement  */
  YYSYMBOL_compound_statement = 162,       /* compound_statement  */
  YYSYMBOL_block_item_list = 163,          /* block_item_list  */
  YYSYMBOL_block_item = 164,               /* block_item  */
  YYSYMBOL_expression_statement = 165,     /* expression_statement  */
  YYSYMBOL_selection_statement = 166,      /* selection_statement  */
  YYSYMBOL_iteration_statement = 167,      /* iteration_statement  */
  YYSYMBOL_jump_statement = 168,           /* jump_statement  */
  YYSYMBOL_translation_unit = 169,         /* translation_unit  */
  YYSYMBOL_external_declaration = 170,     /* external_declaration  */
  YYSYMBOL_function_definition = 171,      /* function_definition  */
  YYSYMBOL_declaration_list = 172          /* declaration_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  55
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2128

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  94
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  79
/* YYNRULES -- Number of rules.  */
#define YYNRULES  277
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  472

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   324


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,     2,     2,     2,    85,    78,     2,
      70,    71,    79,    80,    77,    81,    74,    84,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    91,    93,
      86,    92,    87,    90,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    72,     2,    73,    88,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    75,    89,    76,    82,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    96,    96,    97,    98,    99,   100,   104,   105,   106,
     110,   114,   115,   116,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   134,   135,   139,   140,   141,   142,
     143,   144,   148,   149,   150,   151,   152,   153,   157,   158,
     159,   163,   164,   165,   166,   170,   171,   172,   176,   177,
     178,   182,   183,   184,   185,   186,   190,   191,   192,   196,
     197,   201,   202,   206,   207,   211,   212,   216,   217,   221,
     222,   226,   227,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   245,   246,   250,   254,   255,   256,
     257,   261,   270,   274,   283,   288,   296,   300,   305,   306,
     310,   311,   315,   316,   317,   318,   319,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   340,   345,   351,   360,   361,   365,   370,   371,   372,
     376,   377,   381,   382,   393,   409,   410,   414,   419,   424,
     432,   433,   437,   438,   442,   443,   447,   448,   449,   450,
     454,   455,   459,   460,   461,   465,   469,   472,   478,   483,
     492,   493,   497,   498,   502,   503,   509,   522,   526,   530,
     531,   542,   543,   544,   545,   546,   547,   548,   549,   560,
     566,   572,   578,   587,   588,   589,   597,   603,   604,   612,
     616,   617,   621,   622,   623,   627,   628,   632,   633,   637,
     638,   639,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   667,   668,   669,   673,   674,   675,   676,
     680,   684,   685,   689,   690,   694,   695,   696,   697,   698,
     699,   703,   704,   705,   709,   710,   711,   715,   716,   720,
     721,   725,   726,   727,   731,   732,   733,   737,   738,   739,
     740,   741,   742,   743,   747,   748,   749,   750,   751,   755,
     756,   760,   761,   765,   766,   767,   771,   772
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "I_CONSTANT", "F_CONSTANT", "STRING_LIT", "CHAR_LIT", "PTR_OP", "INC_OP",
  "DEC_OP", "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP",
  "AND_OP", "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN",
  "XOR_ASSIGN", "OR_ASSIGN", "TYPEDEF_NAME", "TYPEDEF", "EXTERN", "STATIC",
  "AUTO", "REGISTER", "SIZEOF", "CHAR", "SHORT", "INT", "LONG", "SIGNED",
  "UNSIGNED", "FLOAT", "DOUBLE", "CONST", "VOLATILE", "VOID", "BOOL",
  "STRUCT", "UNION", "ENUM", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH",
  "WHILE", "DO", "FOR", "GOTO", "CONTINUE", "BREAK", "RETURN", "UNTIL",
  "FUNC_NAME", "ENUMERATION_CONSTANT", "CLASS", "PUBLIC", "PRIVATE",
  "PROTECTED", "'('", "')'", "'['", "']'", "'.'", "'{'", "'}'", "','",
  "'&'", "'*'", "'+'", "'-'", "'~'", "'!'", "'/'", "'%'", "'<'", "'>'",
  "'^'", "'|'", "'?'", "':'", "'='", "';'", "$accept",
  "primary_expression", "constant", "enumeration_constant", "string",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "unary_operator", "cast_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_or_expression", "conditional_expression",
  "assignment_expression", "assignment_operator", "expression",
  "constant_expression", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "class_specifier", "class_member_list", "class_member",
  "access_specifier", "base_clause_opt", "base_specifier_list",
  "base_specifier", "access_specifier_opt", "struct_or_union_specifier",
  "struct_or_union", "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "declarator", "direct_declarator", "pointer",
  "type_qualifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "identifier_list", "type_name",
  "abstract_declarator", "direct_abstract_declarator", "initializer",
  "initializer_list", "designation", "designator_list", "designator",
  "statement", "labeled_statement", "compound_statement",
  "block_item_list", "block_item", "expression_statement",
  "selection_statement", "iteration_statement", "jump_statement",
  "translation_unit", "external_declaration", "function_definition",
  "declaration_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-329)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-102)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    2031,  -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,
    -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,
    -329,    62,    66,  -329,    40,  2031,  2031,  -329,  -329,    73,
    -329,  2031,  1750,  -329,  -329,   -50,    52,   -23,   237,  -329,
    -329,    53,  -329,   134,  -329,    38,  -329,   525,   139,    20,
       3,     3,    18,  2062,     3,  -329,  -329,    52,  -329,    21,
     195,  -329,   237,    46,  -329,  -329,  -329,   150,  -329,    69,
      84,  -329,  -329,   134,  -329,    53,  -329,  -329,   600,  1343,
    -329,    40,  -329,  1948,  1699,   896,   139,  2062,  2062,  1866,
    -329,    72,  2062,   231,  1511,  -329,    26,  -329,    92,  -329,
     183,   237,  -329,  -329,   102,  -329,  -329,  -329,  -329,   104,
     130,  -329,  -329,  -329,  -329,  1560,  1560,  1593,  1511,   146,
     135,   196,   206,   730,   219,   222,   199,   203,   355,  -329,
    -329,   844,  -329,  -329,  -329,  -329,  -329,  -329,  -329,  -329,
    -329,  -329,  -329,   166,   233,  1511,  -329,   148,   232,   306,
      48,   333,   165,   214,   226,   294,    64,  -329,  -329,    50,
    -329,  -329,  -329,  -329,   454,  -329,  -329,  -329,  -329,  -329,
    -329,  1242,  -329,  -329,  -329,  -329,  -329,  -329,    94,   248,
     249,  -329,   -44,  1323,  -329,   259,    39,   948,  1907,  -329,
    -329,  -329,  1511,  -329,   121,  -329,   244,  -329,  -329,    60,
    -329,  -329,  -329,  -329,  -329,   237,  -329,   155,  2031,   730,
     844,  -329,  -329,   844,  -329,   246,   730,  1511,  1511,  1511,
      35,   681,   253,  -329,  -329,  -329,   122,    37,     0,    76,
     336,  -329,  -329,  1376,  1511,   339,  -329,  -329,  -329,  -329,
    -329,  -329,  -329,  -329,  -329,  -329,  -329,  1511,  -329,  1511,
    1511,  1511,  1511,  1511,  1511,  1511,  1511,  1511,  1511,  1511,
    1511,  1511,  1511,  1511,  1511,  1511,  1511,  1511,  -329,  1511,
    -329,  -329,  -329,  -329,  1511,   351,  -329,   245,  1343,   109,
    -329,  1648,  1000,  -329,    80,  -329,   211,  -329,  2031,  -329,
     354,   290,  1323,  -329,  -329,  -329,  1511,  -329,   298,   299,
    -329,  -329,    89,  -329,  1511,  -329,  -329,  -329,  1825,  -329,
     302,   304,   730,  -329,   -11,   113,   117,   307,   310,   763,
     763,  -329,  -329,  -329,  -329,  1781,   229,  -329,  1511,  1425,
    -329,  -329,   157,  -329,   114,  -329,  -329,  -329,  -329,  -329,
     148,   148,   232,   232,   306,   306,   306,   306,    48,    48,
     333,   165,   214,   226,   294,   -18,  -329,   303,  -329,  -329,
    1033,  -329,  -329,  -329,  -329,   311,   312,  1323,  -329,   313,
     314,  1085,   211,  1988,  1137,  -329,  -329,  -329,   315,   316,
    -329,  -329,  -329,  -329,  -329,   309,   309,  -329,   730,   730,
     730,  1511,  1511,  1458,  1478,  -329,  1242,  -329,  -329,  1511,
    -329,  1511,  -329,  -329,  -329,  1343,  -329,  -329,   318,  1323,
    -329,  -329,  1511,  -329,   319,  -329,   322,  1323,  -329,   321,
     323,  1189,  -329,  -329,   331,  -329,  -329,   158,   170,   730,
     173,   730,   174,   274,  -329,  -329,  -329,  -329,   324,   325,
    -329,  -329,   326,  1323,  -329,  -329,  1511,  -329,   348,   730,
     329,   330,  -329,   730,  -329,   730,  -329,  1222,  -329,  -329,
    -329,   356,   357,  -329,  -329,  -329,  -329,  -329,  -329,  -329,
    -329,  -329
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,   120,   102,   103,   104,   105,   106,   108,   109,   110,
     111,   114,   115,   112,   113,   164,   165,   107,   116,   140,
     141,     0,     0,   272,     0,    92,    94,   118,   117,     0,
     119,    96,     0,   269,   271,   159,     0,   131,     0,    89,
     168,     0,    97,   186,    87,     0,    98,     0,   167,     0,
      91,    93,   139,     0,    95,     1,   270,     0,    10,   163,
       0,   160,   135,   123,   127,   128,   129,     0,   124,     0,
       0,   187,   185,   184,    90,     0,    88,   275,     0,     0,
     276,     0,   274,     0,     0,     0,   166,     0,   147,     0,
     142,     0,   149,     0,     0,   155,     0,   136,   130,   132,
       0,     0,   121,   125,     0,   169,   188,   183,    99,   101,
       2,     7,     8,    11,    12,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       9,     0,   244,    32,    33,    34,    35,    36,    37,   251,
      14,     3,     4,    26,    38,     0,    41,    45,    48,    51,
      56,    59,    61,    63,    65,    67,    69,    71,    84,     0,
     249,   250,   235,   236,     0,   247,   237,   238,   239,   240,
       2,     0,   225,   100,   277,   273,   195,   180,   194,     0,
     189,   190,     0,     0,   170,    33,     0,     0,     0,   146,
     137,   143,     0,   144,     0,   150,   154,   148,   157,     0,
      38,    86,   162,   156,   161,   135,   134,     0,     0,     0,
       0,    27,    28,     0,    30,     0,     0,     0,     0,     0,
       0,     0,     0,   265,   266,   267,     0,     0,   198,     0,
       0,    20,    21,     0,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    73,     0,    29,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   253,     0,
     252,   246,   245,   248,     0,     0,   227,     0,     0,     0,
     231,     0,     0,   192,   200,   193,   201,   179,     0,   181,
       0,     0,     0,   171,   182,   178,     0,   177,    33,     0,
     138,   152,     0,   145,     0,   158,   133,   122,     0,   241,
       0,     0,     0,   243,     0,     0,     0,     0,     0,     0,
       0,   264,   268,     6,     5,     0,   200,   197,     0,     0,
      19,    16,     0,    24,     0,    18,    72,    42,    43,    44,
      46,    47,    49,    50,    54,    55,    52,    53,    57,    58,
      60,    62,    64,    66,    68,     0,    85,     0,   234,   223,
       0,   226,   230,   232,   219,     0,     0,     0,   203,    33,
       0,     0,   199,     0,     0,   191,   196,   173,     0,     0,
     174,   176,   151,   153,   126,     0,    31,   242,     0,     0,
       0,     0,     0,     0,     0,    40,     0,    39,    17,     0,
      15,     0,   233,   224,   229,     0,   220,   202,     0,     0,
     204,   210,     0,   209,     0,   221,     0,     0,   211,    33,
       0,     0,   172,   175,   255,   256,   257,     0,     0,     0,
       0,     0,     0,     0,    25,    70,   228,   206,     0,     0,
     208,   222,     0,     0,   212,   218,     0,   217,     0,     0,
       0,     0,   262,     0,   260,     0,    22,     0,   205,   207,
     214,     0,     0,   215,   254,   258,   259,   263,   261,    23,
     213,   216
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -329,  -329,  -329,  -329,  -329,  -329,  -329,   152,  -329,  -125,
     100,   101,     6,   108,   162,   164,   167,   163,   175,  -329,
     -92,   -76,  -329,  -114,  -112,   -26,    22,  -329,   364,  -329,
      -3,  -329,   341,   -51,   -47,  -329,  -329,   227,  -329,  -329,
    -329,   353,   -58,   -64,  -329,   141,  -329,   387,   -77,   -43,
     -17,   -36,   -39,   -74,   -79,  -329,   159,  -329,    77,  -141,
    -248,   -78,    49,  -328,  -329,   171,  -115,  -329,   -12,  -329,
     282,  -203,  -329,  -329,  -329,   241,   -20,  -329,  -329
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   140,   141,    59,   142,   143,   332,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   247,   159,   202,    23,    81,    45,    46,    25,
      26,    27,    67,    68,    69,    63,    98,    99,   100,    28,
      29,    89,    90,    91,   194,   195,    30,    60,    61,    31,
      70,    48,    49,    73,   365,   180,   181,   182,   229,   366,
     286,   276,   277,   278,   279,   280,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    32,    33,    34,    83
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,   173,   201,   172,    72,   179,   215,    47,   220,   186,
      92,   187,    56,    86,   226,    97,   103,   227,   320,   204,
     248,    80,    24,    40,   189,    57,   201,   289,   197,    58,
     106,   191,   405,   290,   107,    82,   372,   285,   323,    74,
     294,    39,    71,    40,    92,    92,    92,    50,    51,    92,
      88,   268,   160,    54,    24,    58,    40,   174,   109,   269,
     388,   256,   257,    58,   109,    35,   269,   228,    62,    37,
     325,   175,   282,   401,   196,    40,    52,   328,   372,    43,
     301,    42,   266,    40,    88,    88,    88,   327,    92,    88,
      41,   317,    40,    87,   309,   172,   227,    40,   318,   227,
     201,   313,   203,   314,   315,   316,   178,   291,   324,   292,
      41,   299,   295,    94,   269,    75,   393,   394,    42,    43,
     334,   101,   204,    41,   337,   338,   339,   269,    88,   405,
     191,    76,    43,    44,   258,   259,   305,    36,   160,   284,
      71,    38,    41,   270,   106,    92,   228,   329,    53,   228,
     281,    43,   282,   355,   267,   105,   103,   333,    97,    41,
     104,   283,   357,   192,   281,   193,   282,    92,    43,   205,
      92,   336,    42,    43,   230,   231,   232,   208,    15,    16,
     192,   274,   201,   275,   389,    88,   206,   400,   390,   326,
     269,   269,   383,   356,   269,   319,    79,   387,   302,   269,
     361,   362,   172,   395,   397,   217,   370,    88,   371,    84,
      88,    85,   201,    43,   303,   322,   378,    64,    65,    66,
     379,   209,    64,    65,    66,   222,   102,   249,   398,   450,
      24,   307,   250,   251,   399,   269,   233,   216,   234,    71,
     235,   451,   284,   262,   453,   455,   200,   269,    86,   106,
     269,   269,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   344,   345,   346,   347,   218,   211,   212,   214,
     200,    95,    96,   424,   425,   426,   219,   427,   428,   430,
     432,   373,   404,   374,   172,   196,   326,   310,    56,   221,
     311,   408,   223,   409,   416,   414,   224,   200,   420,   325,
     421,   282,   263,   178,    64,    65,    66,   198,   199,   435,
     178,   265,   252,   253,   452,   264,   454,   254,   255,   287,
     172,   359,   360,   434,    71,   246,   288,   436,   106,   172,
      24,    71,   293,   438,   464,   304,   439,   312,   467,   330,
     468,   442,   335,   443,   200,   448,   321,   178,   260,   261,
     456,   457,   340,   341,   358,   342,   343,   376,   170,   111,
     112,   113,   114,   377,   115,   116,   106,   461,   348,   349,
     462,   380,   381,   385,    71,   386,   402,   391,   106,   404,
     392,   172,   406,   407,   396,   449,   410,   411,   422,   423,
     117,   437,   440,   441,   444,   178,   445,   458,   459,   460,
     106,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   129,
     130,   463,   465,   466,   350,   131,   200,   351,   353,   470,
     471,   352,   306,   133,   134,   135,   136,   137,   138,   108,
     188,   354,   207,   382,    93,   433,   273,   375,   225,   308,
     363,     0,     0,     0,     0,   271,   200,   110,   111,   112,
     113,   114,     0,   115,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     200,   200,     0,     1,     2,     3,     4,     5,     6,   117,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   118,   119,   120,     0,   121,
     122,   123,   124,   125,   126,   127,   128,     0,   129,   130,
      22,     0,     0,     0,   131,     0,    77,     0,     0,    78,
     272,     0,   133,   134,   135,   136,   137,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
       0,     0,     0,   200,     1,     2,     3,     4,     5,     6,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,  -101,   110,   111,   112,   113,   114,     0,   115,
     116,     0,     0,     0,     0,     0,     0,    79,  -101,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     4,     5,     6,   117,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   118,   119,   120,     0,   121,   122,   123,   124,   125,
     126,   127,   128,     0,   129,   130,    22,     0,     0,     0,
     131,     0,     0,     0,     0,    78,   132,     0,   133,   134,
     135,   136,   137,   138,   170,   111,   112,   113,   114,     0,
     115,   116,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     2,     3,     4,     5,     6,   117,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,   110,   111,   112,   113,   114,     0,   115,
     116,     0,     0,     0,     0,   129,   130,    22,     0,     0,
       0,   131,     0,     0,     0,     0,     0,     0,     0,   133,
     134,   135,   136,   137,   138,   117,   170,   111,   112,   113,
     114,     0,   115,   116,   139,     0,     0,     0,     0,     0,
       0,   118,   119,   120,     0,   121,   122,   123,   124,   125,
     126,   127,   128,     0,   129,   130,     0,     0,   117,     0,
     131,     0,     0,     0,     0,    78,     0,     0,   133,   134,
     135,   136,   137,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,     0,     0,   129,   130,     0,
       0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
       0,   133,   134,   135,   136,   137,   138,   170,   111,   112,
     113,   114,     0,   115,   116,     0,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,   117,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,     0,     0,     0,   170,
     111,   112,   113,   114,     0,   115,   116,     0,   129,   130,
      22,     0,     0,     0,   131,     0,     0,     0,     0,     0,
       0,     0,   133,   134,   135,   136,   137,   138,   183,     0,
       0,   117,     0,     0,     0,     0,     0,     0,     0,     0,
      15,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,   111,   112,   113,   114,     0,   115,   116,     0,
     129,   130,     0,     0,     0,     0,   131,     0,     0,   184,
       0,     0,     0,     0,   133,   185,   135,   136,   137,   138,
     296,     0,     0,   117,     0,     0,     0,     0,     0,     0,
       0,     0,    15,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   111,   112,   113,   114,     0,   115,
     116,     0,   129,   130,     0,     0,     0,     0,   131,     0,
       0,   297,     0,     0,     0,     0,   133,   298,   135,   136,
     137,   138,   367,     0,     0,   117,   170,   111,   112,   113,
     114,     0,   115,   116,    15,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   130,     0,     0,   117,     0,
     131,     0,     0,   368,     0,     0,     0,     0,   133,   369,
     135,   136,   137,   138,     0,     0,     0,     0,   170,   111,
     112,   113,   114,     0,   115,   116,     0,   129,   130,     0,
       0,     0,     0,   131,     0,   274,     0,   275,   171,   403,
       0,   133,   134,   135,   136,   137,   138,   412,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,    15,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   111,   112,   113,   114,     0,   115,   116,     0,   129,
     130,     0,     0,     0,     0,   131,     0,     0,   413,     0,
       0,     0,     0,   133,   134,   135,   136,   137,   138,   417,
       0,     0,   117,     0,     0,     0,     0,     0,     0,     0,
       0,    15,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   111,   112,   113,   114,     0,   115,   116,
       0,   129,   130,     0,     0,     0,     0,   131,     0,     0,
     418,     0,     0,     0,     0,   133,   419,   135,   136,   137,
     138,   446,     0,     0,   117,   170,   111,   112,   113,   114,
       0,   115,   116,    15,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   170,   111,   112,   113,   114,
       0,   115,   116,   129,   130,     0,     0,   117,     0,   131,
       0,     0,   447,     0,     0,     0,     0,   133,   134,   135,
     136,   137,   138,     0,     0,     0,     0,   117,     0,     0,
       0,     0,     0,     0,     0,     0,   129,   130,     0,     0,
       0,     0,   131,     0,   274,     0,   275,   171,   469,     0,
     133,   134,   135,   136,   137,   138,   129,   130,     0,     0,
       0,     0,   131,     0,   274,     0,   275,   171,     0,     0,
     133,   134,   135,   136,   137,   138,   170,   111,   112,   113,
     114,     0,   115,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   111,   112,   113,
     114,     0,   115,   116,     0,     0,     0,     0,   117,     0,
       0,     0,     0,     0,     0,     0,     0,    15,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   117,   170,
     111,   112,   113,   114,     0,   115,   116,   129,   130,     0,
       0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
       0,   133,   134,   135,   136,   137,   138,   129,   130,     0,
       0,   117,     0,   131,     0,     0,     0,     0,   171,     0,
       0,   133,   134,   135,   136,   137,   138,     0,   170,   111,
     112,   113,   114,     0,   115,   116,     0,     0,     0,     0,
     129,   130,     0,     0,     0,     0,   131,   331,     0,     0,
       0,     0,     0,     0,   133,   134,   135,   136,   137,   138,
     117,   170,   111,   112,   113,   114,     0,   115,   116,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,   111,   112,   113,   114,     0,   115,   116,   129,
     130,     0,     0,   117,     0,   131,     0,     0,     0,     0,
     396,     0,     0,   133,   134,   135,   136,   137,   138,     0,
       0,     0,     0,   117,   170,   111,   112,   113,   114,     0,
     115,   116,   129,   130,     0,     0,     0,     0,   131,   429,
       0,     0,     0,     0,     0,     0,   133,   134,   135,   136,
     137,   138,   129,   130,     0,     0,   117,     0,   131,   431,
       0,     0,     0,     0,     0,     0,   133,   134,   135,   136,
     137,   138,     0,   170,   111,   112,   113,   114,     0,   115,
     116,     0,     0,     0,     0,   129,   130,     0,     0,     0,
       0,   131,     0,     0,     0,     0,     0,     0,     0,   133,
     134,   135,   136,   137,   138,   117,   170,   111,   112,   113,
     114,     0,   115,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   130,     0,     0,   117,     0,
     210,     0,     0,     0,     0,     0,     0,     0,   133,   134,
     135,   136,   137,   138,     0,     0,     0,     0,     0,     0,
       0,    40,     0,     0,     0,     0,     0,   129,   130,     0,
       0,     0,     0,   213,     0,     0,     0,     0,     0,     0,
       0,   133,   134,   135,   136,   137,   138,     1,     2,     3,
       4,     5,     6,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,   281,   364,
     282,     0,     0,     0,     0,     0,     0,    43,     1,     2,
       3,     4,     5,     6,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     4,     5,     6,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     2,     3,     4,     5,     6,    22,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,   325,   364,   282,     1,     2,     3,     4,     5,     6,
      43,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     1,     0,     0,     0,     0,
       0,   384,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     1,     0,     0,     0,
       0,     0,   190,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     1,     2,     3,
       4,     5,     6,   300,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     1,     2,     3,
       4,     5,     6,    78,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,   415,
       1,     2,     3,     4,     5,     6,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,    22,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22
};

static const yytype_int16 yycheck[] =
{
      43,    79,    94,    79,    43,    84,   118,    24,   123,    85,
      53,    85,    32,    49,   128,    62,    67,   131,   221,    96,
     145,    47,     0,     3,    88,    75,   118,    71,    92,     3,
      73,    89,   360,    77,    73,    47,   284,   178,     1,     1,
       1,     1,    85,     3,    87,    88,    89,    25,    26,    92,
      53,     1,    78,    31,    32,     3,     3,    83,    75,    77,
      71,    13,    14,     3,    81,     3,    77,   131,    91,     3,
      70,    83,    72,    91,    91,     3,     3,     1,   326,    79,
     192,    78,    18,     3,    87,    88,    89,   228,   131,    92,
      70,    56,     3,    75,   209,   171,   210,     3,    63,   213,
     192,   216,    76,   217,   218,   219,    84,   183,    71,   183,
      70,   187,    73,    92,    77,    77,   319,   320,    78,    79,
     234,    75,   199,    70,   249,   250,   251,    77,   131,   457,
     188,    93,    79,    93,    86,    87,    76,    75,   164,   178,
     183,    75,    70,    93,   187,   188,   210,    71,    75,   213,
      70,    79,    72,   267,    90,    71,   207,   233,   205,    70,
      91,   178,   274,    91,    70,    93,    72,   210,    79,    77,
     213,   247,    78,    79,     8,     9,    10,    75,    44,    45,
      91,    72,   274,    74,    71,   188,     3,    73,    71,   228,
      77,    77,   304,   269,    77,   221,    92,   312,    77,    77,
     278,    92,   278,   328,   329,    70,   282,   210,   282,    70,
     213,    72,   304,    79,    93,    93,   292,    67,    68,    69,
     296,    91,    67,    68,    69,     3,    76,    79,    71,    71,
     208,    76,    84,    85,    77,    77,    70,    91,    72,   282,
      74,    71,   281,    78,    71,    71,    94,    77,   284,   292,
      77,    77,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,   256,   257,   258,   259,    70,   115,   116,   117,
     118,    76,    77,   388,   389,   390,    70,   391,   392,   393,
     394,    70,   360,    72,   360,   302,   325,   210,   308,    70,
     213,   367,    93,   367,   373,   371,    93,   145,   374,    70,
     374,    72,    88,   281,    67,    68,    69,    76,    77,   401,
     288,    17,    80,    81,   429,    89,   431,    11,    12,    71,
     396,    76,    77,   399,   367,    92,    77,   405,   371,   405,
     308,   374,    73,   409,   449,    91,   412,    91,   453,     3,
     455,   417,     3,   417,   192,   421,    93,   325,    15,    16,
      76,    77,   252,   253,     3,   254,   255,     3,     3,     4,
       5,     6,     7,    73,     9,    10,   409,   443,   260,   261,
     446,    73,    73,    71,   417,    71,    73,    70,   421,   457,
      70,   457,    71,    71,    75,    54,    73,    73,    73,    73,
      35,    73,    73,    71,    73,   373,    73,    73,    73,    73,
     443,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,    64,
      65,    73,    93,    93,   262,    70,   274,   263,   265,    73,
      73,   264,   205,    78,    79,    80,    81,    82,    83,    75,
      87,   266,   101,   302,    57,   396,   164,   288,    93,   208,
     279,    -1,    -1,    -1,    -1,     1,   304,     3,     4,     5,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     328,   329,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    -1,     1,    -1,    -1,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   401,    29,    30,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,     3,     4,     5,     6,     7,    -1,
       9,    10,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    82,    83,    35,     3,     4,     5,     6,
       7,    -1,     9,    10,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    -1,    -1,    35,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,     3,     4,     5,
       6,     7,    -1,     9,    10,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,     9,    10,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    32,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,     9,    10,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    32,    -1,    -1,    35,     3,     4,     5,     6,
       7,    -1,     9,    10,    44,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    35,    -1,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    32,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    78,    79,    80,    81,    82,    83,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,     9,    10,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,
      83,    32,    -1,    -1,    35,     3,     4,     5,     6,     7,
      -1,     9,    10,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,     9,    10,    64,    65,    -1,    -1,    35,    -1,    70,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,     3,
       4,     5,     6,     7,    -1,     9,    10,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    64,    65,    -1,
      -1,    35,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    82,    83,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      35,     3,     4,     5,     6,     7,    -1,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,     9,    10,    64,
      65,    -1,    -1,    35,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    35,     3,     4,     5,     6,     7,    -1,
       9,    10,    64,    65,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    64,    65,    -1,    -1,    35,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    -1,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    82,    83,    35,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    35,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    66,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    70,    71,    72,    29,    30,    31,    32,    33,    34,
      79,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    76,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    76,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    76,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    29,    30,    31,
      32,    33,    34,    75,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    71,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    66,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    29,    30,    31,    32,    33,    34,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    66,   119,   120,   123,   124,   125,   133,   134,
     140,   143,   169,   170,   171,     3,    75,     3,    75,     1,
       3,    70,    78,    79,    93,   121,   122,   144,   145,   146,
     120,   120,     3,    75,   120,     0,   170,    75,     3,    97,
     141,   142,    91,   129,    67,    68,    69,   126,   127,   128,
     144,   143,   146,   147,     1,    77,    93,     1,    75,    92,
     119,   120,   162,   172,    70,    72,   145,    75,   124,   135,
     136,   137,   143,   141,    92,    76,    77,   128,   130,   131,
     132,    75,    76,   127,    91,    71,   143,   146,   122,   144,
       3,     4,     5,     6,     7,     9,    10,    35,    51,    52,
      53,    55,    56,    57,    58,    59,    60,    61,    62,    64,
      65,    70,    76,    78,    79,    80,    81,    82,    83,    93,
      95,    96,    98,    99,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   117,
     119,   160,   161,   162,   163,   164,   165,   166,   167,   168,
       3,    75,   115,   155,   119,   162,     3,    71,   120,   148,
     149,   150,   151,    32,    73,    79,   115,   147,   135,   137,
      76,   136,    91,    93,   138,   139,   144,   137,    76,    77,
     101,   114,   118,    76,   142,    77,     3,   126,    75,    91,
      70,   101,   101,    70,   101,   118,    91,    70,    70,    70,
     160,    70,     3,    93,    93,    93,   117,   117,   137,   152,
       8,     9,    10,    70,    72,    74,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    92,   116,   103,    79,
      84,    85,    80,    81,    11,    12,    13,    14,    86,    87,
      15,    16,    78,    88,    89,    17,    18,    90,     1,    77,
      93,     1,    76,   164,    72,    74,   155,   156,   157,   158,
     159,    70,    72,   144,   146,   153,   154,    71,    77,    71,
      77,   115,   147,    73,     1,    73,    32,    73,    79,   115,
      76,   118,    77,    93,    91,    76,   131,    76,   169,   160,
     152,   152,    91,   160,   117,   117,   117,    56,    63,   119,
     165,    93,    93,     1,    71,    70,   146,   153,     1,    71,
       3,    71,   100,   115,   117,     3,   115,   103,   103,   103,
     104,   104,   105,   105,   106,   106,   106,   106,   107,   107,
     108,   109,   110,   111,   112,   117,   115,   118,     3,    76,
      77,   155,    92,   159,    71,   148,   153,    32,    73,    79,
     115,   147,   154,    70,    72,   150,     3,    73,   115,   115,
      73,    73,   139,   118,    76,    71,    71,   160,    71,    71,
      71,    70,    70,   165,   165,   103,    75,   103,    71,    77,
      73,    91,    73,    76,   155,   157,    71,    71,   115,   147,
      73,    73,    32,    73,   115,    71,   148,    32,    73,    79,
     115,   147,    73,    73,   160,   160,   160,   117,   117,    71,
     117,    71,   117,   156,   115,   114,   155,    73,   115,   115,
      73,    71,   115,   147,    73,    73,    32,    73,   115,    54,
      71,    71,   160,    71,   160,    71,    76,    77,    73,    73,
      73,   115,   115,    73,   160,    93,    93,   160,   160,    76,
      73,    73
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    94,    95,    95,    95,    95,    95,    96,    96,    96,
      97,    98,    98,    98,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,   100,   100,   101,   101,   101,   101,
     101,   101,   102,   102,   102,   102,   102,   102,   103,   103,
     103,   104,   104,   104,   104,   105,   105,   105,   106,   106,
     106,   107,   107,   107,   107,   107,   108,   108,   108,   109,
     109,   110,   110,   111,   111,   112,   112,   113,   113,   114,
     114,   115,   115,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   117,   117,   118,   119,   119,   119,
     119,   120,   120,   120,   120,   120,   120,   120,   121,   121,
     122,   122,   123,   123,   123,   123,   123,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   125,   125,   125,   126,   126,   127,   128,   128,   128,
     129,   129,   130,   130,   131,   132,   132,   133,   133,   133,
     134,   134,   135,   135,   136,   136,   137,   137,   137,   137,
     138,   138,   139,   139,   139,   140,   140,   140,   140,   140,
     141,   141,   142,   142,   143,   143,   144,   144,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   146,   146,   146,   146,   147,   147,   148,
     149,   149,   150,   150,   150,   151,   151,   152,   152,   153,
     153,   153,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   155,   155,   155,   156,   156,   156,   156,
     157,   158,   158,   159,   159,   160,   160,   160,   160,   160,
     160,   161,   161,   161,   162,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   166,   166,   166,   167,   167,   167,
     167,   167,   167,   167,   168,   168,   168,   168,   168,   169,
     169,   170,   170,   171,   171,   171,   172,   172
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     4,     3,     3,
       2,     2,     6,     7,     1,     3,     1,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       5,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     2,     3,     2,
       3,     2,     1,     2,     1,     2,     1,     2,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     6,     3,     1,     2,     5,     1,     1,     1,
       2,     0,     1,     3,     2,     0,     1,     4,     5,     2,
       1,     1,     1,     2,     2,     3,     2,     1,     2,     1,
       1,     3,     2,     3,     1,     4,     5,     5,     6,     2,
       1,     3,     3,     1,     1,     1,     2,     1,     1,     3,
       3,     4,     6,     5,     5,     6,     5,     4,     4,     4,
       3,     4,     4,     3,     2,     2,     1,     1,     2,     1,
       1,     3,     2,     2,     1,     1,     3,     2,     1,     2,
       1,     1,     3,     2,     3,     5,     4,     5,     4,     3,
       3,     3,     4,     6,     5,     5,     6,     4,     4,     2,
       3,     3,     4,     3,     4,     1,     2,     1,     4,     3,
       2,     1,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     2,     3,     3,     1,     2,     1,
       1,     1,     2,     2,     7,     5,     5,     5,     7,     7,
       6,     7,     6,     7,     3,     2,     2,     2,     3,     1,
       2,     1,     1,     4,     3,     3,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 6: /* primary_expression: '(' expression error  */
#line 100 "src/grammar.y"
                               { yyerrok; }
#line 1973 "y.tab.cpp"
    break;

  case 40: /* cast_expression: '(' type_name error cast_expression  */
#line 159 "src/grammar.y"
                                              { yyerrok; }
#line 1979 "y.tab.cpp"
    break;

  case 89: /* declaration: declaration_specifiers error  */
#line 256 "src/grammar.y"
                                       { yyerrok; }
#line 1985 "y.tab.cpp"
    break;

  case 90: /* declaration: declaration_specifiers init_declarator_list error  */
#line 257 "src/grammar.y"
                                                        {yyerrok;}
#line 1991 "y.tab.cpp"
    break;

  case 91: /* declaration_specifiers: storage_class_specifier declaration_specifiers  */
#line 262 "src/grammar.y"
              { /* You may choose to ignore storage class in the complete type, or handle it separately. */
			char *combined = malloc(strlen((yyvsp[-1].id)) + strlen((yyvsp[0].id)) + 2);
              sprintf(combined, "%s %s", (yyvsp[-1].id), (yyvsp[0].id));
              (yyval.id) = combined;
              currentType = (yyval.id);
              free((yyvsp[-1].id));
              free((yyvsp[0].id));
		   }
#line 2004 "y.tab.cpp"
    break;

  case 92: /* declaration_specifiers: storage_class_specifier  */
#line 270 "src/grammar.y"
                                 {
		(yyval.id) = (yyvsp[0].id);
        currentType = (yyval.id);
	}
#line 2013 "y.tab.cpp"
    break;

  case 93: /* declaration_specifiers: type_specifier declaration_specifiers  */
#line 275 "src/grammar.y"
              {
              /* Concatenate the current type_specifier ($1) with the following declaration_specifiers ($2).
                 A space is inserted between the two parts. */
              char *combined = malloc(strlen((yyvsp[-1].id)) + strlen((yyvsp[0].id)) + 2);
              sprintf(combined, "%s %s", (yyvsp[-1].id), (yyvsp[0].id));
              (yyval.id) = combined;
              currentType = (yyval.id); /* Update the global currentType */
          }
#line 2026 "y.tab.cpp"
    break;

  case 94: /* declaration_specifiers: type_specifier  */
#line 284 "src/grammar.y"
              { 
              (yyval.id) = (yyvsp[0].id); 
              currentType = (yyval.id); 
          }
#line 2035 "y.tab.cpp"
    break;

  case 95: /* declaration_specifiers: type_qualifier declaration_specifiers  */
#line 289 "src/grammar.y"
            { 
			char *combined = malloc(strlen((yyvsp[-1].id)) + strlen((yyvsp[0].id)) + 2);
			sprintf(combined, "%s %s", (yyvsp[-1].id), (yyvsp[0].id));
			(yyval.id) = combined;
			currentType = (yyval.id); 
			(yyval.id) = (yyvsp[0].id); 
		}
#line 2047 "y.tab.cpp"
    break;

  case 96: /* declaration_specifiers: type_qualifier  */
#line 297 "src/grammar.y"
              { (yyval.id) = (yyvsp[0].id); 
		  currentType = (yyval.id);
		  }
#line 2055 "y.tab.cpp"
    break;

  case 102: /* storage_class_specifier: TYPEDEF  */
#line 315 "src/grammar.y"
                        { (yyval.id) = strdup("typedef"); }
#line 2061 "y.tab.cpp"
    break;

  case 103: /* storage_class_specifier: EXTERN  */
#line 316 "src/grammar.y"
                 { (yyval.id) = strdup("extern"); }
#line 2067 "y.tab.cpp"
    break;

  case 104: /* storage_class_specifier: STATIC  */
#line 317 "src/grammar.y"
                 { (yyval.id) = strdup("static"); }
#line 2073 "y.tab.cpp"
    break;

  case 105: /* storage_class_specifier: AUTO  */
#line 318 "src/grammar.y"
               { (yyval.id) = strdup("auto"); }
#line 2079 "y.tab.cpp"
    break;

  case 106: /* storage_class_specifier: REGISTER  */
#line 319 "src/grammar.y"
                   { (yyval.id) = strdup("register"); }
#line 2085 "y.tab.cpp"
    break;

  case 107: /* type_specifier: VOID  */
#line 323 "src/grammar.y"
               { currentType = strdup("VOID"); (yyval.id) = currentType; }
#line 2091 "y.tab.cpp"
    break;

  case 108: /* type_specifier: CHAR  */
#line 324 "src/grammar.y"
               { currentType = strdup("CHAR"); (yyval.id) = currentType; }
#line 2097 "y.tab.cpp"
    break;

  case 109: /* type_specifier: SHORT  */
#line 325 "src/grammar.y"
                { currentType = strdup("SHORT"); (yyval.id) = currentType; }
#line 2103 "y.tab.cpp"
    break;

  case 110: /* type_specifier: INT  */
#line 326 "src/grammar.y"
              { currentType = strdup("INT"); (yyval.id) = currentType; }
#line 2109 "y.tab.cpp"
    break;

  case 111: /* type_specifier: LONG  */
#line 327 "src/grammar.y"
               { currentType = strdup("LONG"); (yyval.id) = currentType; }
#line 2115 "y.tab.cpp"
    break;

  case 112: /* type_specifier: FLOAT  */
#line 328 "src/grammar.y"
                { currentType = strdup("FLOAT"); (yyval.id) = currentType; }
#line 2121 "y.tab.cpp"
    break;

  case 113: /* type_specifier: DOUBLE  */
#line 329 "src/grammar.y"
                 { currentType = strdup("DOUBLE"); (yyval.id) = currentType; }
#line 2127 "y.tab.cpp"
    break;

  case 114: /* type_specifier: SIGNED  */
#line 330 "src/grammar.y"
                 { currentType = strdup("SIGNED"); (yyval.id) = currentType; }
#line 2133 "y.tab.cpp"
    break;

  case 115: /* type_specifier: UNSIGNED  */
#line 331 "src/grammar.y"
                   { currentType = strdup("UNSIGNED"); (yyval.id) = currentType; }
#line 2139 "y.tab.cpp"
    break;

  case 116: /* type_specifier: BOOL  */
#line 332 "src/grammar.y"
               { currentType = strdup("BOOL"); (yyval.id) = currentType; }
#line 2145 "y.tab.cpp"
    break;

  case 117: /* type_specifier: struct_or_union_specifier  */
#line 333 "src/grammar.y"
                                    { currentType = (yyvsp[0].id); (yyval.id) = currentType; }
#line 2151 "y.tab.cpp"
    break;

  case 118: /* type_specifier: class_specifier  */
#line 334 "src/grammar.y"
                          {currentType = (yyvsp[0].id); (yyval.id) = currentType;}
#line 2157 "y.tab.cpp"
    break;

  case 119: /* type_specifier: enum_specifier  */
#line 335 "src/grammar.y"
                         { currentType = (yyvsp[0].id); (yyval.id) = currentType; }
#line 2163 "y.tab.cpp"
    break;

  case 120: /* type_specifier: TYPEDEF_NAME  */
#line 336 "src/grammar.y"
                        { currentType = strdup(yytext); (yyval.id) = currentType;}
#line 2169 "y.tab.cpp"
    break;

  case 121: /* class_specifier: CLASS '{' class_member_list '}'  */
#line 341 "src/grammar.y"
         {
		 (yyval.id) = malloc(strlen("class") + 14); 
         sprintf((yyval.id), "class (anonymous)");
		   }
#line 2178 "y.tab.cpp"
    break;

  case 122: /* class_specifier: CLASS IDENTIFIER base_clause_opt '{' class_member_list '}'  */
#line 346 "src/grammar.y"
         { 
		    (yyval.id) = malloc( strlen("class") + strlen((yyvsp[-4].id)) + 14 ); // one space plus null
         sprintf((yyval.id), "class %s", (yyvsp[-4].id));
		   
		   }
#line 2188 "y.tab.cpp"
    break;

  case 123: /* class_specifier: CLASS IDENTIFIER base_clause_opt  */
#line 352 "src/grammar.y"
         { 
           (yyval.id) = malloc(strlen("class") + strlen((yyvsp[-1].id)) + 14);
           sprintf((yyval.id), "class %s", (yyvsp[-1].id));
		 }
#line 2197 "y.tab.cpp"
    break;

  case 127: /* access_specifier: PUBLIC  */
#line 370 "src/grammar.y"
             { (yyval.id) = strdup("public"); }
#line 2203 "y.tab.cpp"
    break;

  case 128: /* access_specifier: PRIVATE  */
#line 371 "src/grammar.y"
              { (yyval.id) = strdup("private"); }
#line 2209 "y.tab.cpp"
    break;

  case 129: /* access_specifier: PROTECTED  */
#line 372 "src/grammar.y"
                { (yyval.id) = strdup("protected"); }
#line 2215 "y.tab.cpp"
    break;

  case 130: /* base_clause_opt: ':' base_specifier_list  */
#line 376 "src/grammar.y"
                              { (yyval.id) = (yyvsp[0].id); }
#line 2221 "y.tab.cpp"
    break;

  case 131: /* base_clause_opt: %empty  */
#line 377 "src/grammar.y"
                  { (yyval.id) = NULL; }
#line 2227 "y.tab.cpp"
    break;

  case 132: /* base_specifier_list: base_specifier  */
#line 381 "src/grammar.y"
                     { (yyval.id) = (yyvsp[0].id); }
#line 2233 "y.tab.cpp"
    break;

  case 133: /* base_specifier_list: base_specifier_list ',' base_specifier  */
#line 383 "src/grammar.y"
         { 
           /* Concatenate the list, e.g., "public Base1, private Base2" */
           char *tmp = malloc(strlen((yyvsp[-2].id)) + strlen((yyvsp[0].id)) + 14);
           sprintf(tmp, "%s, %s", (yyvsp[-2].id), (yyvsp[0].id));
           (yyval.id) = tmp;
         }
#line 2244 "y.tab.cpp"
    break;

  case 134: /* base_specifier: access_specifier_opt IDENTIFIER  */
#line 394 "src/grammar.y"
         { 
           if ((yyvsp[-1].id))
           {
              (yyval.id) = malloc(strlen((yyvsp[-1].id)) + strlen((yyvsp[0].id)) + 14);
              sprintf((yyval.id), "%s %s", (yyvsp[-1].id), (yyvsp[0].id));
           }
           else
           {
              (yyval.id) = strdup((yyvsp[0].id));
           }
         }
#line 2260 "y.tab.cpp"
    break;

  case 135: /* access_specifier_opt: %empty  */
#line 409 "src/grammar.y"
                      {(yyval.id) = NULL;}
#line 2266 "y.tab.cpp"
    break;

  case 136: /* access_specifier_opt: access_specifier  */
#line 410 "src/grammar.y"
                           {(yyval.id) = (yyvsp[0].id);}
#line 2272 "y.tab.cpp"
    break;

  case 137: /* struct_or_union_specifier: struct_or_union '{' struct_declaration_list '}'  */
#line 414 "src/grammar.y"
                                                         {
         /* Anonymous struct or union */
         (yyval.id) = malloc(strlen((yyvsp[-3].id)) + 14); // Enough for " (anonymous)" and '\0'
         sprintf((yyval.id), "%s (anonymous)", (yyvsp[-3].id));
    }
#line 2282 "y.tab.cpp"
    break;

  case 138: /* struct_or_union_specifier: struct_or_union IDENTIFIER '{' struct_declaration_list '}'  */
#line 419 "src/grammar.y"
                                                                     {
         /* Named struct/union with body */
         (yyval.id) = malloc(strlen((yyvsp[-4].id)) + strlen((yyvsp[-3].id)) + 2); // one space plus null
         sprintf((yyval.id), "%s %s", (yyvsp[-4].id), (yyvsp[-3].id));
    }
#line 2292 "y.tab.cpp"
    break;

  case 139: /* struct_or_union_specifier: struct_or_union IDENTIFIER  */
#line 424 "src/grammar.y"
                                     {
         /* Named struct/union declaration without body */
         (yyval.id) = malloc(strlen((yyvsp[-1].id)) + strlen((yyvsp[0].id)) + 2);
         sprintf((yyval.id), "%s %s", (yyvsp[-1].id), (yyvsp[0].id));
    }
#line 2302 "y.tab.cpp"
    break;

  case 140: /* struct_or_union: STRUCT  */
#line 432 "src/grammar.y"
                { (yyval.id) = strdup("struct"); }
#line 2308 "y.tab.cpp"
    break;

  case 141: /* struct_or_union: UNION  */
#line 433 "src/grammar.y"
                { (yyval.id) = strdup("union"); }
#line 2314 "y.tab.cpp"
    break;

  case 155: /* enum_specifier: ENUM '{' enumerator_list '}'  */
#line 465 "src/grammar.y"
                                   { 
          /* Anonymous enum; you might choose to simply return "enum" */
          (yyval.id) = strdup("enum");
      }
#line 2323 "y.tab.cpp"
    break;

  case 156: /* enum_specifier: ENUM '{' enumerator_list ',' '}'  */
#line 469 "src/grammar.y"
                                       { 
          (yyval.id) = strdup("enum");
      }
#line 2331 "y.tab.cpp"
    break;

  case 157: /* enum_specifier: ENUM IDENTIFIER '{' enumerator_list '}'  */
#line 472 "src/grammar.y"
                                              { 
          /* Named enum: produce "enum <identifier>" */
          char *tmp = malloc(strlen("enum") + strlen((yyvsp[-3].id)) + 2);
          sprintf(tmp, "enum %s", (yyvsp[-3].id));
          (yyval.id) = tmp;
      }
#line 2342 "y.tab.cpp"
    break;

  case 158: /* enum_specifier: ENUM IDENTIFIER '{' enumerator_list ',' '}'  */
#line 478 "src/grammar.y"
                                                  { 
          char *tmp = malloc(strlen("enum") + strlen((yyvsp[-4].id)) + 2);
          sprintf(tmp, "enum %s", (yyvsp[-4].id));
          (yyval.id) = tmp;
      }
#line 2352 "y.tab.cpp"
    break;

  case 159: /* enum_specifier: ENUM IDENTIFIER  */
#line 483 "src/grammar.y"
                      { 
          char *tmp = malloc(strlen("enum") + strlen((yyvsp[0].id)) + 2);
          sprintf(tmp, "enum %s", (yyvsp[0].id));
          (yyval.id) = tmp;
      }
#line 2362 "y.tab.cpp"
    break;

  case 164: /* type_qualifier: CONST  */
#line 502 "src/grammar.y"
                { (yyval.id) = strdup("CONST"); }
#line 2368 "y.tab.cpp"
    break;

  case 165: /* type_qualifier: VOLATILE  */
#line 503 "src/grammar.y"
                   { (yyval.id) = strdup("VOLATILE"); }
#line 2374 "y.tab.cpp"
    break;

  case 166: /* declarator: pointer direct_declarator  */
#line 509 "src/grammar.y"
                                    {
          int idx = (yyvsp[0].index);  // $2 is the token table index from direct_declarator.
          char newType[256];
         
          sprintf(newType, "%s%s", tokenTable[idx].token_type, (yyvsp[-1].id));
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType);
		  if (strstr(newType, "typedef") != NULL){
			update_symtab(tokenTable[idx].token);
		  }
          (yyval.index) = idx;
          free((yyvsp[-1].id)); /* free the pointer string */
      }
#line 2392 "y.tab.cpp"
    break;

  case 167: /* declarator: direct_declarator  */
#line 522 "src/grammar.y"
                            { (yyval.index) = (yyvsp[0].index); }
#line 2398 "y.tab.cpp"
    break;

  case 168: /* direct_declarator: IDENTIFIER  */
#line 526 "src/grammar.y"
                    {
        add_to_token_table((yyvsp[0].id), currentType ? currentType : "INVALID");
        (yyval.index) = token_count - 1;
    }
#line 2407 "y.tab.cpp"
    break;

  case 169: /* direct_declarator: '(' declarator ')'  */
#line 530 "src/grammar.y"
                             { (yyval.index) = (yyvsp[-1].index); }
#line 2413 "y.tab.cpp"
    break;

  case 170: /* direct_declarator: direct_declarator '[' ']'  */
#line 531 "src/grammar.y"
                                   {
          int idx = (yyvsp[-2].index);
          char newType[256];
          sprintf(newType, "%s[]", tokenTable[idx].token_type);
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType);
		  if (strstr(newType, "typedef") != NULL){
			update_symtab(tokenTable[idx].token);
		  }
          (yyval.index) = idx;
      }
#line 2429 "y.tab.cpp"
    break;

  case 178: /* direct_declarator: direct_declarator '[' assignment_expression ']'  */
#line 549 "src/grammar.y"
                                                         {
          int idx = (yyvsp[-3].index);
          char newType[256];
          sprintf(newType, "%s[%s]", tokenTable[idx].token_type, (yyvsp[-1].id));
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType);
		  if (strstr(newType, "typedef") != NULL){
			update_symtab(tokenTable[idx].token);
		  }
          (yyval.index) = idx;
      }
#line 2445 "y.tab.cpp"
    break;

  case 179: /* direct_declarator: direct_declarator '(' parameter_type_list ')'  */
#line 560 "src/grammar.y"
                                                        {
       int idx = (yyvsp[-3].index);  // $1 is now of type int (the token index)
       free(tokenTable[idx].token_type);
       tokenTable[idx].token_type = strdup("PROCEDURE");
       (yyval.index) = idx;
    }
#line 2456 "y.tab.cpp"
    break;

  case 180: /* direct_declarator: direct_declarator '(' ')'  */
#line 566 "src/grammar.y"
                                    {
       int idx = (yyvsp[-2].index);
       free(tokenTable[idx].token_type);
       tokenTable[idx].token_type = strdup("PROCEDURE");
       (yyval.index) = idx;
    }
#line 2467 "y.tab.cpp"
    break;

  case 181: /* direct_declarator: direct_declarator '(' identifier_list ')'  */
#line 572 "src/grammar.y"
                                                   {
       int idx = (yyvsp[-3].index);
       free(tokenTable[idx].token_type);
       tokenTable[idx].token_type = strdup("PROCEDURE");
       (yyval.index) = idx;
    }
#line 2478 "y.tab.cpp"
    break;

  case 182: /* direct_declarator: direct_declarator '[' assignment_expression error  */
#line 578 "src/grammar.y"
                                                            {
          /* Catch an invalid array declaration (missing ']'). 
             Use the default error message and recover. */
          yyerrok;
          (yyval.index) = (yyvsp[-3].index);  /* Propagate the index as is */
      }
#line 2489 "y.tab.cpp"
    break;

  case 185: /* pointer: '*' pointer  */
#line 589 "src/grammar.y"
                     {
          /* $2 is a string representing the pointer from the recursive production.
             Prepend a '*' to it. */
          int len = strlen((yyvsp[0].id)) + 2;
          (yyval.id) = malloc(len);
          sprintf((yyval.id), "*%s", (yyvsp[0].id));
          free((yyvsp[0].id));
      }
#line 2502 "y.tab.cpp"
    break;

  case 186: /* pointer: '*'  */
#line 597 "src/grammar.y"
              {
          (yyval.id) = strdup("*");
      }
#line 2510 "y.tab.cpp"
    break;

  case 188: /* type_qualifier_list: type_qualifier_list type_qualifier  */
#line 604 "src/grammar.y"
                                            { 
          /* For now, ignore qualifiers */
          (yyval.id) = (yyvsp[-1].id); 
      }
#line 2519 "y.tab.cpp"
    break;

  case 246: /* compound_statement: '{' block_item_list error  */
#line 711 "src/grammar.y"
                                     { yyerrok; }
#line 2525 "y.tab.cpp"
    break;

  case 253: /* expression_statement: expression error  */
#line 727 "src/grammar.y"
                           {yyerrok;}
#line 2531 "y.tab.cpp"
    break;

  case 275: /* function_definition: declaration_specifiers declarator error  */
#line 767 "src/grammar.y"
                                                  { yyerrok; }
#line 2537 "y.tab.cpp"
    break;


#line 2541 "y.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 775 "src/grammar.y"

void yyerror(const char *s) {
	parser_errror++;
	fflush(stdout);
    fprintf(stderr, "Syntax Error: %s at line %d\n", s, line_no);
}


main(int argc, char **argv) {
int parserresult = yyparse(); // Parser calls yylex() internally
 yydebug = 1;
if (parserresult == 0 && error_count == 0 && parser_errror == 0) {
	printf("LEX and Parsing Success\n");
	
	print_token_table();
} else {
	if(error_count > 0){
		printf("Errors in LEX stage:\n PARSING FAILED.");
		print_errors();
	} 
	else {
		printf("Error in parsing with errorcode: %d\n", parserresult);
		
	}
}
return 0;
}
