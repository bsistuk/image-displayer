#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    flag = 0;
    fshow = 0;
    fr = 1;
    config = "config.txt";
    setWindowFlag(Qt::FramelessWindowHint, true);
    QFile f(config);
    if (!f.open(QIODevice::ReadOnly)) {
        flag = 1;
        ui->setupUi(this);
        return;
    }
    QTextStream in(&f);
    filepath = in.readLine(1000);
    QFile tb(filepath);
    if (!tb.exists()) {
        flag = 4;
        return;
    }
    QString wnd = in.readLine(1000);
    for (int i = 0; i < wnd.size(); i++)
            wndform[i] = wnd[i].toLatin1();
    wndform[wnd.size()] = 0;
    QString rec = in.readLine(1000);
    for (int i = 0; i < rec.size(); i++)
            recform[i] = rec[i].toLatin1();
    recform[rec.size()] = 0;
    f.close();
    int a, b, c, d;
    if (sscanf(wndform, "%d%d%d%d", &a, &b, &c, &d) != 4) {
        flag = 2;
        ui->setupUi(this);
        return;
    }
    else {
        wtl = QPoint(a, b);
        wbr = QPoint(c, d);
    }
    if (sscanf(recform, "%d%d%d%d", &a, &b, &c, &d) != 4) {
        flag = 3;
        ui->setupUi(this);
        return;
    }
    else {
        rtl = QPoint(a, b);
        rbr = QPoint(c, d);
    }
    qDebug() << wtl << wbr;
    setFixedSize(QSize(wbr.x() - wtl.x(), wbr.y() - wtl.y()));
    move(wtl);
    watcher.addPath(filepath);
    watcher.addPath(config);
    //tm.setInterval(80);
    connect(&watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(respon(const QString &)));
    //QWidget::setAttribute(WA_OpaquePaintEvent, true);
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::readData() {
    //watcher.removePath(filepath);
    if (!QFile::exists(config)) {
        flag = 5;
        return;
    }
    QFile f(config);
    while (!f.open(QIODevice::ReadOnly));
    QTextStream in(&f);
    filepath = in.readLine(1000);
    QFile tb(filepath);
    if (!tb.exists()) {
        flag = 4;
        return;
    }
    QString wnd = in.readLine(1000);
    for (int i = 0; i < wnd.size(); i++)
            wndform[i] = wnd[i].toLatin1();
    wndform[wnd.size()] = 0;
    QString rec = in.readLine(1000);
    for (int i = 0; i < rec.size(); i++)
            recform[i] = rec[i].toLatin1();
    recform[rec.size()] = 0;
    f.close();
    fr = 0;
    int a, b, c, d;
    if (sscanf(wndform, "%d%d%d%d", &a, &b, &c, &d) != 4) {
        flag = 2;
        return;
    }
    else {
        if (wtl != QPoint(a, b) || wbr != QPoint(c, d)) fr = 1;
        wtl = QPoint(a, b);
        wbr = QPoint(c, d);
    }
    if (sscanf(recform, "%d%d%d%d", &a, &b, &c, &d) != 4) {
        flag = 3;
        return;
    }
    else {
        if (rtl != QPoint(a, b) || rbr != QPoint(c, d)) fr = 1;
        rtl = QPoint(a, b);
        rbr = QPoint(c, d);
    }
//    QFile out("out.txt");
//    out.open(QIODevice::Append);
//    char ss[1100];
//    sprintf(ss, "%d %d %d %d\n", wtl.x(), wtl.y(), wbr.x(), wbr.y());
//    out.write(ss);
//    //watcher.addPath(filepath);
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    if (fshow) return;
    if (flag) {
        fshow = 1;
        if (flag == 1) {
            QMessageBox::warning(this, "Error", "Input file does not exist.");
        }
        else if (flag == 5) {
            QMessageBox::warning(this, "Error", "Config data has been removed");
        }
        else if (flag == 4) {
            QMessageBox::warning(this, "Error", "Image file does not exist.");
        }
        else {
            QMessageBox::warning(this, "Error", "Input format error.");
        }
        return;
    }
    if (!QFile::exists(filepath)) {
        fshow = 1;
        QMessageBox::warning(this, "Error", "Image file Removed");
        return;
    }
    fshow = 0;
    QPixmap img;
    while (!img.load(filepath));
    painter.drawPixmap(QRect(0, 0, wbr.x() - wtl.x(), wbr.y() - wtl.y()), img);
    painter.setPen(QPen(Qt::red, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(rtl - wtl, rbr - wtl));

}

void Widget::respon(const QString &) {
    readData();
    if (fr) {
        setFixedSize(QSize(wbr.x() - wtl.x(), wbr.y() - wtl.y()));
        move(wtl);
    }
    update();
}
