/**
 * @file codegen.h
 * Provides functions to generate java bytecode.
 */

#ifndef _H_CODEGEN_
#define _H_CODEGEN_

#include <stdio.h>
#include <stdlib.h>
#include "semantic_tables.h"

/**
 * Provides numbers of `methodref` constants on RTL functions and methods.
 */
enum cg_rtl_methodrefs {
    // Mixed type methods
    MIXED_ADD = 6,
    MIXED_SUB = 10,
    MIXED_MUL = 14,
    MIXED_DIV = 18,
    MIXED_MOD = 22,
    MIXED_GET = 26,
    MIXED_PUT = 30,
    MIXED_REMOVE = 34,
    MIXED_NOT = 38,
    MIXED_AND = 42,
    MIXED_OR = 46,
    MIXED_EQ = 50,
    MIXED_NEQ = 54,
    MIXED_GR = 58,
    MIXED_GREQ = 62,
    MIXED_LOEQ = 66,
    MIXED_LO = 70,

    // Library functions
    LIB_LASSERT2 = 76,
    LIB_LASSERT = 80,
    LIB_ERROR2 = 94,
    LIB_ERROR = 98,
    LIB_TONUMBER = 102,
    LIB_TONUMBER2 = 106,
    LIB_TOSTRING = 110,
    LIB_PRINT = 114,
    LIB_IOREAD = 118
};

int offset = 0;

/**
 * Calls perror(s) and exit(c);
 * @param [in] c Exit code.
 * @param [in] s Explanatory message.
 */
void crt_exception(int c, const char * s);

/**
 * Similar to stdlib write, but handles errors automatically.
 * @param [in] fd       File descriptor.
 * @param [in] ptr      Data to write.
 * @param [in] numbytes A number of bytes to write.
 * @return write's return.
 */
int  swrite(int fd, void * ptr, int numbytes);

/**
 * Generates .class-files for given tree (with function .class-files).
 * Convenience method.
 * @pre The tree must be prepared with st_fill_tables() function.
 * @param [in] root Tree root.
 */
void cg_generate_bytecode(struct NStmtList * root);

/**
 * Appends RTL functions metodref's to .class file.
 * @param [in] fd File descriptor.
 * @return Number of bytes written.
 */
int cg_write_lib_constants(int fd);

/**
 * Appends Mixed type functions metodref's to .class file.
 * @param [in] fd File descriptor.
 * @return Number of bytes written.
 */
int cg_write_mixed_constants(int fd);

/**
 * Appends RTL function metodref's to .class file.
 * @param [in] fd         File descriptor.
 * @param [in] name       Method name.
 * @param [in] arcgsnum   Number of method's arguments.
 * @param [in] rtype      Java qualified name of return type.
 * @param [in] classnum   Number of CONST_CLASS constant.
 * @param [in] need_name  Also write CONST_UTF8 with function name.
 * @return Number of bytes written.
 */
int cg_write_methodref(int fd, const char * name, int argsnum, const char * rtype, int classnum, int need_name);

/**
 * Writes a single constant.
 * @param [in] fd         File descriptor.
 * @param [in] c          Constant to write.
 * @param [in] offset     Offset for argauments.
 * @return Number of bytes written.
 */
int cg_write_constant(int fd, STConst * c, int offset);

/**
 * Writes constant table entries of given table.
 * All indexes will be shifted by offset.
 * @pre global offset must be calculated.
 * @param [in] fd         File descriptor.
 * @param [in] table      Constant table.
 * @param [in] skip_first Skip first entry.
 * @return Number of bytes written.
 */
int cg_write_constants(int fd, STConst * table, int skip_first);

int cg_code_for_stmtlist(struct NStmtList * list, int file);

int cg_code_for_stmt(struct NStmt * stmt, int file);

int cg_code_for_expr(struct NExpr * expr, int file);

int cg_calculate_offset_stmt(struct NStmt* stmt);

int cg_calculate_offset_stmtlist(struct NStmtList* stmtlist);

int cg_calculate_offset_expr(struct NExpr);

/***************************************************************************************************/

void crt_exception(int c, const char * s) {
    perror(s);
    exit(c);
}

int swrite(int fd, void * ptr, int numbytes) {
    static int w;
    w = write(fd, ptr, numbytes);
    if (w <= 0) {
        crt_exception(2, "Writing .class file data");
    }
    return w;
}

