#ifndef BLUEPRINTPIXMAPLABEL_HPP
#define BLUEPRINTPIXMAPLABEL_HPP

#include "typepixmaplabel.hpp"

class BlueprintPixmapLabel : public TypePixmapLabel
{
  Q_OBJECT
public:
  explicit BlueprintPixmapLabel(QWidget *parent = 0);
protected:
  virtual int filterTypeId(int typeId) const;
};

#endif // BLUEPRINTPIXMAPLABEL_HPP
