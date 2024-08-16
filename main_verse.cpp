#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cstring> 

using namespace std;

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int MAX_ITER = 1000;

struct Color {
    uint8_t r, g, b;

    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0)
        : r(red), g(green), b(blue) {}
};

// Функция для записи BMP файла
void writeBMP(const char* filename, const vector<Color>& data, int width, int height) {
    ofstream file(filename, ios::binary);

    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + width * height * 3 + paddingAmount * height;

    unsigned char fileHeader[fileHeaderSize] = {0};

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = static_cast<unsigned char>(fileSize);
    fileHeader[3] = static_cast<unsigned char>(fileSize >> 8);
    fileHeader[4] = static_cast<unsigned char>(fileSize >> 16);
    fileHeader[5] = static_cast<unsigned char>(fileSize >> 24);
    // Pixel data offset
    fileHeader[10] = fileHeaderSize + infoHeaderSize;

    unsigned char infoHeader[infoHeaderSize] = {0};

    // Header
    infoHeader[0] = infoHeaderSize;
    // Image width
    infoHeader[4] = static_cast<unsigned char>(width);
    infoHeader[5] = static_cast<unsigned char>(width >> 8);
    infoHeader[6] = static_cast<unsigned char>(width >> 16);
    infoHeader[7] = static_cast<unsigned char>(width >> 24);
    // Image height
    infoHeader[8] = static_cast<unsigned char>(height);
    infoHeader[9] = static_cast<unsigned char>(height >> 8);
    infoHeader[10] = static_cast<unsigned char>(height >> 16);
    infoHeader[11] = static_cast<unsigned char>(height >> 24);
    // Planes
    infoHeader[12] = 1;
    // Bits per pixel
    infoHeader[14] = 24;

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    for (int y = 0; y < height; y++) {
        file.write(reinterpret_cast<const char*>(&data[y * width]), width * sizeof(Color));
        file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    file.close();
}

Color getColor(int iterations) {
    if (iterations == MAX_ITER) return { 0, 0, 0 };

    double t = static_cast<double>(iterations) / MAX_ITER;
    uint8_t r = static_cast<uint8_t>(9 * (1 - t) * t * t * t * 255);
    uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
    uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return { r, g, b };
}

int julia(complex<double> z, complex<double> c) {
    int iterations = 0;
    while (abs(z) <= 2 && iterations < MAX_ITER) {
        z = z * z + c;
        iterations++;
    }
    return iterations;
}

int main() {
    vector<Color> data(WIDTH * HEIGHT);
    memset(data.data(), 0, WIDTH * HEIGHT * sizeof(Color));

    complex<double> c(0.355, 0.355); // const 

    double min_re = -1.5;
    double max_re = 1.5;
    double min_im = -1.5;
    double max_im = min_im + (max_re - min_re) * HEIGHT / WIDTH;

    double re_factor = (max_re - min_re) / (WIDTH - 1);
    double im_factor = (max_im - min_im) / (HEIGHT - 1);

    for (int y = 0; y < HEIGHT; y++) {
        double z_im = max_im - y * im_factor;
        for (int x = 0; x < WIDTH; x++) {
            double z_re = min_re + x * re_factor;
            complex<double> z(z_re, z_im);
            int iterations = julia(z, c);
            data[y * WIDTH + x] = getColor(iterations);
        }
    }

    writeBMP("source/Julia2.bmp", data, WIDTH, HEIGHT);

    cout << "julia set with c = 0.355 + 0.355 generated and saved to Julia2.bmp" << endl;

    return 0;
}
