#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(int argc, char** argv)
{
        printk("wassup\n");
        
        return 0;
}