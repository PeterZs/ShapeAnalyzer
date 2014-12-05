///
/// \brief Main class containt the main function. Starts the ShapeAnalyzer-App.
///

#include <exception>

#include <QApplication>
#include <QErrorMessage>

#include "ShapeAnalyzer.h"

int main( int argc, char** argv ) {
    // QT Stuff
    QApplication app( argc, argv );
    
    try {
        ShapeAnalyzer shapeAnalyzer;
        shapeAnalyzer.show();
        return app.exec();
    } catch(exception e) {
        QErrorMessage msgBox;
        msgBox.showMessage(QString::fromStdString(string(e.what())));
        msgBox.exec();
        exit(-1);
    }
}
