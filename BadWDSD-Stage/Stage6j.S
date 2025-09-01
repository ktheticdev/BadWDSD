#if 0

seg002:00000000002B3684 SPU_iso_load_request:                   # CODE XREF: sub_2B56CC+28↓p
seg002:00000000002B3684                 ld        r9, -0x30E0(r2)
seg002:00000000002B3688                 sldi      r3, r3, 19
seg002:00000000002B368C                 li        r0, 3
seg002:00000000002B3690                 ld        r9, 0(r9)
seg002:00000000002B3694                 addis     r9, r9, 4
seg002:00000000002B3698                 addi      r9, r9, 0x401C
seg002:00000000002B369C                 stwx      r0, r3, r9
seg002:00000000002B36A0                 blr
seg002:00000000002B36A0 # End of function SPU_iso_load_request

E9 22 CF 20 78 63 9B 24 38 00 00 03 E9 29 00 00 3D 29 00 04 39 29 40 1C 7C 03 49 2E

seg002:00000000002B312C __get_spu_status:                       # CODE XREF: j___get_spu_status↓j
seg002:00000000002B312C                                         # sub_2B55D4+34↓p
seg002:00000000002B312C                 eieio
seg002:00000000002B3130                 ld        r9, -0x30E0(r2)
seg002:00000000002B3134                 sldi      r3, r3, 19
seg002:00000000002B3138                 ld        r9, 0(r9)
seg002:00000000002B313C                 addis     r9, r9, 4
seg002:00000000002B3140                 addi      r9, r9, 0x4024
seg002:00000000002B3144                 lwzx      r3, r3, r9
seg002:00000000002B3148                 blr
seg002:00000000002B3148 # End of function __get_spu_status

7C 00 06 AC E9 22 CF 20 78 63 9B 24 E9 29 00 00 3D 29 00 04 39 29 40 24 7C 63 48 2E

#endif

// 0x2401F031600

stage_entry:
    li %r10, 0x69 // 1 = SPU_iso_load_request, 2 = __get_spu_status

    li %r11, 0x220
    ld %r11, 0(%r11)
    mtctr %r11
    bctr