

// DEPRECATED

#ifndef RESPONSE_LOGIN_H
#define RESPONSE_LOGIN_H
struct Response_Login : Response
{
	Response_Login(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss, ssh;

		string srequest(request.c_str());

		cerr<<"Request from " << request.value("Host") << endl;	
		cerr << ( ( USE_SSL == 1 ) ? "Secure" : "Plain text" ) << endl;


		if ( USE_SSL )
		{
			if ( ! ( request.sock.auth & KRUNCH_PERMIT_IP ) )
			{
				cerr << "Unauthorized IP asked for login " <<endl;
				LoadFile("WebKruncher.text/nologin.html", ss);
			} else {
				cerr << "Authorized IP asked for login " <<endl;
				LoadFile("WebKruncher.text/login.html", ss);
			}
			status=200;
		} else {
			ss << "<html>Please find the login page at <a href=\"https://jackmthompson.ninja/login\">JackMThompson.ninja</a></html>" << endl;
			status=302;
			ssh << "Location: https://jackmthompson.ninja/login" << endl;
		}

		string file(tbd.c_str());
		const string contenttype(ContentType(srequest));




		stringstream response;
		//response << ( ( USE_SSL == 1 ) ? "HTTPS" : "HTTP" ) << "/1.1 ";
		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(status) << endl;
		response << "Content-Type: " << contenttype << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << ss.str().size() << endl;
		response << endl;
		response << ss.str();
		sock.write(response.str().c_str(), response.str().size());
		sock.flush();
		stringstream ssout; ssout << fence << "[HOME]" << fence << file << fence << contenttype << fence; Log(ssout.str());
	}
	protected:
	const string tbd;
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[home]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};
#endif // RESPONSE_LOGIN_H

