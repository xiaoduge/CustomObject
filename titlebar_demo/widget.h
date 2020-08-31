#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class DTitleBar;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initUI();

private:
    Ui::Widget *ui;
    DTitleBar *m_pTitleBar;
};

#endif // WIDGET_H
