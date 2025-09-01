#include "Stagex_spu_common.c"
#include "../Aes/Aes.c"

struct __attribute__((aligned(8))) Stagex_spu_context_s
{
    uint32_t jobType; // 1 = aes128_decrypt_ctr, 2 = zlib_decompress
};

struct __attribute__((aligned(8))) Stagex_spu_job_aes128_decrypt_ctr_context_s
{
    uint8_t key[16];
    uint8_t iv[16];

    uint64_t in_ea;
    uint64_t out_ea;

    uint64_t size; // size to decrypt
};

void Stagex_spu_job_aes128_decrypt_ctr(const volatile struct Stagex_spu_job_aes128_decrypt_ctr_context_s* job_context)
{
    if ((job_context->size == 0) /*|| ((job_context->size % 16) != 0)*/)
        stop(4);

    // 0x10000 - 0x2FFFF = temp mem

    WORD aes_key[60];
    aes_key_setup((const uint8_t *)job_context->key, aes_key, 128);

    struct aes_decrypt_ctr_stream_context_s aes_ctx;
    aes_decrypt_ctr_stream_init(&aes_ctx, job_context->size, aes_key, 128, job_context->iv);

    static const uint32_t tmpBufSize = (128 * 1024);
    uint8_t* tmpBuf = (uint8_t*)0x10000;

    uint64_t left = job_context->size;

    uint64_t cur_in_ea = job_context->in_ea;
    uint64_t cur_out_ea = job_context->out_ea;

    while (1)
    {
        uint32_t chunkSize = (left > tmpBufSize) ? tmpBufSize : (uint32_t)left;

        DMARead(tmpBuf, cur_in_ea, chunkSize);

        aes_decrypt_ctr_stream(&aes_ctx, tmpBuf, chunkSize);

        DMAWrite(tmpBuf, cur_out_ea, chunkSize);

        cur_in_ea += chunkSize;
        cur_out_ea += chunkSize;

        left -= chunkSize;

        if (left == 0)
            break;
    }
}

#include "../uzlib/adler32.c"
#include "../uzlib/crc32.c"
#include "../uzlib/tinflate.c"
#include "../uzlib/tinfzlib.c"

struct __attribute__((aligned(8))) Stagex_spu_job_zlib_decompress_context_s
{
    uint64_t in_ea;
    uint64_t compressed_size;

    uint64_t out_ea;
    uint64_t out_decompressed_size; // output written by spu

    // internal context, exposed to ppu for debugging

    uint64_t uzlib_cur_in_ea;
    uint64_t uzlib_cur_out_ea;

    uint64_t uzlib_in_left;

    int32_t uzlib_cur_status;

    uint8_t* uzlib_inTmpBuf;
    uint32_t uzlib_inTmpBufSize;

    uint8_t* uzlib_outTmpBuf;
    uint32_t uzlib_outTmpBufSize;

    uint8_t* uzlib_dictTmpBuf;
    uint32_t uzlib_dictTmpBufSize;
};

int32_t Stagex_spu_job_zlib_decompress_readSource_cb(struct uzlib_uncomp* uncomp)
{
    struct Stagex_spu_job_zlib_decompress_context_s* job_context = (struct Stagex_spu_job_zlib_decompress_context_s*)uncomp->userPtr;

    uint32_t n = (job_context->uzlib_in_left < job_context->uzlib_inTmpBufSize) ? (uint32_t)job_context->uzlib_in_left : job_context->uzlib_inTmpBufSize;
    DMARead(job_context->uzlib_inTmpBuf, job_context->uzlib_cur_in_ea, n);

    job_context->uzlib_cur_in_ea += n;
    job_context->uzlib_in_left -= n;

    uncomp->source = job_context->uzlib_inTmpBuf;
    uncomp->source_limit = (uncomp->source + n);
    ++uncomp->source;

    return job_context->uzlib_inTmpBuf[0];
}

void Stagex_spu_job_zlib_decompress(volatile struct Stagex_spu_job_zlib_decompress_context_s* job_context)
{
    if (job_context->compressed_size == 0)
        stop(5);

    //

    job_context->out_decompressed_size = 0;

    //

    job_context->uzlib_inTmpBufSize = (32 * 1024);
    job_context->uzlib_inTmpBuf = (uint8_t*)0x10000;

    job_context->uzlib_outTmpBufSize = (32 * 1024);
    job_context->uzlib_outTmpBuf = (uint8_t*)0x18000;

    job_context->uzlib_dictTmpBufSize = (32 * 1024);
    job_context->uzlib_dictTmpBuf = (uint8_t*)0x20000;

    //

    job_context->uzlib_in_left = job_context->compressed_size;

    job_context->uzlib_cur_in_ea = job_context->in_ea;
    job_context->uzlib_cur_out_ea = job_context->out_ea;

    //

    struct uzlib_uncomp* uncomp = (struct uzlib_uncomp*)0x28000;
    uzlib_uncompress_init(uncomp, job_context->uzlib_dictTmpBuf, job_context->uzlib_dictTmpBufSize);
    
    uncomp->userPtr = job_context;

    uncomp->source = NULL;
    uncomp->source_limit = NULL;
    uncomp->source_read_cb = Stagex_spu_job_zlib_decompress_readSource_cb;

    job_context->uzlib_cur_status = uzlib_zlib_parse_header(uncomp);

    if (job_context->uzlib_cur_status < TINF_OK)
        stop(5);

    //

    while (1)
    {
        uncomp->dest_start = job_context->uzlib_outTmpBuf;
        uncomp->dest = job_context->uzlib_outTmpBuf;
        uncomp->dest_limit = (uncomp->dest + job_context->uzlib_outTmpBufSize);

        job_context->uzlib_cur_status = uzlib_uncompress_chksum(uncomp);

        {
            uint32_t n = ((uint32_t)uncomp->dest - (uint32_t)uncomp->dest_start);

            DMAWrite(job_context->uzlib_outTmpBuf, job_context->uzlib_cur_out_ea, n);
            job_context->uzlib_cur_out_ea += n;

            job_context->out_decompressed_size += n;
        }

        if (job_context->uzlib_cur_status != TINF_OK)
            break;
    }

    //

    if (job_context->uzlib_cur_status != TINF_DONE)
        stop(5);
}

void main()
{
    volatile uint64_t* jobStart = (volatile uint64_t*)0xf00;
    volatile uint64_t* jobDone = (volatile uint64_t*)0xf08;

    *jobStart = 0;
    *jobDone = 0;

    sync();

    volatile uint64_t* spuReady = (volatile uint64_t*)0xf10;
    *spuReady = 1;

    sync();

    while (1)
    {
        if (*jobStart != 0)
        {
            *jobStart = 0;
            sync();

            const volatile struct Stagex_spu_context_s* context = (const volatile struct Stagex_spu_context_s*)0x100;

            if (context->jobType == 1)
                Stagex_spu_job_aes128_decrypt_ctr((const volatile struct Stagex_spu_job_aes128_decrypt_ctr_context_s*)0x200);
            else if (context->jobType == 2)
                Stagex_spu_job_zlib_decompress((volatile struct Stagex_spu_job_zlib_decompress_context_s*)0x200);
            else
                stop(3);

            sync();
            *jobDone = 1;
            sync();
        }
    }

    stop(99);
}