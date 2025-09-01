#pragma GCC push_options
//#pragma GCC optimize("O0")

struct __attribute__((aligned(8))) mymetldr_context_s
{
    uint64_t myappldrElfAddress;
};

FUNC_DEF void Stage6_IsoLoadRequest(uint64_t spu_id)
{
    //lv1_puts("Stage6_IsoLoadRequest()\n");
    
    //lv1_puts("spu_id = ");
    //lv1_print_decimal(spu_id);
    //lv1_puts("\n");

    struct Stagex_Context_s* ctx = GetStagexContext();

    uint64_t myappldrElfAddress = ctx->cached_myappldrElfAddress;
    
    if ((spu_id != 4) || !ctx->stage6_isAppldr || (myappldrElfAddress == 0))
    {
        SPU_PS_Write32(spu_id, 0x0401C, 0x3);

        ctx->stage6_isAppldr = 0;
        return;
    }

    ctx->stage6_isAppldr = 0;

    uint64_t mymetldrElfAddress = ctx->cached_mymetldrElfAddress;

    if (mymetldrElfAddress == 0)
    {
        lv1_puts("mymetldr.elf not found!\n");
        dead_beep();
    }

    static const uint32_t SPU_STATUS_RUN_MASK = (1 << 0);
    static const uint32_t SPU_STATUS_ISOLATED_MASK = (1 << 7);

    uint32_t status = SPU_PS_Read32(spu_id, 0x04024);
    if ((status & SPU_STATUS_RUN_MASK) != 0)
    {
        // stop request
        SPU_PS_Write32(spu_id, 0x0401C, 0x0);

        while ((status & SPU_STATUS_RUN_MASK) != 0)
        {
            status = SPU_PS_Read32(spu_id, 0x04024);
        }
    }

    status = SPU_PS_Read32(spu_id, 0x04024);
    if ((status & SPU_STATUS_ISOLATED_MASK) != 0)
    {
        // iso exit request
        SPU_PS_Write32(spu_id, 0x0401C, 0x2);

        while ((status & SPU_STATUS_ISOLATED_MASK) != 0)
        {
            status = SPU_PS_Read32(spu_id, 0x04024);
        }
    }

    //lv1_puts("Loading mymetldr.elf...\n");
    LoadElfSpu(mymetldrElfAddress, spu_id, 1);
    
    {
        struct mymetldr_context_s mctx;
        mctx.myappldrElfAddress = myappldrElfAddress;

        memcpy((void*)SPU_CalcMMIOAddress_LS(spu_id, 0x100), &mctx, sizeof(mctx));
    }

    eieio();
    SPU_PS_Write32(spu_id, 0x0401C, 0x1);
}

FUNC_DEF uint32_t Stage6_GetSpuStatus(uint64_t spu_id)
{
    //lv1_puts("Stage6_GetSpuStatus()\n");
    
    //lv1_puts("spu_id = ");
    //lv1_print_decimal(spu_id);
    //lv1_puts("\n");

    static const uint32_t SPU_STATUS_ISOLATED_MASK = (1 << 7);

    uint32_t status = SPU_PS_Read32(spu_id, 0x04024);

    if ((spu_id == 4) && (SPU_LS_Read64(spu_id, 0x39100) == 0x123456789))
        status |= SPU_STATUS_ISOLATED_MASK;

#if 0

    lv1_puts("status = ");
    lv1_print_hex(status);
    lv1_puts("\n");

    uint32_t npc = SPU_PS_Read32(spu_id, 0x04034);
    lv1_puts("npc = ");
    lv1_print_hex(npc);
    lv1_puts("\n");

    struct Stagex_spu_DMACmd_s dmaCmd;
    memcpy(&dmaCmd, (const void*)SPU_CalcMMIOAddress_LS(spu_id, 0x10), sizeof(dmaCmd));

    lv1_puts("dmaCmd.ls = ");
    lv1_print_hex(dmaCmd.ls);
    lv1_puts("\n");

    lv1_puts("dmaCmd.ea = ");
    lv1_print_hex(dmaCmd.ea);
    lv1_puts("\n");

    lv1_puts("dmaCmd.size = ");
    lv1_print_decimal(dmaCmd.size);
    lv1_puts("\n");

    lv1_puts("dmaCmd.cmd = ");
    lv1_print_hex(dmaCmd.cmd);
    lv1_puts("\n");

#endif

    return status;
}

