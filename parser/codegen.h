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
    CLASS_MIXED = 2,

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

int class_boolean = 0;
int class_integer = 0;
int class_float   = 0;
int class_string  = 0;
int class_table   = 0;

int field_value_boolean = 0;
int field_value_float   = 0;
int field_value_integer = 0;
int field_value_string  = 0;

int method_init_boolean = 0;
int method_init_float   = 0;
int method_init_integer = 0;
int method_init_string  = 0;
int method_init_table   = 0;

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

int cg_code_for_stmtlist(struct NStmtList * list, int file);

int cg_code_for_stmt(struct NStmt * stmt, int file);

int cg_code_for_expr(struct NExpr * expr, int file);

int cg_calculate_offset_stmt(struct NStmt* stmt);

int cg_calculate_offset_stmtlist(struct NStmtList* stmtlist);

int cg_calculate_offset_expr(struct NExpr);

int cg_astore_var(struct NExpr* expr,int file);

int cg_push(struct NExpr* expr,bool first_call,int file);

int cg_calculate_offset_push(struct NExpr * expr);

int cg_calculate_offset_astore_var(struct NExpr* expr);

int cg_push_withoutlast(struct NExpr* expr, bool first_call, int file);

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
    unsigned char u1;
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
    strcpy(buf, "Lrtl/Table;");
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

    // Table UTF8
    tmpc.type = CONST_UTF8;
    strcpy(buf, "rtl/Table");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_table = numconst;

    // Boolean Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_boolean;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    class_boolean = numconst;

    // Integer Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_integer;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    class_integer = numconst;

    // Float Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_float;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    class_float = numconst;

    // String Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_string;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    class_string = numconst;

    // Table Class
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_table;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    class_table = numconst;

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
    strcpy(buf, "java/lang/String");
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
    strcpy(buf, "(Z)V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_boolean_descriptor = numconst;

    // integer <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(I)V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_integer_descriptor = numconst;

    // float <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(D)V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_float_descriptor = numconst;

    // string <init> descriptor
    tmpc.type = CONST_UTF8;
    strcpy(buf, "(Ljava/lang/String;)V");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_string_descriptor = numconst;

    // <init>()V Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = init_name;
    tmpc.value.args.arg2 = vv_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_vv_nametype = numconst;

    // Boolean <init>(Z)V Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = init_name;
    tmpc.value.args.arg2 = init_boolean_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_boolean_nametype = numconst;

    // Integer <init>(I)V Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = init_name;
    tmpc.value.args.arg2 = init_integer_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_integer_nametype = numconst;

    // Float <init>(D)V Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = init_name;
    tmpc.value.args.arg2 = init_float_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_float_nametype = numconst;

    // String <init>(Ljava/lang/Object;)V Name and type
    tmpc.type = CONST_NAMETYPE;
    tmpc.value.args.arg1 = init_name;
    tmpc.value.args.arg2 = init_string_descriptor;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int init_string_nametype = numconst;

    // boolean <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_boolean;
    tmpc.value.args.arg2 = init_boolean_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_boolean = numconst;

    // integer <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_integer;
    tmpc.value.args.arg2 = init_integer_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_integer = numconst;

    // float <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_float;
    tmpc.value.args.arg2 = init_float_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_float = numconst;

    // string <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_string;
    tmpc.value.args.arg2 = init_string_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_string = numconst;

    // table <init> methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_table;
    tmpc.value.args.arg2 = init_vv_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    method_init_table = numconst;

    // Object UTF8
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "java/lang/Object");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int utf8_object = numconst;

    // Class Object
    tmpc.type = CONST_CLASS;
    tmpc.value.args.arg1 = utf8_object;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int class_object = numconst;

    // object <init>()V methodref
    tmpc.type = CONST_METHODREF;
    tmpc.value.args.arg1 = class_object;
    tmpc.value.args.arg2 = init_vv_nametype;
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int method_init_object = numconst;

    // Code UTF8
    tmpc.type = CONST_UTF8;
    tmpc.value.utf8 = buf;
    strcpy(buf, "Code");
    bytes_written += cg_write_constant(mc, &tmpc, 0);
    numconst++;
    int code_attribute = numconst;

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
    tmpm.attr_num   = 1;
    cg_write_method(mc, &tmpm);

    // Attribute name
    u2 = htons(code_attribute);
    swrite(mc, (void *)&u2, 2);
    // Attribute length
    u4 = htonl(23);
    swrite(mc, (void *)&u4, 4);
    // Stack size
    u2 = htons(2);
    swrite(mc, (void *)&u2, 2);
    // Locals number
    u2 = 0;
    swrite(mc, (void *)&u2, 2);
    // Actual code length
    u4 = htonl(11);
    swrite(mc, (void *)&u4, 4);
    // Code
    u1 = 0xBB; // new
    swrite(mc, (void *)&u1, 1);
    u2 = htons(class_table);
    swrite(mc, (void *)&u2, 2);
    u1 = 0x59; // dup
    swrite(mc, (void *)&u1, 1);
    u1 = 0xB7; // invokespecial
    swrite(mc, (void *)&u1, 1);
    u2 = htons(method_init_table);
    swrite(mc, (void *)&u2, 2);
    u1 = 0xB3; // putstatic
    swrite(mc, (void *)&u1, 1);
    u2 = htons(field_G_fieldref);
    swrite(mc, (void *)&u2, 2);
    u1 = 0xB1; // return
    swrite(mc, (void *)&u1, 1);

    // Exceptions number
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // Attributes of attribute number
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // <init>
    tmpm.flags      = METHOD_STATIC;
    tmpm.name       = init_name;
    tmpm.descriptor = vv_descriptor;
    tmpm.attr_num   = 1;
    cg_write_method(mc, &tmpm);

    // Attribute name
    u2 = htons(code_attribute);
    swrite(mc, (void *)&u2, 2);
    // Attribute length
    u4 = htonl(17);
    swrite(mc, (void *)&u4, 4);
    // Stack size
    u2 = htons(1);
    swrite(mc, (void *)&u2, 2);
    // Locals number
    u2 = htons(1);
    swrite(mc, (void *)&u2, 2);
    // Actual code length
    u4 = htonl(5);
    swrite(mc, (void *)&u4, 4);
    // Code
    u1 = 0x2A; // aload_0
    swrite(mc, (void *)&u1, 1);
    u1 = 0xB7; // invokespecial
    swrite(mc, (void *)&u1, 1);
    u2 = htons(method_init_object);
    swrite(mc, (void *)&u2, 2);
    u1 = 0xB1; // return
    swrite(mc, (void *)&u1, 1);

    // Exceptions number
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // Attributes of attribute number
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

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
    u2 = htons(method->attr_num);
    swrite(fd, (void *)&u2, 2);
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
            offset +=3;
            u1 = 167;
            break;
        case STMT_EXPR:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            break;
        case STMT_FUNC:
            break;
        case STMT_BLOCK:
            bytes_written += cg_code_for_stmtlist(stmt->list,file);
            break;
        case STMT_LFUNC:
            break;
        case STMT_REPEAT:
            break;
        case STMT_ASSIGN:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            bytes_written += cg_astore_var(stmt->var,file);
            break;
        case STMT_LASSIGN:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            bytes_written += cg_astore_var(stmt->var,file);
            break;
        case STMT_ASSIGN_MAS:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            bytes_written += cg_astore_var(stmt->var,file);
            break;
        case STMT_LASSIGN_MAS:
            bytes_written += cg_code_for_expr(stmt->expr,file);
            bytes_written += cg_astore_var(stmt->var,file);
            break;
        case STMT_RETURN:
            break;
        case STMT_BREAK:
            break;
        case STMT_IF:
            break;
    }
    return bytes_written;
}

