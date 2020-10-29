/*
 *
 */

#ifndef __EXAMPLES_CALENDAR_WEEKDAY__
#define __EXAMPLES_CALENDAR_WEEKDAY__

// system includes
#include <memory>
#include <vector>

// base object includes
#include "examples/BaseInterface.h"

// project sub directory includes - alphabetical order
#include "examples/BaseInterface.h"
#include "examples/calendar/WorkingTime.h"

// project includes - alphabetical order


namespace examples { namespace calendar {

class Weekday : public BaseInterface {

public:
    Weekday();
    virtual ~Weekday();

    uint8_t getDayType() const;
    const std::vector< std::unique_ptr<WorkingTime> >& getWorkingTimes() const;

    void setDayType(uint8_t type);
    WorkingTime* addWorkingTime();

private:
    uint8_t mDayType;
    std::vector< std::unique_ptr<WorkingTime> > mWorkingTimes;

};

} }

#endif /* __EXAMPLES_CALENDAR_WEEKDAY__ */
