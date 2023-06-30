#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    int flag;
    bool fshow;
    QString filepath;
    char wndform[1100], recform[1100];
    QString config;
    QFileSystemWatcher watcher;
    QPoint wtl, wbr, rtl, rbr;
    QTimer tm;
    bool fr;

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void readData();
    ~Widget();

public slots:
    void respon(const QString &);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
