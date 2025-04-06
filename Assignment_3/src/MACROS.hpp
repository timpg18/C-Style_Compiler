#pragma once

// MACRO for expression checking
// THIS MUST BE ADDED TO CHECK IF ITS A PROCEDURE THEN IT MUST BE CALLED
// and Removing const and static
#define CHECK_PROCEDURE_AND_CLEAN_TYPE(node1, node2) \
    do { \
        if (isPROCEDURE((node1).kind)) { yyerror("incompatible return type "); } \
        if (isPROCEDURE((node2).kind)) { yyerror("incompatible return type "); } \
        type1 = ts.prepareString(std::string((node1).type)); \
        type2 = ts.prepareString(std::string((node2).type)); \
        (node1).type = strdup(type1.c_str()); \
        (node2).type = strdup(type2.c_str()); \
    } while(0)

// macro for checking if constant and changing the code
#define HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS(EXPR, IRGEN)                          \
    do {                                                                               \
        std::vector<std::string> tmpcheck = EXPR.backpatcher->getTrueList();          \
        if (tmpcheck.empty()) {                                                       \
            std::string label1 = IRGEN.new_tmp_label();                               \
            std::string label2 = IRGEN.new_tmp_label();                               \
            std::string condition = std::string(EXPR.ir.tmp) + " == 0";               \
            std::string if_goto = IRGEN.create_if_goto(condition, label2);            \
            std::string goto_ = IRGEN.create_goto(label1);                            \
            EXPR.backpatcher->addToTrueList(label1);                                  \
            EXPR.backpatcher->addToFalseList(label2);                                 \
            EXPR.ir.code = strdup(IRGEN.concatenate(EXPR.ir.code, if_goto).c_str());         \
            EXPR.ir.code = strdup(IRGEN.concatenate(EXPR.ir.code, goto_).c_str());         \
        }                                                                              \
    } while (0)

// for evaluation where there is a bool expression 
#define HANDLE_BOOL_RESULT_ASSIGN(E1, EFinal, E2, irgen)                                            \
    do {                                                                                            \
        std::string label1 = irgen.new_label();                                                     \
        std::string true_label = irgen.add_label(label1);                                           \
                                                                                                    \
        std::string label2 = irgen.new_label();                                                     \
        std::string false_label = irgen.add_label(label2);                                          \
                                                                                                    \
        std::string label3 = irgen.new_label();                                                     \
        std::string end_label = irgen.add_label(label3);                                            \
        std::string goto_end = irgen.create_goto(label3);                                           \
                                                                                                    \
        std::string assign_true = irgen.assign(E1.ir.tmp, "1");                                     \
        std::string assign_false = irgen.assign(E1.ir.tmp, "0");                                    \
                                                                                                    \
        E2.ir.code = strdup(E2.backpatcher->backPatchTrueList(std::string(E2.ir.code), label1).c_str()); \
        E2.ir.code = strdup(E2.backpatcher->backPatchFalseList(std::string(E2.ir.code), label2).c_str()); \
                                                                                                    \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(E2.ir.code), true_label).c_str());    \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(EFinal.ir.code), assign_true).c_str()); \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(EFinal.ir.code), goto_end).c_str());  \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(EFinal.ir.code), false_label).c_str()); \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(EFinal.ir.code), assign_false).c_str()); \
        EFinal.ir.code = strdup(irgen.concatenate(std::string(EFinal.ir.code), end_label).c_str()); \
    } while(0)

#define CONVERT_BOOL_EXPR_TO_VALUE(EXPR)                                                                 \
    {                                                                                                    \
        std::vector<std::string> tmpcheck = EXPR.backpatcher->getTrueList();                             \
        int size = tmpcheck.size();                                                                      \
        if (size > 0) {                                                                                  \
            /* Label making */                                                                           \
            std::string label1 = irgen.new_label();                                                      \
            std::string True_label = irgen.add_label(label1);                                            \
            std::string label2 = irgen.new_label();                                                      \
            std::string False_label = irgen.add_label(label2);                                           \
            std::string label3 = irgen.new_label();                                                      \
            std::string False_end = irgen.add_label(label3);                                             \
            std::string goto_false_end = irgen.create_goto(label3);                                      \
                                                                                                         \
            /* Temp assignment for result */                                                             \
            std::string tmpreg1 = irgen.new_temp();                                                      \
            EXPR.ir.tmp = strdup(tmpreg1.c_str());                                                       \
            std::string true_assign = irgen.assign(EXPR.ir.tmp, "1");                                    \
            std::string false_assign = irgen.assign(EXPR.ir.tmp, "0");                                   \
                                                                                                         \
            /* Backpatching */                                                                           \
            EXPR.ir.code = strdup(EXPR.backpatcher->backPatchTrueList(std::string(EXPR.ir.code), label1).c_str()); \
            EXPR.ir.code = strdup(EXPR.backpatcher->backPatchFalseList(std::string(EXPR.ir.code), label2).c_str());\
                                                                                                         \
            /* Code generation */                                                                        \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), True_label).c_str());     \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), true_assign).c_str());    \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), goto_false_end).c_str()); \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), False_label).c_str());    \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), false_assign).c_str());   \
            EXPR.ir.code = strdup(irgen.concatenate(std::string(EXPR.ir.code), False_end).c_str());      \
        }                                                                                                \
    }


#define IS_INT_LIKE_TYPE(t) ( \
    contains(t, "INT") || \
    contains(t, "CHAR") || \
    contains(t, "SHORT") || \
    contains(t, "BOOL") || \
    contains(t, "UNSIGNED INT") || \
    contains(t, "UNSIGNED CHAR") || \
    contains(t, "UNSIGNED SHORT") \
)

#define CHECK_ACCESS(s) \
    do { \
        char* kind_str = strdup(st.lookup(s)->kind.c_str()); \
        if (contains(kind_str, "PRIVATE")) { \
            yyerror("Cannot access the private member of the class"); \
        } else if (contains(kind_str, "PROTECTED")) { \
            yyerror("Cannot access the protected member of the class"); \
        } \
    } while (0)
