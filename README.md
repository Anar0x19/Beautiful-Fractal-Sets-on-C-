# Beautiful-Fractal-Sets-on-Cpp
`#include <iostream>
#include <fstream>
#include <complex>
#include <vector>`

using namespace std;
Подключаются стандартные библиотеки: 
iostream
 для ввода/вывода, 
fstream 
для работы с файлами, 
complex 
для работы с комплексными числами и 
vector 
для работы с динамическими массивами.
юзается пространство имён std для удобства.


`const int WIDTH = 1024;
const int HEIGHT = 1024;
const int MAX_ITER = 1000;`

здесь думаю все понятно скажу лишь  пару слов об константе 
`MAX_ITER`
она задает максимальное количество итераций для алгоритма (MAX_ITER).

`struct Color {
    unsigned char r, g, b;
};`

Определяется структура 
Color
представляющая цвет с тремя компонентами: красный (r), зелёный (g) и синий (b).

`void writeBMP(const char* filename, const vector<Color>& data, int width, int height) {
    ofstream file(filename, ios::binary);`
Объявляется функция 
writeBMP 
для записи изображения в формате .bmp
Открывается бинарный файл для записи.
   
 `unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (width * 3) % 4) % 4);`
Определяется массив для добавления отступов (bmpPad) и рассчитывается количество байтов, необходимых для выравнивания строк изображения до 4 байт (paddingAmount).

    
`const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + width * height * 3 + paddingAmount * height;`
Определяются размеры заголовков файла и информации и рассчитывается общий размер файла.

    
`unsigned char fileHeader[fileHeaderSize];

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // Reserved one
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // Reserved tu
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel data offset
    fileHeader[10] = fileHeaderSize + infoHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;`
Заполняется заголовок файла (fileHeader) для BMP формата.
    
unsigned char infoHeader[infoHeaderSize];

    // Header 
    infoHeader[0] = infoHeaderSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;
    // Image width
    infoHeader[4] = width;
    infoHeader[5] = width >> 8;
    infoHeader[6] = width >> 16;
    infoHeader[7] = width >> 24;
    // Image height
    infoHeader[8] = height;
    infoHeader[9] = height >> 8;
    infoHeader[10] = height >> 16;
    infoHeader[11] = height >> 24;
    // Planes
    infoHeader[12] = 1;
    infoHeader[13] = 0;
    // Bits per pixel
    infoHeader[14] = 24;
    infoHeader[15] = 0;
    // Compression
    infoHeader[16] = 0;
    infoHeader[17] = 0;
    infoHeader[18] = 0;
    infoHeader[19] = 0;
    // Image size (no compression)
    infoHeader[20] = 0;
    infoHeader[21] = 0;
    infoHeader[22] = 0;
    infoHeader[23] = 0;
    // X pixels per meter
    infoHeader[24] = 0;
    infoHeader[25] = 0;
    infoHeader[26] = 0;
    infoHeader[27] = 0;
    // Y pixels per meter
    infoHeader[28] = 0;
    infoHeader[29] = 0;
    infoHeader[30] = 0;
    infoHeader[31] = 0;
    // Total colors
    infoHeader[32] = 0;
    infoHeader[33] = 0;
    infoHeader[34] = 0;
    infoHeader[35] = 0;
    // important colors
    infoHeader[36] = 0;
    infoHeader[37] = 0;
    infoHeader[38] = 0;
    infoHeader[39] = 0;
Тут самое сложное заголовок информации (infoHeader) для BMP формата.
    
file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);
Заголовки записываются в файл.

    
for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Color& pixel = data[y * width + x];
            unsigned char color[] = { pixel.b, pixel.g, pixel.r };
            file.write(reinterpret_cast<char*>(color), 3);
        }
        file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    file.close();
}
Пиксели изображения записываются в файл построчно. Каждая строка дополняется необходимым количеством байтов для выравнивания.

сolor getColor(int iterations) {
    if (iterations == MAX_ITER) return { 0, 0, 0 }; 
 double t = (double)iterations / MAX_ITER;
    unsigned char r = (unsigned char)(9 * (1 - t) * t * t * t * 255);
    unsigned char g = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255);
    unsigned char b = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return { r, g, b };
}
Функция 
getColor 
вычисляет цвет пикселя в зависимости от количества итераций до достижения предела (MAX_ITER). Цвет определяется с использованием градиента.

i
nt julia(complex<double> z, complex<double> c) {
    int iterations = 0;
    while (abs(z) <= 2 && iterations < MAX_ITER) {
        z = z * z + c;
        iterations++;
    }
    return iterations;
}

Функция julia(функция названа так т.к реализует фрактальное множество Жулиа) вычисляет количество итераций для комплексного числа z в соответствии со знаменитой формулой z = z*z + c. Итерации продолжаются до тех пор, пока модуль z не превысит 2 или не будет достигнут максимальный предел итераций.

int main() {
    vector<Color> data(WIDTH * HEIGHT);

    complex<double> c(0.355, 0.355); // Julia constant
Создаётся вектор data для хранения цветов пикселей изображения.
Задаётся самое главное во всей программе - константа Жюлиа c.

    
double min_re = -1.5;
    double max_re = 1.5;
    double min_im = -1.5;
    double max_im = min_im + (max_re - min_re) * HEIGHT / WIDTH;

    double re_factor = (max_re - min_re) / (WIDTH - 1);
    double im_factor = (max_im - min_im) / (HEIGHT - 1);
Определяются границы области комплексной плоскости, которая будет визуализироваться.
Вычисляются коэффициенты масштабирования для действительной и мнимой частей.

    
`for (int y = 0; y < HEIGHT; y++) {
        double z_im = max_im - y * im_factor;
        for (int x = 0; x < WIDTH; x++) {
            double z_re = min_re + x * re_factor;
            complex<double> z(z_re, z_im);
            int iterations = julia(z, c);
            data[y * WIDTH + x] = getColor(iterations);
        }
    }`
Для каждого пикселя изображения вычисляется соответствующее комплексное число z.
Для каждого z вызывается функция julia, которая возвращает количество итераций.
Полученное количество итераций используется для определения цвета пикселя с помощью функции getColor.

`writeBMP("source/Julia2.bmp", data, WIDTH, HEIGHT);

    cout << "julia set with c = 0.355 + 0.355 generated and saved to Julia2.bmp" << endl;

    return 0;
}`
Результат записывается в BMP файл с помощью функции 
writeBMP
Сообщение о завершении работы выводится на консоль.

Также я с недавнего времени я добавил файлы main_verse.cpp и main_verse1.cpp где вместо заполнение массива поэлементно вручную юзал POD структуру. опять-же, инициализировать туеву хучу однобайтных элементов 0-ми... поэтому использовал memset.
`struct { uchar r,g,b; }; — сделал cout « sizeof(Color);`
`reinterpret_cast<char*>( int ) — это вообще опасно. так что little/big endian
`
ну и самое прикольное, что там было 3 однобайтника r, g, b, чтобы их запихнуть в структуру, которую вернуть по значению, потом по значению запихать с копированием в вектор, откуда потом вынуть 3 однобайтника, сложить в массив и отдать по указателю в функцию. это просто ооочень много лишней работы как на этапу программирования, так и в рантайме
