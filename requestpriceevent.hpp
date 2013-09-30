#ifndef REQUESTPRICEEVENT_HPP
#define REQUESTPRICEEVENT_HPP

#include <QEvent>

class RequestPriceEvent : public QEvent
{
public:
  static Type RequestPriceEventType;
public:
  RequestPriceEvent();
  Type type() const;
};

#endif // REQUESTPRICEEVENT_HPP
