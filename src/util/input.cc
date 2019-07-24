#include "util.h"
#include <ncurses.h>
#include <sys/select.h>
#include <unistd.h>


int util::input::pressed_key = ERR;

void util::input::Update(int delay_ms) {
    pressed_key = GetInput(delay_ms);
}

bool util::input::NoKeyPressed() {
	return (pressed_key == ERR);
}

bool util::input::IsPressed(int key) {
	return (pressed_key == key);
}

char util::input::GetAlphabet() {
	for (int i = 65; i <= 90; ++i)
	{
		if (util::input::IsPressed(i)) return (char)(i);
	}
	for (int i = 97; i <= 122; ++i)
	{
		if (util::input::IsPressed(i)) return (char)(i);
	}
	for (int i = 48; i <= 57; ++i)
	{
		if (util::input::IsPressed(i)) return (char)(i);
	}
	if (util::input::IsPressed(95)) return (char)(95);
	return ' ';
}


bool util::input::GetIcon(char &c) {
	for (int i = 32; i <= 126; ++i)
	{
		if (util::input::IsPressed(i)) {
			c = (char)(i);
			return true;
		}
	}
	return false;
}

int util::input::ctrl(char x) {
    return (x) & 0x1f;
}

int util::input::GetInput(int delay_ms) {
	int retval = 0;
	int c      = 0;

	fd_set input;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = delay_ms * 1000; // microseconds

	struct timeval* timeout_p = NULL;
	if (delay_ms != -1)
		timeout_p = &timeout;

	FD_ZERO(&input);
	FD_SET(STDIN_FILENO, &input);

	retval = select(FD_SETSIZE, &input, NULL, NULL, timeout_p);

	c = getch();

	if ((retval == 1) && (c == ERR)) // ERROR
		return -1;

	if (retval == 0)
		return ERR;

	return c;
}