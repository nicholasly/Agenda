#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "AgendaUI.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "Meeting.hpp"

using namespace std;

AgendaUI::AgendaUI() {
	startAgenda();
}

void AgendaUI::OperationLoop(void) {
	string operation;
	cout << "---------------------- Agenda -------------------------------" 
		 << endl << "Action :"
		 << endl << "l   - log in Agenda by user name and password"
		 << endl << "r   - register an Agenda account"
		 << endl << "q   - quit Agenda"
		 << endl << "-------------------------------------------------------------"
		 << endl << endl << "Agenda : ^$ ";
    cin >> operation;
    while (operation[0] != 'q') {
    	if (operation[0] == 'l') {
    		userLogIn();
    	}
    	if (operation[0] == 'r') {
    		userRegister();
    	}
    	cout << "---------------------- Agenda -------------------------------" 
		 << endl << "Action :"
		 << endl << "l   - log in Agenda by user name and password"
		 << endl << "r   - register an Agenda account"
		 << endl << "q   - quit Agenda"
		 << endl << "-------------------------------------------------------------"
		 << endl << endl << "Agenda : ^$ ";
		 cin >> operation;
    }
    quitAgenda();
}

void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
}

string AgendaUI::getOperation() {
	string operation;
	cout << "---------------------- Agenda -------------------------------"
	     << endl << "Action : "
	     << endl << "o   - log out Agenda"
	     << endl << "dc  - delete Agenda account"
	     << endl << "re  - reset password"
	     << endl << "lu  - list all Agenda user"
	     << endl << "cm  - create a meeting"
	     << endl << "la  - list all meetings"
	     << endl << "las - list all sponsor meetings"
	     << endl << "lap - list all participate meetings"
	     << endl << "qm  - query meeting by title"
	     << endl << "qt  - query meeting by time interval"
	     << endl << "dm  - delete meeting by title"
	     << endl << "da  - delete all meetings"
	     << endl << "-------------------------------------------------------------"
	     << endl << "Agenda@" << m_userName << " :# ";
	cin >> operation;
	return operation;
}

bool AgendaUI::executeOperation(string t_operation) {
	if (t_operation == "o") {
		userLogOut();
		return false;
	}
	if (t_operation == "dc") {
		deleteUser();
		return false;
	}
	if (t_operation == "lu") {
		listAllUsers();
		return true;
	}
	if (t_operation == "cm") {
		createMeeting();
		return true;
	}
	if (t_operation == "la") {
		listAllMeetings();
		return true;
	}
	if (t_operation == "las") {
		listAllSponsorMeetings();
		return true;
	}
	if (t_operation == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	if (t_operation == "qm") {
		queryMeetingByTitle();
		return true;
	}
	if (t_operation == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	if (t_operation == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	if (t_operation == "da") {
		deleteAllMeetings();
		return true;
	}
	return true;
}

void AgendaUI::userLogIn(void) {
	string userName, password;
	cout << "[log in] [user name] [password]"
	     << endl << "[log in] ";
	cin >> userName >> password;
	if (m_agendaService.userLogIn(userName, password)) {
		m_userName = userName;
		m_userPassword = password;
		cout << "[log in] succeed!" << endl;
		while (executeOperation(getOperation()));
	} else {
		cout << "[error] log in fail!" << endl;
	}
}

void AgendaUI::userRegister(void) {
	string userName, password, email, phone;
	cout << "[register] [user name] [password] [email] [phone]"
	     << endl << "[register] ";
	cin >> userName >> password >> email >> phone;
	if (m_agendaService.userRegister(userName, password, email, phone)) {
		m_userName = userName;
		m_userPassword = password;
		cout << "[register] succeed!" << endl;
		while (executeOperation(getOperation()));
	} else {
		cout << "[error] register fail!" << endl;
	}
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut(void) {
	m_userName.clear();
	m_userPassword.clear();
}

void AgendaUI::deleteUser(void) {
	if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
		cout << "[delete user] succeed!" << endl;
		userLogOut();
	}
	else cout << "[delete user] error!" << endl;
}

void AgendaUI::listAllUsers(void) {
	cout << "[list all users]"
	     << endl
	     << endl << setiosflags(ios::left) << setw(18) << 
	     "name" << setw(18) << "email" << setw(18) << "phone" << endl;
	for (auto it : m_agendaService.listAllUsers()) {
		cout << setw(18) << it.getName()
		     << setw(18) << it.getEmail()
		     << setw(18) << it.getPhone() << endl;
	}
}

void AgendaUI::createMeeting(void) {
	string title, participators, startDate, endDate;
	cout << "[create meeting] [the number of the participators]"
	     <<  endl << "[create meeting] ";
	int num;
	cin >> num;
	vector<string> temp;
	for (int i = 1; i <= num; i++) {
		cout << "[create meeting] [please enter the participator "
			 << i << " ]"
             << endl << "[create meeting] ";
        string name;
        cin >> name;
        temp.push_back(name);
	}
	cout << "[create meeting] [title][start time(yyyy-mm-dd/hh:mm)] "
	     << "[end time(yyyy-mm-dd/hh:mm)]"
	     << endl << "[create meeting] ";
	cin >> title >> startDate >> endDate;
	if (m_agendaService.createMeeting(m_userName, title, startDate, endDate, temp)) {
		cout << "[create meeting] succeed!" << endl;
	} else {
		cout << "[create meeting] error!" << endl;
	}
}

void AgendaUI::listAllMeetings(void) {
	cout << "[list all meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllMeetings(m_userName));
}

void AgendaUI::listAllSponsorMeetings(void) {
	cout << "[list all sponsor meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

void AgendaUI::listAllParticipateMeetings(void) {
	cout << "[list all participator meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

void AgendaUI::queryMeetingByTitle(void) {
	string title;
	cout << "[query meeting] [title]:"
	     << endl << "[query meeting] ";
	cin >> title;
	printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

void AgendaUI::queryMeetingByTimeInterval(void) {
	string startTime, endTime;
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] "
	     << "[end time(yyyy-mm-dd/hh:mm)]"
	     << endl << "[query meetings] ";
	cin >> startTime >> endTime;
	printMeetings(m_agendaService.meetingQuery(m_userName, startTime, endTime));
}

void AgendaUI::deleteMeetingByTitle(void) {
	string title;
	cout << "[delete meeting] [title]"
	     << endl << "[delete meeting] ";
	cin >> title;
	cout << endl;
	if (m_agendaService.deleteMeeting(m_userName, title)) {
		cout << "[delete meeting by title] succeed!" << endl;
	} else {
		cout << "[error] delete meeting fail!" << endl;
	}
}

void AgendaUI::deleteAllMeetings(void) {
	if (m_agendaService.deleteAllMeetings(m_userName)) {
		cout << "[delete all meetings] succeed!" << endl;
	}
}

void AgendaUI::printMeetings(list<Meeting> t_meetings) {
	cout << setiosflags(ios::left) << setw(18) << "title" << setw(18) << "sponsor" <<setw(18) <<"start time"
	     << setw(18) << "end time" << setw(18) << "participators" << endl;
	for (auto i : t_meetings) {
		cout << setw(18) << i.getTitle() << setw(18)
		     << i.getSponsor() << setw(18)
		     << Date::dateToString(i.getStartDate()) << setw(18) 
		     << Date::dateToString(i.getEndDate());
		 for (auto it : i.getParticipator()) {
		 	cout << it;
		 	if (it != i.getParticipator().back()) cout << ",";
		 }
		 cout << endl;
	}
}
