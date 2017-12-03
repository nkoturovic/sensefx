/* Assertion header file, provides
 * functionality for error checking */

#ifndef __AST_TOOLS_HEADER
#define __AST_TOOLS_HEADER

/* Prototype of function used for error checking */
void ast_impl(int statement, const char * err_msg, const char * file, const char * function, int line_number);

/* Assertion macro used for error checking */
#define ast(statement, err_msg) \
	do { \
		if(!(statement)) \
			ast_impl(statement, err_msg, __FILE__, __func__, __LINE__);\
	} while (0)

#endif
