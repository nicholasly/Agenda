#include <iostream>
#include <string>
#include <sstream>

#include "Date.hpp"

using namespace std;

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) :
   m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

Date::Date(string dateString) {
	m_year = stringToDate(dateString).getYear();
	m_month = stringToDate(dateString).getMonth();
	m_day = stringToDate(dateString).getDay();
	m_hour = stringToDate(dateString).getHour();
	m_minute = stringToDate(dateString).getMinute();
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}


bool isLeap(Date date) {
	if (date.getYear() % 4 == 0 && date.getYear() % 100 != 0) return true;
	else if (date.getYear() % 400 == 0) return true;
	else return false;
}

string Zero(int num) {
	stringstream st;
	if (num < 10) st << "0" << num;
	else st << num;
	return st.str();
}

bool Date::isValid(const Date t_date) {
	if (t_date.getYear() < 1000 || t_date.getYear() > 9999)
		return false;
	if (t_date.getMonth() < 1 || t_date.getMonth() > 12)
		return false;
	if (t_date.getMonth() != 2) {
		if (t_date.getMonth() == 1 || t_date.getMonth() == 3 || \
			 t_date.getMonth() == 5 || t_date.getMonth() == 7 || \
			 t_date.getMonth() == 8 || t_date.getMonth() == 10 || \
			 t_date.getMonth() == 12) {
			   if (t_date.getDay() < 1 || t_date.getDay() > 31)
			   	return false;
		} else {
			   if (t_date.getDay() < 1 || t_date.getDay() > 30)
			   	return false;
		}
	} else {
		if (isLeap(t_date)) {
			if (t_date.getDay() < 1 || t_date.getDay() > 29)
				return false;
		} else {
			if (t_date.getDay() < 1 || t_date.getDay() > 28)
				return false;
		}
	}
	if (t_date.getHour() < 0 || t_date.getHour() > 23)
		return false;
	if (t_date.getMinute() < 0 || t_date.getMinute() > 59)
		return false;
	return true;
}

string Date::dateToString(Date t_date) {
	if (!Date::isValid(t_date)) {
		string temp = "0000-00-00/00:00";
		return temp;
	}
	stringstream st;
	st << t_date.getYear() << "-" << Zero(t_date.getMonth())
	   << "-" << Zero(t_date.getDay()) << "/" << Zero(t_date.getHour())
	   << ":" << Zero(t_date.getMinute());
	return st.str();
}

Date Date::stringToDate(const string t_dateString) {
	if (t_dateString.size() != 16) return Date(0, 0, 0, 0, 0);
	if (t_dateString[4] != '-' || t_dateString[7] != '-'
		|| t_dateString[10] != '/' || t_dateString[13] != ':')
		return Date(0, 0, 0, 0, 0);
	for (int i = 0; i < 16; i++) {
		if (i == 4 || i == 7 || i == 10 || i == 13) continue;
		if (t_dateString[i] < '0' || t_dateString[i] > '9') {
			return Date(0, 0, 0, 0, 0);
		}
	}
	string t;
	if (t_dateString[0] == '0') return Date(0, 0, 0, 0, 0);
	for (int i = 0; i < 4; i++) {
		t += t_dateString[i];
	}
	string t1;
	if (t_dateString[5] == '0') t1 = t_dateString[6];
	else {
		t1 += t_dateString[5];
		t1 += t_dateString[6];
	}
	string t2;
	if (t_dateString[8] == '0') t2 = t_dateString[9];
	else {
		t2 += t_dateString[8];
		t2 += t_dateString[9];
	}
	string t3;
	if (t_dateString[11] == '0') t3 = t_dateString[12];
	else {
		t3 += t_dateString[11];
		t3 += t_dateString[12];
	}
	string t4;
	if (t_dateString[14] == '0') t4 = t_dateString[15];
	else {
		t4 += t_dateString[14];
		t4 += t_dateString[15];
	}
	int y, mon, d, h, min;
	stringstream st1, st2, st3, st4, st5;
	st1 << t;
	st1 >> y;
	st2 << t1;
	st2 >> mon;
	st3 << t2;
	st3 >> d;
	st4 << t3;
	st4 >> h;
	st5 << t4;
	st5 >> min;
	Date da = Date(y, mon, d, h, min);
	if (isValid(da)) return da;
	else return Date(0, 0, 0, 0, 0);
}

Date& Date::operator = (const Date& t_date) {
	setYear(t_date.getYear());
	setMonth(t_date.getMonth());
	setDay(t_date.getDay());
	setHour(t_date.getHour());
	setMinute(t_date.getMinute());
	return *this;
}

bool Date::operator == (const Date &t_date) const {
	if (getYear() != t_date.getYear()) return false;
	if (getMonth() != t_date.getMonth()) return false;
	if (getDay() != t_date.getDay()) return false;
	if (getHour() != t_date.getHour()) return false;
	if (getMinute() != t_date.getMinute()) return false;
	return true;
}

bool Date::operator <(const Date& t_date) const {
	if (getYear() < t_date.getYear()) return true;
	else if (getYear() > t_date.getYear()) return false;
	if (getMonth() < t_date.getMonth()) return true;
	else if (getMonth() > t_date.getMonth()) return false;
	if (getDay() < t_date.getDay()) return true;
	else if (getDay() > t_date.getDay()) return false;
	if (getHour() < t_date.getHour()) return true;
	else if (getHour() > t_date.getHour()) return false;
	if (getMinute() < t_date.getMinute()) return true;
	else if (getMinute() > t_date.getMinute()) return false;
	return false;
}

bool Date::operator >(const Date& t_date) const {
	if (getYear() > t_date.getYear()) return true;
	else if (getYear() < t_date.getYear()) return false;
	if (getMonth() > t_date.getMonth()) return true;
	else if (getMonth() < t_date.getMonth()) return false;
	if (getDay() > t_date.getDay()) return true;
	else if (getDay() < t_date.getDay()) return false;
	if (getHour() > t_date.getHour()) return true;
	else if (getHour() < t_date.getHour()) return false;
	if (getMinute() > t_date.getMinute()) return true;
	else if (getMinute() < t_date.getMinute()) return false;
	return false;
}

bool Date::operator <=(const Date& t_date) const {
	if (getYear() < t_date.getYear()) return true;
	else if (getYear() > t_date.getYear()) return false;
	if (getMonth() < t_date.getMonth()) return true;
	else if (getMonth() > t_date.getMonth()) return false;
	if (getDay() < t_date.getDay()) return true;
	else if (getDay() > t_date.getDay()) return false;
	if (getHour() < t_date.getHour()) return true;
	else if (getHour() > t_date.getHour()) return false;
	if (getMinute() < t_date.getMinute()) return true;
	else if (getMinute() > t_date.getMinute()) return false;
	return true;
}

bool Date::operator >=(const Date& t_date) const {
	if (getYear() > t_date.getYear()) return true;
	else if (getYear() < t_date.getYear()) return false;
	if (getMonth() > t_date.getMonth()) return true;
	else if (getMonth() < t_date.getMonth()) return false;
	if (getDay() > t_date.getDay()) return true;
	else if (getDay() < t_date.getDay()) return false;
	if (getHour() > t_date.getHour()) return true;
	else if (getHour() < t_date.getHour()) return false;
	if (getMinute() > t_date.getMinute()) return true;
	else if (getMinute() < t_date.getMinute()) return false;
	return true;
}