#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FBDEVICE "/dev/fb0"

typedef unsigned char ubyte;

unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

static int DrawPoint(int fd, int x, int y, unsigned short color)
{
    struct fb_var_screeninfo vinfo;

    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    int offset = (x + y*vinfo.xres_virtual)*2;
    lseek(fd, offset, SEEK_SET);
    write(fd, &color, 2);

    return 0;
}

static int DrawLine(int fd, int start_x, int end_x, int y, unsigned short color) 
{
    int x, offset;
    struct fb_var_screeninfo vinfo;

    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    for(x = start_x; x < end_x; x++) {
        offset = (x+y*vinfo.xres_virtual)*2;
        lseek(fd, offset, SEEK_SET);
        write(fd, &color, 2);
    };

    return 0;
}

void DrawCircle(int fd, int center_x, int center_y, int radius, unsigned short color)
{
    int x = radius, y = 0;
    int radiusError = 1 - x;
 
    while(x >= y) {
        DrawPoint(fd,  x + center_x,  y + center_y, color);
        DrawPoint(fd,  y + center_x,  x + center_y, color);
        DrawPoint(fd, -x + center_x,  y + center_y, color);
        DrawPoint(fd, -y + center_x,  x + center_y, color);
        DrawPoint(fd, -x + center_x, -y + center_y, color);
        DrawPoint(fd, -y + center_x, -x + center_y, color);
        DrawPoint(fd,  x + center_x, -y + center_y, color);
        DrawPoint(fd, y + center_x, -x + center_y, color);

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    };
}

static int DrawFace(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color) 
{
    int x, y, offset;
    struct fb_var_screeninfo vinfo;

    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    if(end_x == 0) end_x = vinfo.xres_virtual;
    if(end_y == 0) end_y = vinfo.yres_virtual;

    for(x = start_x; x < end_x; x++) {
        for(y = start_y; y < end_y; y++) {
            offset = (x + y*vinfo.xres_virtual)*2;
            lseek(fd, offset, SEEK_SET);
            write(fd, &color, 2);
        };
    };

    return 0;
}

static int DrawFaceMMAP(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color)
{
    int x, y, offset;
    struct fb_var_screeninfo vinfo;
    unsigned short *pfb;

    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    if(end_x == 0) end_x = vinfo.xres_virtual;
    if(end_y == 0) end_y = vinfo.yres_virtual;
    pfb = (unsigned short *)mmap(0, vinfo.xres_virtual*vinfo.yres_virtual*2, PROT_READ | PROT_WRITE,
                                                     MAP_SHARED, fd, 0);

    for(x = start_x; x < end_x; x++) {
        for(y = start_y; y < end_y; y++) {
            *(pfb + x + y * vinfo.xres_virtual) = color;                  //            pfb[x + y * vinfo.xres] = color;
        };
    };

    munmap(pfb, vinfo.xres_virtual*vinfo.yres_virtual*2);

    return 0;
}

int main(int argc, char** argv)
{
    int fbfd, status, offset;
    unsigned short pixel;

    fbfd = open(FBDEVICE, O_RDWR);  
    if(fbfd < 0) {
        perror("Error: cannot open framebuffer device");
        return -1;
    }

//    DrawFace(fbfd, 0, 0, 0, 0, makepixel(255, 255, 0)) ;         /* Yellow????????? ?????? */ 
    DrawFaceMMAP(fbfd, 0, 0, 0, 0, makepixel(255, 255, 0)) ;         /* Yellow????????? ?????? */ 

    DrawPoint(fbfd, 50, 50, makepixel(255, 0, 0));            /*  Red ?????? ?????? */
    DrawPoint(fbfd, 100, 100, makepixel(0, 255, 0));        /*  Green ?????? ?????? */
    DrawPoint(fbfd, 150, 150, makepixel(0, 0, 255));        /*  Blue ?????? ?????? */

    DrawLine(fbfd, 0, 100, 200, makepixel(0, 255, 255)) ;          /* Cyan ????????? ?????? */ 

    DrawCircle(fbfd, 200, 200, 100, makepixel(255, 0, 255)) ;         /* Magenta????????? ?????? */ 

    close(fbfd);                                           /* ????????? ?????? ????????????
??? ????????? ?????????. */

    return 0;
}

