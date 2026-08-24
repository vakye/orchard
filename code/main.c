
__attribute__((force_align_arg_pointer))
void EntryPoint(void)
{
    __asm__ volatile ("syscall" :: "a"(60), "D"(0));
}

