/*
 *
 */

#ifndef __EXAMPLES_CALENDAR_WORKINGTIME__
#define __EXAMPLES_CALENDAR_WORKINGTIME__

// system includes
#include <cstdint>

// base object includes
#include "examples/BaseInterface.h"

// project sub directory includes - alphabetical order

// project includes - alphabetical order


namespace examples { namespace calendar {

class WorkingTime : public BaseInterface {

public:
    WorkingTime();
    virtual ~WorkingTime();

    uint16_t getFromTime() const;
    uint16_t getToTime() const;

    void setFromTime(uint16_t time);
    void setToTime(uint16_t time);

private:

    uint16_t mFromTime;
    uint16_t mToTime;

};

} }

#endif /* __EXAMPLES_CALENDAR_WORKINGTIME__ */
