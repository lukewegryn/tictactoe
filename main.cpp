#include <ttt.h>
#include <QObject>
#include <QApplication>

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    MainWindow myMainWindow;

    myMainWindow.show();
    return app.exec();   // enters event loop
  }