/**************************************************************************
 
endianness 는 multi-byte 에 적용되는 byte endian 외에 bit 단위 endian
도 있습니다. 따라서 union 을 이용해 bit 값을 추출할 때는 little endian
과 big endian 의 bit 값 순서를 각각 다르게 적용해야 합니다.

https://www.linuxjournal.com/article/6788

Bit order usually follows the same endianness as the byte order for a given 
computer system. That is, in a big endian system the most significant bit is
stored at the lowest bit address; in a little endian system, 
the least significant bit is stored at the lowest bit address.

Here is how we would write the integer 0x0a0b0c0d for both big endian and 
little endian systems, according to the rule above:
   
   
Write Integer for Big Endian System

byte  addr       0         1       2        3
bit  offset  01234567 01234567 01234567 01234567
     binary  00001010 00001011 00001100 00001101
        hex     0a       0b      0c        0d

Write Integer for Little Endian System

byte  addr      3         2       1        0
bit  offset  76543210 76543210 76543210 76543210
     binary  00001010 00001011 00001100 00001101
        hex     0a       0b      0c        0d

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define BS_CHAR   sizeof(char)
#define BS_SHORT  sizeof(short)
#define BS_INT    sizeof(int)
#define BS_LONG   sizeof(long)

/*
byte 값이 00000001 일 경우 endian 에 따라 다르게 저장됩니다.

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

/*
 *  little endian will return : 1
 *     big endian will return : 0
 */
int check_endian()
{
    unsigned int x = 1;
    char *c = (char*) &x;
    return *c;
}

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

    if ( check_endian() ) {

        /* little endian */
        for ( int i = size - 1 ; i >= 0; i-- )
            print_binary_little( arr[i] );

    } else {

        /* big endian */
        int j = BS_LONG - size;
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
            convert( argNum, BS_CHAR );

        else if ( argNum >= SHRT_MIN )
            convert( argNum, BS_SHORT );

        else if ( argNum >= INT_MIN )
            convert( argNum, BS_INT );

        else if ( argNum >= LONG_MIN )
            convert( argNum, BS_LONG );

    } else {

        unsigned long argNum = strtoul( argv[1], NULL, 10 );

        if ( errno ) err_msg();

        if ( argNum <= UCHAR_MAX )
            convert( argNum, BS_CHAR );

        else if ( argNum <= USHRT_MAX )
            convert( argNum, BS_SHORT );

        else if ( argNum <= UINT_MAX )
            convert( argNum, BS_INT );

        else if ( argNum <= ULONG_MAX )
            convert( argNum, BS_LONG );

    }

    return EXIT_SUCCESS;	
}
