#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model* model = NULL;
const int width = 200;
const int height = 200;
//draw a line ,逐个顶点 0.01连成线
void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color) {
#pragma region 输入检查
    bool steep = false;//标记
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {//line 高度大于宽度
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x > p1.x) { // 如果输入坐标顺序不对，左右反转，保证从左到右
        std::swap(p0, p1);
    }
#pragma endregion

#pragma region 绘制
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    //预先算出关键参数斜率，进行优化，避免for中重复计算
    //优化掉浮点数，直接和x比较
    int derror2 = std::abs(dy) * 2;
    float error2 = 0;
    int y = p0.y;
    
    //因为限制了x比y长，因此在右边三角形区域即可
    //如果符合增长条件就上升（实际是点的形式）
    
    if (steep) {//把陡峭判断从for循环中拆出来进行优化，双倍代码 双倍速度
        for (int x = p0.x; x <= p1.x; ++x) {
            image.set(y, x, color);
            error2 += derror2;
            if (error2 > dx) {
                y += (p1.y > p0.y ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    }
    else {
        for (int x = p0.x; x <= p1.x; ++x) {
            image.set(x, y, color);
            error2 += derror2;
            //震荡方式画像素,优化增长过程（因为本来也是取整）
            if (error2 > dx) {//优化掉浮点数，直接2y和x比较
                y += (p1.y > p0.y ? 1 : -1);
                error2 -= dx * 2;//同样更改震荡方式
            }
        }
    }
#pragma endregion
    }
//绘制三角形
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
    //t0是最低点，t2是最高点（按照y坐标排），t0-t2是边界点
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    //绘制另一边界的下半部分，按y绘制
    int total_height = t2.y - t0.y;
    /*合并写法
    //三角形下半部分（按中间点切割为界限）
    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;//按照中间那段的高度切分
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero 
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t0 + (t1 - t0) * beta;//从底部绘制到切分线
        //确保顺序
        //扫描线，逐行填充x
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
        }
    }
    //三角形上半部分
    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height = t2.y - t1.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t1.y) / segment_height; // be careful with divisions by zero 
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t1 + (t2 - t1) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
        }
    }
    */

    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y 
        }
    }
}
int main(int argc, char** argv) {
#pragma region 模型读入
    ////文件绘制
   //if (2 == argc) {
   //    model = new Model(argv[1]);
   //}
   //else {
   //    model = new Model("obj/african_head.obj");
   //}
   ////读取二维图像顶点
#pragma endregion

   
    TGAImage image(width, height, TGAImage::RGB);
    Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
    Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
    Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);
#pragma region 文件输出
    //文件输出
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("trangle4.tga");
    
#pragma endregion

    
    return 0;
}