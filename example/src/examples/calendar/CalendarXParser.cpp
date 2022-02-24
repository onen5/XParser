/**
 * @class CalendarXParser
 *
 * @brief This class is used to parse the Calendar XML
 *
 * @section DESCRIPTION
 *
 */

// system includes
#include <memory>
#include <string>

// base object includes
#include "examples/calendar/CalendarXParser.h" 

// project sub directory includes - alphabetical order
#include "examples/calendar/Weekday.h"
#include "examples/calendar/WorkingTime.h"

// project includes - alphabetical order


using namespace std;

namespace examples { namespace calendar {

/*
 * Public Methods
 */

CalendarXParser::CalendarXParser() : 
    XmlParser("") 
{
}

CalendarXParser::CalendarXParser(const string& fileName) :
    XmlParser(fileName),
    mCalPtr(nullptr),
    mObjStack()
{
    setDomain("CalendarXParser");

    addStartTagHandler( "calendar", [this] ()
        {
            if( mCalPtr == nullptr ) {
                XML_ERROR("No Calendar object was provided.");
            }

            mObjStack.push_back(mCalPtr);
        }
    );

    addEndTagHandler( "calendar", [this] ()
        {
            mObjStack.pop_back();

            if( !mObjStack.empty() ) {
                XML_ERROR("Calendar was not fully processed.");
            }
        }
    );

    addEndTagHandler( "name", [this] ()
        {
            dynamic_cast<Calendar*>(mObjStack.back())->setName( *getContent() );
        }
    );

    addEndTagHandler( "description", [this] ()
        {
            dynamic_cast<Calendar*>(mObjStack.back())->setDescription( *getContent() );
        }
    );

    addStartTagHandler( "weekday", [this] ()
        {
            cout << "processing push weekday" << endl;

            if(inUserHandler()) {
                auto weekday = make_shared<Weekday>();
                mObjStack.push_back(weekday.get());
                setUserHandlerObject( move(weekday) );
            }
            else {
                mObjStack.push_back( mCalPtr->addWeekday() );
            }
        }
    );

    addEndTagHandler( "weekday", [this] ()
        {
            cout << "processing pop weekday" << endl;
            mObjStack.pop_back();
        }
    );

    addEndTagHandler( "daytype", [this] ()
        {
            cout << "processing daytype" << endl;
            dynamic_cast<Weekday*>(mObjStack.back())->setDayType( getUint16(getContent()) );
        }
    );

    addStartTagHandler( "working-time", [this] ()
        {
            cout << "processing push working-time" << endl;
            auto wtptr = dynamic_cast<Weekday*>(mObjStack.back())->addWorkingTime( );
            mObjStack.push_back( wtptr );
        }
    );

    addEndTagHandler( "working-time", [this] ()
        {
            cout << "processing pop working-time" << endl;
            mObjStack.pop_back();
        }
    );

    addEndTagHandler( "from-time", [this] ()
        {
            cout << "processing from-time" << endl;
            dynamic_cast<WorkingTime*>(mObjStack.back())->setFromTime( getUint16(getContent()) );
        }
    );

    addEndTagHandler( "to-time", [this] ()
        {
            cout << "processing to-time" << endl;
            dynamic_cast<WorkingTime*>(mObjStack.back())->setToTime( getUint16(getContent()) );
        }
    );

}

void CalendarXParser::setCalendar(Calendar* cal) {
    mCalPtr = cal;
}

} }
