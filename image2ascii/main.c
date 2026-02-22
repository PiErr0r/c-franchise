#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define UNUSED(x) ((void)(x))

#define GET_VALUE(data, y, x, X, n, N) (*((data) + (y)*(X)*(N) + (x)*(N) + (n)))
#define GET_INDEX(y, x, X, n, N) ((y)*(X)*(N) + (x)*(N) + (n))

#define INTENSITY " .'^*&@#"

#define P_BLACK()   printf("\033[30m")
#define P_RED()     printf("\033[31m")
#define P_GREEN()   printf("\033[32m")
#define P_YELLOW()  printf("\033[33m")
#define P_BLUE()    printf("\033[34m")
#define P_MAGENTA() printf("\033[35m")
#define P_CYAN()    printf("\033[36m")
#define P_WHITE()   printf("\033[37m")
#define P_RESET()   printf("\033[m")

/*
BG  FG  Color
30	40	Black
31	41	Red
32	42	Green
33	43	Yellow
34	44	Blue
35	45	Magenta
36	46	Cyan
37	47	White
*/

// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data);
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result


void usage(const char* name) {
    printf("[ERROR] Usage:\n");
    printf("\t%s [path-to-image]\n", name);
}

unsigned int get_intensity(int i, int j, unsigned char* data, int x, int y, int n, const int cell_w, const int cell_h) {
    UNUSED(y);
    unsigned int result = 0;
    for (int ii = 0; ii < cell_h; ++ii) {
        for (int jj = 0; jj < cell_w; ++jj) {
            for (int ch = 0; ch < n; ++ch) {
                result += GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, ch, n);
            }
        }
    }
    return result / (cell_w * cell_h * n);
}

// TODO(get_value);
// TODO(get_lightness);

unsigned int get_hue(int i, int j, unsigned char* data, int x, int y, int n, const int cell_w, const int cell_h) {
    if (n < 3) {
        return get_intensity(i, j, data, x, y, n, cell_w, cell_h);
    }
    unsigned int r = 0, g = 0, b = 0;
    for (int ii = 0; ii < cell_h; ++ii) {
        for (int jj = 0; jj < cell_w; ++jj) {
            r += GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 0, n);
            g += GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 1, n);
            unsigned int bi = GET_INDEX(ii+i*cell_h, jj+j*cell_w, x, 2, n);
            b += (bi < x * y * n) ? GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 2, n) : 0;
        }
    }
    r /= (cell_w * cell_h);
    g /= (cell_w * cell_h);
    b /= (cell_w * cell_h);
    unsigned int M = r > g && r > b ? r : g > b ? g : b;
    unsigned int m = r < g && r < b ? r : g < b ? g : b;
    unsigned int C = M - m;
    if (C == 0) {
        printf("[ERROR] C\n");
        return 0;
    }
    unsigned int Hp = (M == r) ? (((g-b)%256)/C) % 6 : (M == g) ? ((b-r)%256)/C + 2 : ((r-g)%256)/C + 4;
    return 60 * (Hp);
}

void resize_data(unsigned char* old, int x, int y, int n, unsigned int* new, int w, int h) {
    const int cell_w = x / w;
    const int cell_h = y / h;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            unsigned int intensity = get_intensity(i, j, old, x, y, n, cell_w, cell_h);
            *(new + i * w + j) = intensity;
            unsigned int hue = get_hue(i, j, old, x, y, n, cell_w, cell_h);
            *(new + i * w + j) += (hue << 16);
        }
    }
}

char get_intensity_from_value(unsigned int v) {
    static const int DIV = 256 / 8;
    v = v & 0xFFFF;
    return INTENSITY[(v/DIV)];
}

unsigned int get_color_from_value(unsigned int v) {
    v >>= 16;
    return v / 60;
}

int G = 0;
void print_color(unsigned int color) {
    // if (color > 8) printf("[ERROR]\n");
    G |= (1 << color);
    switch (color) {
		case 0: P_BLACK();      break;
		case 1: P_RED();        break;
		case 2: P_GREEN();      break;
		case 3: P_YELLOW();     break;
		case 4: P_BLUE();       break;
		case 5: P_MAGENTA();    break;
		case 6: P_CYAN();       break;
		case 7: P_WHITE();      break;
        default: break;
    }
}

void print_ascii(unsigned int* data, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            unsigned int color = get_color_from_value(*(data + i * w + j));
            print_color(color);
            char c = get_intensity_from_value(*(data + i * w + j));
            printf("%c", c);
            P_RESET();
        }
        printf("\n");
    }
}


int main(int argc, const char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }

    int x, y, n;
    unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);
    printf("%d %d %d\n", x, y, n);
    printf("%d %d\n", GET_VALUE(data, 0, 0, x, 0, n), GET_VALUE(data, 200, 200, x, 0, n));

    int W = 120;
    int H = y * (float)W / (float)x / 2.f;
    unsigned int *new_data = malloc(W * H * sizeof(int));
    resize_data(data, x, y, n, new_data, W, H);
    print_ascii(new_data, W, H);
    for (int i = 31; i >= 0; --i) {
        printf("%d", (G&(1<<i)) ? 1 : 0);
    }
    printf("\n");

    return 0;
}
