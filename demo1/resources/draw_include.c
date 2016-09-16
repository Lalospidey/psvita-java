extern "C" {
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include <psp2/display.h>
    #include <psp2/gxm.h>
    #include <psp2/kernel/sysmem.h>
    #include <psp2/types.h>

    #define align_mem(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))
    #define lerp(value, from_max, to_max) ((((value*10) * (to_max*10))/(from_max*10))/10)
    #define abs(x) ((x) < 0 ? (-x) : (x))

    #define RGBA8(r, g, b, a)      ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

    #define SCREEN_W 960
    #define SCREEN_H 544

    #define RED   RGBA8(255, 0,   0,   255)
    #define GREEN RGBA8(0,   255, 0,   255)
    #define BLUE  RGBA8(0,   0,   255, 255)
    #define CYAN  RGBA8(0,   255, 255, 255)
    #define LIME  RGBA8(50,  205, 50,  255)
    #define PURP  RGBA8(147, 112, 219, 255)
    #define WHITE RGBA8(255, 255, 255, 255)
    #define BLACK RGBA8(0,   0,   0,   255)

    void init_video();
    void end_video();
    void swap_buffers();
    void clear_screen();
    void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
    void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
    void draw_circle(uint32_t x, uint32_t y, uint32_t radius, uint32_t color);

    void font_draw_char(int x, int y, uint32_t color, char c);
    void font_draw_string(int x, int y, uint32_t color, const char *string);
    void font_draw_stringf(int x, int y, uint32_t color, const char *s, ...);

    const unsigned char msx_font[] __attribute((aligned(4))) =
    /*"\x00\x00\x00\x00\x00\x00\x00\x00\x3c\x42\xa5\x81\xa5\x99\x42\x3c"
    "\x3c\x7e\xdb\xff\xff\xdb\x66\x3c\x6c\xfe\xfe\xfe\x7c\x38\x10\x00"
    "\x10\x38\x7c\xfe\x7c\x38\x10\x00\x10\x38\x54\xfe\x54\x10\x38\x00"
    "\x10\x38\x7c\xfe\xfe\x10\x38\x00\x00\x00\x00\x30\x30\x00\x00\x00"
    "\xff\xff\xff\xe7\xe7\xff\xff\xff\x38\x44\x82\x82\x82\x44\x38\x00"
    "\xc7\xbb\x7d\x7d\x7d\xbb\xc7\xff\x0f\x03\x05\x79\x88\x88\x88\x70"
    "\x38\x44\x44\x44\x38\x10\x7c\x10\x30\x28\x24\x24\x28\x20\xe0\xc0"
    "\x3c\x24\x3c\x24\x24\xe4\xdc\x18\x10\x54\x38\xee\x38\x54\x10\x00"
    "\x10\x10\x10\x7c\x10\x10\x10\x10\x10\x10\x10\xff\x00\x00\x00\x00"
    "\x00\x00\x00\xff\x10\x10\x10\x10\x10\x10\x10\xf0\x10\x10\x10\x10"
    "\x10\x10\x10\x1f\x10\x10\x10\x10\x10\x10\x10\xff\x10\x10\x10\x10"
    "\x10\x10\x10\x10\x10\x10\x10\x10\x00\x00\x00\xff\x00\x00\x00\x00"
    "\x00\x00\x00\x1f\x10\x10\x10\x10\x00\x00\x00\xf0\x10\x10\x10\x10"
    "\x10\x10\x10\x1f\x00\x00\x00\x00\x10\x10\x10\xf0\x00\x00\x00\x00"
    "\x81\x42\x24\x18\x18\x24\x42\x81\x01\x02\x04\x08\x10\x20\x40\x80"
    "\x80\x40\x20\x10\x08\x04\x02\x01\x00\x10\x10\xff\x10\x10\x00\x00"*/
    "\x00\x00\x00\x00\x00\x00\x00\x00\x20\x20\x20\x20\x00\x00\x20\x00"
    "\x50\x50\x50\x00\x00\x00\x00\x00\x50\x50\xf8\x50\xf8\x50\x50\x00"
    "\x20\x78\xa0\x70\x28\xf0\x20\x00\xc0\xc8\x10\x20\x40\x98\x18\x00"
    "\x40\xa0\x40\xa8\x90\x98\x60\x00\x10\x20\x40\x00\x00\x00\x00\x00"
    "\x10\x20\x40\x40\x40\x20\x10\x00\x40\x20\x10\x10\x10\x20\x40\x00"
    "\x20\xa8\x70\x20\x70\xa8\x20\x00\x00\x20\x20\xf8\x20\x20\x00\x00"
    "\x00\x00\x00\x00\x00\x20\x20\x40\x00\x00\x00\x78\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x60\x60\x00\x00\x00\x08\x10\x20\x40\x80\x00"
    "\x70\x88\x98\xa8\xc8\x88\x70\x00\x20\x60\xa0\x20\x20\x20\xf8\x00"
    "\x70\x88\x08\x10\x60\x80\xf8\x00\x70\x88\x08\x30\x08\x88\x70\x00"
    "\x10\x30\x50\x90\xf8\x10\x10\x00\xf8\x80\xe0\x10\x08\x10\xe0\x00"
    "\x30\x40\x80\xf0\x88\x88\x70\x00\xf8\x88\x10\x20\x20\x20\x20\x00"
    "\x70\x88\x88\x70\x88\x88\x70\x00\x70\x88\x88\x78\x08\x10\x60\x00"
    "\x00\x00\x20\x00\x00\x20\x00\x00\x00\x00\x20\x00\x00\x20\x20\x40"
    "\x18\x30\x60\xc0\x60\x30\x18\x00\x00\x00\xf8\x00\xf8\x00\x00\x00"
    "\xc0\x60\x30\x18\x30\x60\xc0\x00\x70\x88\x08\x10\x20\x00\x20\x00"
    "\x70\x88\x08\x68\xa8\xa8\x70\x00\x20\x50\x88\x88\xf8\x88\x88\x00"
    "\xf0\x48\x48\x70\x48\x48\xf0\x00\x30\x48\x80\x80\x80\x48\x30\x00"
    "\xe0\x50\x48\x48\x48\x50\xe0\x00\xf8\x80\x80\xf0\x80\x80\xf8\x00"
    "\xf8\x80\x80\xf0\x80\x80\x80\x00\x70\x88\x80\xb8\x88\x88\x70\x00"
    "\x88\x88\x88\xf8\x88\x88\x88\x00\x70\x20\x20\x20\x20\x20\x70\x00"
    "\x38\x10\x10\x10\x90\x90\x60\x00\x88\x90\xa0\xc0\xa0\x90\x88\x00"
    "\x80\x80\x80\x80\x80\x80\xf8\x00\x88\xd8\xa8\xa8\x88\x88\x88\x00"
    "\x88\xc8\xc8\xa8\x98\x98\x88\x00\x70\x88\x88\x88\x88\x88\x70\x00"
    "\xf0\x88\x88\xf0\x80\x80\x80\x00\x70\x88\x88\x88\xa8\x90\x68\x00"
    "\xf0\x88\x88\xf0\xa0\x90\x88\x00\x70\x88\x80\x70\x08\x88\x70\x00"
    "\xf8\x20\x20\x20\x20\x20\x20\x00\x88\x88\x88\x88\x88\x88\x70\x00"
    "\x88\x88\x88\x88\x50\x50\x20\x00\x88\x88\x88\xa8\xa8\xd8\x88\x00"
    "\x88\x88\x50\x20\x50\x88\x88\x00\x88\x88\x88\x70\x20\x20\x20\x00"
    "\xf8\x08\x10\x20\x40\x80\xf8\x00\x70\x40\x40\x40\x40\x40\x70\x00"
    "\x00\x00\x80\x40\x20\x10\x08\x00\x70\x10\x10\x10\x10\x10\x70\x00"
    "\x20\x50\x88\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf8\x00"
    "\x40\x20\x10\x00\x00\x00\x00\x00\x00\x00\x70\x08\x78\x88\x78\x00"
    "\x80\x80\xb0\xc8\x88\xc8\xb0\x00\x00\x00\x70\x88\x80\x88\x70\x00"
    "\x08\x08\x68\x98\x88\x98\x68\x00\x00\x00\x70\x88\xf8\x80\x70\x00"
    "\x10\x28\x20\xf8\x20\x20\x20\x00\x00\x00\x68\x98\x98\x68\x08\x70"
    "\x80\x80\xf0\x88\x88\x88\x88\x00\x20\x00\x60\x20\x20\x20\x70\x00"
    "\x10\x00\x30\x10\x10\x10\x90\x60\x40\x40\x48\x50\x60\x50\x48\x00"
    "\x60\x20\x20\x20\x20\x20\x70\x00\x00\x00\xd0\xa8\xa8\xa8\xa8\x00"
    "\x00\x00\xb0\xc8\x88\x88\x88\x00\x00\x00\x70\x88\x88\x88\x70\x00"
    "\x00\x00\xb0\xc8\xc8\xb0\x80\x80\x00\x00\x68\x98\x98\x68\x08\x08"
    "\x00\x00\xb0\xc8\x80\x80\x80\x00\x00\x00\x78\x80\xf0\x08\xf0\x00"
    "\x40\x40\xf0\x40\x40\x48\x30\x00\x00\x00\x90\x90\x90\x90\x68\x00"
    "\x00\x00\x88\x88\x88\x50\x20\x00\x00\x00\x88\xa8\xa8\xa8\x50\x00"
    "\x00\x00\x88\x50\x20\x50\x88\x00\x00\x00\x88\x88\x98\x68\x08\x70"
    "\x00\x00\xf8\x10\x20\x40\xf8\x00\x18\x20\x20\x40\x20\x20\x18\x00"
    "\x20\x20\x20\x00\x20\x20\x20\x00\xc0\x20\x20\x10\x20\x20\xc0\x00"
    "\x40\xa8\x10\x00\x00\x00\x00\x00\x00\x00\x20\x50\xf8\x00\x00\x00"
    "\x70\x88\x80\x80\x88\x70\x20\x60\x90\x00\x00\x90\x90\x90\x68\x00"
    /*"\x10\x20\x70\x88\xf8\x80\x70\x00\x20\x50\x70\x08\x78\x88\x78\x00"
    "\x48\x00\x70\x08\x78\x88\x78\x00\x20\x10\x70\x08\x78\x88\x78\x00"
    "\x20\x00\x70\x08\x78\x88\x78\x00\x00\x70\x80\x80\x80\x70\x10\x60"
    "\x20\x50\x70\x88\xf8\x80\x70\x00\x50\x00\x70\x88\xf8\x80\x70\x00"
    "\x20\x10\x70\x88\xf8\x80\x70\x00\x50\x00\x00\x60\x20\x20\x70\x00"
    "\x20\x50\x00\x60\x20\x20\x70\x00\x40\x20\x00\x60\x20\x20\x70\x00"
    "\x50\x00\x20\x50\x88\xf8\x88\x00\x20\x00\x20\x50\x88\xf8\x88\x00"
    "\x10\x20\xf8\x80\xf0\x80\xf8\x00\x00\x00\x6c\x12\x7e\x90\x6e\x00"
    "\x3e\x50\x90\x9c\xf0\x90\x9e\x00\x60\x90\x00\x60\x90\x90\x60\x00"
    "\x90\x00\x00\x60\x90\x90\x60\x00\x40\x20\x00\x60\x90\x90\x60\x00"
    "\x40\xa0\x00\xa0\xa0\xa0\x50\x00\x40\x20\x00\xa0\xa0\xa0\x50\x00"
    "\x90\x00\x90\x90\xb0\x50\x10\xe0\x50\x00\x70\x88\x88\x88\x70\x00"
    "\x50\x00\x88\x88\x88\x88\x70\x00\x20\x20\x78\x80\x80\x78\x20\x20"
    "\x18\x24\x20\xf8\x20\xe2\x5c\x00\x88\x50\x20\xf8\x20\xf8\x20\x00"
    "\xc0\xa0\xa0\xc8\x9c\x88\x88\x8c\x18\x20\x20\xf8\x20\x20\x20\x40"
    "\x10\x20\x70\x08\x78\x88\x78\x00\x10\x20\x00\x60\x20\x20\x70\x00"
    "\x20\x40\x00\x60\x90\x90\x60\x00\x20\x40\x00\x90\x90\x90\x68\x00"
    "\x50\xa0\x00\xa0\xd0\x90\x90\x00\x28\x50\x00\xc8\xa8\x98\x88\x00"
    "\x00\x70\x08\x78\x88\x78\x00\xf8\x00\x60\x90\x90\x90\x60\x00\xf0"
    "\x20\x00\x20\x40\x80\x88\x70\x00\x00\x00\x00\xf8\x80\x80\x00\x00"
    "\x00\x00\x00\xf8\x08\x08\x00\x00\x84\x88\x90\xa8\x54\x84\x08\x1c"
    "\x84\x88\x90\xa8\x58\xa8\x3c\x08\x20\x00\x00\x20\x20\x20\x20\x00"
    "\x00\x00\x24\x48\x90\x48\x24\x00\x00\x00\x90\x48\x24\x48\x90\x00"
    "\x28\x50\x20\x50\x88\xf8\x88\x00\x28\x50\x70\x08\x78\x88\x78\x00"
    "\x28\x50\x00\x70\x20\x20\x70\x00\x28\x50\x00\x20\x20\x20\x70\x00"
    "\x28\x50\x00\x70\x88\x88\x70\x00\x50\xa0\x00\x60\x90\x90\x60\x00"
    "\x28\x50\x00\x88\x88\x88\x70\x00\x50\xa0\x00\xa0\xa0\xa0\x50\x00"
    "\xfc\x48\x48\x48\xe8\x08\x50\x20\x00\x50\x00\x50\x50\x50\x10\x20"
    "\xc0\x44\xc8\x54\xec\x54\x9e\x04\x10\xa8\x40\x00\x00\x00\x00\x00"
    "\x00\x20\x50\x88\x50\x20\x00\x00\x88\x10\x20\x40\x80\x28\x00\x00"
    "\x7c\xa8\xa8\x68\x28\x28\x28\x00\x38\x40\x30\x48\x48\x30\x08\x70"
    "\x00\x00\x00\x00\x00\x00\xff\xff\xf0\xf0\xf0\xf0\x0f\x0f\x0f\x0f"
    "\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x3c\x3c\x00\x00\x00\xff\xff\xff\xff\xff\xff\x00\x00"
    "\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\x0f\x0f\x0f\x0f\xf0\xf0\xf0\xf0"
    "\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\x03\x03\x03\x03\x03\x03\x03\x03"
    "\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x11\x22\x44\x88\x11\x22\x44\x88"
    "\x88\x44\x22\x11\x88\x44\x22\x11\xfe\x7c\x38\x10\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x10\x38\x7c\xfe\x80\xc0\xe0\xf0\xe0\xc0\x80\x00"
    "\x01\x03\x07\x0f\x07\x03\x01\x00\xff\x7e\x3c\x18\x18\x3c\x7e\xff"
    "\x81\xc3\xe7\xff\xff\xe7\xc3\x81\xf0\xf0\xf0\xf0\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x00\x00\x00\x00"
    "\x00\x00\x00\x00\xf0\xf0\xf0\xf0\x33\x33\xcc\xcc\x33\x33\xcc\xcc"
    "\x00\x20\x20\x50\x50\x88\xf8\x00\x20\x20\x70\x20\x70\x20\x20\x00"
    "\x00\x00\x00\x50\x88\xa8\x50\x00\xff\xff\xff\xff\xff\xff\xff\xff"
    "\x00\x00\x00\x00\xff\xff\xff\xff\xf0\xf0\xf0\xf0\xf0\xf0\xf0\xf0"
    "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\xff\xff\xff\xff\x00\x00\x00\x00"
    "\x00\x00\x68\x90\x90\x90\x68\x00\x30\x48\x48\x70\x48\x48\x70\xc0"
    "\xf8\x88\x80\x80\x80\x80\x80\x00\xf8\x50\x50\x50\x50\x50\x98\x00"
    "\xf8\x88\x40\x20\x40\x88\xf8\x00\x00\x00\x78\x90\x90\x90\x60\x00"
    "\x00\x50\x50\x50\x50\x68\x80\x80\x00\x50\xa0\x20\x20\x20\x20\x00"
    "\xf8\x20\x70\xa8\xa8\x70\x20\xf8\x20\x50\x88\xf8\x88\x50\x20\x00"
    "\x70\x88\x88\x88\x50\x50\xd8\x00\x30\x40\x40\x20\x50\x50\x50\x20"
    "\x00\x00\x00\x50\xa8\xa8\x50\x00\x08\x70\xa8\xa8\xa8\x70\x80\x00"
    "\x38\x40\x80\xf8\x80\x40\x38\x00\x70\x88\x88\x88\x88\x88\x88\x00"
    "\x00\xf8\x00\xf8\x00\xf8\x00\x00\x20\x20\xf8\x20\x20\x00\xf8\x00"
    "\xc0\x30\x08\x30\xc0\x00\xf8\x00\x18\x60\x80\x60\x18\x00\xf8\x00"
    "\x10\x28\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xa0\x40"
    "\x00\x20\x00\xf8\x00\x20\x00\x00\x00\x50\xa0\x00\x50\xa0\x00\x00"
    "\x00\x18\x24\x24\x18\x00\x00\x00\x00\x30\x78\x78\x30\x00\x00\x00"
    "\x00\x00\x00\x00\x30\x00\x00\x00\x3e\x20\x20\x20\xa0\x60\x20\x00"
    "\xa0\x50\x50\x50\x00\x00\x00\x00\x40\xa0\x20\x40\xe0\x00\x00\x00"
    "\x00\x38\x38\x38\x38\x38\x38\x00\x00\x00\x00\x00\x00\x00\x00"*/;


    static SceDisplayFrameBuf fb[2];
    static SceUID fb_memuid[2];
    static int cur_fb = 0;

    static void *alloc_gpu_mem(uint32_t type, uint32_t size, SceGxmMemoryAttribFlags attribs, SceUID *uid)
    {
        int ret;
        void *mem = NULL;

        if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW)
            size = align_mem(size, 256 * 1024);
        else
            size = align_mem(size, 4 * 1024);

        *uid = sceKernelAllocMemBlock("gxm", type, size, NULL);

        printf("MemBlock uid: 0x%08X\n", *uid);

        ret = sceKernelGetMemBlockBase(*uid, &mem);
        printf("sceKernelGetMemBlockBase(): 0x%08X\n", ret);
        printf("MemBlockBase addr: %p\n", mem);
        if (ret != 0) {
            return NULL;
        }

        ret = sceGxmMapMemory(mem, size, attribs);
        printf("sceGxmMapMemory(): 0x%08X\n", ret);
        if (ret != 0) {
            return NULL;
        }

        return mem;
    }

    void init_video()
    {
        int ret;

        SceGxmInitializeParams params;

        params.flags                        = 0x0;
        params.displayQueueMaxPendingCount  = 0x2; //Double buffering
        params.displayQueueCallback         = 0x0;
        params.displayQueueCallbackDataSize = 0x0;
        params.parameterBufferSize          = (16 * 1024 * 1024);

        /* Initialize the GXM */
        ret = sceGxmInitialize(&params);
        printf("sceGxmInitialize(): 0x%08X\n", ret);

        /* Setup framebuffers */
        fb[0].size        = sizeof(fb[0]);
        fb[0].pitch       = SCREEN_W;
        fb[0].pixelformat = SCE_DISPLAY_PIXELFORMAT_A8B8G8R8;
        fb[0].width       = SCREEN_W;
        fb[0].height      = SCREEN_H;

        fb[1].size        = sizeof(fb[1]);
        fb[1].pitch       = SCREEN_W;
        fb[1].pixelformat = SCE_DISPLAY_PIXELFORMAT_A8B8G8R8;
        fb[1].width       = SCREEN_W;
        fb[1].height      = SCREEN_H;

        /* Allocate memory for the framebuffers */
        fb[0].base = alloc_gpu_mem(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            SCREEN_W * SCREEN_H * 4, SCE_GXM_MEMORY_ATTRIB_RW, &fb_memuid[0]);

        if (fb[0].base == NULL) {
            printf("Could not allocate memory for fb[0]. %p", fb[0].base);
            return;
        }

        fb[1].base = alloc_gpu_mem(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            SCREEN_W * SCREEN_H * 4, SCE_GXM_MEMORY_ATTRIB_RW, &fb_memuid[1]);

        if (fb[1].base == NULL) {
            printf("Could not allocate memory for fb[1]. %p", fb[1].base);
            return;
        }

        /* Display the framebuffer 0 */
        cur_fb = 0;
        swap_buffers();

        printf(
            "\nframebuffer 0:\n"
            "\tsize:           0x%08X\n"
            "\tbase:           0x%08X\n"
            "\tpitch:          0x%08X\n"
            "\tpixelformat:    0x%08X\n"
            "\twidth:          0x%08X\n"
            "\theight          0x%08X\n",
            fb[0].size, (uintptr_t)fb[0].base,
            fb[0].pitch, fb[0].pixelformat, fb[0].width, fb[0].height);

        printf(
            "\nframebuffer 1:\n"
            "\tsize:           0x%08X\n"
            "\tbase:           0x%08X\n"
            "\tpitch:          0x%08X\n"
            "\tpixelformat:    0x%08X\n"
            "\twidth:          0x%08X\n"
            "\theight          0x%08X\n",
            fb[1].size, (uintptr_t)fb[1].base,
            fb[1].pitch, fb[1].pixelformat, fb[1].width, fb[1].height);
    }

    void end_video()
    {
        sceGxmUnmapMemory(fb[0].base);
        sceGxmUnmapMemory(fb[1].base);
        sceGxmTerminate();
    }

    void swap_buffers()
    {
        sceDisplaySetFrameBuf(&fb[cur_fb], SCE_DISPLAY_SETBUF_NEXTFRAME);
        cur_fb ^= 1;
    }

    void clear_screen()
    {
        memset(fb[cur_fb].base, 0xFF, SCREEN_W*SCREEN_H*4);
    }

    void draw_pixel(uint32_t x, uint32_t y, uint32_t color)
    {
        ((uint32_t *)fb[cur_fb].base)[x + y*fb[cur_fb].pitch] = color;
    }

    void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
    {
        int i, j;
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j++) {
                ((uint32_t *)fb[cur_fb].base)[(x + j) + (y + i)*fb[cur_fb].pitch] = color;
            }
        }
    }

    void draw_circle(uint32_t x, uint32_t y, uint32_t radius, uint32_t color)
    {
        int r2 = radius * radius;
        int area = r2 << 2;
        int rr = radius << 1;

        int i;
        for (i = 0; i < area; i++) {
            int tx = (i % rr) - radius;
            int ty = (i / rr) - radius;

            if (tx * tx + ty * ty <= r2) {
                draw_pixel(x + tx, y + ty, color);
            }
        }
    }

    void font_draw_char(int x, int y, uint32_t color, char c)
    {
        unsigned char *font = (unsigned char *)(msx_font + (c - (uint32_t)' ') * 8);
        int i, j, pos_x, pos_y;
        for (i = 0; i < 8; ++i) {
            pos_y = y + i*2;
            for (j = 0; j < 8; ++j) {
                pos_x = x + j*2;
                if ((*font & (128 >> j))) {
                    draw_pixel(pos_x + 0, pos_y + 0, color);
                    draw_pixel(pos_x + 1, pos_y + 0, color);
                    draw_pixel(pos_x + 0, pos_y + 1, color);
                    draw_pixel(pos_x + 1, pos_y + 1, color);
                }
            }
            ++font;
        }
    }

    void font_draw_string(int x, int y, uint32_t color, const char *string)
    {
        if (string == NULL) return;

        int startx = x;
        const char *s = string;

        while (*s) {
            if (*s == '\n') {
                x = startx;
                y += 16;
            } else if (*s == ' ') {
                x += 16;
            } else if(*s == '\t') {
                x += 16*4;
            } else {
                font_draw_char(x, y, color, *s);
                x += 16;
            }
            ++s;
        }
    }

    void font_draw_stringf(int x, int y, uint32_t color, const char *s, ...)
    {
        char buf[256];
        va_list argptr;
        va_start(argptr, s);
        vsnprintf(buf, sizeof(buf), s, argptr);
        va_end(argptr);
        font_draw_string(x, y, color, buf);
    }

	static SceCtrlData pad = {0};
	static SceTouchData touch = {0};

    void input_read() {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        sceTouchPeek(0, &touch, 1);
    }

    int pad_buttons() {
        return pad.buttons;
    }

    void frame_end() {
        sceDisplayWaitVblankStart();
    }
}