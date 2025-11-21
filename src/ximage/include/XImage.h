/**
 * @file   XImage.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-11-21
 */

#ifndef XIMAGE_H
#define XIMAGE_H

#include <QtWidgets/QWidget>

namespace Ui
{
    class XImageClass;
}

class XImage : public QWidget
{
    Q_OBJECT
public:
    XImage(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~XImage() override;

protected Q_SLOTS:
    auto Open() -> void;
    auto Deal() -> void;

public:
    auto ViewImage(QImage* img) -> void;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;

    Ui::XImageClass* ui_;
};

#endif // XIMAGE_H
