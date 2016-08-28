#include <iostream>
#include <string>

#include "User.hpp"

using namespace std;

User::User(string t_userName, string t_userPassword, string t_userEmail,
	string t_userPhone) : m_name(t_userName), m_password(t_userPassword), 
        m_email(t_userEmail), m_phone(t_userPhone) {}

User::User(const User &t_user) {
	m_name = t_user.getName();
    m_password = t_user.getPassword(); 
    m_email = t_user.getEmail();
    m_phone = t_user.getPhone(); 
}

string User::getName() const {
	return m_name;
}

void User::setName(string t_name) {
	m_name = t_name;
}

string User::getPassword() const {
	return m_password;
}

void User::setPassword(string t_password) {
	m_password = t_password;
}

string User::getEmail() const {
	return m_email;
}

void User::setEmail(string t_email) {
	m_email = t_email;
}

string User::getPhone() const {
	return m_phone;
}

void User::setPhone(string t_phone) {
	m_phone = t_phone;
}
