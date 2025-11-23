#include "ImageThread.h"

#include <utility>

static std::vector<ImageThread *> threads;

ImageThread::ImageThread()
{
}

ImageThread::~ImageThread()
{
}

void ImageThread::DealImage(unsigned char *img, unsigned int size, float bright, int threadCount)
{
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->wait();
        delete threads[i];
    }

    threads.clear();
    int tsize = size / threadCount;
    for (int i = 0; i < threadCount; i++)
    {
        ImageThread *th = new ImageThread();
        th->begin       = i * tsize;
        th->end         = th->begin + tsize;
        th->img         = img;
        th->bright      = bright;
        if (i == threadCount - 1)
        {
            th->end = size - 1;
        }
        threads.push_back(th);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->start();
    }
}

int ImageThread::DealPercent()
{
    int cur   = 0;
    int total = 0;
    for (int i = 0; i < threads.size(); i++)
    {
        cur += (threads[i]->cur - threads[i]->begin);
        total += (threads[i]->end - threads[i]->begin);
    }
    if (total == 0)
        return 0;
    int per = cur * 100 / total;
    return per;
}

void ImageThread::Wait()
{
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->wait();
    }
}

void ImageThread::run()
{
    if (!img || begin < 0 || end <= 0 || end - begin <= 0)
    {
        return;
    }

    for (int i = begin; std::cmp_less_equal(i, end); i++)
    {
        /// 当前像素在数据数组中的起始位置是 i * 4
        /// 因为每个像素占4个字节：B, G, R, A (可能是BGRA格式)

        /// 提取并调整蓝色通道亮度
        /// data[i * 4] 是蓝色分量，乘以亮度系数b
        int B = img[i * 4] * bright;

        /// 提取并调整绿色通道亮度
        /// data[i * 4 + 1] 是绿色分量
        int G = img[i * 4 + 1] * bright;

        /// 提取并调整红色通道亮度
        /// data[i * 4 + 2] 是红色分量
        int R = img[i * 4 + 2] * bright;

        /// 限制RGB值在有效范围内(0-255)
        /// 防止亮度调整后值溢出（超过255会截断为255）
        B = std::clamp(B, 0, 255);
        G = std::clamp(G, 0, 255);
        R = std::clamp(R, 0, 255);

        /// 将处理后的值写回图像数据
        img[i * 4]     = B; /// 更新蓝色通道
        img[i * 4 + 1] = G; /// 更新绿色通道
        img[i * 4 + 2] = R; /// 更新红色通道
        /// 注意：Alpha通道(data[i * 4 + 3])保持不变，保持透明度
        cur = i;
    }
}
