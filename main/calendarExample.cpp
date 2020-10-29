
// system includes
#include <iostream>
#include <string>

// project sub directory includes - alphabetical order
#include "examples/calendar/Calendar.h"
#include "examples/calendar/CalendarXParser.h"

// project includes - alphabetical order


using namespace std;
using namespace examples::calendar;

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "You must supply an XML" << endl;
        return 1;
    }

    string xml_filepath = argv[1];
    

    cout << "Calendar parsering file: " << xml_filepath << endl;


    CalendarXParser parser(xml_filepath);
    Calendar cal;
    parser.setCalendar(&cal);

    parser.parse();

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

    return 0;
}
