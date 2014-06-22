/**
 * @file codegen.h
 * Provides functions to generate java bytecode.
 */

#ifndef _H_CODEGEN_
#define _H_CODEGEN_

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

int    offset    = 1;

/**
 * Calls perror(s) and exit(c);
 * @param [in] c Exit code.
 * @param [in] s Explanatory message.
 */
void crt_exception(int c, char * s);

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
 * Append RTL functions metodref's to constant table in excute file.
 */
void cg_lib_funcs();

/**
 * Append Mixed type functions metodref's to constant table in excute file.
 */
void cg_mixed_funcs();

/**
 * Append RTL function metodref's to constant table in excute file.
 * @param [in] name       Name of metod.
 * @param [in] arcgscount Argument count for method.
 * @param [in] rtype      Type of return value of metod.
 * @param [in] classnum   Table string number in Constant table.
 */
void cg_lib_func(const char* name, int argscount, const char* rtype, int classnum);


/***************************************************************************************************/

void crt_exception(int c, char * s) {
    perror(s);
    exit(c);
}

int  swrite(int fd, void * ptr, int numbytes) {
    static int w = write(fd, ptr, numbytes);
    if (w <= 0) {
        crt_exception(2, "Writing .class file data");
    }
    return w;
}

void cg_generate_bytecode(struct NStmtList * root) {
    int mc = open("Main.class", O_WRONLY, 644);
    int s4;
    unsigned int u4;
    short int s2;
    unsigned short int u2;

    if (mc < 0) {
        crt_exception(2, "Main.class");
    }

    // Magic number
    u4 = 0xCAFEBABE;
    swrite(mc, (void *)&u4, 4);

    // Subversion and version
    u2 = 1;
    swrite(mc, (void *)&u2, 2);
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // TODO Number of constants
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // TODO Constants table

    // TODO Current class (CONST_CLASS)
    

    // TODO Parent class (CONST_CLASS)
    

    // Number of interfaces
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    // TODO Number of fields
    

    // TODO Fields table
    

    // TODO Number of methods
    

    // TODO Methods table
    

    // Number of attributes
    u2 = 0;
    swrite(mc, (void *)&u2, 2);

    close(mc);
}

void cg_lib_funcs(int mc) {
    //class name UTF8
    int s4;
    unsigned int u4;
    short int s2;
    unsigned short int u2;
    unsigned char u1;
    int i,name;
    unsigned char* buf= (unsigned char*)malloc(sizeof(char)*1024);
    u1 = 1;
    swrite(mc,(void*)&u1,1);
    u2 = strlen("Lrtl/Lib");
    swrite(mc,(void*)&u2,2);
    strcpy(buf,"Lrtl/Lib");
    swrite(mc,(void*)&buff,strlen(buf));
    name = offset;
    offset++;
    // Class
    u1 = 7;
    swrite(mc,(void*)&u1,1);
    u2 = name;
    swrite(mc,(void*)&u2,2);
    int cn = offset;
    offset++;
    //lassert 2
    cg_lib_func(mc,"lassert",2,"Lrtl/Mixed;", cn, true);
    //lassert 1
    cg_lib_func(mc,"lassert",1,"Lrtl/Mixed;", cn, false);
    //error 2
    cg_lib_func(mc,"error",2,"void;", cn, true);
    //error 1
    cg_lib_func(mc,"error",1,"void;", cn, false);
    //tonumber 1
    cg_lib_func(mc,"tonumber",1,"Lrtl/Mixed;", cn, true);
    //tonumber 2

    cg_lib_func(mc,"tonumber",2,"Lrtl/Mixed;", cn, false);
    //tostring
    cg_lib_func(mc,"tostring",1,"Lrtl/Mixed;", cn, true);
    //print
    cg_lib_func(mc,"print",1,"void;", cn, true);
    //ioread
    cg_lib_func(mc,"ioread",1,"Lrtl/Mixed;", cn, true);
}

