#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cstdint>
#include <cstring> 

using namespace std;

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int MAX_ITER = 1000;

#pragma pack(push, 1) // Отключаем выравнивание
struct BMPFileHeader {
    uint16_t fileType{ 0x4D42 };     // BM
    uint32_t fileSize{ 0 };          // Размер файла в байтах
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offsetData{ 0 };        // Смещение к пиксельным данным
};

struct BMPInfoHeader {
    uint32_t size{ 0 };              // Размер заголовка
    int32_t width{ 0 };              // Ширина изображения
    int32_t height{ 0 };             // Высота изображения
    uint16_t planes{ 1 };            // Количество плоскостей (всегда 1)
    uint16_t bitCount{ 24 };         // Бит на пиксель
    uint32_t compression{ 0 };       // Тип сжатия (0 - без сжатия)
    uint32_t sizeImage{ 0 };         // Размер изображения 
    int32_t xPixelsPerMeter{ 0 };    // Горизонтальное разрешение
    int32_t yPixelsPerMeter{ 0 };    // Вертикальное разрешение
    uint32_t colorsUsed{ 0 };        // Количество используемых цветов (0 - все цвета)
    uint32_t colorsImportant{ 0 };   // Важные цвета (0 - все важны)
};
#pragma pack(pop)

struct Color {
    uint8_t r, g, b;

    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0)
        : r(red), g(green), b(blue) {}
};

void writeBMP(const char* filename, const vector<Color>& data, int width, int height) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width * height * 3 + paddingAmount * height;
    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    infoHeader.size = sizeof(BMPInfoHeader);
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.sizeImage = width * height * 3 + paddingAmount * height;

    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    unsigned char bmpPad[3] = {0, 0, 0};

    for (int y = 0; y < height; y++) {
        file.write(reinterpret_cast<const char*>(&data[y * width]), width * sizeof(Color));
        file.write(reinterpret_cast<const char*>(bmpPad), paddingAmount);
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

    cout << "Julia set with c = 0.355 + 0.355i generated and saved to Julia2.bmp" << endl;

    return 0;
}
