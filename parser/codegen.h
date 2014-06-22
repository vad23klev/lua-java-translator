/**
 * @file codegen.h
 * Provides functions to generate java bytecode.
 */

#ifndef _H_CODEGEN_
#define _H_CODEGEN_

/**
 * Provides numbers of `methodref` constants on RTL functions and methods.
 */
enum cg_rtl_methodref {
    // Mixed type methods
    MIXED_FUNC = 1,

    // Library functions
    LIB_PRINT = 10000
};

/**
 * Generates .class-files for given tree (with function .class-files).
 * @pre The tree must be prepared with st_fill_tables() function.
 * @param [in] root Tree root.
 */
void cg_generate_bytecode(struct NStmtList * root);

/***************************************************************************************************/

void cg_generate_bytecode(struct NStmtList * root) {
    // Not implemented yet.
}

#endif

