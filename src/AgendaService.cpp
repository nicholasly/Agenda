#include <iostream>
#include <string>
#include <list>
#include <vector>

#include "AgendaService.hpp"

using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const string userName, const string password) {
	auto filter = [userName](const User& user) {
		return userName == user.getName();
	};
	if  (m_storage->queryUser(filter).size() == 0) return false;
	//  Judge if the user exists
	auto filter1 = [userName, password](const User& user) {
		return (userName == user.getName() && password == user.getPassword());
	};
	return m_storage->queryUser(filter1).size() > 0;
	//  Judge if the user and the password is correct
}

bool AgendaService::userRegister(const string userName, const string password, 
	const string email, const string phone) {
	if (userName.empty() || password.empty() || email.empty() || phone.empty())
		return false;
	//  Judge if the input is correct
	auto filter = [userName](const User& user) {
		return (userName == user.getName());
	};
	if (m_storage->queryUser(filter).size() == 0) {
		m_storage->createUser(User(userName, password, email, phone));
		return true;
	}
	//  If the user does not exists, create a new user
	return false;
}

bool AgendaService::deleteUser(const string userName, const string password) {
	auto filter = [userName, password](const User& user) {
		return (userName == user.getName() && password == user.getPassword());
	};
	if (m_storage->queryUser(filter).size() == 0) return false;
	//   Judge the user and the password
	int temp = m_storage->deleteUser(filter);
	deleteAllMeetings(userName);
	//   Delete all meetings sponsored by the user
	auto filter2 = [userName](const Meeting& meeting) {
		return meeting.isParticipator(userName);
	};
	m_storage->deleteMeeting(filter2);
	//   Delete all meetings participated by the user
	return true;
}

list<User> AgendaService::listAllUsers(void) const {
	auto filter = [](const User& user) {
		return true;
	};
	//   Find all users
	return m_storage->queryUser(filter);
}

bool AgendaService::createMeeting(const string userName, const string title,
	const string startDate, const string endDate, const vector<string> participator) {
	if (startDate == "0000-00-00/00:00" || endDate == "0000-00-00/00:00") return false;
	if (!Date::isValid(Date::stringToDate(startDate)) || !Date::isValid(Date::stringToDate(endDate)))
		return false;
	//  Judge if the date input is valid
	if (Date::stringToDate(startDate) >= Date::stringToDate(endDate)) return false;
    //  Judge if the startdate is larger than the enddate
	if (title.empty()) return false;
	//  Judge if the title is empty(not neccessary)
	for (auto iter = participator.begin(); iter != participator.end(); iter++) {
		if (*iter == userName) return false;
	}
	//  Judge the user input is among the participators input
	auto filter1 = [participator](const User& user) {
		for (auto i = participator.begin(); i != participator.end(); i++) {
			if (*i == user.getName()) return true; 
		}
		return false;
	};
    if (m_storage->queryUser(filter1).size() != participator.size()) return false;
    //  Judge if the participators are all in the userlist
    //  and the participators are duplicate
	auto filter2 = [userName](const User& user) {
		return userName == user.getName();
	};
	if (m_storage->queryUser(filter2).size() == 0) return false;
    //   Judge if the user exists 
	auto filter3 = [title](const Meeting& meeting) {
		return (title == meeting.getTitle());
	};
	if (m_storage->queryMeeting(filter3).size() > 0) return false;
    //   Judge if the title exists
	auto filter4 = [userName, startDate, endDate](const Meeting& meeting) {
		bool flag1 = false, flag2 = false;
		if (userName == meeting.getSponsor()) flag1 = true;
		for (auto i : meeting.getParticipator()) {
			if (userName == i) flag2 = true;
		}
		if (flag1 || flag2) {
			if ((meeting.getStartDate() >= Date::stringToDate(endDate) &&
			     meeting.getStartDate() > Date::stringToDate(startDate)) ||
			     (meeting.getEndDate() < Date::stringToDate(endDate) &&
			     meeting.getEndDate() <= Date::stringToDate(startDate)))
				return false;
			else return true;
		}
		else return false;
	};
    //  Judge if the time of the user is suitable
	auto filter5 = [participator, startDate, endDate](const Meeting& meeting) {
		int count = 0;
		for (auto p = participator.begin(); p != participator.end(); p++) {
			bool flag1 = false, flag2 = false;
		    if (*p == meeting.getSponsor()) flag1 = true;
		    if (meeting.isParticipator(*p)) flag2 = true;
		    if (flag1 || flag2) {
		    	bool superflag;
		    	if ((meeting.getStartDate() >= Date::stringToDate(endDate) &&
			         meeting.getStartDate() > Date::stringToDate(startDate)) ||
			        (meeting.getEndDate() < Date::stringToDate(endDate) &&
			         meeting.getEndDate() <= Date::stringToDate(startDate)))
		    		superflag = false;
			    else superflag = true;
			    if (superflag) count++;
		    }
		}
		if (count != 0) return true;
		return false;
	};
	//  Judge if the time of the participators is suitable
	bool flag = true;
	if (m_storage->queryMeeting(filter4).size() > 0) flag = false;
	if (m_storage->queryMeeting(filter5).size() > 0) flag = false;
	if (flag) {
		m_storage->createMeeting(Meeting(userName, participator, 
			Date::stringToDate(startDate), Date::stringToDate(endDate), title));
		return true;
	}
	else return false;
}

