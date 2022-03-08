void sys_clock_init(void);
// void sys_dram_init(void);
void sys_uart_init(void);
void sys_mmu_init(void);

void hardware_init_weak(void)
{
#if defined(__CC_ARM)
    sys_clock_init();
    // sys_dram_init(); //已在boot程序中执行, 这里执行整个程序会崩溃
    sys_uart_init();
    sys_mmu_init();
#endif
}
