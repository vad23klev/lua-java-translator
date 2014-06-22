/**
 * @file codegen.h
 * Provides functions to generate java bytecode.
 */

#ifndef _H_CODEGEN_
#define _H_CODEGEN_

/**
 * Provides numbers of `methodref` constants on RTL functions and methods.
 */
enum cg_rtl_methodrefs {
    // Mixed type methods
    MIXED_FUNC = 1,

    // Library functions
    LIB_PRINT = 10000
};

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

#endif

