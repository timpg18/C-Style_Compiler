/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_HPP_INCLUDED
# define YY_YY_Y_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    I_CONSTANT = 259,              /* I_CONSTANT  */
    F_CONSTANT = 260,              /* F_CONSTANT  */
    STRING_LIT = 261,              /* STRING_LIT  */
    CHAR_LIT = 262,                /* CHAR_LIT  */
    PTR_OP = 263,                  /* PTR_OP  */
    INC_OP = 264,                  /* INC_OP  */
    DEC_OP = 265,                  /* DEC_OP  */
    LEFT_OP = 266,                 /* LEFT_OP  */
    RIGHT_OP = 267,                /* RIGHT_OP  */
    LE_OP = 268,                   /* LE_OP  */
    GE_OP = 269,                   /* GE_OP  */
    EQ_OP = 270,                   /* EQ_OP  */
    NE_OP = 271,                   /* NE_OP  */
    AND_OP = 272,                  /* AND_OP  */
    OR_OP = 273,                   /* OR_OP  */
    MUL_ASSIGN = 274,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 275,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 276,              /* MOD_ASSIGN  */
    ADD_ASSIGN = 277,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 278,              /* SUB_ASSIGN  */
    LEFT_ASSIGN = 279,             /* LEFT_ASSIGN  */
    RIGHT_ASSIGN = 280,            /* RIGHT_ASSIGN  */
    AND_ASSIGN = 281,              /* AND_ASSIGN  */
    XOR_ASSIGN = 282,              /* XOR_ASSIGN  */
    OR_ASSIGN = 283,               /* OR_ASSIGN  */
    TYPEDEF_NAME = 284,            /* TYPEDEF_NAME  */
    TYPEDEF = 285,                 /* TYPEDEF  */
    EXTERN = 286,                  /* EXTERN  */
    STATIC = 287,                  /* STATIC  */
    AUTO = 288,                    /* AUTO  */
    REGISTER = 289,                /* REGISTER  */
    SIZEOF = 290,                  /* SIZEOF  */
    CHAR = 291,                    /* CHAR  */
    SHORT = 292,                   /* SHORT  */
    INT = 293,                     /* INT  */
    LONG = 294,                    /* LONG  */
    SIGNED = 295,                  /* SIGNED  */
    UNSIGNED = 296,                /* UNSIGNED  */
    FLOAT = 297,                   /* FLOAT  */
    DOUBLE = 298,                  /* DOUBLE  */
    CONST = 299,                   /* CONST  */
    VOLATILE = 300,                /* VOLATILE  */
    VOID = 301,                    /* VOID  */
    BOOL = 302,                    /* BOOL  */
    STRUCT = 303,                  /* STRUCT  */
    UNION = 304,                   /* UNION  */
    ENUM = 305,                    /* ENUM  */
    CASE = 306,                    /* CASE  */
    DEFAULT = 307,                 /* DEFAULT  */
    IF = 308,                      /* IF  */
    ELSE = 309,                    /* ELSE  */
    SWITCH = 310,                  /* SWITCH  */
    WHILE = 311,                   /* WHILE  */
    DO = 312,                      /* DO  */
    FOR = 313,                     /* FOR  */
    GOTO = 314,                    /* GOTO  */
    CONTINUE = 315,                /* CONTINUE  */
    BREAK = 316,                   /* BREAK  */
    RETURN = 317,                  /* RETURN  */
    UNTIL = 318,                   /* UNTIL  */
    FUNC_NAME = 319,               /* FUNC_NAME  */
    ENUMERATION_CONSTANT = 320,    /* ENUMERATION_CONSTANT  */
    CLASS = 321,                   /* CLASS  */
    PUBLIC = 322,                  /* PUBLIC  */
    PRIVATE = 323,                 /* PRIVATE  */
    PROTECTED = 324                /* PROTECTED  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 53 "src/grammar.y"

	char *id;
    int index;

#line 138 "y.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_HPP_INCLUDED  */
