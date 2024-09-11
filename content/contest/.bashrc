alias c='g++ -Wall -Wconversion -Wfatal-errors -g -std=c++14 \
	-fsanitize=undefined,address -D_GLIBCXX_DEBUG'
xmodmap -e 'clear lock' -e 'keycode 66=less greater' #caps = <>
