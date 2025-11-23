/**
 * @file   ImageThread.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-11-23
 */

#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include <QtCore/QThread>

class ImageThread : public QThread
{
public:
    ImageThread();
    ~ImageThread() override;

public:
    static void DealImage(unsigned char *img, unsigned int size, float bright, int threadCount);
    static int  DealPercent();
    static void Wait();
    void        run() override;

public:
    unsigned int   begin  = 0;
    unsigned int   end    = 0;
    unsigned int   cur    = 0;
    unsigned char *img    = nullptr;
    float          bright = 1;
};


#endif // IMAGETHREAD_H
