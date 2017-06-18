#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include "locadora.h"
#include "db-fun.c"
#include "sis-fun.c"

int main()
{
    cria_bd();
    menu();
    return 0;
}
