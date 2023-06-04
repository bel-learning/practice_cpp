#include "event.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "util.h"

using namespace std;

Event::Event(const string & title, const string & description, Repeat repeat)
    : m_Title(title), m_Description(description), m_Repeat(repeat)
{}
Event::Event(const Event * event) : m_Title(event->m_Title), m_Description(event->m_Description), m_Repeat(event->m_Repeat) 
{};
Event::Event(const Event & event) : m_Title(event.m_Title), m_Description(event.m_Description), m_Repeat(event.m_Repeat) 
{};
Event::~Event() {}
// Event Event::operator= (const Event & event) {
//     if(*this == event) return *this;
//     m_Title = event.m_Title;
//     m_Description = event.m_Description;
//     m_Repeat = event.m_Repeat;
//     return *this;
// }

const string & Event::getTitle() const {
    return m_Title;
}
const string & Event::getDescription() const {
    return m_Description;
}
const Event::Repeat & Event::getRepeat() const {
    return m_Repeat;
}

bool operator == (const Event & event1, const Event & event2) {
    if(event1.m_Title != event2.m_Title) return false;
    if(event1.m_Description != event2.m_Description) return false;
    if(event1.m_Repeat != event2.m_Repeat) return false;
    return true;
}

// void Event::display() {};


NormalEvent::NormalEvent(const Event & event, const string & location, Datetime start, Datetime end)
    : Event(event), m_Location(location), m_Start(start), m_End(end) {};

NormalEvent::NormalEvent(const string & title, const string & description, Repeat repeat, const string & location, const Datetime & start, const Datetime & end) 
    : Event(title, description, repeat), m_Location(location), m_Start(start), m_End(end) {};

// NormalEvent::NormalEvent(const NormalEvent & event) 
bool NormalEvent::insideInterval(const Datetime & start, const Datetime & end) const {
    if(m_Start >= start && m_End <= end)
        return true;
    return false;
}

void NormalEvent::display() {};

Event * NormalEvent::makeCopy() const {
    return new NormalEvent(this->getTitle(), this->getDescription(), this->getRepeat(), m_Location, m_Start, m_End);
};

void NormalEvent::renderInHours(WINDOW * win) const {
    wprintw(win, "%02d:%02d-%02d:%02d %s\n", m_Start.hour, m_Start.minute, m_End.hour, m_End.minute, getTitle().c_str());
    wrefresh(win);
};
void NormalEvent::renderInDays(WINDOW * win) const {
    wprintw(win, "%02d %s : %s\n", m_Start.day, getMonthName(m_Start.month).c_str(), getTitle().c_str());
    wrefresh(win);
};



bool NormalEvent::isLowerThan(const Event * e) const {
    Datetime selfDate = this->getCompareDate();
    Datetime otherDate = e->getCompareDate();
    return selfDate < otherDate;
};
Datetime NormalEvent::getCompareDate() const {
    return m_Start;
};


Task::Task(const string & title, const string & description, Repeat repeat, const Datetime & start, const Datetime & end, bool finished)
    : Event(title, description, repeat), m_Start(start), m_End(end), m_Finished(finished) {};

void Task::renderInHours(WINDOW * win) const {
    // str.append()
    if(m_Finished)
        wattron(win, COLOR_PAIR(1));
    else 
        wattron(win, COLOR_PAIR(2));
    wprintw(win, "%02d:%02d-%02d:%02d %s\n", m_Start.hour, m_Start.minute, m_End.hour, m_End.minute, getTitle().c_str());
    wattroff(win, COLOR_PAIR(1));
    wattroff(win, COLOR_PAIR(2));

    wrefresh(win);
};
void Task::renderInDays(WINDOW * win) const {
    if(m_Finished)
        wattron(win, COLOR_PAIR(1));
    else 
        wattron(win, COLOR_PAIR(2));
    wprintw(win, "%02d %s : %s\n", m_Start.day, getMonthName(m_Start.month).c_str(), getTitle().c_str());
    wattroff(win, COLOR_PAIR(1));
    wattroff(win, COLOR_PAIR(2));


    wrefresh(win);
};

Event * Task::makeCopy() const {
    return new Task(this->getTitle(), this->getDescription(), this->getRepeat(), m_Start, m_End, m_Finished);
}

bool Task::insideInterval(const Datetime & start, const Datetime & end) const {
    if(m_Start >= start && m_End <= end)
        return true;
    return false;
}

void Task::display() {};

bool Task::isLowerThan(const Event * e) const {
    Datetime selfDate = this->getCompareDate();
    Datetime otherDate = e->getCompareDate();
    return selfDate < otherDate;
};
Datetime Task::getCompareDate() const {
    return m_Start;
};

// Deadline::Deadline(const string & formatted) {}
Deadline::Deadline(const string & title, const string & description, Repeat repeat, const Datetime & end)
    : Event(title, description, repeat), m_End(end) {};

Event * Deadline::makeCopy() const {
    return new Deadline(this->getTitle(), this->getDescription(), this->getRepeat(), m_End);
};

bool Deadline::insideInterval(const Datetime & start, const Datetime & end) const {
    if(m_End >= start && m_End <= end)
        return true;
    return false;
};
void Deadline::renderInHours(WINDOW * win) const {
    wattron(win, COLOR_PAIR(4));
    wprintw(win, "!%02d:%02d: %s\n", m_End.hour, m_End.minute, getTitle().c_str());
    wattroff(win, COLOR_PAIR(4));
    wrefresh(win);
};
void Deadline::renderInDays(WINDOW * win) const {
    wattron(win, COLOR_PAIR(4));
    wprintw(win, "!%02d %s : %s\n", m_End.day, getMonthName(m_End.month).c_str(), getTitle().c_str());
    wattroff(win, COLOR_PAIR(4));
    wrefresh(win);
};


void Deadline::display() {

};
bool Deadline::isLowerThan(const Event * e) const {
    Datetime selfDate = this->getCompareDate();
    Datetime otherDate = e->getCompareDate();
    return selfDate < otherDate;
};
Datetime Deadline::getCompareDate() const {
    return m_End;
};