#ifndef CorrespondenceListItem_H
#define CorrespondenceListItem_H

#include <QString>
#include <QListWidgetItem>

#include "Correspondence.h"

using namespace std;

class CorrespondenceListItem : public QListWidgetItem {
public:
    CorrespondenceListItem(const QString &text, Correspondence* correspondence, QListWidget *view = 0, int type = Type) : QListWidgetItem(text, view, type), correspondence_(correspondence) {};
    
    Correspondence* getCorrespondence() {
        return correspondence_;
    }
private:
    Correspondence* correspondence_;
};

#endif