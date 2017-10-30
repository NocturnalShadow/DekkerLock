#include "dekker_lock.h"
#include "tests.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void page1();
void page2(int test);

int main()
{
	page1();
	return 0;
}

int getch(void)
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

	return ch;
}

void page1(void)
{
	bool needUpdate = true;
	int key = 1;
	int code = 0;
	while (code != 10)
	{
		if (needUpdate) {
			printf("\e[1;1H\e[2J");

			if (key == 1) {
				cout <<
					"Conditions checking: \n"														  \
					"-->No two processes can be simultaneosly inside their critical regions.\n"		  \
					"   No assumptions can be made about speeds or the number of CPUs.\n"			  \
					"   No process running outside its critical region may block other processes.\n"  \
					"   No process should have to wait forever to enter its critical region.\n";
			}
			else if (key == 2) {
				cout <<
					"Conditions checking: \n"
					"   No two processes can be simultaneosly inside their critical regions.\n"		 \
					"-->No assumptions can be made about speeds or the number of CPUs.\n"			 \
					"   No process running outside its critical region may block other processes.\n" \
					"   No process should have to wait forever to enter its critical region.\n";
			}
			else if (key == 3) {
				cout <<
					"Conditions checking: \n"															\
					"   No two processes can be simultaneosly inside their critical regions.\n"			\
					"   No assumptions can be made about speeds or the number of CPUs.\n"				\
					"-->No process running outside its critical region may block other processes.\n"	\
					"   No process should have to wait forever to enter its critical region.\n";
			}
			else if (key == 4) {
				cout <<
					"Conditions checking: \n"
					"   No two processes can be simultaneosly inside their critical regions.\n"		\
					"   No assumptions can be made about speeds or the number of CPUs.\n"				\
					"   No process running outside its critical region may block other processes.\n"	\
					"-->No process should have to wait forever to enter its critical region.\n";
			}
			needUpdate = false;
		}

		code = getch();
		if (code == 105)
		{
			if (key == 1) { key = 4; }
			else { key--; }
			needUpdate = true;
		}
		else if (code == 109)
		{
			if (key == 4) { key = 1; }
			else { key++; }
			needUpdate = true;
		}
	}
	page2(key);
}

void page2(int test)
{
	printf("\e[1;1H\e[2J");

	DekkerLock mutex;

	switch (test)
	{
	case 1:
		test1(mutex);
		break;
	case 2:
		test2(mutex);
		break;
	case 3:
		test3(mutex);
		break;
	case 4:
		test4(mutex);
		break;
	}

	getch();
	page1();
}
