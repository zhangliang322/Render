#include "tgaimage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
//draw a line ,逐个顶点 0.01连成线
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
#pragma region 输入检查
    bool steep = false;//标记
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {//line 高度大于宽度
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // 如果输入坐标顺序不对，左右反转，保证从左到右
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
#pragma endregion

#pragma region 绘制
    int dx = x1 - x0;
    int dy = y1 - y0;
    //预先算出关键参数斜率，进行优化，避免for中重复计算
    float derror = std::abs(dy / float(dx));
    float error = 0;
    int y = y0;
    
    //因为限制了x比y长，因此在右边三角形区域即可
    //如果符合增长条件就上升（实际是点的形式）
    for (int x = x0; x <= x1; x++)
    {
 
        if (steep) {
            image.set(y, x, color);//反转x,y
        }
        else{
            image.set(x, y, color);
        }
        //震荡方式画像素,优化增长过程（因为本来也是取整）
        error += derror;
        if (error > .5) {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.;
        }

    }
#pragma endregion

    
}
int main(int argc, char** argv) {
    //文件绘制
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);//红线正确执行，对称性输入修复
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("line1-3.tga");
    return 0;
}