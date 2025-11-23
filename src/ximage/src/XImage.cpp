#include "XImage.h"
#include "ui_ximage.h"

#include "ImageThread.h"

#include <QtWidgets/QFileDialog>

class XImage::PImpl
{
public:
    PImpl(XImage *owenr);
    ~PImpl() = default;

public:
    XImage *owenr_ = nullptr;
    QImage *img_   = nullptr;
    QImage  imgDeal_;
};

XImage::PImpl::PImpl(XImage *owenr) : owenr_(owenr)
{
}

XImage::XImage(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    impl_ = std::make_unique<PImpl>(this);
    ui_   = new Ui::XImageClass();
    ui_->setupUi(this);
}

XImage::~XImage()
{
}

auto XImage::Open() -> void
{
    QString filename = QFileDialog::getOpenFileName(this, "选择图片文件");
    if (filename.isEmpty())
    {
        return;
    }
    if (!impl_->img_)
    {
        delete impl_->img_;
        impl_->img_ = NULL;
    }
    impl_->img_ = new QImage(filename);
    if (!impl_->img_)
    {
        return;
    }
    ViewImage(impl_->img_);
}


/// @brief 处理图像亮度调整
/// @note 该函数通过调整RGB通道的亮度值来实现图像亮度处理
auto XImage::Deal() -> void
{
    /// 获取亮度调整系数
    /// ui_->bright->value() 返回滑块的值(0-100)，除以100转换为比例系数(0.0-1.0)
    float b      = (float)ui_->bright->value() / 100;
    int   thread = ui_->thread->value();

    /// 安全检查：确保源图像存在
    if (!impl_->img_)
    {
        return; /// 如果源图像为空，直接返回避免崩溃
    }

    /// 创建图像副本用于处理，避免修改原始图像
    /// copy() 创建图像的深拷贝，保证原始图像数据不被修改
    impl_->imgDeal_ = impl_->img_->copy();

    /// 获取处理后图像的像素数据指针
    /// bits() 返回指向图像原始字节数据的指针
    /// 假设图像格式为32位ARGB（每个像素4字节：Alpha, Red, Green, Blue）
    unsigned char *data = impl_->imgDeal_.bits();

    /// 计算图像总像素数
    /// width() * height() 得到图像包含的总像素数量
    int           pixCount = impl_->img_->width() * impl_->img_->height();
    QElapsedTimer t;
    t.start();
    // /// 遍历图像的所有像素进行处理
    // for (int i = 0; i < pixCount; i++)
    // {
    //     /// 当前像素在数据数组中的起始位置是 i * 4
    //     /// 因为每个像素占4个字节：B, G, R, A (可能是BGRA格式)
    //
    //     /// 提取并调整蓝色通道亮度
    //     /// data[i * 4] 是蓝色分量，乘以亮度系数b
    //     int B = data[i * 4] * b;
    //
    //     /// 提取并调整绿色通道亮度
    //     /// data[i * 4 + 1] 是绿色分量
    //     int G = data[i * 4 + 1] * b;
    //
    //     /// 提取并调整红色通道亮度
    //     /// data[i * 4 + 2] 是红色分量
    //     int R = data[i * 4 + 2] * b;
    //
    //     /// 限制RGB值在有效范围内(0-255)
    //     /// 防止亮度调整后值溢出（超过255会截断为255）
    //     B = std::clamp(B, 0, 255);
    //     G = std::clamp(G, 0, 255);
    //     R = std::clamp(R, 0, 255);
    //
    //     /// 将处理后的值写回图像数据
    //     data[i * 4]     = B; /// 更新蓝色通道
    //     data[i * 4 + 1] = G; /// 更新绿色通道
    //     data[i * 4 + 2] = R; /// 更新红色通道
    //     /// 注意：Alpha通道(data[i * 4 + 3])保持不变，保持透明度
    // }

    ImageThread::DealImage(data, pixCount, b, thread);
    for (;;)
    {
        int percent = ImageThread::DealPercent();
        ui_->progress->setValue(percent);
        if (percent >= 100)
        {
            break;
        }
    }
    ImageThread::Wait();

    int  ms       = t.elapsed();
    char buf[128] = { 0 };
    sprintf(buf, "%d毫秒", ms);
    ui_->dealtime->setText(buf);
    /// 显示处理后的图像
    /// ViewImage函数将处理后的图像显示在UI上
    ViewImage(&impl_->imgDeal_);
}
auto XImage::ViewImage(QImage *img) -> void
{
    QImage simg;
    if (img->width() > img->height())
    {
        simg = img->scaledToWidth(ui_->view->width());
    }
    else
    {
        simg = img->scaledToHeight(ui_->view->height());
    }
    ui_->view->setPixmap(QPixmap::fromImage(simg));
}
