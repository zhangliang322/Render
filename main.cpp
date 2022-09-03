#include "tgaimage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
//draw a line ,������� 0.01������
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
#pragma region ������
    bool steep = false;//���
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {//line �߶ȴ��ڿ��
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // �����������˳�򲻶ԣ����ҷ�ת����֤������
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
#pragma endregion

#pragma region ����
    int dx = x1 - x0;
    int dy = y1 - y0;
    //Ԥ������ؼ�����б�ʣ������Ż�������for���ظ�����
    float derror = std::abs(dy / float(dx));
    float error = 0;
    int y = y0;
    
    //��Ϊ������x��y����������ұ����������򼴿�
    //�����������������������ʵ���ǵ����ʽ��
    for (int x = x0; x <= x1; x++)
    {
 
        if (steep) {
            image.set(y, x, color);//��תx,y
        }
        else{
            image.set(x, y, color);
        }
        //�𵴷�ʽ������,�Ż��������̣���Ϊ����Ҳ��ȡ����
        error += derror;
        if (error > .5) {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.;
        }

    }
#pragma endregion

    
}
int main(int argc, char** argv) {
    //�ļ�����
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);//������ȷִ�У��Գ��������޸�
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("line1-3.tga");
    return 0;
}