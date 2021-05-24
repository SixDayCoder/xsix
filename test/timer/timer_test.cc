#include "asio.hpp"
#include "xsix/time/timestamp.h"

#include <iostream>
#include <random>
#include <chrono>
using namespace std;


struct timer_callback
{
	void operator() (const asio::error_code& ec)
	{
		cout << "timer end : " << xsix::Timestamp::now().unix() << endl;
	}
};


asio::io_context context;

void ticker(asio::steady_timer* t, int* count, const asio::error_code& ec)
{
	cout << "now : " << xsix::Timestamp::now().unix() << endl;
	if ((*count)++ < 5)
	{
		cout << *count << endl;
		t->expires_at(t->expiry() + std::chrono::seconds(1));
		t->async_wait(std::bind(ticker, t, count, std::placeholders::_1));
	}
}

class Printer
{
public:

	Printer(asio::io_context& ctx) :
			m_strand(asio::make_strand(ctx)),
			m_timerA(ctx, asio::chrono::seconds(1)),
			m_timerB(ctx, asio::chrono::seconds(1))
	{

		//m_timerA.async_wait(std::bind(&Printer::printA, this));
		//m_timerB.async_wait(std::bind(&Printer::printB, this));

		m_timerA.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::printA, this)));
		m_timerB.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::printB, this)));

		//asio::ip::tcp::socket s(ctx);

		char buf[1024] = { 0 };
		asio::ip::tcp::socket s(ctx);
		std::future<std::size_t> f = asio::async_write(s, asio::buffer(buf), asio::use_future);
	}

	~Printer()
	{
		cout << "final count is " << m_count << endl;
	}


public:

	void printA()
	{
		if (m_count < 10)
		{
			cout << "timerA : " << m_count << endl;
			m_count++;

			m_timerA.expires_at(m_timerA.expiry() + asio::chrono::seconds(1));
			m_timerA.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::printA, this)));
		}
	}

	void printB()
	{
		if (m_count < 10)
		{
			cout << "timerB : " << m_count << endl;
			m_count++;

			m_timerB.expires_at(m_timerB.expiry() + asio::chrono::seconds(1));
			m_timerB.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::printB, this)));
		}
	}

private:

	asio::strand<asio::io_context::executor_type> m_strand;
	asio::steady_timer m_timerA;
	asio::steady_timer m_timerB;
	int m_count = 0;

};

int main()
{
	//asio::steady_timer t(context, std::chrono::seconds(1));
	//int count = 0;
	//t.async_wait(std::bind(ticker, &t, &count, std::placeholders::_1));
	//context.run();
	//cout << "finished!" << endl;

	asio::io_context ctx;
	Printer p(ctx);

	std::thread t([&]() { ctx.run(); });
	ctx.run();

	t.join();
	
	return 0;
}