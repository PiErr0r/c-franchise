#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define UNUSED(x) ((void)(x))

#define GET_VALUE(data, y, x, X, n, N) (*((data) + (y)*(X)*(N) + (x)*(N) + (n)))
#define GET_INDEX(y, x, X, n, N) ((y)*(X)*(N) + (x)*(N) + (n))

#define INTENSITY " .'^*&@#"

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

unsigned int get_datum(int i, int j, unsigned char* data, int x, int y, int n, const int cell_w, const int cell_h) {
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

void resize_data(unsigned char* old, int x, int y, int n, unsigned int* new, int w, int h) {
    const int cell_w = x / w;
    const int cell_h = y / h;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            unsigned int datum = get_datum(i, j, old, x, y, n, cell_w, cell_h);
            *(new + i * w + j) = datum;
        }
    }
}

char get_intensity(unsigned int v) {
    static const int DIV = 256 / 8;
    return INTENSITY[7-(v/DIV)];
}

void print_ascii(unsigned int* data, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            char c = get_intensity(*(data + i * w + j));
            printf("%c", c);
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

    return 0;
}