void cg_generate_bytecode(struct NStmtList * root) {
    int mc;
    int s4;
    unsigned int u4;
    short int s2;
    unsigned short int u2;
    int bytes_written, numconst;
    STConst tmpc;
    char buf[512];

    mc = open("Main.class", O_WRONLY | O_CREAT | O_TRUNC, 644);
    if (mc < 0) {
        crt_exception(2, "Main.class");
    }

    // Magic number
    u4 = htonl(0xCAFEBABE);
    swrite(mc, (void *)&u4, 4);

    // Subversion and version
    u2 = 0;
    swrite(mc, (void *)&u2, 2);
    u2 = htons(51);
    swrite(mc, (void *)&u2, 2);

    // Temporary number of constants
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // TODO Constants table
    bytes_written = 0;
    bytes_written += cg_write_mixed_constants(mc);
    bytes_written += cg_write_lib_constants(mc);
    bytes_written += cg_write_constants(mc, st_const_table, 1);

    numconst = offset + st_const_count(st_const_table) - 1;

    // Current class constant
    // UTF8
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "LMain");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    // Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = numconst;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;

    // Actual number of constants
    lseek(mc, -bytes_written - 2, SEEK_CUR);
    u2 = htons((unsigned short int)numconst + 1);
    swrite(mc, (void *)&u2, 2);
    lseek(mc, 0, SEEK_END);

    // Access flags
    u2 = htons(0x02 | 0x01);
    swrite(mc, (void *)&u2, 2);

    // Current class (CONST_CLASS)
    u2 = htons(numconst);
    swrite(mc, (void *)&u2, 2);

    // TODO Parent class (CONST_CLASS)
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // Number of interfaces
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // TODO Number of fields
    swrite(mc, (void *)&u2, 2);

    // TODO Fields table
    

    // TODO Number of methods
    swrite(mc, (void *)&u2, 2);

    // TODO Methods table
    

    // Number of attributes
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    close(mc);
}

int cg_write_lib_constants(int fd) {
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    static int i,name;
    static int bytes_written;
    static char buf[1024];

    bytes_written = 0;

    strcpy(buf,"Lrtl/Lib");
    u1 = 1;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(strlen("Lrtl/Lib"));
    bytes_written += swrite(fd, (void *)&u2, 2);
    bytes_written += swrite(fd, (void *)&buf, 8);
    offset++;
    name = offset;

    // Class
    u1 = 7;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(name);
    bytes_written += swrite(fd, (void *)&u2, 2);
    offset++;
    int cn = offset;

    //lassert 2
    bytes_written += cg_write_methodref(fd, "lassert",2,"Lrtl/Mixed;", cn, 1);
    //lassert 1
    bytes_written += cg_write_methodref(fd, "lassert",1,"Lrtl/Mixed;", cn, 0);
    //error 2
    bytes_written += cg_write_methodref(fd, "error",2,"void;", cn, 1);
    //error 1
    bytes_written += cg_write_methodref(fd, "error",1,"void;", cn, 0);
    //tonumber 1
    bytes_written += cg_write_methodref(fd, "tonumber",1,"Lrtl/Mixed;", cn, 1);
    //tonumber 2
    bytes_written += cg_write_methodref(fd, "tonumber",2,"Lrtl/Mixed;", cn, 0);
    //tostring
    bytes_written += cg_write_methodref(fd, "tostring",1,"Lrtl/Mixed;", cn, 1);
    //print
    bytes_written += cg_write_methodref(fd, "print",1,"void;", cn, 1);
    //ioread
    bytes_written += cg_write_methodref(fd, "ioread",1,"Lrtl/Mixed;", cn, 1);

    return bytes_written;
}

