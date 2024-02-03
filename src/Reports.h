#pragma once
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>




void examAttendance(const gui::Image*, td::INT4 SubjectID);
void examGrades(const gui::Image*, td::INT4 SubjectID);
void extraGrades(const gui::Image*, td::INT4 SubjectID, td::INT4 type);
void examGrade(const gui::Image*, td::INT4 SubjectID, td::INT4 id_kor);