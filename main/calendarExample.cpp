
// system includes
#include <iostream>
#include <string>

// project sub directory includes - alphabetical order
#include "examples/calendar/Calendar.h"
#include "examples/calendar/CalendarXParser.h"
#include "examples/calendar/Weekday.h"

// project includes - alphabetical order


using namespace std;
using namespace examples::calendar;

void fullProcessExample(const string& xml_filepath) {
    CalendarXParser parser(xml_filepath);
    Calendar cal;
    parser.setCalendar(&cal);

    parser.parse();

    cout << endl << "----- EXAMPLE #1 -----" << endl << endl;

    cout << endl
         << "          Name: " << cal.getName() << endl
         << "   Description: " << cal.getDescription() << endl
         << "      Weekdays: " << endl << endl;

    auto wdCount = 1;
    for (const auto& wd: cal.getWeekdays()) {
        cout << "      " << wdCount << ":" << endl;
        cout << "       Day Type: " << static_cast<uint16_t>(wd->getDayType()) << endl;

        auto wtCount = 1;
        for (const auto& wt: wd->getWorkingTimes()) {
            cout << "       Working Time " << wtCount << ": " << endl;
            cout << "             " << wt->getFromTime() << " - " << wt->getToTime() << endl;

            wtCount++;
        }

        wdCount++;
    }

    return;
}

void userTagHandlerExample(const string& xml_filepath) {
    CalendarXParser parser(xml_filepath);
    
    // These are here but commented out.  Since we are only processing specific tags, we can skip creating 
    //  the root node because we are not storing the entire processed file.
    // Calendar cal;
    // parser.setCalendar(&cal);

    cout << endl << "----- EXAMPLE #2 -----" << endl << endl;

    // Here we have added a special localized handler for the tag.
    // This essential skips any processing and loading of the Calendar object, and allows you to just grab out 
    //  and process the tag(s) you want.
    // In order for this to work, you'll need to also had the hook into your start tag of your xyzXParser.
    parser.addUserTagHandler("weekday", [] (void* obj) {
        auto weekday = static_cast<Weekday*>( obj );
        cout << "Found daytype: " << static_cast<int>(weekday->getDayType()) << endl;
    });

    cout << "Simple tag handler for Weekday:" << endl;
    parser.parse();

    return;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "You must supply an XML" << endl;
        return 1;
    }

    string xml_filepath = argv[1];
    

    cout << "Calendar parsing file: " << xml_filepath << endl;

    fullProcessExample(xml_filepath);

    userTagHandlerExample(xml_filepath);

    return 0;
}
