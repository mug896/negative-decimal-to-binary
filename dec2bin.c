/**************************************************************************
 
long, int, short 같은 multi-byte values 를 byte 단위로 다룰땐 byte endian 이 
적용되고, bit 단위로 다룰땐 bit endian 이 적용됩니다. 보통 둘은 같습니다.
( byte endian 이 little endian 이면 bit endian 도 little endian )
따라서 union 을 이용해 bit 단위로 값을 추출할 경우 little endian 과 big endian 
의 bit 값 순서를 각각 다르게 적용해야 합니다.

https://www.linuxjournal.com/article/6788

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/*
 *  little endian will be : 1
 *     big endian will be : 0
 */
const int i = 1;
#define is_little_endian() ( *(char*)&i )

/*
byte 값이 00000001 일 경우 endian 에 따라 bit 값이 다르게 저장됩니다.

big endian 은 

    b1 b2 b3 b4 b5 b6 b7 b8
    0  0  0  0  0  0  0  1

little endian 은

    b1 b2 b3 b4 b5 b6 b7 b8
    1  0  0  0  0  0  0  0
*/
union {
    char ch;

    struct {
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
        unsigned char b8 : 1;
    } m;
} U;

void print_binary_big( char ch ) {

    U.ch = ch;

    printf(" %d%d%d%d%d%d%d%d"
            ,U.m.b1
            ,U.m.b2
            ,U.m.b3
            ,U.m.b4
            ,U.m.b5
            ,U.m.b6
            ,U.m.b7
            ,U.m.b8 );
}

void print_binary_little( char ch ) {

    U.ch = ch;

    printf(" %d%d%d%d%d%d%d%d"
            ,U.m.b8
            ,U.m.b7
            ,U.m.b6
            ,U.m.b5
            ,U.m.b4
            ,U.m.b3
            ,U.m.b2
            ,U.m.b1 );
}

void convert( long num, int size ) {

    char *arr = (char*) &num;

    if ( is_little_endian() ) {
        /* little endian */
        for ( int i = size - 1 ; i >= 0; i-- )
            print_binary_little( arr[i] );
    } else {
        /* big endian */
        int j = sizeof(long) - size;
        for ( int i = 0; i < size; i++, j++ ) {
            print_binary_big( arr[j] );
        }
    }

    putchar('\n');
}

void usage() {

    fputs( "\n\
 This is a decimal to binary converter.\n\
 Enter a decimal number to convert.    \n\n"
    , stderr );

    exit(EXIT_FAILURE);
}

void err_msg() {

    fputs( "\n\
 Error: Maximum (signed, unsigned) long type allowed.\n\n"
    , stderr );

    exit(EXIT_FAILURE);
}



int main( int argc, char *argv[] )
{

    if ( argc == 1 ) usage();

    char signChar = argv[1][0];

    if ( signChar == '-' ) {

        long argNum = strtol( argv[1], NULL, 10 );

        if ( errno ) err_msg();

        if ( argNum >= CHAR_MIN )
            convert( argNum, sizeof(char) );

        else if ( argNum >= SHRT_MIN )
            convert( argNum, sizeof(short) );

        else if ( argNum >= INT_MIN )
            convert( argNum, sizeof(int) );

        else if ( argNum >= LONG_MIN )
            convert( argNum, sizeof(long) );

    } else {

        unsigned long argNum = strtoul( argv[1], NULL, 10 );

        if ( errno ) err_msg();

        if ( argNum <= UCHAR_MAX )
            convert( argNum, sizeof(char) );

        else if ( argNum <= USHRT_MAX )
            convert( argNum, sizeof(short) );

        else if ( argNum <= UINT_MAX )
            convert( argNum, sizeof(int) );

        else if ( argNum <= ULONG_MAX )
            convert( argNum, sizeof(long) );

    }

    return EXIT_SUCCESS;	
}
