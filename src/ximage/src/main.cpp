#include <XThread.h> /// 自定义线程库头文件
#include <windows.h> /// Windows API 头文件
#include <iostream>  /// C++ 标准输入输出

//////////////////////////////////////////////////////////////////
/// 全局变量和线程类定义

static char *arr = NULL; /// 全局字符数组指针，用于存储数据

/**
 * @brief 自定义线程类，继承自 XThread
 * 用于并行处理数组的赋值操作
 */
class MyThread : public XThread
{
public:
    int b = 0; ///<  线程处理的起始索引
    int e = 0; ///<  线程处理的结束索引

    /**
     * @brief 线程主函数，重写 XThread 的虚函数
     * 在线程启动时自动调用，执行实际的数组赋值工作
     */
    void Main()
    {
        /// 遍历分配的数组区间，进行赋值操作
        for (int i = b; i <= e; i++)
        {
            arr[i] = i; /// 将数组元素值设为其索引值
            /// 注意：这里会发生 int 到 char 的隐式转换，只保留低8位
        }
    }
};

//////////////////////////////////////////////////////////////////

#define USE_THREAD 1 /// 编译开关：1使用多线程，0使用单线程

/**
 * @brief 主函数
 * 测试多线程与单线程处理大规模数组赋值的性能差异
 */
int main(int argc, char *argv[])
{
    /// 分配 1GB 的内存空间 (1000 * 1000 * 1000 bytes = 约 1GB)
    int size = 1000 * 1000 * 1000;
    arr      = new char[size]; /// 动态分配数组内存

    /// 记录开始时间（Windows API，单位：毫秒）
    long long start = GetTickCount();

/// 根据 USE_THREAD 宏选择执行路径
#if USE_THREAD
    /// 多线程处理路径
    int        threadCount = 16;                          /// 使用的线程数量
    MyThread **ths         = new MyThread *[threadCount]; /// 线程指针数组

    /// 创建并配置所有线程
    for (int i = 0; i < threadCount; i++)
    {
        ths[i] = new MyThread(); /// 创建线程对象

        /// 计算该线程负责的数组区间
        ths[i]->b = (size / threadCount) * i;           /// 起始索引
        ths[i]->e = (size / threadCount) * (i + 1) - 1; /// 结束索引

        /// 处理最后一个线程的边界情况，确保覆盖整个数组
        if (i == threadCount - 1)
        {
            ths[i]->e = size - 1; /// 最后一个线程处理到数组末尾
        }

        ths[i]->Start(); /// 启动线程
    }

    /// 等待所有线程完成
    for (int i = 0; i < threadCount; i++)
    {
        ths[i]->Wait(); /// 等待线程执行完毕
        delete ths[i];  /// 释放线程对象内存
    }
    delete[] ths; /// 释放线程指针数组内存

    ///<<< 156ms
#else
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
    /// <<< 注释显示单线程版本耗时约 813ms
#endif

    /// 记录结束时间并计算耗时
    long long end = GetTickCount();

    /// 输出处理耗时
    std::cout << "main deal ms = " << end - start << std::endl;

    /// 等待用户输入，防止程序立即退出（便于查看结果）
    getchar();
    return 0;
}