int cg_write_mixed_constants(int fd) {
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    static int i, name;
    static int bytes_written;
    static char buf[1024];

    bytes_written = 0;

    // Class name UTF8
    strcpy(buf, "Lrtl/Mixed");
    u1 = 1;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(strlen(buf));
    bytes_written += swrite(fd, (void *)&u2, 2);
    bytes_written += swrite(fd, (void *)&buf, strlen(buf));
    offset++;
    name = offset;

    // Class
    u1 = 7;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(name);
    bytes_written += swrite(fd,(void*)&u2,2);
    offset++;
    int cn = offset;

    //add
    bytes_written += cg_write_methodref(fd, "add",1,"Lrtl/Mixed;", cn, 1);
    //sub
    bytes_written += cg_write_methodref(fd, "sub",1,"Lrtl/Mixed;", cn, 1);
    //mul
    bytes_written += cg_write_methodref(fd, "mul",1,"Lrtl/Mixed;", cn, 1);
    //div
    bytes_written += cg_write_methodref(fd, "div",1,"Lrtl/Mixed;", cn, 1);
    //modcgfcb,j
    bytes_written += cg_write_methodref(fd, "mod",1,"Lrtl/Mixed;", cn, 1);
    //get
    bytes_written += cg_write_methodref(fd, "get",1,"Lrtl/Mixed;", cn, 1);
    //put
    bytes_written += cg_write_methodref(fd, "put",2,"Lrtl/Mixed;", cn, 1);
    //remove
    bytes_written += cg_write_methodref(fd, "remove",1,"Lrtl/Mixed;", cn, 1);
    //not
    bytes_written += cg_write_methodref(fd, "not",0,"Lrtl/Mixed;", cn, 1);
    //and
    bytes_written += cg_write_methodref(fd, "and",1,"Lrtl/Mixed;", cn, 1);
    //or
    bytes_written += cg_write_methodref(fd, "or",1,"Lrtl/Mixed;", cn, 1);
    //eq
    bytes_written += cg_write_methodref(fd, "eq",1,"Lrtl/Mixed;", cn, 1);
    //neq
    bytes_written += cg_write_methodref(fd, "neq",1,"Lrtl/Mixed;", cn, 1);
    //gr
    bytes_written += cg_write_methodref(fd, "gr",1,"Lrtl/Mixed;", cn, 1);
    //greq
    bytes_written += cg_write_methodref(fd, "greq",1,"Lrtl/Mixed;", cn, 1);
    //loeq
    bytes_written += cg_write_methodref(fd, "loeq",1,"Lrtl/Mixed;", cn, 1);
    //lo
    bytes_written += cg_write_methodref(fd, "lo",1,"Lrtl/Mixed;", cn, 1);

    return bytes_written;
}

int cg_write_methodref(int fd, const char* name, int argsnum, const char* rtype, int classnum, int need_name) {
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    static int i, namen, handle;
    static int bytes_written;
    static char buf[1024];

    bytes_written= 0;

    // Method name UTF8
    if (need_name) {
        u1 = 1;
        bytes_written += swrite(fd, (void *)&u1, 1);
        strcpy(buf, name);
        u2 = htons(strlen(buf));
        bytes_written += swrite(fd, (void *)&u2, 2);
        bytes_written += swrite(fd, (void *)&buf, strlen(buf));
        offset++;
        namen = offset;
    }

    // Method handle UTF8
    strcpy(buf, "(");
    for (i = 0; i < argsnum; i++) {
        strcat(buf, "Lrtl/Mixed;");
    }
    strcat(buf, ")");
    strcat(buf, rtype);
    u1 = 1;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(strlen(buf));
    bytes_written += swrite(fd, (void *)&u2, 2);
    bytes_written += swrite(fd, (void *)&buf, strlen(buf));
    offset++;cgfcb,j
    handle = offset;

    // Name and type
    u1 = 12;
    bytes_written += swrite(fd, (void*)&u1, 1);
    u2 = htons(namen);
    bytes_written += swrite(fd, (void*)&u2, 2);
    u2 = htons(handle);
    bytes_written += swrite(fd, (void*)&u2, 2);
    offset++;
    int nt = offset;

    // Methodref
    u1 = 10;
    bytes_written += swrite(fd, (void*)&u1, 1);
    u2 = htons(classnum);
    bytes_written += swrite(fd, (void*)&u2, 2);
    u2 = htons(nt);
    bytes_written += swrite(fd, (void*)&u2, 2);
    offset++;

    return bytes_written;
}

int cg_write_constant(int fd, STConst * c, int offset) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    static int bytes_written;

    bytes_written = 0;

    u1 = (unsigned char)c->type;
    bytes_written += swrite(fd, (void *)&u1, 1);

    switch (c->type) {
        case CONST_UTF8:
            u2 = htons(strlen(c->value.utf8));
            bytes_written += swrite(fd, (void *)&u2, 2);
            bytes_written += swrite(fd, (void *)c->value.utf8, strlen(c->value.utf8));
        break;

        case CONST_INT:
            s4 = htonl(c->value.val_int);
            bytes_written += swrite(fd, (void *)&(s4), 4);
        break;

        case CONST_FLOAT:
            sf4 = htonl((float)c->value.val_float);
            bytes_written += swrite(fd, (void *)&sf4, 4);
        break;

        case CONST_CLASS:
        case CONST_STRING:
            u2 = htons(c->value.args.arg1 + offset);
            bytes_written += swrite(fd, (void *)&u2, 2);
        break;

        case CONST_FIELDREF:
        case CONST_METHODREF:
        case CONST_NAMETYPE:
            u2 = htons(c->value.args.arg1 + offset);
            bytcgfcb,jes_written += swrite(fd, (void *)&u2, 2);
            u2 = htons(c->value.args.arg2 + offset);
            bytes_written += swrite(fd, (void *)&u2, 2);
        break;

        default:
            fprintf(stderr, "[ERROR] Found constant of unknown type.\n");
            exit(2);
        break;
    }

    return bytes_written;
}

