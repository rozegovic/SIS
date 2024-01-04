#pragma once
#include <td/Types.h>


enum class NamingType : td::INT4 {ADMIN = 1, PROFESSOR = 2, ASSISTANT = 3, STUDENT = 4, SAO = 5}; //UserRoles, ne NamingType
//enum class QuestionID : td::UINT2 { Save };
enum class DlgID : td::UINT4 { Login = 100, Cancel = 200, Attendance = 300, Curriculum=400 };
enum class QuestionID : td::UINT2 { Save };
struct Globals
{
	static td::INT4 _currentUserID;
	static td::INT4 _currentUserRole; //ovo je tip UserRoles?

	static bool isAdmin;
	static bool isProfessor;
	static bool isAssistant;
	static bool isStudent;
	static bool isSAO;

};

