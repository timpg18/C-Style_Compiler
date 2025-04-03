// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 2 "grammar.y"

#include "header.h"
#include <string>
Symboltable st;
std::string currentType; 



#line 50 "grammar.tab.cc"


#include "grammar.tab.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 128 "grammar.tab.cc"

  /// Build a parser object.
  parser::parser (SymbolTable& st_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      st (st_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // primary_expression: IDENTIFIER
#line 55 "grammar.y"
                     {
		if (!st.lookup(*(yystack_[0].value.id))) {
            std::cerr << "Undeclared identifier: " << *(yystack_[0].value.id) << std::endl;
            error_count++;
        }
        delete (yystack_[0].value.id);
	}
#line 591 "grammar.tab.cc"
    break;

  case 6: // primary_expression: '(' expression error
#line 65 "grammar.y"
                               { yyerrok; }
#line 597 "grammar.tab.cc"
    break;

  case 40: // cast_expression: '(' type_name error cast_expression
#line 124 "grammar.y"
                                              { yyerrok; }
#line 603 "grammar.tab.cc"
    break;

  case 71: // assignment_expression: conditional_expression
#line 191 "grammar.y"
          { (yylhs.value.id) = (yystack_[0].value.id); }
#line 609 "grammar.tab.cc"
    break;

  case 86: // constant_expression: conditional_expression
#line 215 "grammar.y"
          { (yylhs.value.id) = (yystack_[0].value.id); }
#line 615 "grammar.tab.cc"
    break;

  case 89: // declaration: declaration_specifiers error
#line 221 "grammar.y"
                                       { yyerrok; }
#line 621 "grammar.tab.cc"
    break;

  case 90: // declaration: declaration_specifiers init_declarator_list error
#line 222 "grammar.y"
                                                        {yyerrok;}
#line 627 "grammar.tab.cc"
    break;

  case 91: // declaration_specifiers: storage_class_specifier declaration_specifiers
#line 227 "grammar.y"
              { /* You may choose to ignore storage class in the complete type, or handle it separately. */
			 currentType = *(yystack_[1].value.id) + " " + *(yystack_[0].value.id);
        delete (yystack_[1].value.id);
        delete (yystack_[0].value.id);
        (yylhs.value.id) = new std::string(currentType);
		   }
#line 638 "grammar.tab.cc"
    break;

  case 92: // declaration_specifiers: storage_class_specifier
#line 233 "grammar.y"
                                 {
		(yylhs.value.id) = (yystack_[0].value.id);
        currentType = (yylhs.value.id);
	}
#line 647 "grammar.tab.cc"
    break;

  case 93: // declaration_specifiers: type_specifier declaration_specifiers
#line 238 "grammar.y"
              {
           currentType = *(yystack_[1].value.id) + " " + *(yystack_[0].value.id);
        delete (yystack_[1].value.id);
        delete (yystack_[0].value.id);
        (yylhs.value.id) = new std::string(currentType);
          }
#line 658 "grammar.tab.cc"
    break;

  case 94: // declaration_specifiers: type_specifier
#line 245 "grammar.y"
              { 
              (yylhs.value.id) = (yystack_[0].value.id); 
              currentType = (yylhs.value.id); 
          }
#line 667 "grammar.tab.cc"
    break;

  case 95: // declaration_specifiers: type_qualifier declaration_specifiers
#line 250 "grammar.y"
            { 
			currentType = *(yystack_[1].value.id) + " " + *(yystack_[0].value.id);
        delete (yystack_[1].value.id);
        delete (yystack_[0].value.id);
    		(yylhs.value.id) = (yystack_[0].value.id);
		}
#line 678 "grammar.tab.cc"
    break;

  case 96: // declaration_specifiers: type_qualifier
#line 257 "grammar.y"
              { (yylhs.value.id) = (yystack_[0].value.id); 
		  currentType = (yylhs.value.id);
		  }
#line 686 "grammar.tab.cc"
    break;

  case 97: // declaration_specifiers: declaration_specifiers '&'
#line 260 "grammar.y"
          { (yylhs.value.id) = (yystack_[1].value.id); }
#line 692 "grammar.tab.cc"
    break;

  case 102: // storage_class_specifier: TYPEDEF
#line 275 "grammar.y"
              { (yylhs.value.id) = strdup("typedef"); }
#line 698 "grammar.tab.cc"
    break;

  case 103: // storage_class_specifier: EXTERN
#line 276 "grammar.y"
             { (yylhs.value.id) = strdup("extern"); }
#line 704 "grammar.tab.cc"
    break;

  case 104: // storage_class_specifier: STATIC
#line 277 "grammar.y"
             { (yylhs.value.id) = strdup("static"); }
#line 710 "grammar.tab.cc"
    break;

  case 105: // storage_class_specifier: AUTO
#line 278 "grammar.y"
           { (yylhs.value.id) = strdup("auto"); }
#line 716 "grammar.tab.cc"
    break;

  case 106: // storage_class_specifier: REGISTER
#line 279 "grammar.y"
               { (yylhs.value.id) = strdup("register"); }
#line 722 "grammar.tab.cc"
    break;

  case 107: // type_specifier: VOID
#line 283 "grammar.y"
           { 
          std::string s = "VOID";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 732 "grammar.tab.cc"
    break;

  case 108: // type_specifier: CHAR
#line 288 "grammar.y"
           { 
          std::string s = "CHAR";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 742 "grammar.tab.cc"
    break;

  case 109: // type_specifier: SHORT
#line 293 "grammar.y"
            { 
          std::string s = "SHORT";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 752 "grammar.tab.cc"
    break;

  case 110: // type_specifier: INT
#line 298 "grammar.y"
          { 
          std::string s = "INT";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 762 "grammar.tab.cc"
    break;

  case 111: // type_specifier: LONG
#line 303 "grammar.y"
           { 
          std::string s = "LONG";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 772 "grammar.tab.cc"
    break;

  case 112: // type_specifier: FLOAT
#line 308 "grammar.y"
            { 
          std::string s = "FLOAT";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 782 "grammar.tab.cc"
    break;

  case 113: // type_specifier: DOUBLE
#line 313 "grammar.y"
             { 
          std::string s = "DOUBLE";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 792 "grammar.tab.cc"
    break;

  case 114: // type_specifier: SIGNED
#line 318 "grammar.y"
             { 
          std::string s = "SIGNED";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 802 "grammar.tab.cc"
    break;

  case 115: // type_specifier: UNSIGNED
#line 323 "grammar.y"
               { 
          std::string s = "UNSIGNED";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 812 "grammar.tab.cc"
    break;

  case 116: // type_specifier: BOOL
#line 328 "grammar.y"
           { 
          std::string s = "BOOL";
          currentType = strdup(s.c_str());
          (yylhs.value.id) = currentType;
       }
#line 822 "grammar.tab.cc"
    break;

  case 117: // type_specifier: struct_or_union_specifier
#line 333 "grammar.y"
                                { currentType = (yystack_[0].value.id); (yylhs.value.id) = currentType; }
#line 828 "grammar.tab.cc"
    break;

  case 118: // type_specifier: class_specifier
#line 334 "grammar.y"
                      { currentType = (yystack_[0].value.id); (yylhs.value.id) = currentType; }
#line 834 "grammar.tab.cc"
    break;

  case 119: // type_specifier: enum_specifier
#line 335 "grammar.y"
                     { currentType = (yystack_[0].value.id); (yylhs.value.id) = currentType; }
#line 840 "grammar.tab.cc"
    break;

  case 120: // type_specifier: TYPEDEF_NAME
#line 336 "grammar.y"
                   { 
          currentType = strdup(yytext);
          (yylhs.value.id) = currentType;
       }
#line 849 "grammar.tab.cc"
    break;

  case 121: // class_specifier: CLASS '{' class_member_list '}'
#line 343 "grammar.y"
                                      {
          std::string combined = "class (anonymous)";
          (yylhs.value.id) = strdup(combined.c_str());
       }
#line 858 "grammar.tab.cc"
    break;

  case 122: // class_specifier: CLASS IDENTIFIER base_clause_opt '{' class_member_list '}'
#line 347 "grammar.y"
                                                                 {
          std::string combined = "class " + std::string((yystack_[4].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
       }
#line 867 "grammar.tab.cc"
    break;

  case 123: // class_specifier: CLASS IDENTIFIER base_clause_opt
#line 351 "grammar.y"
                                       {
          std::string combined = "class " + std::string((yystack_[1].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
       }
#line 876 "grammar.tab.cc"
    break;

  case 127: // access_specifier: PUBLIC
#line 367 "grammar.y"
             { (yylhs.value.id) = strdup("public"); }
#line 882 "grammar.tab.cc"
    break;

  case 128: // access_specifier: PRIVATE
#line 368 "grammar.y"
              { (yylhs.value.id) = strdup("private"); }
#line 888 "grammar.tab.cc"
    break;

  case 129: // access_specifier: PROTECTED
#line 369 "grammar.y"
                { (yylhs.value.id) = strdup("protected"); }
#line 894 "grammar.tab.cc"
    break;

  case 130: // base_clause_opt: ':' base_specifier_list
#line 374 "grammar.y"
                              { (yylhs.value.id) = (yystack_[0].value.id); }
#line 900 "grammar.tab.cc"
    break;

  case 131: // base_clause_opt: %empty
#line 375 "grammar.y"
                  { (yylhs.value.id) = NULL; }
#line 906 "grammar.tab.cc"
    break;

  case 132: // base_specifier_list: base_specifier
#line 379 "grammar.y"
                     { (yylhs.value.id) = (yystack_[0].value.id); }
#line 912 "grammar.tab.cc"
    break;

  case 133: // base_specifier_list: base_specifier_list ',' base_specifier
#line 380 "grammar.y"
                                             { 
          std::string combined = std::string((yystack_[2].value.id)) + ", " + std::string((yystack_[0].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
       }
#line 921 "grammar.tab.cc"
    break;

  case 134: // base_specifier: access_specifier_opt IDENTIFIER
#line 388 "grammar.y"
                                      {
          if ((yystack_[1].value.id)) {
              std::string combined = std::string((yystack_[1].value.id)) + " " + std::string((yystack_[0].value.id));
              (yylhs.value.id) = strdup(combined.c_str());
          } else {
              (yylhs.value.id) = strdup((yystack_[0].value.id));
          }
       }
#line 934 "grammar.tab.cc"
    break;

  case 135: // access_specifier_opt: %empty
#line 400 "grammar.y"
                  { (yylhs.value.id) = NULL; }
#line 940 "grammar.tab.cc"
    break;

  case 136: // access_specifier_opt: access_specifier
#line 401 "grammar.y"
                       { (yylhs.value.id) = (yystack_[0].value.id); }
#line 946 "grammar.tab.cc"
    break;

  case 137: // struct_or_union_specifier: struct_or_union '{' struct_declaration_list '}'
#line 406 "grammar.y"
                                                      {
          // Anonymous struct/union: concatenate $1 and " (anonymous)"
          std::string combined = std::string((yystack_[3].value.id)) + " (anonymous)";
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 956 "grammar.tab.cc"
    break;

  case 138: // struct_or_union_specifier: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
#line 411 "grammar.y"
                                                                 {
          // Named struct/union with body: concatenate $1, a space, and $2
          std::string combined = std::string((yystack_[4].value.id)) + " " + std::string((yystack_[3].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 966 "grammar.tab.cc"
    break;

  case 139: // struct_or_union_specifier: struct_or_union IDENTIFIER
#line 416 "grammar.y"
                                 {
          // Named struct/union declaration without body: concatenate $1 and $2 with a space
          std::string combined = std::string((yystack_[1].value.id)) + " " + std::string((yystack_[0].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 976 "grammar.tab.cc"
    break;

  case 140: // struct_or_union: STRUCT
#line 424 "grammar.y"
             { (yylhs.value.id) = strdup("struct"); }
#line 982 "grammar.tab.cc"
    break;

  case 141: // struct_or_union: UNION
#line 425 "grammar.y"
             { (yylhs.value.id) = strdup("union"); }
#line 988 "grammar.tab.cc"
    break;

  case 155: // enum_specifier: ENUM '{' enumerator_list '}'
#line 457 "grammar.y"
                                   {
          // Anonymous enum
          (yylhs.value.id) = strdup("enum");
      }
#line 997 "grammar.tab.cc"
    break;

  case 156: // enum_specifier: ENUM '{' enumerator_list ',' '}'
#line 461 "grammar.y"
                                       {
          (yylhs.value.id) = strdup("enum");
      }
#line 1005 "grammar.tab.cc"
    break;

  case 157: // enum_specifier: ENUM IDENTIFIER '{' enumerator_list '}'
#line 464 "grammar.y"
                                              {
          // Named enum: produce "enum <identifier>"
          std::string combined = std::string("enum ") + std::string((yystack_[3].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 1015 "grammar.tab.cc"
    break;

  case 158: // enum_specifier: ENUM IDENTIFIER '{' enumerator_list ',' '}'
#line 469 "grammar.y"
                                                  {
          std::string combined = std::string("enum ") + std::string((yystack_[4].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 1024 "grammar.tab.cc"
    break;

  case 159: // enum_specifier: ENUM IDENTIFIER
#line 473 "grammar.y"
                      {
          std::string combined = std::string("enum ") + std::string((yystack_[0].value.id));
          (yylhs.value.id) = strdup(combined.c_str());
      }
#line 1033 "grammar.tab.cc"
    break;

  case 164: // type_qualifier: CONST
#line 490 "grammar.y"
                { (yylhs.value.id) = strdup("CONST"); }
#line 1039 "grammar.tab.cc"
    break;

  case 165: // type_qualifier: VOLATILE
#line 491 "grammar.y"
                { (yylhs.value.id) = strdup("VOLATILE"); }
#line 1045 "grammar.tab.cc"
    break;

  case 166: // declarator: pointer direct_declarator
#line 496 "grammar.y"
                                {
          int idx = (yystack_[0].value.index);  // $2 is the token table index from direct_declarator.
          // Concatenate tokenTable[idx].token_type and the pointer string ($1)
          std::string newType = std::string(tokenTable[idx].token_type) + std::string((yystack_[1].value.id));
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType.c_str());
          if (newType.find("typedef") != std::string::npos) {
              update_symtab(tokenTable[idx].token);
          }
          (yylhs.value.index) = idx;
          free((yystack_[1].value.id)); // free the pointer string
      }
#line 1062 "grammar.tab.cc"
    break;

  case 167: // declarator: direct_declarator
#line 508 "grammar.y"
                        { (yylhs.value.index) = (yystack_[0].value.index); }
#line 1068 "grammar.tab.cc"
    break;

  case 168: // direct_declarator: IDENTIFIER
#line 512 "grammar.y"
                 {
          insert_table((yystack_[0].value.id), currentType ? currentType : "INVALID");
          (yylhs.value.index) = token_count - 1;
      }
#line 1077 "grammar.tab.cc"
    break;

  case 169: // direct_declarator: '(' declarator ')'
#line 516 "grammar.y"
                         { (yylhs.value.index) = (yystack_[1].value.index); }
#line 1083 "grammar.tab.cc"
    break;

  case 170: // direct_declarator: direct_declarator '[' ']'
#line 517 "grammar.y"
                                {
          int idx = (yystack_[2].value.index);
          std::string newType = std::string(tokenTable[idx].token_type) + "[]";
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType.c_str());
          if (newType.find("typedef") != std::string::npos) {
              update_symtab(tokenTable[idx].token);
          }
          (yylhs.value.index) = idx;
      }
#line 1098 "grammar.tab.cc"
    break;

  case 171: // direct_declarator: direct_declarator '[' '*' ']'
#line 527 "grammar.y"
      { (yylhs.value.index) = (yystack_[3].value.index); }
#line 1104 "grammar.tab.cc"
    break;

  case 172: // direct_declarator: direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
#line 528 "grammar.y"
      { (yylhs.value.index) = (yystack_[5].value.index); }
#line 1110 "grammar.tab.cc"
    break;

  case 173: // direct_declarator: direct_declarator '[' STATIC assignment_expression ']'
#line 529 "grammar.y"
      { (yylhs.value.index) = (yystack_[4].value.index); }
#line 1116 "grammar.tab.cc"
    break;

  case 174: // direct_declarator: direct_declarator '[' type_qualifier_list '*' ']'
#line 530 "grammar.y"
      { (yylhs.value.index) = (yystack_[4].value.index); }
#line 1122 "grammar.tab.cc"
    break;

  case 175: // direct_declarator: direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
#line 531 "grammar.y"
      { (yylhs.value.index) = (yystack_[5].value.index); }
#line 1128 "grammar.tab.cc"
    break;

  case 176: // direct_declarator: direct_declarator '[' type_qualifier_list assignment_expression ']'
#line 532 "grammar.y"
      { (yylhs.value.index) = (yystack_[4].value.index); }
#line 1134 "grammar.tab.cc"
    break;

  case 177: // direct_declarator: direct_declarator '[' type_qualifier_list ']'
#line 533 "grammar.y"
      { (yylhs.value.index) = (yystack_[3].value.index); }
#line 1140 "grammar.tab.cc"
    break;

  case 178: // direct_declarator: direct_declarator '[' assignment_expression ']'
#line 534 "grammar.y"
                                                      {
          int idx = (yystack_[3].value.index);
          std::string newType = std::string(tokenTable[idx].token_type) + "[" + std::string((yystack_[1].value.id)) + "]";
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup(newType.c_str());
          if (newType.find("typedef") != std::string::npos) {
              update_symtab(tokenTable[idx].token);
          }
          (yylhs.value.index) = idx;
      }
#line 1155 "grammar.tab.cc"
    break;

  case 179: // direct_declarator: direct_declarator '(' PushScope parameter_type_list ')'
#line 544 "grammar.y"
                                                              {
          // When there are parameters (e.g. fun(int a, int b))
          int idx = (yystack_[4].value.index);  // $1 is now the token table index.
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup("PROCEDURE");
          (yylhs.value.index) = idx;
      }
#line 1167 "grammar.tab.cc"
    break;

  case 180: // direct_declarator: direct_declarator '(' PushScope ')'
#line 551 "grammar.y"
                                          {
          int idx = (yystack_[3].value.index);
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup("PROCEDURE");
          (yylhs.value.index) = idx;
      }
#line 1178 "grammar.tab.cc"
    break;

  case 181: // direct_declarator: direct_declarator '(' PushScope identifier_list ')'
#line 557 "grammar.y"
                                                          {
          int idx = (yystack_[4].value.index);
          free(tokenTable[idx].token_type);
          tokenTable[idx].token_type = strdup("PROCEDURE");
          (yylhs.value.index) = idx;
      }
#line 1189 "grammar.tab.cc"
    break;

  case 182: // direct_declarator: direct_declarator '[' assignment_expression error
#line 563 "grammar.y"
                                                        {
          /* Catch an invalid array declaration (missing ']') */
          yyerrok;
          (yylhs.value.index) = (yystack_[3].value.index);  // Propagate the index as is.
      }
#line 1199 "grammar.tab.cc"
    break;

  case 185: // pointer: '*' pointer
#line 573 "grammar.y"
                  {
          // Prepend '*' to the pointer string from $2.
          std::string result = "*" + std::string((yystack_[0].value.id));
          (yylhs.value.id) = strdup(result.c_str());
          free((yystack_[0].value.id));
      }
#line 1210 "grammar.tab.cc"
    break;

  case 186: // pointer: '*'
#line 579 "grammar.y"
          {
          (yylhs.value.id) = strdup("*");
      }
#line 1218 "grammar.tab.cc"
    break;

  case 187: // type_qualifier_list: type_qualifier
#line 585 "grammar.y"
      { (yylhs.value.id) = (yystack_[0].value.id); }
#line 1224 "grammar.tab.cc"
    break;

  case 188: // type_qualifier_list: type_qualifier_list type_qualifier
#line 586 "grammar.y"
                                         { (yylhs.value.id) = (yystack_[1].value.id); }
#line 1230 "grammar.tab.cc"
    break;

  case 246: // compound_statement: '{' PushScope block_item_list error
#line 689 "grammar.y"
                                          { pop_scope(); yyerrok; }
#line 1236 "grammar.tab.cc"
    break;

  case 253: // expression_statement: expression error
#line 705 "grammar.y"
                       { yyerrok; }
#line 1242 "grammar.tab.cc"
    break;

  case 276: // function_definition: declaration_specifiers declarator error
#line 748 "grammar.y"
                                              { yyerrok; }
#line 1248 "grammar.tab.cc"
    break;

  case 279: // PushScope: %empty
#line 757 "grammar.y"
      { push_scope(); }
#line 1254 "grammar.tab.cc"
    break;

  case 280: // PopScope: %empty
#line 761 "grammar.y"
      { pop_scope(); }
#line 1260 "grammar.tab.cc"
    break;


#line 1264 "grammar.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short parser::yypact_ninf_ = -304;

  const signed char parser::yytable_ninf_ = -102;

  const short
  parser::yypact_[] =
  {
    -304,    62,   842,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,    47,    53,  -304,    71,   842,   842,
    -304,  -304,    56,  -304,   842,   842,  -304,  -304,    -5,    84,
      15,   207,  -304,  -304,    33,  -304,   126,  -304,    30,  -304,
     546,   165,    35,    20,    20,    55,  2004,    20,  -304,  -304,
      84,  -304,    59,   181,  -304,   207,    85,  -304,  -304,  -304,
     174,  -304,    72,   106,  -304,  -304,   126,  -304,    33,  -304,
    -304,  -304,  1413,  -304,    71,  -304,  1928,  -304,   966,   165,
    2004,  2004,  1846,  -304,   116,  2004,   223,  1581,  -304,    18,
    -304,   112,  -304,   179,   207,  -304,  -304,   117,  -304,  -304,
    -304,  -304,   134,   621,  -304,  -304,  -304,  -304,  -304,  1630,
    1630,  1663,  -304,  -304,   914,  1312,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,   159,   192,  1581,  -304,   109,
     221,   292,    34,   291,   145,   146,   173,   231,    43,  -304,
    -304,  -304,  -304,  -304,  -304,  1718,  1393,  -304,   220,    75,
    1018,  1887,  -304,  -304,  -304,  1581,  -304,   -36,  -304,   206,
    -304,  -304,    49,  -304,  -304,  -304,  -304,  -304,   207,  -304,
     187,   842,   230,  1581,   235,   247,   263,   265,   751,   272,
     341,   259,   267,   784,  -304,  -304,  -304,    31,  -304,  -304,
    -304,  -304,   460,  -304,  -304,  -304,  -304,  -304,   914,  -304,
    -304,   914,  -304,    25,   127,    76,  1581,   353,  -304,   237,
    1413,   111,  -304,   358,  -304,  -304,  1446,  1581,   359,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    1581,  -304,  1581,  1581,  1581,  1581,  1581,  1581,  1581,  1581,
    1581,  1581,  1581,  1581,  1581,  1581,  1581,  1581,  1581,  1581,
    1581,  -304,  -304,  -304,    66,   293,   286,  -304,   131,   294,
    1393,  -304,  -304,  -304,  1581,  -304,   295,   296,  -304,  -304,
      22,  -304,  1581,  -304,  -304,  -304,  1805,   751,   274,   751,
    1581,  1581,  1581,   176,  -304,   273,  -304,  -304,  -304,     7,
    -304,  -304,  1581,  -304,  -304,  -304,  -304,   299,   300,  -304,
    -304,  1761,  1070,   208,  -304,   226,  1581,  1495,   301,  -304,
    -304,  1103,  -304,  -304,  -304,  -304,  -304,   133,  -304,   172,
    -304,  -304,  -304,  -304,  -304,   109,   109,   221,   221,   292,
     292,   292,   292,    34,    34,   291,   145,   146,   173,   231,
     110,   402,  -304,    86,  -304,  -304,   842,  -304,   369,  -304,
     303,   304,  -304,  -304,  -304,  -304,  -304,  -304,   751,  -304,
     150,   175,   188,   310,   313,   702,  -304,  -304,  -304,  -304,
     309,   309,  -304,   315,   317,  1393,  -304,   316,   319,  1155,
     226,  1968,  1207,  -304,  1312,  -304,  -304,  -304,  -304,  1413,
    -304,  1581,  -304,  1581,  -304,  -304,  -304,  -304,  -304,   751,
     751,   751,  1581,  1581,   833,   833,  -304,  -304,   321,  1393,
    -304,  -304,  1581,  -304,   322,  -304,   325,  1393,  -304,   324,
     326,  1259,   254,  -304,  -304,  -304,   347,  -304,  -304,   190,
     193,  1528,  1548,  -304,   329,   331,  -304,  -304,   333,  1393,
    -304,  -304,  1581,  -304,   334,  -304,  1292,   751,   318,   344,
     751,   200,   751,   202,  -304,  -304,  -304,   335,   336,  -304,
    -304,  -304,  -304,  -304,  -304,   751,  -304,   751,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304
  };

  const short
  parser::yydefact_[] =
  {
     279,     0,     0,     1,   120,   102,   103,   104,   105,   106,
     108,   109,   110,   111,   114,   115,   112,   113,   164,   165,
     107,   116,   140,   141,     0,     0,   273,     0,    92,    94,
     118,   117,     0,   119,    96,   280,   270,   272,   159,     0,
     131,     0,    89,   168,     0,    97,   186,    87,     0,    98,
       0,   167,     0,    91,    93,   139,     0,    95,   271,   269,
       0,    10,   163,     0,   160,   135,   123,   127,   128,   129,
       0,   124,     0,     0,   187,   185,   184,    90,     0,    88,
     276,   279,     0,   277,     0,   280,     0,   279,     0,   166,
       0,   147,     0,   142,     0,   149,     0,     0,   155,     0,
     136,   130,   132,     0,     0,   121,   125,     0,   169,   188,
     183,    99,   101,     0,     2,     7,     8,    11,    12,     0,
       0,     0,    13,     9,     0,     0,    32,    33,    34,    35,
      36,    37,    14,     3,     4,    26,    38,     0,    41,    45,
      48,    51,    56,    59,    61,    63,    65,    67,    69,    71,
     225,   100,   275,   278,   280,     0,     0,   170,    33,     0,
       0,     0,   146,   137,   143,     0,   144,     0,   150,   154,
     148,   157,     0,    38,    86,   162,   156,   161,   135,   134,
       0,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,   251,    84,     0,   249,   250,
     235,   236,     0,   247,   237,   238,   239,   240,     0,    27,
      28,     0,    30,     0,   198,     0,     0,     0,   227,     0,
       0,     0,   231,     0,    20,    21,     0,     0,     0,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    73,
       0,    29,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   195,   180,   194,     0,   189,   190,     0,     0,
       0,   171,   182,   178,     0,   177,    33,     0,   138,   152,
       0,   145,     0,   158,   133,   122,     0,     0,     0,     0,
       0,     0,     0,     0,   279,     0,   265,   266,   267,     0,
     244,   253,     0,   252,   246,   280,   248,     0,     0,     6,
       5,     0,     0,   200,   197,   201,     0,     0,     0,   234,
     223,     0,   226,   230,   232,    19,    16,     0,    24,     0,
      18,    72,    42,    43,    44,    46,    47,    49,    50,    54,
      55,    52,    53,    57,    58,    60,    62,    64,    66,    68,
       0,     0,   192,   200,   193,   179,     0,   181,     0,   173,
       0,     0,   174,   176,   151,   153,   126,   241,     0,   243,
       0,     0,     0,     0,     0,     0,   264,   268,    85,   245,
       0,    31,   219,     0,     0,     0,   203,    33,     0,     0,
     199,     0,     0,    40,     0,    39,   233,   224,   229,     0,
      17,     0,    15,     0,   191,   196,   172,   175,   242,     0,
       0,     0,     0,     0,     0,     0,   220,   202,     0,     0,
     204,   210,     0,   209,     0,   221,     0,     0,   211,    33,
       0,     0,     0,   228,    25,    70,   255,   256,   257,     0,
       0,     0,     0,   206,     0,     0,   208,   222,     0,     0,
     212,   218,     0,   217,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,   205,   207,   214,     0,     0,   215,
      23,   254,   258,   259,   280,     0,   280,     0,   213,   216,
     262,   280,   260,   280,   263,   261
  };

  const short
  parser::yypgoto_[] =
  {
    -304,  -304,  -304,  -304,  -304,  -304,  -304,   171,  -304,  -126,
      92,    93,    37,   105,   155,   156,   197,   198,   196,  -304,
     -84,   -74,  -304,  -118,  -125,   -47,    -1,  -304,   379,  -304,
     -27,  -304,   354,   -51,   -43,  -304,  -304,   281,  -304,  -304,
    -304,   370,   -57,   -71,  -304,   182,  -304,   411,   -45,   -46,
     -11,   -48,   -39,   -76,  -140,  -304,   119,  -304,  -123,   -88,
    -258,   -77,    82,  -303,  -304,   256,  -128,  -304,    -7,  -304,
     276,  -236,  -304,  -304,  -304,  -304,   298,   -26,  -304,  -304,
     -64,   -83
  };

  const short
  parser::yydefgoto_[] =
  {
       0,   132,   133,    62,   134,   135,   327,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   196,   240,   197,   175,    26,    84,    48,    49,    28,
      29,    30,    70,    71,    72,    66,   101,   102,   103,    31,
      32,    92,    93,    94,   167,   168,    33,    63,    64,    34,
      73,    51,    52,    76,   383,   266,   267,   268,   215,   384,
     315,   218,   219,   220,   221,   222,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     1,    35,    36,    37,    86,
       2,    59
  };

  const short
  parser::yytable_[] =
  {
      74,    27,   152,    83,    89,   151,   213,    75,   150,    58,
      95,   241,   160,   174,   159,   265,    50,   113,   399,   106,
     162,    61,   100,   155,   170,    43,   309,    53,    54,    91,
     109,    77,   301,    57,    27,   164,    43,   110,    43,   153,
     279,   280,    74,    85,    95,    95,    95,   249,   250,    95,
      38,   150,    61,   214,   177,   390,    40,   281,   288,    55,
     293,   259,     3,    91,    91,    91,   198,   112,    91,    43,
      60,   261,    42,   112,    43,   299,   272,   316,    95,   154,
     270,   174,   269,   169,   302,   307,   277,    61,   308,    43,
     213,   318,    44,   213,   176,   390,   310,    91,    45,   174,
     377,    46,   302,    44,   164,    44,    65,    78,   302,   329,
      74,   300,    46,   165,   109,    95,   332,   333,   334,    43,
     251,   252,    39,    79,   303,   283,   314,   177,    41,   106,
      90,    56,   174,   260,    91,   100,   351,   214,   312,   415,
     214,    44,   350,   322,    45,    46,   150,   317,   273,    45,
      46,    97,   328,   399,   264,   198,   351,   365,   312,   367,
     104,   369,    95,   107,    47,    95,   331,   223,   224,   225,
      18,    19,   370,   371,   372,   313,   354,   108,   441,   442,
      27,    91,   179,   216,    91,   217,    44,   302,   242,   178,
     393,   395,   181,   243,   244,    46,   360,   311,   174,   312,
     361,   403,   357,   323,   400,    46,    46,   165,   358,   166,
     401,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   409,   379,   255,   109,   353,    82,   302,   378,   226,
     375,   227,   373,   228,   256,    87,   389,    88,   388,   374,
     408,    67,    68,    69,   398,   402,   410,   150,   258,   302,
     105,   426,   302,   352,    67,    68,    69,    98,    99,   411,
      58,   458,   257,   285,   459,   302,    74,   302,   173,   169,
     302,   475,   313,   477,    67,    68,    69,   302,   311,   302,
     312,   436,   437,   438,   239,    27,   339,   340,   341,   342,
     209,   210,   212,   271,   439,   440,   391,   282,   392,   171,
     172,   245,   246,   247,   248,    89,   253,   254,   173,   419,
     264,   418,   353,   320,   321,   424,   431,   290,   430,   435,
     150,   287,   433,   461,   463,   150,   289,   434,   414,   471,
     455,   456,   474,   291,   476,   292,   173,   335,   336,    74,
     337,   338,   294,   109,   295,   444,    74,   481,   445,   483,
     264,   449,   296,   448,   173,   264,   319,   454,   343,   344,
     297,   325,   330,   356,   355,   368,   376,   359,   362,   363,
     380,   381,   405,   109,   396,   467,   406,   407,   468,   398,
     412,    74,   150,   413,   394,   109,   416,   173,   417,   420,
     264,   480,   421,   482,   443,   446,   447,   450,   484,   451,
     485,   457,   464,   109,   465,    43,   466,   469,   478,   479,
     345,   472,   346,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,     4,     5,     6,     7,     8,     9,   473,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   173,   347,   349,   348,   111,   180,   284,
     161,   304,   364,   182,   115,   116,   117,   118,    25,   119,
     120,    96,   351,   382,   312,   404,   432,   324,   306,   286,
       0,    46,     0,     0,     0,     0,     0,   173,   173,     4,
       5,     6,     7,     8,     9,   121,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,   183,   184,   185,     0,   186,   187,   188,   189,   190,
     191,   192,   193,     0,   122,   123,    25,     0,     0,     0,
     124,     0,     0,     0,     0,    81,   305,     0,   126,   127,
     128,   129,   130,   131,     0,     0,     0,    80,     0,     0,
       0,     0,     0,   195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     4,     5,     6,     7,     8,
       9,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,     0,     0,
       0,    81,     0,  -101,   182,   115,   116,   117,   118,     0,
     119,   120,     0,     0,     0,     0,     0,     0,    82,  -101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,   121,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,   183,   184,   185,     0,   186,   187,   188,   189,
     190,   191,   192,   193,     0,   122,   123,    25,     0,     0,
       0,   124,     0,     0,     0,     0,    81,   194,     0,   126,
     127,   128,   129,   130,   131,   114,   115,   116,   117,   118,
       0,   119,   120,     0,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     9,   121,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,   182,   115,   116,   117,   118,     0,
     119,   120,     0,     0,     0,     0,   122,   123,    25,     0,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
     126,   127,   128,   129,   130,   131,   121,   114,   115,   116,
     117,   118,     0,   119,   120,   195,     0,     0,     0,     0,
       0,     0,   183,   184,   185,     0,   186,   187,   188,   189,
     190,   191,   192,   193,     0,   122,   123,     0,     0,   121,
       0,   124,     0,     0,     0,     0,    81,     0,     0,   126,
     127,   128,   129,   130,   131,     0,   114,   115,   116,   117,
     118,     0,   119,   120,   195,     0,     0,     0,   122,   123,
       0,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,   126,   127,   128,   129,   130,   131,   121,     0,
       0,     4,     5,     6,     7,     8,     9,   298,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,     0,     0,     0,   122,   123,     0,
       0,     0,     0,   124,     0,     0,     0,     0,    25,     0,
       0,   126,   127,   128,   129,   130,   131,   114,   115,   116,
     117,   118,     0,   119,   120,     0,   195,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     0,     0,     0,     0,   121,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,   114,
     115,   116,   117,   118,     0,   119,   120,     0,   122,   123,
      25,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,   126,   127,   128,   129,   130,   131,   156,     0,
       0,   121,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,     0,   119,   120,     0,
     122,   123,     0,     0,     0,     0,   124,     0,     0,   157,
       0,     0,     0,     0,   126,   158,   128,   129,   130,   131,
     274,     0,     0,   121,     0,     0,     0,     0,     0,     0,
       0,     0,    18,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,   117,   118,     0,   119,
     120,     0,   122,   123,     0,     0,     0,     0,   124,     0,
       0,   275,     0,     0,     0,     0,   126,   276,   128,   129,
     130,   131,   385,     0,     0,   121,   114,   115,   116,   117,
     118,     0,   119,   120,    18,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,   123,     0,     0,   121,     0,
     124,     0,     0,   386,     0,     0,     0,     0,   126,   387,
     128,   129,   130,   131,     0,     0,     0,     0,   114,   115,
     116,   117,   118,     0,   119,   120,     0,   122,   123,     0,
       0,     0,     0,   124,     0,   216,     0,   217,   125,   397,
       0,   126,   127,   128,   129,   130,   131,   422,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   115,   116,   117,   118,     0,   119,   120,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,   423,     0,
       0,     0,     0,   126,   127,   128,   129,   130,   131,   427,
       0,     0,   121,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,   115,   116,   117,   118,     0,   119,   120,
       0,   122,   123,     0,     0,     0,     0,   124,     0,     0,
     428,     0,     0,     0,     0,   126,   429,   128,   129,   130,
     131,   452,     0,     0,   121,   114,   115,   116,   117,   118,
       0,   119,   120,    18,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
       0,   119,   120,   122,   123,     0,     0,   121,     0,   124,
       0,     0,   453,     0,     0,     0,     0,   126,   127,   128,
     129,   130,   131,     0,     0,     0,     0,   121,     0,     0,
       0,     0,     0,     0,     0,     0,   122,   123,     0,     0,
       0,     0,   124,     0,   216,     0,   217,   125,   470,     0,
     126,   127,   128,   129,   130,   131,   122,   123,     0,     0,
       0,     0,   124,     0,   216,     0,   217,   125,     0,     0,
     126,   127,   128,   129,   130,   131,   114,   115,   116,   117,
     118,     0,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   115,   116,   117,
     118,     0,   119,   120,     0,     0,     0,     0,   121,     0,
       0,     0,     0,     0,     0,     0,     0,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   114,
     115,   116,   117,   118,     0,   119,   120,   122,   123,     0,
       0,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,   126,   127,   128,   129,   130,   131,   122,   123,     0,
       0,   121,     0,   124,     0,     0,     0,     0,   125,     0,
       0,   126,   127,   128,   129,   130,   131,     0,   114,   115,
     116,   117,   118,     0,   119,   120,     0,     0,     0,     0,
     122,   123,     0,     0,     0,     0,   124,   326,     0,     0,
       0,     0,     0,     0,   126,   127,   128,   129,   130,   131,
     121,   114,   115,   116,   117,   118,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,     0,   119,   120,   122,
     123,     0,     0,   121,     0,   124,     0,     0,     0,     0,
     394,     0,     0,   126,   127,   128,   129,   130,   131,     0,
       0,     0,     0,   121,   114,   115,   116,   117,   118,     0,
     119,   120,   122,   123,     0,     0,     0,     0,   124,   460,
       0,     0,     0,     0,     0,     0,   126,   127,   128,   129,
     130,   131,   122,   123,     0,     0,   121,     0,   124,   462,
       0,     0,     0,     0,     0,     0,   126,   127,   128,   129,
     130,   131,     0,   114,   115,   116,   117,   118,     0,   119,
     120,     0,     0,     0,     0,   122,   123,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,   126,
     127,   128,   129,   130,   131,   121,   114,   115,   116,   117,
     118,     0,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,   123,     0,     0,   121,     0,
     208,     0,     0,     0,     0,     0,     0,     0,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,     0,   122,   123,     0,
       0,     0,     0,   211,     0,     0,     0,     0,     0,     0,
       0,   126,   127,   128,   129,   130,   131,     4,     5,     6,
       7,     8,     9,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,   263,
       4,     5,     6,     7,     8,     9,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,   311,   382,   312,     4,     5,     6,     7,     8,     9,
      46,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     4,     0,     0,     0,     0,
       0,   366,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     4,     0,     0,     0,
       0,     0,   163,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     4,     5,     6,
       7,     8,     9,   278,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     4,     5,     6,
       7,     8,     9,    81,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,    25,     0,     0,     0,     0,   425,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25
  };

  const short
  parser::yycheck_[] =
  {
      46,     2,    85,    50,    52,    82,   124,    46,    82,    35,
      56,   137,    88,    97,    88,   155,    27,    81,   321,    70,
      91,     3,    65,    87,    95,     3,     1,    28,    29,    56,
      76,     1,     1,    34,    35,    92,     3,    76,     3,    86,
     165,    77,    88,    50,    90,    91,    92,    13,    14,    95,
       3,   125,     3,   124,    99,   313,     3,    93,   183,     3,
     188,    18,     0,    90,    91,    92,   113,    78,    95,     3,
      75,   154,     1,    84,     3,   193,     1,     1,   124,    86,
     156,   165,   156,    94,    77,   208,   160,     3,   211,     3,
     208,   216,    70,   211,    76,   353,    71,   124,    78,   183,
      93,    79,    77,    70,   161,    70,    91,    77,    77,   227,
     156,   194,    79,    91,   160,   161,   242,   243,   244,     3,
      86,    87,    75,    93,    93,    76,   214,   172,    75,   180,
      75,    75,   216,    90,   161,   178,    70,   208,    72,   375,
     211,    70,   260,   220,    78,    79,   220,    71,    73,    78,
      79,    92,   226,   456,   155,   202,    70,   282,    72,   287,
      75,   289,   208,    91,    93,   211,   240,     8,     9,    10,
      44,    45,   290,   291,   292,   214,   264,    71,   414,   415,
     181,   208,     3,    72,   211,    74,    70,    77,    79,    77,
     316,   317,    75,    84,    85,    79,   270,    70,   282,    72,
     274,    91,    71,    92,    71,    79,    79,    91,    77,    93,
      77,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    71,   305,    78,   270,   264,    92,    77,   302,    70,
     294,    72,    56,    74,    88,    70,   312,    72,   312,    63,
     368,    67,    68,    69,   321,    73,    71,   321,    17,    77,
      76,   391,    77,   264,    67,    68,    69,    76,    77,    71,
     286,    71,    89,    76,    71,    77,   312,    77,    97,   280,
      77,    71,   311,    71,    67,    68,    69,    77,    70,    77,
      72,   409,   410,   411,    92,   286,   249,   250,   251,   252,
     119,   120,   121,    73,   412,   413,    70,    91,    72,    76,
      77,    80,    81,    11,    12,   353,    15,    16,   137,   385,
     311,   385,   351,    76,    77,   389,   392,    70,   392,   403,
     394,    91,   399,   441,   442,   399,    91,   401,   375,   457,
      76,    77,   460,    70,   462,    70,   165,   245,   246,   385,
     247,   248,    70,   389,     3,   419,   392,   475,   422,   477,
     351,   427,    93,   427,   183,   356,     3,   431,   253,   254,
      93,     3,     3,    77,    71,    91,    93,    73,    73,    73,
      71,    71,     3,   419,    73,   449,    73,    73,   452,   456,
      70,   427,   456,    70,    75,   431,    71,   216,    71,    73,
     391,   474,    73,   476,    73,    73,    71,    73,   481,    73,
     483,    54,    73,   449,    73,     3,    73,    73,    73,    73,
     255,    93,   256,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    29,    30,    31,    32,    33,    34,    93,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,   282,   257,   259,   258,    78,   104,   178,
      90,     1,   280,     3,     4,     5,     6,     7,    66,     9,
      10,    60,    70,    71,    72,   356,   394,   221,   202,   181,
      -1,    79,    -1,    -1,    -1,    -1,    -1,   316,   317,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,     1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   403,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,     3,     4,     5,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,     3,     4,     5,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    83,    35,     3,     4,     5,
       6,     7,    -1,     9,    10,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    -1,    -1,    35,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    82,    83,    -1,     3,     4,     5,     6,
       7,    -1,     9,    10,    93,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    35,    -1,
      -1,    29,    30,    31,    32,    33,    34,    93,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    66,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    71,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    38,
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
      -1,    -1,    -1,    29,    66,    -1,    -1,    -1,    -1,    71,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,   169,   174,     0,    29,    30,    31,    32,    33,    34,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    66,   119,   120,   123,   124,
     125,   133,   134,   140,   143,   170,   171,   172,     3,    75,
       3,    75,     1,     3,    70,    78,    79,    93,   121,   122,
     144,   145,   146,   120,   120,     3,    75,   120,   171,   175,
      75,     3,    97,   141,   142,    91,   129,    67,    68,    69,
     126,   127,   128,   144,   143,   146,   147,     1,    77,    93,
       1,    75,    92,   119,   120,   162,   173,    70,    72,   145,
      75,   124,   135,   136,   137,   143,   141,    92,    76,    77,
     128,   130,   131,   132,    75,    76,   127,    91,    71,   143,
     146,   122,   144,   174,     3,     4,     5,     6,     7,     9,
      10,    35,    64,    65,    70,    75,    78,    79,    80,    81,
      82,    83,    95,    96,    98,    99,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   155,   175,   119,   162,   174,    32,    73,    79,   115,
     147,   135,   137,    76,   136,    91,    93,   138,   139,   144,
     137,    76,    77,   101,   114,   118,    76,   142,    77,     3,
     126,    75,     3,    51,    52,    53,    55,    56,    57,    58,
      59,    60,    61,    62,    76,    93,   115,   117,   119,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    70,   101,
     101,    70,   101,   117,   137,   152,    72,    74,   155,   156,
     157,   158,   159,     8,     9,    10,    70,    72,    74,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    92,
     116,   103,    79,    84,    85,    80,    81,    11,    12,    13,
      14,    86,    87,    15,    16,    78,    88,    89,    17,    18,
      90,   175,     3,    71,   120,   148,   149,   150,   151,   115,
     147,    73,     1,    73,    32,    73,    79,   115,    76,   118,
      77,    93,    91,    76,   131,    76,   170,    91,   118,    91,
      70,    70,    70,   160,    70,     3,    93,    93,    93,   117,
     175,     1,    77,    93,     1,    76,   164,   152,   152,     1,
      71,    70,    72,   146,   153,   154,     1,    71,   118,     3,
      76,    77,   155,    92,   159,     3,    71,   100,   115,   117,
       3,   115,   103,   103,   103,   104,   104,   105,   105,   106,
     106,   106,   106,   107,   107,   108,   109,   110,   111,   112,
     117,    70,   144,   146,   153,    71,    77,    71,    77,    73,
     115,   115,    73,    73,   139,   118,    76,   160,    91,   160,
     117,   117,   117,    56,    63,   174,    93,    93,   115,   175,
      71,    71,    71,   148,   153,    32,    73,    79,   115,   147,
     154,    70,    72,   103,    75,   103,    73,    76,   155,   157,
      71,    77,    73,    91,   150,     3,    73,    73,   160,    71,
      71,    71,    70,    70,   119,   165,    71,    71,   115,   147,
      73,    73,    32,    73,   115,    71,   148,    32,    73,    79,
     115,   147,   156,   155,   115,   114,   160,   160,   160,   117,
     117,   165,   165,    73,   115,   115,    73,    71,   115,   147,
      73,    73,    32,    73,   115,    76,    77,    54,    71,    71,
      71,   117,    71,   117,    73,    73,    73,   115,   115,    73,
      76,   160,    93,    93,   160,    71,   160,    71,    73,    73,
     175,   160,   175,   160,   175,   175
  };

  const unsigned char
  parser::yyr1_[] =
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
     170,   170,   171,   171,   172,   172,   172,   173,   173,   174,
     175
  };

  const signed char
  parser::yyr2_[] =
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
       3,     4,     6,     5,     5,     6,     5,     4,     4,     5,
       4,     5,     4,     3,     2,     2,     1,     1,     2,     1,
       1,     3,     2,     2,     1,     1,     3,     2,     1,     2,
       1,     1,     3,     2,     3,     5,     4,     5,     4,     3,
       3,     3,     4,     6,     5,     5,     6,     4,     4,     2,
       3,     3,     4,     3,     4,     1,     2,     1,     4,     3,
       2,     1,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     5,     4,     1,     2,     1,
       1,     1,     2,     2,     7,     5,     5,     5,     7,     7,
       8,     9,     8,     9,     3,     2,     2,     2,     3,     3,
       1,     2,     1,     1,     5,     4,     3,     1,     2,     0,
       0
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
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
  "selection_statement", "iteration_statement", "jump_statement", "Global",
  "translation_unit", "external_declaration", "function_definition",
  "declaration_list", "PushScope", "PopScope", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    55,    55,    62,    63,    64,    65,    69,    70,    71,
      75,    79,    80,    81,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    99,   100,   104,   105,   106,   107,
     108,   109,   113,   114,   115,   116,   117,   118,   122,   123,
     124,   128,   129,   130,   131,   135,   136,   137,   141,   142,
     143,   147,   148,   149,   150,   151,   155,   156,   157,   161,
     162,   166,   167,   171,   172,   176,   177,   181,   182,   186,
     187,   191,   192,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   210,   211,   215,   219,   220,   221,
     222,   226,   233,   237,   244,   249,   256,   260,   265,   266,
     270,   271,   275,   276,   277,   278,   279,   283,   288,   293,
     298,   303,   308,   313,   318,   323,   328,   333,   334,   335,
     336,   343,   347,   351,   358,   359,   363,   367,   368,   369,
     374,   375,   379,   380,   388,   400,   401,   406,   411,   416,
     424,   425,   429,   430,   434,   435,   439,   440,   441,   442,
     446,   447,   451,   452,   453,   457,   461,   464,   469,   473,
     480,   481,   485,   486,   490,   491,   496,   508,   512,   516,
     517,   527,   528,   529,   530,   531,   532,   533,   534,   544,
     551,   557,   563,   571,   572,   573,   579,   585,   586,   590,
     594,   595,   599,   600,   601,   605,   606,   610,   611,   615,
     616,   617,   621,   622,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   645,   646,   647,   651,   652,   653,   654,
     658,   662,   663,   667,   668,   672,   673,   674,   675,   676,
     677,   681,   682,   683,   687,   688,   689,   693,   694,   698,
     699,   703,   704,   705,   709,   710,   711,   714,   715,   716,
     717,   718,   719,   720,   724,   725,   726,   727,   728,   732,
     736,   737,   741,   742,   746,   747,   748,   752,   753,   757,
     761
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
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
    // Last valid token kind.
    const int code_max = 324;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 2309 "grammar.tab.cc"

#line 763 "grammar.y"


void yyerror(const char *s) {
    parser_error++;
    fflush(stdout);
    std::cerr << "Syntax Error: " << s << " at line " << line_no << std::endl;
}

int main(int argc, char **argv) {
    yydebug = 1;
    int parserresult = yyparse(); // Parser calls yylex() internally

    if (parserresult == 0 && error_count == 0 && parser_error == 0) {
        std::cout << "LEX and Parsing Success" << std::endl;
        print_token_table();
    } else {
        if (error_count > 0) {
            std::cout << "Errors in LEX stage:" << std::endl
                      << "PARSING FAILED." << std::endl;
            print_errors();
        } else {
            std::cout << "Error in parsing with errorcode: " << parserresult << std::endl;
        }
    }
    return 0;
}
