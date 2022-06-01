/*
 *
 */

#ifndef __EXAMPLES_CALENDAR_XPARSER_H__
#define __EXAMPLES_CALENDAR_XPARSER_H__

// system includes
#include <string>
#include <unordered_map>
#include <vector>

// base object includes
#include "xparser/XmlParser.h"

// project sub directory includes - alphabetical order
#include "examples/BaseInterface.h"
#include "examples/calendar/Calendar.h"

// project includes - alphabetical order

namespace examples::calendar {

class CalendarXParser: public xparser::XmlParser {
  public:
    CalendarXParser();
    CalendarXParser( const std::string& );

    void setCalendar( Calendar* );

  private:
    Calendar* mCalPtr;

    std::vector< BaseInterface* > mObjStack;
};

}  // namespace examples::calendar

#endif /* __EXAMPLES_CALENDAR_XPARSER_H__ */
