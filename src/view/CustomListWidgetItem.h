#ifndef ShapeAnalyzer_CustomListWidgetItem_h
#define ShapeAnalyzer_CustomListWidgetItem_h

#include <QString>
#include <QListWidgetItem>

///
/// General QListWidgetItem that can contain a pointer to an
/// object of class T.
///
/// @author Emanuel Laude and Zorah Lähner
///
///
template<class T>
class CustomListWidgetItem : public QListWidgetItem {
public:
    /// \brief Constructor.
    /// \details Creates a new QListWidgetItem with a pointer to item.
    /// @param text name that should be displayed in the QListWidget
    /// @param item pointer to the item this list item is referring to.
    /// @param view optional pointer to the ListWidget
    /// @param type see reference of QListWidgetItem for details
    CustomListWidgetItem(
                  const QString &text,
                  T*            item,
                  QListWidget   *view   = 0,
                  int           type    = Type
                  ) : QListWidgetItem(text, view, type), item_(item) {};
    
    /// Returns a pointer to the referenced item.
    T* getItem() {
        return item_;
    }
private:
    T* item_;
};

#endif