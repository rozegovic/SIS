#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <dp/IStatement.h>
#include <dp/IDatabase.h>
#include "ViewIDs.h"

class MsgSender
{///
private:
	
	bool ponovljeno(td::INT4 a, td::String subject, td::String poruka, td::INT4 userID, td::INT4 procitano);
	bool ponovljeno2(td::INT4 a, td::String subject, td::String poruka, std::vector<td::INT4> userIDs, td::INT4 procitano);
public:
	bool sendMsgtoUser(td::String subject, td::String poruka, td::INT4 userID, td::INT4 procitano);
	bool sendMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs, td::INT4 procitano);
	bool sendSystemMsgtoUser(td::String subject, td::String poruka, td::INT4 userID, td::INT4 procitano);
	bool sendSystemMsgtoUsers(td::String subject, td::String poruka, std::vector<td::INT4> userIDs, td::INT4 procitano);

};




