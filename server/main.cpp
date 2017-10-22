#include <stdio.h>
#include "server.h"
int main(void){
    FServer f;
    printf("----");
    printf("faslfdjalkdjf");
    if(f.flisten(8091) == -1)
    {
        printf("listen error!\n");
        return -1;
    }

    f.wait();
    return 0;
}
