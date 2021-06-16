#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <utility>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dataReady(std::map<QString, std::vector<double>>* data);

private:
    Ui::MainWindow *ui;

    void TableWidgetDisplay();
};

#endif // MAINWINDOW_H
