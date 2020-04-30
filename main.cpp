#include <QApplication>
#include <GUI/MainWindow.h>

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    GUI::MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
