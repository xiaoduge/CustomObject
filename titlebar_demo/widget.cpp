#include "widget.h"
#include "ui_widget.h"
#include "dtitlebar.h"
#include "dframeless.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUI();
}

Widget::~Widget()
{
    qDebug() << "~Widget()";
    delete ui;
}

void Widget::initUI()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    m_pTitleBar = new DTitleBar(this);
    m_pTitleBar->move(0, 0);
    m_pTitleBar->showTitleIcon(false);
    m_pTitleBar->setTitleFlags(DTitleBar::AllButtonShow);
//    m_pTitleBar->setBackgroundColor(Qt::gray);

    DFrameless *frameless = new DFrameless(this);
    frameless->setWidget(this);
}
