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

int field_value_boolean = 0;
int field_value_float   = 0;
int field_value_integer = 0;
int field_value_string  = 0;

int method_init_boolean = 0;
int method_init_float   = 0;
int method_init_integer = 0;
int method_init_string  = 0;

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

/**
 * Writes field record of fileds table.
 * @param [in] fd    File descriptor.
 * @param [in] field Filed to write.
 */
void cg_write_field(int fd, STField * field);

/**
 * Writes method record of methods table.
 * @param [in] fd     File descriptor.
 * @param [in] method Method to write.
 */
void cg_write_method(int fd, STMethod * method);

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
    STField tmpf;
    STMethod tmpm;
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

    // Constants table
    bytes_written = 0;
    bytes_written += cg_write_mixed_constants(mc);
    bytes_written += cg_write_lib_constants(mc);
    bytes_written += cg_write_constants(mc, st_const_table, 1);

    numconst = offset + st_const_count(st_const_table) - 1;

    // Current class constant
    // UTF8
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "Main");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    // Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = numconst;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int current_class = numconst;

    // _G field name
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "_G");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_G_name = numconst;

    // _G field descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "Lrtl/Mixed;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_G_descriptor = numconst;

    // _G Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = field_G_name;
    tmpc.value.args.arg2 = field_G_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_G_nametype = numconst;

    // _G fieldref
    tmpc.type = CONST_FIELDREF;
    tmpc.value.args.arg1 = current_class;
    tmpc.value.args.arg2 = field_G_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_G_fieldref = numconst;

    // <clinit> name UTF8
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "<clinit>");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int clinit_name = numconst;

    // <init> name UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "<init>");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_name = numconst;

    // main name UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "main");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int main_name = numconst;

    // ()V descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "()V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int vv_descriptor = numconst;

    // main method descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "([Ljava/lang/String;)V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int main_descriptor = numconst;

    // Boolean UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "rtl/Boolean");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_boolean = numconst;

    // Integer UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "rtl/Integer");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_integer = numconst;

    // Float UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "rtl/Float");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_float = numconst;

    // String UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "rtl/String");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_string = numconst;

    // Boolean Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_boolean;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int class_boolean = numconst;

    // Integer Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_integer;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int class_integer = numconst;

    // Float Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_float;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int class_float = numconst;

    // String Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_string;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int class_string = numconst;

    // value field name
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "value");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_value_name = numconst;

    // boolean field descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "Z");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_boolean_descriptor = numconst;

    // int field descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "I");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_integer_descriptor = numconst;

    // double field descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "D");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_double_descriptor = numconst;

    // string field descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "Ljava/lang/String;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_string_descriptor = numconst;

    // boolean Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = field_value_name;
    tmpc.value.args.arg2 = field_boolean_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_boolean_nametype = numconst;

    // Integer Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = field_value_name;
    tmpc.value.args.arg2 = field_integer_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_integer_nametype = numconst;

    // Float Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = field_value_name;
    tmpc.value.args.arg2 = field_double_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_double_nametype = numconst;

    // String Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = field_value_name;
    tmpc.value.args.arg2 = field_string_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int field_string_nametype = numconst;

    // boolean value fieldref
    tmpc.type = CONST_FIELDREF;
    tmpc.value.args.arg1 = class_boolean;
    tmpc.value.args.arg2 = field_boolean_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    field_value_boolean = numconst;

    // Integer value fieldref
    tmpc.type = CONST_FIELDREF;
    tmpc.value.args.arg1 = class_integer;
    tmpc.value.args.arg2 = field_integer_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    field_value_integer = numconst;

    // Float value fieldref
    tmpc.type = CONST_FIELDREF;
    tmpc.value.args.arg1 = class_float;
    tmpc.value.args.arg2 = field_double_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    field_value_float = numconst;

    // String value fieldref
    tmpc.type = CONST_FIELDREF;
    tmpc.value.args.arg1 = class_string;
    tmpc.value.args.arg2 = field_string_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    field_value_string = numconst;

    // boolean <init> descriptor
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "(Z)Lrtl/Mixed;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_boolean_descriptor = numconst;

    // integer <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(I)Lrtl/Mixed;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_integer_descriptor = numconst;

    // float <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(D)Lrtl/Mixed;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_float_descriptor = numconst;

    // string <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(Ljava/lang/String;)Lrtl/Mixed;");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_string_descriptor = numconst;

    // boolean <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_boolean;
    tmpc.value.args.arg2 = init_boolean_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_boolean = numconst;

    // integer <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_integer;
    tmpc.value.args.arg2 = init_integer_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_integer = numconst;

    // float <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_float;
    tmpc.value.args.arg2 = init_float_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_float = numconst;

    // string <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_string;
    tmpc.value.args.arg2 = init_string_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_string = numconst;

    // Actual number of constants
    lseek(mc, -bytes_written - 2, SEEK_CUR);
    u2 = htons((unsigned short int)numconst + 1);
    swrite(mc, (void *)&u2, 2);
    lseek(mc, 0, SEEK_END);

    // Access flags
    u2 = htons(0x02 | 0x01);
    swrite(mc, (void *)&u2, 2);

    // Current class (CONST_CLASS)
    u2 = htons(current_class);
    swrite(mc, (void *)&u2, 2);

    // Parent class (CONST_CLASS)
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // Number of interfaces
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // Number of fields
    u2 = htons(1);
    swrite(mc, (void *)&u2, 2);

    // Fields table
    tmpf.flags = FIELD_ACCESS_PUBLIC | FIELD_STATIC;
    tmpf.name = field_G_name;
    tmpf.descriptor = field_G_descriptor;
    cg_write_field(mc, &tmpf);

    // Number of methods
    u2 = htons(3);
    swrite(mc, (void *)&u2, 2);

    // Methods table

    // <clinit>
    tmpm.flags      = METHOD_STATIC;
    tmpm.name       = clinit_name;
    tmpm.descriptor = vv_descriptor;
    tmpm.attr_num   = 0;
    cg_write_method(mc, &tmpm);

    // <init>
    tmpm.flags      = METHOD_STATIC;
    tmpm.name       = init_name;
    tmpm.descriptor = vv_descriptor;
    tmpm.attr_num   = 0;
    cg_write_method(mc, &tmpm);

    // main
    tmpm.flags      = METHOD_ACCESS_PUBLIC | METHOD_STATIC;
    tmpm.name       = main_name;
    tmpm.descriptor = main_descriptor;
    tmpm.attr_num   = 0;
    cg_write_method(mc, &tmpm);

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

    strcpy(buf,"rtl/Lib");
    u1 = 1;
    bytes_written += swrite(fd, (void *)&u1, 1);
    u2 = htons(strlen("rtl/Lib"));
    bytes_written += swrite(fd, (void *)&u2, 2);
    bytes_written += swrite(fd, (void *)&buf, 7);
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
    strcpy(buf, "rtl/Mixed");
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
    //mod
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
    offset++;
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
            bytes_written += swrite(fd, (void *)&u2, 2);
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

void cg_write_field(int fd, STField * field) {
    static unsigned short int u2;

    // Flags
    u2 = htons(field->flags);
    swrite(fd, (void *)&u2, 2);

    // Field name
    u2 = htons(field->name);
    swrite(fd, (void *)&u2, 2);

    // Descriptor
    u2 = htons(field->descriptor);
    swrite(fd, (void *)&u2, 2);

    // Number of attributes
    u2 = 0;
    swrite(fd, (void *)&u2, 2);
}

void cg_write_method(int fd, STMethod * method) {
    static unsigned short int u2;

    // Flags
    u2 = htons(method->flags);
    swrite(fd, (void *)&u2, 2);

    // Field name
    u2 = htons(method->name);
    swrite(fd, (void *)&u2, 2);

    // Descriptor
    u2 = htons(method->descriptor);
    swrite(fd, (void *)&u2, 2);

    // Number of attributes
    u2 = 0;
    swrite(fd, (void *)&u2, 2);
}

#endif

