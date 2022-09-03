#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;
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
    //�Ż�����������ֱ�Ӻ�x�Ƚ�
    int derror2 = std::abs(dy) * 2;
    float error2 = 0;
    int y = y0;
    
    //��Ϊ������x��y����������ұ����������򼴿�
    //�����������������������ʵ���ǵ����ʽ��
    
    if (steep) {//�Ѷ����жϴ�forѭ���в���������Ż���˫������ ˫���ٶ�
        for (int x = x0; x <= x1; ++x) {
            image.set(y, x, color);
            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    }
    else {
        for (int x = x0; x <= x1; ++x) {
            image.set(x, y, color);
            error2 += derror2;
            //�𵴷�ʽ������,�Ż��������̣���Ϊ����Ҳ��ȡ����
            if (error2 > dx) {//�Ż�����������ֱ��2y��x�Ƚ�
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;//ͬ�������𵴷�ʽ
            }
        }
    }
#pragma endregion
    }

int main(int argc, char** argv) {
    //�ļ�����
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
    }
    //��ȡ��άͼ�񶥵�
    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            //��ȡ��άͼ�񶥵�
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            line(x0, y0, x1, y1, image, white);
            //ֻ��ȡ����ͼv3������
        }
    }
#pragma region �ļ����
    //�ļ����
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("model.tga");
    delete model;
#pragma endregion

    
    return 0;
}