#ifndef __PRINTK__
#define __PRINTK__

// Custom printf function with color support.
// printk("Hello, {{r}}%s{{0}}! The answer is {{g}}%d{{0}}.\n", "World", 42);
void printk(const char *format, ...);

#endif