void cg_mixed_funcs(int mc) {
    //class name UTF8
    int s4;
    unsigned int u4;
    short int s2;
    unsigned short int u2;
    unsigned char u1;
    int i,name;
    unsigned char* buf= (unsigned char*)malloc(sizeof(char)*1024);
    u1 = 1;
    swrite(mc,(void*)&u1,1);
    u2 = strlen("Lrtl/Mixed");
    swrite(mc,(void*)&u2,2);
    strcpy(buf,"Lrtl/Mixed");
    swrite(mc,(void*)&buff,strlen(buf));
    name = offset;
    offset++;
    // Class
    u1 = 7;
    swrite(mc,(void*)&u1,1);
    u2 = name;
    swrite(mc,(void*)&u2,2);
    int cn = offset;
    offset++;
    //add
    cg_lib_func(mc,"add",1,"Lrtl/Mixed;", cn, true);
    //sub
    cg_lib_func(mc,"sub",1,"Lrtl/Mixed;", cn, true);
    //mul
    cg_lib_func(mc,"mul",1,"Lrtl/Mixed;", cn, true);
    //div
    cg_lib_func(mc,"div",1,"Lrtl/Mixed;", cn, true);
    //mod
    cg_lib_func(mc,"mod",1,"Lrtl/Mixed;", cn, true);
    //get
    cg_lib_func(mc,"get",1,"Lrtl/Mixed;", cn, true);
    //put
    cg_lib_func(mc,"put",2,"Lrtl/Mixed;", cn, true);
    //remove
    cg_lib_func(mc,"remove",1,"Lrtl/Mixed;", cn, true);
    //not
    cg_lib_func(mc,"not",0,"Lrtl/Mixed;", cn, true);
    //and
    cg_lib_func(mc,"and",1,"Lrtl/Mixed;", cn, true);
    //or
    cg_lib_func(mc,"or",1,"Lrtl/Mixed;", cn, true);
    //eq
    cg_lib_func(mc,"eq",1,"Lrtl/Mixed;", cn, true);
    //neq
    cg_lib_func(mc,"neq",1,"Lrtl/Mixed;", cn, true);
    //gr
    cg_lib_func(mc,"gr",1,"Lrtl/Mixed;", cn, true);
    //greq
    cg_lib_func(mc,"greq",1,"Lrtl/Mixed;", cn, true);
    //loeq
    cg_lib_func(mc,"loeq",1,"Lrtl/Mixed;", cn, true);
    //lo
    cg_lib_func(mc,"lo",1,"Lrtl/Mixed;", cn, true);
}

void cg_lib_func(int mc, const char* name, int argscount, const char* rtype, int classnum, bool need_name) {
    int s4;
    unsigned int u4;
    short int s2;
    unsigned short int u2;
    unsigned char u1;
    int i,namen,handle;
    unsigned char* buf= (unsigned char*)malloc(sizeof(char)*1024);
    //Metod name UTF8
    if (need_name) {
        u1 = 1;
        swrite(mc,(void*)&u1,1);
        strcpy(buf,name);
        u2 = strlen(buf);
        swrite(mc,(void*)&u2,2);
        swrite(mc,(void*)&buf,strlen(buf));
        namen = offset;
        offset++;
    }
    //Metod handle
    buf= (unsigned char*)malloc(sizeof(char)*1024);
    strcpy(buf, "(");
    for (i = 0; i < argscount; i++) {
        strcat(buf, "Lrtl/Mixed;");
    }
    strcat(buf, ")");
    strcat(buf, rtype);
    u1 = 1;
    swrite(mc,(void*)&u1,1);
    u2 = strlen(buf);
    swrite(mc,(void*)&u2,2);
    swrite(mc,(void*)&buf,strlen(buf));
    handle = offset;
    offset++;
    // Name and type
    u1 = 12;
    swrite(mc,(void*)&u1,1);
    u2 = namen;
    swrite(mc,(void*)&u2,2);
    u2 = handle;
    swrite(mc,(void*)&u2,2);
    int nt = offset;
    offset++;
    // Metod reference
    u1 = 10;
    swrite(mc,(void*)&u1,1);
    u2 = classnum;
    swrite(mc,(void*)&u2,2);
    u2 = nt;
    swrite(mc,(void*)&u2,2);
    offset++;

}
#endif

