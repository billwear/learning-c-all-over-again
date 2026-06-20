Bug Number :: Bug Text

---------------------------------------

0001 :: default output is test (debug) output; add -d flag for test output, and use standard calendar output with no flag.  standard calendar output is just a list of active reminders that are relevant to today. :: **FIXED**

---------------------------------------

0002 :: code doesn't parse the date variant "daily" which should always print the reminder output for that line. :: **FIXED**

---------------------------------------

0003 :: code doesn't parse the date variants "weekday" and "weekend" which should print "weekday" lines on monday through friday, and "weekend" lines on saturday and sunday.

---------------------------------------

0004 :: the output doesn't handle days of the week like "Tue", "tues", "tuesday", or "Tuesday".

---------------------------------------

0005 :: the parser is required to handle different cases when decoding some date formats; the date token should be made lowercase before the date parser has to act, so that a reduced set of comparisons will work; also, the parser should only compare the first three letters of the current day of the week to the date token when looking for day of week markers. :: **FIXED**

---------------------------------------

0006 :: the code doesn't handle ordinal days of the week (e.g., "3rd wednesday").

---------------------------------------
