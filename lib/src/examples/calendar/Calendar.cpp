/**
 * @class Calendar
 *
 * @brief Calendar
 *
 * @section DESCRIPTION
 * Calendar
 *
 */

// system includes

// base object includes
#include "examples/calendar/Calendar.h"

// project sub directory includes - alphabetical order

// project includes - alphabetical order

using namespace std;

namespace examples { namespace calendar {

Calendar::Calendar() : 
    mName("__not set__"),
    mDescription("__not set__"),
    mWeekdays()
{
}

Calendar::~Calendar() {}

const string& Calendar::getName() const {
    return mName;
}

const string& Calendar::getDescription() const {
    return mDescription;
}

const vector< unique_ptr<Weekday> >& Calendar::getWeekdays() const {
    return mWeekdays;
}

void Calendar::setName(const std::string& name) {
    mName = name;
}

void Calendar::setDescription(const std::string& desc) {
    mDescription = desc;
}

Weekday* Calendar::addWeekday() {
    mWeekdays.emplace_back( new Weekday() );
    return mWeekdays.back().get();
}

} }