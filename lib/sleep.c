#include <system.h>
extern unsigned long long pit_counter;
int usleep(unsigned long useconds)
{
long long rem=useconds;
unsigned long long oldcounter, counter;
oldcounter=pit_counter;
while(rem>0) {
__asm__("hlt");
counter=pit_counter;
rem-=(counter-oldcounter)*10;
oldcounter=counter;
}
return 0;
}
