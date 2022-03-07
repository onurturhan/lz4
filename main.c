//int LZ4_compress_fast(const char* source, char* dest, int inputSize, int maxOutputSize, int acceleration);

//int LZ4_compress_default(const char* src, char* dst, int srcSize, int maxOutputSize);
//int LZ4_decompress_safe (const char* src, char* dst, int compressedSize, int dstCapacity);

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "lz4.h"

char src[0x1000000]; //MAX 16MB
char dst[0x1000000]; //MAX 16MB

int srcSize, dstSize;

int compSize, decompSize;

int readfile(void)
{
    const char* filename = "./input.bin";

    FILE* in_file = fopen(filename, "rb");
    if (!in_file) {
        perror("fopen");
        return(EXIT_FAILURE);
    }

    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("stat");
        return(EXIT_FAILURE);
    }
    
	srcSize = sb.st_size;
	
    fread(src, srcSize, 1, in_file);

    printf("Read data : [0]0x%02X [1]0x%02X / LEN:%d \n", src[0], src[1], srcSize);
    
    fclose(in_file);

    return(EXIT_SUCCESS);
}    

int writefile(void)
{
    const char* filename = "./output.bin";
    
    FILE* output_file = fopen(filename, "wb+");
    
    if (!output_file) {
        perror("fopen");
        return(EXIT_FAILURE);
    }

    printf("Write data: [0]0x%02X [1]0x%02X / LEN:%d \n", src[0], src[1], srcSize);

    fwrite(src, 1, decompSize, output_file);

    fclose(output_file);
    
    return(EXIT_SUCCESS);
}

int main(void)
{
	int Status;

    if( (Status = readfile()) != EXIT_SUCCESS)
    	goto destructor;

    if( (compSize = LZ4_compress_default(src, dst, srcSize, sizeof(dst))) <= 0 )
    	goto destructor;

	memset(src, 0, sizeof(src));
	
    if( (decompSize = LZ4_decompress_safe(dst, src, compSize, sizeof(src))) <= 0 )
    	goto destructor;
    		
    if( (Status = writefile()) != EXIT_SUCCESS)
    	goto destructor;	
    	
    printf("srcSize:%d - compSize:%d - decompSize:%d \n", srcSize, compSize, decompSize);
	
destructor:	
	return 0;
}
