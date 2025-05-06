#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/stat.h>

#define round_up(x, n) (-(-(x) & -(n)))

size_t get_file_size(FILE *f)
{
    size_t old_off = ftell(f);

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    fseek(f, old_off, SEEK_SET);
    return size;
}

uint32_t endswap32(uint32_t v)
{
    return __bswap_32(v);
}

uint64_t endswap64(uint64_t v)
{
    return __bswap_64(v);
}

struct Zelf_Header_s
{
public:
    uint64_t magic;

    uint64_t original_size;
    uint64_t compressed_size;
};

bool SearchAndReplace(void* in_data, uint64_t dataSize, const void* in_searchData, uint64_t searchDataSize, const void* in_replaceData, uint64_t replaceDataSize)
{
    uint8_t* data = (uint8_t*)in_data;

    const uint8_t* searchData = (const uint8_t*)in_searchData;
    const uint8_t* replaceData = (const uint8_t*)in_replaceData;

    for (uint64_t i = 0; i < dataSize; ++i)
    {
        if (!memcmp(&data[i], searchData, searchDataSize))
        {
            memcpy(&data[i], replaceData, replaceDataSize);
            return true;
        }
    }

    return false;
}

void lv0gen(const char* inFilePath, const char* outFilePath, const char* stage2jFilePath)
{
    printf("lv0gen()\n");
    
    printf("inFilePath = %s\n", inFilePath);
    printf("outFilePath = %s\n", outFilePath);

    printf("stage2jFilePath = %s\n", stage2jFilePath);

    FILE* inFile = fopen(inFilePath, "rb");
    FILE* outFile = fopen(outFilePath, "wb");
    FILE* stage2jFile = fopen(stage2jFilePath, "rb");

    if (inFile == NULL || outFile == NULL || stage2jFile == NULL)
    {
        printf("open file failed!\n");

        abort();
        return;
    }

    size_t inFileSize = get_file_size(inFile);
    printf("inFileSize = %lu\n", inFileSize);

    uint8_t* inData = (uint8_t*)malloc(inFileSize);

    if (inData == NULL)
    {
        printf("malloc failed!\n");
        
        abort();
        return;
    }

    fread(inData, 1, inFileSize, inFile);
    fclose(inFile);

    size_t stage2jFileSize = get_file_size(stage2jFile);
    printf("stage2jFileSize = %lu\n", stage2jFileSize);

    if (stage2jFileSize > 32)
    {
        printf("bad stage2j file size!\n");
        
        abort();
        return;
    }

    uint8_t* stage2jData = (uint8_t*)malloc(stage2jFileSize);

    if (stage2jData == NULL)
    {
        printf("stage2jData failed!\n");
        
        abort();
        return;
    }

    fread(stage2jData, 1, stage2jFileSize, stage2jFile);
    fclose(stage2jFile);

    uint8_t* outData = (uint8_t*)malloc(inFileSize);

    if (outData == NULL)
    {
        printf("malloc failed!\n");
        
        abort();
        return;
    }

    memcpy(outData, inData, inFileSize);

    {
        uint8_t searchData[] = { 0x38, 0x60, 0x01, 0x00, 0x7C, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20, 0x60, 0x00, 0x00, 0x00 };
        
        printf("Installing stage2j...\n");

        if (!SearchAndReplace(outData, inFileSize, searchData, 16, stage2jData, stage2jFileSize))
        {
            printf("install failed!\n");
        
            abort();
            return;
        }
    }

    fwrite(outData, 1, inFileSize, outFile);

    free(outData);
    
    free(stage2jData);
    free(inData);

    fclose(outFile);
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "lv0gen"))
        lv0gen(argv[2], argv[3], argv[4]);
    else
    {
        printf("lv0gen <inFile> <outFile> <stage2j>\n");
    }

    return 0;
}