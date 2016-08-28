#include <iostream>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "Meeting.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include "Path.hpp"
#include "Storage.hpp"

using namespace std;

shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    readFromFile();
    m_dirty = true;
}

bool Storage::readFromFile(void) {
    ifstream fin1(Path::userPath, ifstream::in);
    if (!fin1.is_open())
        return false;
    while(!fin1.eof()) {
        string line;
        getline(fin1, line);
        if (line.empty()) continue;
        string name, password, email, phone;
        string::iterator it1;
        for (it1 = line.begin(); *it1 != ','; it1++) {
            if (*it1 == '\"') continue;
            name += *it1;
        }
        it1++;
        for ( ; *it1 != ','; it1++) {
            if (*it1 == '\"') continue;
            password += *it1;
        }
        it1++;
        for ( ; *it1 != ','; it1++) {
            if (*it1 == '\"') continue;
            email += *it1;
        }
        it1++;
        for ( ; it1 != line.end(); it1++) {
            if (*it1 == '\"') continue;
            phone += *it1;
        }
        User temp = User(name, password, email, phone);
        m_userList.push_back(temp);
        line.clear();
    }

    ifstream fin2(Path::meetingPath, ifstream::in);
    if (!fin2.is_open())
        return false;
    while(!fin2.eof()) {
        string line;
        getline(fin2, line);
        if (line.empty()) continue;
        string sponsor, sdate, edate, title;
        vector<string> list;
        string::iterator it;
        for (it = line.begin(); *it != ','; it++) {
            if (*it == '\"') continue;
            sponsor += *it;
        }
        it++;
        string participator;
        for ( ; *it != ','; it++) {
            if (*it == '\"') continue;
            if (*it == '&') {
                list.push_back(participator);
                participator.clear();
                continue;
            }
            participator += *it;
        }
        list.push_back(participator);
        it++;
        for ( ; *it != ','; it++) {
            if (*it == '\"') continue;
            sdate += *it;
        }
        it++;
        for ( ; *it != ','; it++) {
            if (*it == '\"') continue;
            edate += *it;
        }
        it++;
        for ( ; it != line.end(); it++) {
            if (*it == '\"') continue;
            title += *it;
        }
        Meeting temp = Meeting(sponsor, list, Date::stringToDate(sdate), 
            Date::stringToDate(edate), title);
        m_meetingList.push_back(temp);
        line.clear();
    }
    fin1.close();
    fin2.close();
    return true;
}

string usersToFile(const User& user) {
    stringstream st;
    st << "\"" << user.getName() << "\",\"" << user.getPassword()
       << "\",\"" << user.getEmail() << "\",\"" << user.getPhone()
       << "\"";
    return st.str(); 
}

string meetingsToFile(const Meeting& meeting) {
    stringstream st1;
    st1 << "\"" << meeting.getSponsor() << "\",\"";
    for (auto it : meeting.getParticipator()) {
        st1 << it;
        if (it != meeting.getParticipator().back()) st1 << "&";
    }
    st1 << "\",\"" << Date::dateToString(meeting.getStartDate())
       << "\",\"" << Date::dateToString(meeting.getEndDate())
       << "\",\"" << meeting.getTitle() << "\"";
    return st1.str()
;}

bool Storage::writeToFile(void) {
    ofstream fout1(Path::userPath, ofstream::out | ofstream::trunc);
    if (!fout1.is_open())
        return false;
    for (auto it1 : m_userList) {
        fout1 << usersToFile(it1) << endl;
    }
    ofstream fout2(Path::meetingPath, ofstream::out | ofstream::trunc);
    if (!fout2.is_open())
        return false;
    for (auto it2 : m_meetingList) {
        fout2 << meetingsToFile(it2) << endl;
    }
    fout1.close();
    fout2.close();
    m_dirty = false;
    return true;
}

shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == nullptr) {
        m_instance = static_cast<shared_ptr<Storage>>(new Storage());
    }
    return m_instance;
}

Storage::~Storage() {
    sync();
}

void Storage::createUser(const User & t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

list<User> Storage::queryUser(function<bool(const User &)> filter) const {
    list<User> tempList;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++) {
        if (filter(*it)) tempList.push_back(*it);
    }
    return tempList;
}

int Storage::updateUser(function<bool(const User &)> filter, 
    function<void(User &)> switcher) {
    int count = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++) {
        if (filter(*it)) {
            switcher(*it);
            count++;
        }
    }
    m_dirty = true;
    return count;
}

int Storage::deleteUser(function<bool(const User &)> filter) {
    int count = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); ) {
        if (filter(*it)) {
            it = m_userList.erase(it);
            count++;
            continue;
        }
        it++;
    }
}

void Storage::createMeeting(const Meeting & t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const {
    list<Meeting> tempList;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        if (filter(*it)) tempList.push_back(*it);
    }
    return move(tempList);
}

int Storage::updateMeeting(function<bool(const Meeting &)> filter,
    function<void(Meeting &)> switcher) {
    int count = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        if (filter(*it)) {
            switcher(*it);
            count++;
        }
    }
    m_dirty = true;
    return count;
}

int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
    int count = 0;
    for (auto it = m_meetingList.begin(); it != m_meetingList.end(); ) {
        if (filter(*it)) {
            it = m_meetingList.erase(it);
            count++;
            continue;
        }
        it++;
    }
    m_dirty = true;
    return count;
}

bool Storage::sync(void) {
    if (m_dirty) return writeToFile();
    return true;
}