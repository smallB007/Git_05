#include "stdafx.h"
#include "EmailValidator.hpp"


EmailValidator::EmailValidator(CString potentialEmailAddress)
{
	
	if (potentialEmailAddress.GetLength() > 0)
	{
		CT2CA characterString(potentialEmailAddress);
		LPSTR cStr = characterString.operator LPSTR();
		if (!boost::regex_match(cStr, email_regex_))
		{
			throw wrong_email_address_format();
		}
	}
}


EmailValidator::~EmailValidator()
{
}
