#include <QApplication>
#include <GUI/MainWindow.h>
#include <Tests/TestSuite.h>
#include <QtTest/qtestcase.h>

int main(int argc, char *argv[])
{
    auto suite = Tests::TestSuite::suite();

    for(const auto &i : suite)
    {
       // if(i.first == "Test_Basic_Save_Load")
          //  QTest::qExec(i.second);
    }

    QApplication app{argc, argv};

    GUI::MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
