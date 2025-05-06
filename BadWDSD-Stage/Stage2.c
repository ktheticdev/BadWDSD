#undef ENTRY_WAIT_IN_MS

FUNC_DEF void Stage2()
{
    puts("BadWDSD Stage2 by Kafuu(aomsin2526)\n");

    puts("(Build Date: ");
    puts(__DATE__);
    puts(" ");
    puts(__TIME__);
    puts(")\n");

    {
        uint8_t os_bank_indicator = sc_read_os_bank_indicator();

        puts("os_bank_indicator = ");
        print_hex(os_bank_indicator);
        puts("\n");

        if (os_bank_indicator == 0xff)
            puts("Will use ros0\n");
        else
            puts("Will use ros1\n");

        uint64_t coreOSStartAddress = (os_bank_indicator == 0xff) ? 0x2401F0C0000 : 0x2401F7C0000;

        uint64_t lv1FileAddress;
        uint64_t lv1FileSize;

        {
            uint8_t found = 0;

            if (found == 0)
            {
                puts("Searching for lv1.elf...\n");

                if (CoreOS_FindFileEntry(coreOSStartAddress, "lv1.elf", &lv1FileAddress, &lv1FileSize))
                    found = 1;
                else
                    puts("File not found!\n");
            }

            if (found == 0)
            {
                puts("Searching for lv1.zelf...\n");

                uint64_t zelfFileAddress;
                uint64_t zelfFileSize;

                if (CoreOS_FindFileEntry(coreOSStartAddress, "lv1.zelf", &zelfFileAddress, &zelfFileSize))
                {
                    found = 1;

                    puts("zelfFileAddress = ");
                    print_hex(zelfFileAddress);

                    puts(", zelfFileSize = ");
                    print_decimal(zelfFileSize);

                    puts("\n");

                    lv1FileAddress = 0xC000000;
                    lv1FileSize = (8 * 1024 * 1024);

                    ZelfDecompress(zelfFileAddress, (void*)lv1FileAddress, &lv1FileSize);
                }
                else
                    puts("File not found!\n");
            }

            if (found == 0)
            {
                dead_beep();
            }
        }

        puts("lv1FileAddress = ");
        print_hex(lv1FileAddress);
        puts("\n");

        puts("lv1FileSize = ");
        print_decimal(lv1FileSize);
        puts("\n");

        puts("Loading lv1...\n");
        LoadElf(lv1FileAddress, 0x0);

        puts("Booting lv1...\n");

        eieio();

        asm volatile("li 3, 0x100");
        asm volatile("mtctr 3");
        asm volatile("bctr");
    }
}

__attribute__((section("main2"))) void stage2_main()
{
    sc_puts_init();

    Stage2();

    dead();
}

__attribute__((noreturn, section("entry2"))) void stage2_entry()
{
    // set stage_entry_ra
    asm volatile("bl 4");
    asm volatile("mflr %0" : "=r"(stage_entry_ra)::);
    stage_entry_ra -= 4;

    // set is_lv1 to 0
    is_lv1 = 0;

    // set stage_zero to 0
    stage_zero = 0;

    // set stage_rtoc
    stage_rtoc = stage_entry_ra;
    stage_rtoc += 0x300; // .toc
    stage_rtoc += 0x8000;

    // set r2 to stage_rtoc
    asm volatile("mr 2, %0" ::"r"(stage_rtoc) :);

    // set stage_sp to 0xE000000
    stage_sp = 0xE000000;

    // set r1 to stage_sp
    asm volatile("mr 1, %0" ::"r"(stage_sp) :);

#if ENTRY_WAIT_IN_MS > 0
    // optional wait
    asm volatile("li 3, %0" ::"i"(ENTRY_WAIT_IN_MS) :);
    asm volatile("bl WaitInMs2");
#endif

    // sync
    asm volatile("sync");

    // jump to stage_main
    asm volatile("b stage2_main");

    __builtin_unreachable();
}