int cg_write_constants(int fd, STConst * table, int skip_first) {
    int bytes_written = 0;

    STConst * cur = table;

    if (skip_first) {
        cur = cur->next;
    }

    while (cur != NULL) {
        bytes_written += cg_write_constant(fd, cur, offset);
        cur = cur->next;
    }

    return bytes_written;
}

int cg_code_for_stmtlist(struct NStmtList * list, int file) {
    struct NStmt * cur = list->first;
    int count = 0;
    while (cur!=NULL) {
        count += cg_code_for_stmt(cur, file);
    }
    return count;
}

int cg_code_for_stmt(struct NStmt * stmt, int file) {
    int bytes_written = 0;
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    int offset = 0;
    switch(stmt->type)
    {
        case STMT_WHILE:
            bytes_written = cg_code_for_expr(stmt->while_loop->condition,file);
            offset = cg_code_for_stmtlist(stmt->while_loop->body,file);
            offest +=3;
            u1 = 167;
            bytes_written += swrite(fd, (void *)&u1, 1);
            u2 = htons(strlen(c->value.utf8));
            bytes_written += swrite(fd, (void *)&u2, 2);
            break;
        case ,j:
            break;
        case STMT_EXPR:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            break;
        case STMT_FUNC:
            break;
        case STMT_BLOCK:
            break;
        case STMT_LFUNC:
            break;
        case STMT_REPEAT:
            break;
        case STMT_ASSIGN:
            break;
        case STMT_LASSIGN:
            break;
        case STMT_ASSIGN_MAS:
            break;
        case STMT_LASSIGN_MAS:
            break;
        case STMT_RETURN:
            break;
        case STMT_BREAK:
            break;
        case STMT_IF:
            break;
    }
    return count;
}

int cg_code_for_expr(struct NExpr * expr, int file) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    int count = 0;
    switch (expr->type)
    {
        case EXPR_EQ:
            break;
        case EXPR_NQ:
            break;
        case EXPR_PLUS:
            break;
        case EXPR_MINUS:
            break;
        case EXPR_DIV:
            break;
        case EXPR_MUL:
            break;
        case EXPR_LE:
            if (expr->left->type == EXPR_STR || expr->left->type == EXPR_INT || expr->left->type == EXPR_DOUBLE || expr->left->type == EXPR_BOOL)
                cg_code_for_expr(expr->left);
            else
                cg_push(expr->left);
            if (expr->right->type == EXPR_STR || expr->right->type == EXPR_INT || expr->right->type == EXPR_DOUBLE || expr->right->type == EXPR_BOOL)
                cg_code_for_expr(expr->right);
            else 
                cg_push(expr->right);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_LOEQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_GE:
            break;
        case EXPR_LT:
            break;
        case EXPR_GT:
            break;
        case EXPR_MOD:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            sf4 = htonl(expr->Double);
            bytes_written += swrite(file, (void *)&sf4, 4);
            u2 = htons(method_init_integer);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_ID:
            break;
        case EXPR_INT:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 17;
            bytes_written += swrite(file, (void *)&u1, 1);
            s4 = htonl(expr->Int);
            bytes_written += swrite(file, (void *)&s4, 4);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_integer);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_DOUBLE:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 17;
            bytes_written += swrite(file, (void *)&u1, 1);
            sf4 = htonl(expr->Double);
            bytes_written += swrite(file, (void *)&sf4, 4);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_float);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_CONC:
            break;
        case EXPR_STR:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_string);
            bytes_written += swrite(file, (void *)&u2, 2);
            sf4 = htonl(expr->name);
            bytes_written += swrite(file, (void *)&sf4, 4);
            break;
        case EXPR_MET:
            break;
        case EXPR_AND:
            break;
        case EXPR_NOT:
            break;
        case EXPR_OR:
            break;
        case EXPR_MAS:
            break;
        case EXPR_BOOL:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 17;
            bytes_written += swrite(file, (void *)&u1, 1);
            s4 = htonl(expr->Int);
            bytes_written += swrite(file, (void *)&s4, 4);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_boolean);
            bytes_written += swrite(file, (void *)&u2, 2);
            new
            break;
        case EXPR_NIL:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_UMIN:
            break;
        case EXPR_ID_LIST:
            break;
        case EXPR_TABLE:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(fd, (void *)&u2, 2);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons("класс реф");
            bytes_written += swrite(fd, (void *)&u2, 2);
            break;
    }
    return count;
}

int cg_calculate_offset_stmt(struct NStmt* stmt) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
}

int cg_calculate_offset_stmtlist(struct NStmtList* stmtlist) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
}

int cg_calculate_offset_expr(struct NExpr) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
}
#endif

