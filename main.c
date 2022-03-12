#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*
    int LZ4_compress_fast(const char* source, char* dest, int inputSize, int maxOutputSize, int acceleration);
    int LZ4_compress_default(const char* src, char* dst, int srcSize, int maxOutputSize);
    int LZ4_decompress_safe (const char* src, char* dst, int compressedSize, int dstCapacity);
*/

#include "lz4.h"

#define MAX_FILE_SIZE  0x1000000 //MAX 16MB

char inp[MAX_FILE_SIZE];
char out[MAX_FILE_SIZE];

int srcSize, dstSize;

int compSize, decompSize;

int readfile(const char* filename, char* src)
{
    FILE* in_file = fopen(filename, "rb");
    struct stat sb;

    if(!in_file)
    {
        perror("fopen");
        return(EXIT_FAILURE);
    }

    if (stat(filename, &sb) == -1)
    {
        perror("stat");
        return(EXIT_FAILURE);
    }

    srcSize = sb.st_size;

    fread(src, srcSize, 1, in_file);

    printf("Read data : [0]0x%02X [1]0x%02X / LEN:%d \n", src[0], src[1], srcSize);

    fclose(in_file);

    return(EXIT_SUCCESS);
}

int writefile(const char* filename , char * src, size_t size )
{

    FILE* file = fopen(filename, "wb+");

    if(!file)
    {
        perror("fopen");
        return(EXIT_FAILURE);
    }

    printf("Write data: [0]0x%02X [1]0x%02X / LEN:%d \n", src[0], src[1], size);

    fwrite(src, 1, size, file);

    fclose(file);

    return(EXIT_SUCCESS);
}

int main(int argc,  char* argv[])
{
    int Status;

    if(argc == 4)
    {
            if(strcmp(argv[1],"-d") == 0)
            {
                if( (Status = readfile(argv[2], inp )) != EXIT_SUCCESS)
                    goto destructor;

                printf("Decompressing %s to %s\n", argv[2], argv[3]);

                if( (decompSize = LZ4_decompress_safe(inp, out, srcSize, sizeof(out))) <= 0 )
                    goto destructor;

                writefile(argv[3],out,decompSize);
            }
            else if(strcmp(argv[1],"-c") == 0)
            {
                if( (Status = readfile(argv[2], inp )) != EXIT_SUCCESS)
                    goto destructor;

                printf("Compressing %s to %s\n", argv[2], argv[3]);

                if( (compSize = LZ4_compress_default(inp, out, srcSize, sizeof(out))) <= 0 )
                    goto destructor;

                writefile(argv[3],out,compSize);
            }
            else
            {
                printf("Usage:\n     for compression:    ./lz4 -c input_filepath out_file_path \n     for decompression:  ./lz4 -d input_file_path out_file_path\n");
                goto destructor;
            }
    }
    else
    {
        printf("Usage:\n     for compression:    ./lz4 -c input_filepath out_file_path \n     for decompression:  ./lz4 -d input_file_path out_file_path\n");
        goto destructor;
    }

    printf("srcSize:%d - compSize:%d - decompSize:%d \n", srcSize, compSize, decompSize);

destructor:
    return 0;

}
