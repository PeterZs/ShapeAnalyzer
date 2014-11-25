///
/// \brief Starts the ShapeAnalyzer-App.
///

#include <exception>

#include <QApplication>

#include "ShapeAnalyzer.h"

int main( int argc, char** argv ) {
    // QT Stuff
    try {
        QApplication app( argc, argv );
  
        ShapeAnalyzer shapeAnalyzer;
        shapeAnalyzer.show();
  
        return app.exec();
    } catch(exception e) {
        cout << "Exception: " << e.what() <<endl;
        exit(-1);
    }
}
