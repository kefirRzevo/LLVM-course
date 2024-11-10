# My ISA

> addi r1 r2 r3 <r1 = r2 + r3>

> subi r1 r2 r3 <r1 = r2 - r3>

> muli r1 r2 r3 <r1 = r2 * r3>

> divi r1 r2 r3 <r1 = r2 / r3>

> remi r1 r2 r3 <r1 = r2 % r3>

> andi r1 r2 r3 <r1 = r2 & r3>

> xori r1 r2 r3 <r1 = r2 ^ r3>

> shli r1 r2 r3 <r1 = r2 << r3>

> shri r1 r2 r3 <r1 = r2 >> r3>

> mov r1 r2 <r1 = r2>

> mov_imm r1 imm <r1 = imm>

> ilt r1 r2 r3 <r1 = r2 < r3>

> imet r1 r2 r3 <r1 = r2 >= r3>

> neq r1 r2 r3 <r1 = r2 != r3>

> select r1 r2 r3 r4 <r1 = r2 ? r3 : r4>

> rand r1 <r1 = std::rand()>

> cf r1 <r1 = new bool[GUI_WIDTH * GUI_HEIGHT]>

> df r1 <r1 = delete r1[]>

> lea r1 r2 r3 imm <r1 = r2 + r3 * imm>

> flush <gui_flush()>

> has_quit r1 <r1 = gui_quit_event()>

> set_pixel r1 r2 r3 <gui_set_pixel(r1, r2, r3)>

> rgb_reg r1 r2 r3 r4 imm <r1 = (r2 << 24) | (r3 << 16) | (r4 << 8) | imm>

> lbi r1 r2 <r1 = *(uint8_t*)r2>

> lwi r1 r2 <r1 = *(uint32_t*)r2>

> ldi r1 r2 <r1 = *(uint64_t*)r2>

> sbi r1 r2 <*(uint8_t*)r1 = r2>

> swi r1 r2 <*(uint32_t*)r1 = r2>

> sdi r1 r2 <*(uint64_t*)r1 = r2>

> br label1 <pc = label1>

> brif r1 label1 label2 <pc = r1 ? label1 : label2>

> call label1 <sp = label1>

> ret <sp /* prev function */>

> push r1 <*(sp++) = r1>

> pop r1 <r1 = *(sp--)>

> push_regs <*(sp+=32) = x0...x31>

> pop_regs <x0...x31 = <*(sp-=32)>

> dump_regs <std::cout << x0 << ... << x31>

> dump_mem r1 r2 <std::cout << *(uint8_t*) r1 << ... << *((uint8_t*) r1 + r2)>
