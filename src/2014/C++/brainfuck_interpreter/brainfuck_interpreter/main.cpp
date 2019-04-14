
#include <cstdio>
#include <memory>

using namespace std;

#define MAX_TAPE_LENGTH 30000

int main(int argc, char** argv)
{

	char sourcecode[8192] =
		"+++++++++[>++++++++>+++++++++++>+++++<<<-]>.>++.+++++++..+++.>-.------------.<++++++++.--------.+++.------.--------.>+.";
		//"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++. "
		//">>>                          "
		//"+++++++++++++++++++++++++    "
		//"[                            "
		//"<<<                          "
		//"[>+>+<<-]>>[<<+>>-]          "
		//"<+.                          "
		//">>-                          "
		//"[>+<-]                       "
		//">                            "
		//"]";
	char optimize_sourcecode[8192] = {0};

	int src_pos = 0;
	int dst_pos = 0;
	while (sourcecode[src_pos])
	{
		char c = sourcecode[src_pos++];
		if (c != '+' &&
			c != '-' &&
			c != '>' &&
			c != '<' &&
			c != '[' &&
			c != ']' &&
			c != '.' &&
			c != ',')
		{
		}
		else
		{
			optimize_sourcecode[dst_pos++] = c;
		}
	}

	unsigned char* tape = new unsigned char[MAX_TAPE_LENGTH];
	memset(tape, 0, MAX_TAPE_LENGTH);
	unsigned char* tape_ptr = tape;
	int level = 0;

	char* src_ptr = optimize_sourcecode;
	while (*src_ptr)
	{
		if (*src_ptr == '+')
		{
			(*tape_ptr)++;
		}
		else if (*src_ptr == '-')
		{
			(*tape_ptr)--;
		}
		else if (*src_ptr == '[')
		{
			if (*tape_ptr)
			{
				level++;
			}
			else
			{
				while (*(++src_ptr) != ']'){}
				src_ptr++;
				level--;
				continue;
			}
		}
		else if (*src_ptr == ']')
		{
			while (*(--src_ptr) != '['){}
			continue;
		}
		else if (*src_ptr == '>')
		{
			tape_ptr++;
		}
		else if (*src_ptr == '<')
		{
			tape_ptr--;
		}
		else if (*src_ptr == '.')
		{
			putchar(*tape_ptr);
		}
		else
		{
		}
		src_ptr++;
	}

	tape_ptr = tape;
	while (*tape_ptr)
	{
		int i = (int)*tape_ptr;
		char c[2] = { 0 };
		itoa(i, c, 10);
		putchar(c[0]);
		tape_ptr++;
	}
	putchar('\n');

	delete[] tape;

	return 0;
}