int cg_code_for_expr(struct NExpr * expr, int file) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
    int bytes_written = 0;
    switch (expr->type)
    {
        case EXPR_EQ:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_EQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_NQ:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_NEQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_PLUS:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_ADD);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_MINUS:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_SUB);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_DIV:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_DIV);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_MUL:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_MUL);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_LE:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_LOEQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_GE:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_GREQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_LT:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_LO);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_GT:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_GR);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_MOD:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_MOD);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_ID:
            break;
        case EXPR_INT:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(class_integer);
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
            u2 = htons(class_float);
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
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_ADD);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_STR:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(class_string);
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_string);
            bytes_written += swrite(file, (void *)&u2, 2);
            u2 = htons(expr->constant_index + offset);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        //case EXPR_MET:
        //    break;
        case EXPR_AND:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_AND);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_NOT:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_NOT);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_OR:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_OR);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_MAS:
            bytes_written = cg_push(expr,true,file);
            break;
        case EXPR_BOOL:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(class_boolean);
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 17;
            bytes_written += swrite(file, (void *)&u1, 1);
            s4 = htonl(expr->Int);
            bytes_written += swrite(file, (void *)&s4, 4);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_boolean);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_NIL:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(class_nil);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_UMIN:
            bytes_written += cg_code_for_expr(expr->left,file);
            bytes_written += cg_code_for_expr(expr->right,file);
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_LOEQ);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_TABLE:
            u1 = 187;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(class_table);
            bytes_written += swrite(file, (void *)&u2, 2);
            u1 = 183;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(method_init_table);
            bytes_written += swrite(file, (void *)&u2, 2);
            break;
        case EXPR_ID_LIST:
            bytes_written = cg_push(expr,true,file);
            break;
    }
    return bytes_written;
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

