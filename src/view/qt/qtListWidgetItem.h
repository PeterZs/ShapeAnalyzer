//
//  qtListWidgetItem.h
//  ShapeAnalyzer
//
//  General QListWidgetItem that can contain a pointer to an
//  object of class T and a name, that can be changed
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef ShapeAnalyzer_qtListWidgetItem_h
#define ShapeAnalyzer_qtListWidgetItem_h

#include <QString>
#include <QListWidgetItem>

template<class T>
class qtListWidgetItem : public QListWidgetItem {
public:
    qtListWidgetItem(
                  const QString               &text,
                  T*   item,
                  QListWidget                 *view   = 0,
                  int                         type    = Type
                  ) : QListWidgetItem(text, view, type), item_(item) {};
    
    T* getItem() {
        return item_;
    }
private:
    T* item_;
};

#endif
