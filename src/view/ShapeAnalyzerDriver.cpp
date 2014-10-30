///
/// \brief Starts the ShapeAnalyzer-App.
///

#include <QApplication>
#include "ShapeAnalyzer.h"

int main( int argc, char** argv ) {
    // QT Stuff
    QApplication app( argc, argv );
  
    ShapeAnalyzer shapeAnalyzer;
    shapeAnalyzer.show();
  
    return app.exec();
}
