/**
 * @class WorkingTime
 *
 * @brief WorkingTime
 *
 * @section DESCRIPTION
 * WorkingTime
 *
 */

// system includes

// base object includes
#include "examples/calendar/WorkingTime.h"

// project sub directory includes - alphabetical order

// project includes - alphabetical order

using namespace std;

namespace examples { namespace calendar {

WorkingTime::WorkingTime() :
    mFromTime(1010),
    mToTime(1101)
{
}   

WorkingTime::~WorkingTime() {}

uint16_t WorkingTime::getFromTime() const {
    return mFromTime;
}

uint16_t WorkingTime::getToTime() const {
    return mToTime;
}

void WorkingTime::setFromTime(uint16_t time) {
    mFromTime = time;
}

void WorkingTime::setToTime(uint16_t time) {
    mToTime = time;
}

} }