/*
 * Copyright 2018 Humdinger <humdingerb@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "AboutTipster.h"

#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "About"

AboutTipster::AboutTipster()
	:
	BAboutWindow(B_TRANSLATE_SYSTEM_NAME("Tipster"),
		"application/x-vnd.tipster")
{
	AddDescription(B_TRANSLATE("An application to show "
		"usability tips for Haiku."));
	const char* extraCopyrights[] = {
		"2016 Hannah Pan",
		"2016-2018 Humdinger",
		"2017 Akshay Agarwal",
		"2017 vanishakesswani",
		"2018 Janus",
		NULL
	};
	const char* authors[] = {
		"Akshay Agarwal",
		"Humdinger",
		"Hannah Pan",
		"Vale Tolpegin",
		"vanishakesswani",
		NULL
	};
	AddCopyright(2015, "Vale Tolpegin", extraCopyrights);
	AddAuthors(authors);
}
