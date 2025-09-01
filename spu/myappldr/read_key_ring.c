typedef unsigned int uint32_t;
typedef int int32_t;

int32_t start(uint32_t skip, uint32_t* buf, uint32_t len)
{
    volatile uint32_t* data = (volatile uint32_t*)0x39000;

    for (uint32_t i = 0; i < len; i++)
        buf[i] = data[i];

    return 0;
}