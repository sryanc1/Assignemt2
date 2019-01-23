#include <stdio.h>
#include <stdlib.h>
int main(void)
{
        char fred[] = "fred";
        char* iter = fred;
        while (*iter)
        {
                printf("%c\n", *iter);
                ++iter;
        }
        return EXIT_SUCCESS;
}