#pragma GCC pop_options

__attribute__((section("main6"))) uint64_t stage6_main(uint64_t spu_id)
{
    register uint64_t r10 asm("r10");
    uint64_t r10_2 = r10;

    sc_puts_init();

    if (r10_2 == 1)
        Stage6_IsoLoadRequest(spu_id);
    else if (r10_2 == 2)
        return Stage6_GetSpuStatus(spu_id);
    else
    {
        lv1_puts("stage6_main bad r10!\n");
        dead();
    }

    return 0;
}

__attribute__((noreturn, section("entry6"))) void stage6_entry()
{
    // push stack
    asm volatile("addi 1, 1, -512");

    // store all registers to stack
    asm volatile("std 0, %0(1)" ::"i"(8 * 0) :);
    asm volatile("std 1, %0(1)" ::"i"(8 * 1) :);
    asm volatile("std 2, %0(1)" ::"i"(8 * 2) :);
    asm volatile("std 3, %0(1)" ::"i"(8 * 3) :);
    asm volatile("std 4, %0(1)" ::"i"(8 * 4) :);
    asm volatile("std 5, %0(1)" ::"i"(8 * 5) :);
    asm volatile("std 6, %0(1)" ::"i"(8 * 6) :);
    asm volatile("std 7, %0(1)" ::"i"(8 * 7) :);
    asm volatile("std 8, %0(1)" ::"i"(8 * 8) :);
    asm volatile("std 9, %0(1)" ::"i"(8 * 9) :);
    asm volatile("std 10, %0(1)" ::"i"(8 * 10) :);
    asm volatile("std 11, %0(1)" ::"i"(8 * 11) :);
    asm volatile("std 12, %0(1)" ::"i"(8 * 12) :);
    asm volatile("std 13, %0(1)" ::"i"(8 * 13) :);
    asm volatile("std 14, %0(1)" ::"i"(8 * 14) :);
    asm volatile("std 15, %0(1)" ::"i"(8 * 15) :);
    asm volatile("std 16, %0(1)" ::"i"(8 * 16) :);
    asm volatile("std 17, %0(1)" ::"i"(8 * 17) :);
    asm volatile("std 18, %0(1)" ::"i"(8 * 18) :);
    asm volatile("std 19, %0(1)" ::"i"(8 * 19) :);
    asm volatile("std 20, %0(1)" ::"i"(8 * 20) :);
    asm volatile("std 21, %0(1)" ::"i"(8 * 21) :);
    asm volatile("std 22, %0(1)" ::"i"(8 * 22) :);
    asm volatile("std 23, %0(1)" ::"i"(8 * 23) :);
    asm volatile("std 24, %0(1)" ::"i"(8 * 24) :);
    asm volatile("std 25, %0(1)" ::"i"(8 * 25) :);
    asm volatile("std 26, %0(1)" ::"i"(8 * 26) :);
    asm volatile("std 27, %0(1)" ::"i"(8 * 27) :);
    asm volatile("std 28, %0(1)" ::"i"(8 * 28) :);
    asm volatile("std 29, %0(1)" ::"i"(8 * 29) :);
    asm volatile("std 30, %0(1)" ::"i"(8 * 30) :);
    asm volatile("std 31, %0(1)" ::"i"(8 * 31) :);

#if 1

    register uint64_t r8 asm("r8");

    // push stack
    asm volatile("addi 1, 1, -64");

    // store original rtoc to stack
    asm volatile("std 2, 0(1)");

    // store original lr to stack
    asm volatile("mflr %0" : "=r"(r8)::);
    asm volatile("std %0, 8(1)" ::"r"(r8) :);

    // set stage_entry_ra
    asm volatile("bl 4");
    asm volatile("mflr %0" : "=r"(stage_entry_ra)::);
    stage_entry_ra -= (4 * 38);

    // set lv1_rtoc
    asm volatile("mr %0, 2" : "=r"(lv1_rtoc)::);

    // set interrupt_depth to 0
    interrupt_depth = 0;

    // set is_lv1 to 0x9669
    is_lv1 = 0x9669;

    // set stage_zero to 0
    stage_zero = 0;

    // set stage_rtoc
    stage_rtoc = stage_entry_ra;
    stage_rtoc += 0x200; // .toc
    stage_rtoc += 0x8000;

    // set r2 to stage_rtoc
    asm volatile("mr 2, %0" ::"r"(stage_rtoc) :);

    // set lv1_sp
    asm volatile("mr %0, 1" :"=r"(lv1_sp)::);

    // set stage_sp to 0xE000000
    //stage_sp = 0xE000000;

    // set r1 to stage_sp
    //asm volatile("mr 1, %0" ::"r"(stage_sp) :);

    // sync
    asm volatile("sync");

    // push stack
    asm volatile("addi 1, 1, -128");

    // jump to stage6_main
    asm volatile("bl stage6_main");

    // pop stack
    asm volatile("addi 1, 1, 128");

    // set r1 to lv1_sp
    asm volatile("mr 1, %0" ::"r"(lv1_sp) :);

    // restore original lr from stack
    asm volatile("ld %0, 8(1)" : "=r"(r8)::);
    asm volatile("mtlr %0" ::"r"(r8));

    // restore original rtoc from stack
    asm volatile("ld %0, 0(1)" : "=r"(r8)::);
    asm volatile("mr 2, %0" ::"r"(r8));

    // pop stack
    asm volatile("addi 1, 1, 64");

#endif

    // restore all registers from stack
    asm volatile("ld 0, %0(1)" ::"i"(8 * 0) :);
    asm volatile("ld 1, %0(1)" ::"i"(8 * 1) :);
    asm volatile("ld 2, %0(1)" ::"i"(8 * 2) :);
    //asm volatile("ld 3, %0(1)" ::"i"(8 * 3) :);
    asm volatile("ld 4, %0(1)" ::"i"(8 * 4) :);
    asm volatile("ld 5, %0(1)" ::"i"(8 * 5) :);
    asm volatile("ld 6, %0(1)" ::"i"(8 * 6) :);
    asm volatile("ld 7, %0(1)" ::"i"(8 * 7) :);
    asm volatile("ld 8, %0(1)" ::"i"(8 * 8) :);
    asm volatile("ld 9, %0(1)" ::"i"(8 * 9) :);
    asm volatile("ld 10, %0(1)" ::"i"(8 * 10) :);
    asm volatile("ld 11, %0(1)" ::"i"(8 * 11) :);
    asm volatile("ld 12, %0(1)" ::"i"(8 * 12) :);
    asm volatile("ld 13, %0(1)" ::"i"(8 * 13) :);
    asm volatile("ld 14, %0(1)" ::"i"(8 * 14) :);
    asm volatile("ld 15, %0(1)" ::"i"(8 * 15) :);
    asm volatile("ld 16, %0(1)" ::"i"(8 * 16) :);
    asm volatile("ld 17, %0(1)" ::"i"(8 * 17) :);
    asm volatile("ld 18, %0(1)" ::"i"(8 * 18) :);
    asm volatile("ld 19, %0(1)" ::"i"(8 * 19) :);
    asm volatile("ld 20, %0(1)" ::"i"(8 * 20) :);
    asm volatile("ld 21, %0(1)" ::"i"(8 * 21) :);
    asm volatile("ld 22, %0(1)" ::"i"(8 * 22) :);
    asm volatile("ld 23, %0(1)" ::"i"(8 * 23) :);
    asm volatile("ld 24, %0(1)" ::"i"(8 * 24) :);
    asm volatile("ld 25, %0(1)" ::"i"(8 * 25) :);
    asm volatile("ld 26, %0(1)" ::"i"(8 * 26) :);
    asm volatile("ld 27, %0(1)" ::"i"(8 * 27) :);
    asm volatile("ld 28, %0(1)" ::"i"(8 * 28) :);
    asm volatile("ld 29, %0(1)" ::"i"(8 * 29) :);
    asm volatile("ld 30, %0(1)" ::"i"(8 * 30) :);
    asm volatile("ld 31, %0(1)" ::"i"(8 * 31) :);

    // pop stack
    asm volatile("addi 1, 1, 512");

    // sync
    asm volatile("sync");

    //
    asm volatile("blr");

    __builtin_unreachable();
}