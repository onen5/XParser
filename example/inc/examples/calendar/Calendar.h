/*
 *
 */

#ifndef __EXAMPLES_CALENDAR_CALENDAR__
#define __EXAMPLES_CALENDAR_CALENDAR__

// system includes
#include <memory>
#include <string>
#include <vector>

// base object includes
#include "examples/BaseInterface.h"

// project sub directory includes - alphabetical order
#include "examples/calendar/Weekday.h"

// project includes - alphabetical order

namespace examples::calendar {

class Calendar: public BaseInterface {
  public:
    Calendar();
    virtual ~Calendar();

    const std::string& getName() const;
    const std::string& getDescription() const;

    const std::vector< std::unique_ptr< Weekday > >& getWeekdays() const;

    void setName( const std::string& name );
    void setDescription( const std::string& desc );

    Weekday* addWeekday();

  private:
    std::string mName;
    std::string mDescription;

    std::vector< std::unique_ptr< Weekday > > mWeekdays;
};

}  // namespace examples::calendar

#endif /* __EXAMPLES_CALENDAR_CALENDAR__ */
