#include "xsix/buffer.h"

#include <iostream>
using namespace std;

int main()
{
	xsix::buffer buf("helloworld");
	cout << buf.retrieve_all_as_string() << endl;
	return 0;
}