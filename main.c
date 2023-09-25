#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t BYTE;

BYTE*
str2byte(const char* str)
{
    BYTE* byte_str = malloc(strlen(str));
    for(uint8_t i = 0; str[i] != '\0'; i++)
    {
        byte_str[i] == str[i];
    }

    return byte_str;
}

void
add2array(BYTE* from_array, BYTE* to_array, uint64_t from_size, uint64_t to_size, uint64_t to_array_start_index)
{
    if(from_size > (to_size - to_array_start_index))
    {
        printf("array size error\n");
        return;
    }

    int i = 0;
    int j = to_array_start_index;
    while(i < from_size) 
    {
        to_array[j++] = from_array[i++];
    }
}

void
uint64t2uint8t(uint64_t num, uint8_t output[8])
{
    for(int i = 0; i < 8; i++)
    {
        output[i] = num >> i;
    }
}

uint64_t
num_of_padding_bytes(uint64_t input_size)
{
    uint64_t num_of_bytes;
    uint64_t input_mod_64 = input_size % 64;

    if(input_mod_64 < 56)
    {
        num_of_bytes = 56 - input_mod_64;
    } else if (input_mod_64 >= 56)
    {
        num_of_bytes = 64 + 56 - input_mod_64;
    }
    
    return num_of_bytes;
}

int
main(int argv, char **argc)
{
    // ------------- argument check -------------
    if(argv < 2)
    {
        printf("give input string\n");
        return 1;
    }

    // ------------- bit prep -------------
    uint64_t byte_str_size = strlen(argc[1]);
    printf("byte str size: %ld\n", byte_str_size);
    uint64_t padding_bytes_size = num_of_padding_bytes(byte_str_size);
    printf("padding size: %ld\n", padding_bytes_size);
    uint64_t length_padding_bytes_size = 8;
    BYTE* byte_str = str2byte(argc[1]);

    uint64_t byte_arr_size = byte_str_size + padding_bytes_size + length_padding_bytes_size;
    printf("%ld\n", byte_arr_size % 64);
    BYTE* byte_arr = (BYTE* ) malloc(byte_arr_size);
    
    add2array(byte_str, byte_arr, byte_str_size, byte_arr_size, 0);

    // first padding byte in md5 is 10000000
    BYTE first_padding_byte[] = {(uint8_t) 1 << 7};
    //printf("1\n");
    add2array(first_padding_byte, byte_arr, 1, byte_arr_size, byte_str_size);

    // all other padding bytes are 00000000
    BYTE padding_byte[] = {(uint8_t) 0};
    for(int i = 0; i < (padding_bytes_size - 1); i++)
    {
        //printf("%d\n", i + 2);
        add2array(padding_byte, byte_arr, 1, byte_arr_size, (byte_str_size + 1 + i));
    }

    // length padding bytes
    BYTE length_padding_bytes[8];
    uint64t2uint8t(byte_str_size, length_padding_bytes);
    add2array(length_padding_bytes, byte_arr, 8, byte_arr_size, (byte_str_size + padding_bytes_size));

    // ------------- operations start -------------

    return 0;
}