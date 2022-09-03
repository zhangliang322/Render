#include "tgaimage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
//draw a line ,������� 0.01������
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    for (int x=x0; x <=x1; x++)
    {
        float t = (x - x0) / (float)(x1 - x0);//����������
        int y = y0 * (1. - t) + y1 * t;
        image.set(x, y, color);

    }
}
int main(int argc, char** argv) {
    //�ļ�����
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);//����ȱʧ����line1���ظ���һ���ߣ�˵���жԳ�������
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("line1-2.tga");
    return 0;
}