list<Meeting> AgendaService::meetingQuery(const string userName, const string title) const {
	if (title.empty()) return list<Meeting>();
	auto filter = [userName, title](const Meeting& meeting) {
		if (title != meeting.getTitle()) return false;
		if (userName == meeting.getSponsor()) return true;
		if (meeting.isParticipator(userName)) return true;
		return false;
	};
	return m_storage->queryMeeting(filter);
	//   Find meeting by user and title
}

list<Meeting> AgendaService::meetingQuery(const string userName, const string startDate,
    const string endDate) const {
    if (startDate == "0000-00-00/00:00" || endDate == "0000-00-00/00:00") return list<Meeting>();
    if (!Date::isValid(Date::stringToDate(startDate)) || !Date::isValid(Date::stringToDate(endDate))) 
    	return list<Meeting>();
	if (Date::stringToDate(startDate) > Date::stringToDate(endDate)) return list<Meeting>();
	auto filter = [userName, startDate, endDate](const Meeting& meeting) {
		if (userName == meeting.getSponsor() || meeting.isParticipator(userName)) {
			    return ((meeting.getStartDate() >= Date::stringToDate(startDate)
				       && meeting.getStartDate() <= Date::stringToDate(endDate))
		               || (meeting.getEndDate() >= Date::stringToDate(startDate)
		               && meeting.getEndDate() <= Date::stringToDate(endDate))
		               || (meeting.getStartDate() <= Date::stringToDate(startDate)
		               && meeting.getEndDate() >= Date::stringToDate(endDate))
		               || (meeting.getStartDate() >= Date::stringToDate(startDate)
		               && meeting.getEndDate() <= Date::stringToDate(endDate)));
		}
		return false;
	};
	return m_storage->queryMeeting(filter);
	//  Find all time overlap
}

list<Meeting> AgendaService::listAllMeetings(const string userName) const {
	auto filter = [userName](const Meeting& meeting) {
		return (userName == meeting.getSponsor() || meeting.isParticipator(userName));
	};
	return m_storage->queryMeeting(filter);
	//  List all meetings
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string userName) const {
	auto filter = [userName](const Meeting& meeting) {
		return (userName == meeting.getSponsor());
	};
	return m_storage->queryMeeting(filter);
	//   List all meetings sponsored by the user
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string userName) const {
	auto filter = [userName](const Meeting& meeting) {
		return meeting.isParticipator(userName);
	};
	return m_storage->queryMeeting(filter);
	//   List all meetings participated by the user
}

bool AgendaService::deleteMeeting(const string userName, const string title) {
	auto filter = [userName, title](const Meeting& meeting) {
		return (title == meeting.getTitle() && userName == meeting.getSponsor());
	};
	return m_storage->deleteMeeting(filter) > 0;
	//   Delete the meeting sponsored by the user by the title
}

bool AgendaService::deleteAllMeetings(const string userName) {
	auto filter = [userName](const Meeting& meeting) {
		return userName == meeting.getSponsor();
	};
	return m_storage->deleteMeeting(filter) > 0;
	//   Delete all meetings sponsored by the user
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage->sync();
}