int cg_calculate_offset_expr(struct NExpr* expr) {
    static float sf4;
    static int s4;
    static unsigned int u4;
    static short int s2;
    static unsigned short int u2;
    static unsigned char u1;
}

int cg_astore_var(struct NExpr* expr,int file) {
    static unsigned short int u2;
    static unsigned char u1;
    int bytes_written = cg_push_withoutlast(expr,true,file);
    if(expr->idlist->first == expr->idlist->last && expr->idlist->first->type != EXPR_MAS)
        bytes_written += cg_push(expr->idlist->first,true,file);//astore
    else
        bytes_written += cg_push(expr->idlist->last,true,file);
        u1 = 182;
        bytes_written += swrite(file, (void *)&u1, 1);
        u2 = htons(MIXED_PUT);
        bytes_written += swrite(file, (void *)&u2, 2);
    return bytes_written;
}

int cg_push_withoutlast(struct NExpr* expr, bool first_call, int file) {
    int bytes_written = 0;
    switch (expr->type) {
        case EXPR_ID:
            bytes_written = cg_push(expr,first_call,file);
            break;
        case EXPR_MAS:
            bytes_written = cg_push(expr,first_call,file);
            bytes_written += cg_push(expr,false,file);
            break;
        default:
            struct NExpr * end = NULL, * cur = expr->idlist->first;
            if(first_call)
                end = expr->idlist->last;
            while (cur!=end) {
                bytes_written += cg_push(cur,first_call,file);
                first_call = false;
                cur = cur->next;
            }
            break;
    }
    return bytes_written;
}
int cg_push(struct NExpr* expr, bool first_call,int file) {
    int bytes_written = 0;
    static unsigned short int u2;
    static unsigned char u1;
    if (expr->type = EXPR_ID_LIST)
        bytes_written += cg_push_withoutlast(expr,first_call,file);
    if(expr->type == EXPR_ID_LIST && expr->idlist->first == expr->idlist->last)
    {
        //astore
        bytes_written += cg_push(expr->idlist->first,first_call,file);
    }
    else if (expr->type == EXPR_MAS)
    {
        bytes_written = cg_push(expr,first_call,file);
        bytes_written += cg_push(expr,false,file);
        u1 = 182;
        bytes_written += swrite(file, (void *)&u1, 1);
        u2 = htons(MIXED_GET);
        bytes_written += swrite(file, (void *)&u2, 2);
    }
    else if (expr->type == EXPR_ID)
    {
        u1 = 58;
        bytes_written += swrite(file, (void *)&u1, 1);
        u1 = expr->constant_index + offset;
        bytes_written += swrite(file, (void *)&u1, 1);
        if(!first_call)
        {
            u1 = 182;
            bytes_written += swrite(file, (void *)&u1, 1);
            u2 = htons(MIXED_GET);
            bytes_written += swrite(file, (void *)&u2, 2);
        }
    }
    return bytes_written;
}

int cg_calculate_offset_push(struct NExpr * expr) {
}

int cg_calculate_offset_astore_var(struct NExpr* expr) {
}

#endif

