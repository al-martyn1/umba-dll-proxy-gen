@set BRIEF_OPTS=--verbose=quet ^
--split-group ^
--filename-width=40 ^
--text-width=140 ^
--scan=.

@rem --main

umba-brief-scanner        %BRIEF_OPTS% --exclude-files=*.git^,*.bat^,*.md^,*.txt^,*.html^,*.txt^,*.cfg^ _brief.txt
umba-brief-scanner --html %BRIEF_OPTS% --exclude-files=*.git^,*.bat^,*.md^,*.txt^,*.html^,*.txt^,*.cfg^ _brief.html
