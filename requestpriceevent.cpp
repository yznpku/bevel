#include "requestpriceevent.hpp"

QEvent::Type RequestPriceEvent::RequestPriceEventType = QEvent::None;

RequestPriceEvent::RequestPriceEvent()
  : QEvent(RequestPriceEvent::type())
{
}

QEvent::Type RequestPriceEvent::type() const
{
  if (!RequestPriceEventType)
    RequestPriceEventType = static_cast<QEvent::Type>(registerEventType());
  return RequestPriceEventType;
}
