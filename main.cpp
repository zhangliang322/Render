#include <vector>
#include <cmath>
#include <iostream> 
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
//注意向量指针的传递值
//返回三角形内所有坐标的值
Vec3f barycentric(Vec2i* pts, Vec2i P) {
    Vec3f u = Vec3f
       (pts[2].x - pts[0].x, 
        pts[1].x - pts[0].x,
        pts[0].x - P.x)
        ^ Vec3f(pts[2].y - pts[0].y,
                pts[1].y - pts[0].y,
                pts[0].y - P.y);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}
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
//绘制三角形，优化方式：先找到包围的范围，然后在包围范围内绘制，if(inside)
//用重心的描述方式进行绘制，uv就可以表示内部的所有点
void triangle(Vec2i* pts, TGAImage& image, TGAColor color) {
    Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
    Vec2i bboxmax(0, 0);//边界范围
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

    //边界范围内所有点的绘制
    for (int i = 0; i < 3; i++) {
        bboxmin.x = std::max(0, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, pts[i].y));

        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
    }
    Vec2i P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts, P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            image.set(P.x, P.y, color);
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

   
    TGAImage frame(200, 200, TGAImage::RGB);
    Vec2i pts[3] = { Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160) };
    triangle(pts, frame, red);
#pragma region 文件输出
    //文件输出
    frame.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    frame.write_tga_file("framebuffer.tga");
    
#pragma endregion

    
    return 0;
}