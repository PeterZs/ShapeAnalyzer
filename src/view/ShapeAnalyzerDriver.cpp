///
/// \brief Main class containt the main function. Starts the ShapeAnalyzer-App.
///

#include <exception>

#include <QApplication>
#include <QErrorMessage>
#include <QMessageBox>

#include "ShapeAnalyzer.h"

class MyApplication : public QApplication
{
public:
    MyApplication(int argc, char** argv) : QApplication(argc, argv) {}
    // ~MyApplication();
private:
    bool notify(QObject *receiver, QEvent *event) {
        try {
            return QApplication::notify(receiver, event);
        } catch (std::exception &e) {
            QMessageBox::critical( NULL, "Exception",
                                  ("Error sending event %s to object %s (%s)\nAn exception of type \"%s\" has been thrown: %s", typeid(*event).name(),
                                  qPrintable(receiver->objectName()), typeid(*receiver).name(), typeid(e).name(), e.what()));
        } catch (...) {
            QMessageBox::critical( NULL, "Exception",
                                  ("Error <unknown> sending event %s to object %s (%s)", typeid(*event).name(), qPrintable(receiver->objectName()),
                                   typeid(*receiver).name()));
        }
        
        // qFatal aborts, so this isn't really necessary
        // but you might continue if you use a different logging lib
        return false;
    }
};

int main( int argc, char** argv ) {
    // QT Stuff
    try {
        MyApplication app(argc, argv);
        
        ShapeAnalyzer shapeAnalyzer;
        shapeAnalyzer.show();
        return app.exec();
    } catch(...) {
        QMessageBox::critical( NULL, "Exception",
                              "Error <unknown> has occured\n");
    }
}



