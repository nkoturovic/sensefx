#include <stdio.h>
#include <stdlib.h>

void ast_impl(int statement, const char * err_msg, const char * file, const char * function, int line_number)
{
    if (!statement) {
        fprintf(stderr, "[Error]\nMessage: %s\nFile: %s\nFunction: %s\nLine: %d\n"\
					,err_msg, file, function, line_number);
        exit(EXIT_FAILURE);
    }
}
