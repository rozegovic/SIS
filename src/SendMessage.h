#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <dp/IStatement.h>
#include <dp/IDatabase.h>
#include "ViewIDs.h"

class MsgSender
{///
private:
	
	bool ponovljeno(td::INT4 a, td::String subject, td::String poruka, td::INT4 userID);
	bool ponovljeno2(td::INT4 a, td::String subject, td::String poruka, std::vector<td::INT4> userIDs);
public:
	bool sendMsgtoUser(td::String subject, td::String poruka, td::INT4 userID);
	bool sendMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs);
	bool sendSystemMsgtoUser(td::String subject, td::String poruka, td::INT4 userID);
	bool sendSystemMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs);

};




