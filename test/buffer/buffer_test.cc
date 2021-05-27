#include "xsix/buffer.hpp"
#include <assert.h>

#include <iostream>
#include <random>
using namespace std;

#define ASSERT_STRING_EQUAL(_buf_, _pattern_)\
{\
	assert(strcmp(_buf_, _pattern_) == 0);\
}\

#define ASSERT_CONDITION(_buffer_inst, _head, _tail, _len) \
{\
	assert(_buffer_inst.headpos() == _head);\
	assert(_buffer_inst.tailpos() == _tail);\
	assert(_buffer_inst.length() == _len);\
	cout << __LINE__ << " : " << _buffer_inst.get_debug_text() << endl;\
}\

std::string build_x_string(int cnt)
{
	std::string x;
	for (int i = 0; i < cnt; ++i) {
		x.push_back('x');
	}
	return x;
}

int main()
{
	char tmp[1024];

	{
		//test build from simple string
		cout << "********==============test for simple====================*********" << endl;

		const char* s = "helloworld";
		xsix::buffer buf(s);
		ASSERT_CONDITION(buf, 0, strlen("helloworld"), strlen("helloworld"));
	}

	{
		//test write_to
		cout << "\n********==============test for write_to====================*********" << endl;

		const char* s = "0123456789";
		xsix::buffer buf(s);
		ASSERT_CONDITION(buf, 0, 10, 10);

		memset(tmp, 0, sizeof(tmp));
		buf.write_to(tmp, 5);
		ASSERT_STRING_EQUAL(tmp, "01234");
		ASSERT_CONDITION(buf, 5, 10, 5);
		
		memset(tmp, 0, sizeof(tmp));
		buf.write_to(tmp, 4);
		ASSERT_STRING_EQUAL(tmp, "5678");
		ASSERT_CONDITION(buf, 9, 10, 1);

		buf.append("abcdefghij", strlen("abcdefghij"));
		ASSERT_STRING_EQUAL(buf.retrieve_all_as_string().c_str(), "9abcdefghij");
		ASSERT_CONDITION(buf, 9, 5, 11);

		memset(tmp, 0, sizeof(tmp));
		buf.write_to(tmp, 10);
		ASSERT_STRING_EQUAL(tmp, "9abcdefghi");
		ASSERT_CONDITION(buf, 4, 5, 1);
		ASSERT_STRING_EQUAL(buf.retrieve_all_as_string().c_str(), "j");
	}

	{
		//test append
		cout << "\n********==============test for append====================*********" << endl;

		xsix::buffer buf("1234");
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, "1234");
		ASSERT_CONDITION(buf, 0, 4, 4);

		buf.append("567", 3);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, "1234567");
		ASSERT_CONDITION(buf, 0, 7, 7);

		buf.append(build_x_string(58).c_str(), 58);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, buf.retrieve_all_as_string().c_str());
		ASSERT_CONDITION(buf, 0, 65, 65);

		memset(tmp, 0, sizeof(tmp));
		buf.write_to(tmp, 50);
		ASSERT_STRING_EQUAL(tmp, (std::string("1234567") + build_x_string(43)).c_str());
		ASSERT_CONDITION(buf, 50, 65, 15);

		buf.append(build_x_string(100).c_str(), 100);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, build_x_string(115).c_str());
		ASSERT_CONDITION(buf, 50, 37, 115);

		buf.append(build_x_string(12).c_str(), 12);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, build_x_string(127).c_str());
		ASSERT_CONDITION(buf, 50, 49, 127);

		buf.append(build_x_string(1).c_str(), 1);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, build_x_string(128).c_str());
		ASSERT_CONDITION(buf, 50, 50, 128);

		buf.append(build_x_string(1).c_str(), 1);
		memset(tmp, 0, sizeof(tmp));
		buf.peek(tmp, buf.length());
		ASSERT_STRING_EQUAL(tmp, build_x_string(129).c_str());
		ASSERT_CONDITION(buf, 0, 129, 129);
	}

	getchar();

	return 0;
}