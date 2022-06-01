/**
 * @class Weekday
 *
 * @brief Weekday
 *
 * @section DESCRIPTION
 * Weekday
 *
 */

// system includes

// base object includes
#include "examples/calendar/Weekday.h"

// project sub directory includes - alphabetical order

// project includes - alphabetical order

using namespace std;

namespace examples::calendar {

Weekday::Weekday() :
    mDayType( 0 ),
    mWorkingTimes()
{
}

Weekday::~Weekday() { }

uint8_t Weekday::getDayType() const
{
    return mDayType;
}

const vector< unique_ptr< WorkingTime > >& Weekday::getWorkingTimes() const
{
    return mWorkingTimes;
}

void Weekday::setDayType( uint8_t type )
{
    mDayType = type;
}

WorkingTime* Weekday::addWorkingTime()
{
    mWorkingTimes.emplace_back( new WorkingTime() );
    return mWorkingTimes.back().get();
}

}  // namespace examples::calendar