#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
using boost::asio::ip::tcp;
using boost::asio::ip::address;

class session:public boost::enable_shared_from_this<session>
{
public:
	session(boost::asio::io_service &io_service):socket_(io_service){}
	~session()
	{
		std::cout << "destruct of session: " << this << std::endl;
	}
	tcp::socket &get_socket() { return socket_;}
	void start()
	{
		std::cout << __func__ << std::endl;
		std::cout << this << std::endl;
		
		auto self = shared_from_this();
		boost::asio::async_read(socket_, sbuf_, std::bind(&session::handle_read, self, std::placeholders::_1, std::placeholders::_2));	
	}
	void handle_read(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
		std::cout << __func__ << std::endl;
		boost::asio::async_write(socket_, sbuf_, std::bind(&session::handle_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));	
	}
	void handle_write(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
		std::cout << __func__ << std::endl;
		boost::asio::async_read(socket_, sbuf_, std::bind(&session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));	
	}
private:
	tcp::socket socket_;
	boost::asio::streambuf sbuf_;
};

class echo_server
{
public:
	echo_server(boost::asio::io_service &io_service, tcp::endpoint &endpoint):
			io_service_(io_service), endpoint_(endpoint), socket_(io_service), acceptor_(io_service,endpoint)
	{
	}
	void start()
	{
		std::shared_ptr<session> newsession(new session(io_service_));	
		acceptor_.async_accept(newsession->get_socket(), std::bind(&echo_server::handle_accept, this, newsession, std::placeholders::_1));
	}
	void handle_accept(std::shared_ptr<session> session_ptr, const boost::system::error_code& error)
	{
		if(error)
		{
			return;
		}
		std::cout << "accept from " << session_ptr->get_socket().remote_endpoint().address() << ":" << session_ptr->get_socket().remote_endpoint().port() << std::endl;
		session_ptr->start();
		session_ptr.reset(new session(io_service_));	
		acceptor_.async_accept(session_ptr->get_socket(), std::bind(&echo_server::handle_accept, this, session_ptr, std::placeholders::_1));
	}
private:	
	boost::asio::io_service &io_service_;
	tcp::endpoint &endpoint_;
	tcp::socket socket_;
	tcp::acceptor acceptor_;
};
void print_accept(tcp::socket& local_socket, const boost::system::error_code& error)
{
}
int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	tcp::endpoint endpoint(address::from_string("127.0.0.1"), 10028);

	echo_server echo_server(io_service, endpoint);
	echo_server.start();

	io_service.run();
	return 0;
}
