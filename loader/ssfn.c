#define NULL 0L
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <ssfn.h>
#include <multiboot.h>

extern ssfn_font_t _binary_console2_sfn_start;
extern multiboot_info_t *multiboot_info;
void init_ssfn()
{
	ssfn_src = &_binary_console2_sfn_start;
	ssfn_dst.ptr = (unsigned char *)(unsigned long) multiboot_info->framebuffer_addr;
	ssfn_dst.p = multiboot_info->framebuffer_pitch;
	ssfn_dst.w = multiboot_info->framebuffer_width;
	ssfn_dst.h = multiboot_info->framebuffer_height;
	ssfn_dst.fg = 0xFFFFFFFF;                   /* colors, white on black */
	ssfn_dst.bg = 0;
	ssfn_dst.x = 100;                           /* coordinates to draw to */
	ssfn_dst.y = 200;
}

void puts_ssfn(char *s)
{
	while(*s)
	{
		ssfn_putc(*s++);
	}
}
