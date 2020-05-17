#include <QApplication>
#include <GUI/MainWindow.h>
#include <Tests/TestSuite.h>
#include <QtTest/qtestcase.h>

int main(int argc, char *argv[])
{
    auto testSuite = Tests::TestSuite::suite();

    // Only run the tests if they were specified in the program arguments.
    // Note: index 0 is the program name; it is not checked if it corresponds to a test.
    for(int i = 1; i < argc; ++i)
    {
        for(auto &test: testSuite)
        {
            if(test.first == argv[i])
            {
                QTest::qExec(test.second);
            }
        }
    }

    QApplication app{argc, argv};

    GUI::MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
