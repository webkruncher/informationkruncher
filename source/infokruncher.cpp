/*
* Copyright (c) Jack M. Thompson WebKruncher.com, exexml.com
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the WebKruncher nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Jack M. Thompson ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Jack M. Thompson BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// GenerateCerts  ca.crt         ca.key         openssl.cnf    server.crt     server.csr
#define HOME "/home/jmt/.ssh/"

#define CA_DIR  "comodo/"
#define CERT_FILE  HOME CA_DIR "jackmthompson_ninja.crt"
#define KEY_FILE  HOME  "jackmthompson.key"

//#define CA_FILE "ca.crt"
#define CA_FILE  "COMODORSADomainValidationSecureServerCA.crt"

#define KEY_PASSWD ""
#include <stdio.h>
#include <time.h>

#include <deque>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
using namespace std;
#include "infossock.h"
using namespace InformationSocket;
#include "tools.h"
using namespace JTools;
#include "Directory.h"

#include <oformat.h>
using namespace OFormat;

#include <sys/ioctl.h>

#include <spinlock.h>

#include "hyper.h"
using namespace Hyper;
volatile bool KILL(false);

#include "securitycheck.h"
//#include "login.h" // Deprecated, unused
#include "User.h"


string SourceTarget(const string& who, const string& what, const string dflt="/index.html")
{
	if (FileExists("src/sys/index.xml")) return "src/sys/index.xml";
	return "notfound";
}

string Target(const string& who, const string& _what, const string dflt="/index.html")
{
	string what(_what);
	const size_t q(what.find("?"));
	if (q!=string::npos) what.erase(q, what.size()-q);
	size_t s(what.find("/"));
	if (s==string::npos) return "notfound";
	size_t e(what.find(" ", s));
	if (s==string::npos) return "notfound";
	const string furl(what.substr(s, e-s));
	const string url((furl=="/")?dflt:furl);
	const string maybe(who+url);
	stringstream ssout;
	ssout << fence << who << fence;
	ssout << what << fence;
	ssout << furl << fence;
	ssout << url << fence;
	ssout << maybe << fence;
	if (FileExists(maybe.c_str())) 
	{
		ssout << aglin << maybe << aglout << fence;
		Log(ssout.str());
		return maybe;
	}
	ssout << brktin << url << brktout << fence ;
	Log(ssout.str());
	return url;
}

string Example(const icstring& request, const Request& req)
{
	string srequest(request.c_str());
		stringstream ssout; ssout << fence << "[EXAMPLE]" << fence << request << fence ; Log(ssout.str());
	const string target(Target("Example", RequestUrl(request.c_str())));
	return target;
}

string Foward2Ninja(const icstring& request, const Request& req)
{
        string srequest(request.c_str());
        stringstream ssout; ssout << fence << "[2NINJA]" << fence << request << fence ; Log(ssout.str());
        return "WebKruncher.text/Fwd2Ninja.html";
}

string Jmt(const icstring& request, const Request& req)
{
        string srequest(request.c_str());
                stringstream ssout; ssout << fence << "[JMT]" << fence << request << fence ; Log(ssout.str());
        if ( request.find("wip.html") != icstring::npos)
        {
                stringstream ssout; ssout << fence << "[WIP]" << fence << request << fence ; Log(ssout.str());
                return "WebKruncher.text/wip.html";
        }
        const string target(Target("WebKruncher.text", RequestUrl(request.c_str())));
        //cout << "<target name="<<target<<" />" << endl;
        stringstream ssout2; ssout2 << fence << "[JMT]" << fence << target << fence ; Log(ssout2.str());
        return target;
}

string Tbd(const icstring& request, const Request& req)
{
	const icstring host(req.Host().c_str());
	{stringstream ssout; ssout << fence << "[TBD]" << fence << host << fence << request.c_str() << fence ; Log(ssout.str());}

                if (request.find("get /test.json ") == 0 )
		{
				
			cerr << "TESTREQUEST:" << request.c_str() << endl;
                        return "WebKruncher.text/test.json";
		}


#if 0
        {
                if (request.find("get / ") == 0 )
                {
			//cerr << "getting the index " << endl;
                        stringstream ssout; ssout << fence << "[SomethingElse]" << fence << request << fence ; Log(ssout.str());
                        return "WebKruncher.text/index.html";
                }
        }
#endif

	return Jmt(request, req);

	//string ret("WebKruncher.text/index.html");
	//return ret;
	//return NULL;
}

struct Response_NotFound : Response
{
	Response_NotFound(Request& _request, int& _status) : request(_request), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;

		string srequest(request.c_str());
		Log(NoBreaks(srequest));

		string file("notfound.html");
		string contenttype("text/html");

		LoadFile(file.c_str(), ss);
		status=404;

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
		stringstream ssout; ssout << fence << "[NOTFOUND]" << fence << file << fence << request.c_str() << fence << request.Headers();  Log(ssout.str());
	}
	protected:
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[NOTFOUND]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence << request.Headers(); return o; }
};

struct Response_Secure : Response
{
	Response_Secure(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;

		string srequest(request.c_str());

		string file(tbd.c_str());
		const string contenttype(ContentType(srequest));


		LoadFile(file.c_str(), ss);

		const string Cookie(request.Cookie());

		cerr << fence << "SECURE" << fence << Cookie << fence << endl;


		status=200;
		if ( Cookie.empty() ) 
		{
			if ( sock.auth & KRUNCH_PERMIT_IP )
				if ( USE_SSL == 1 )
				{
					stringstream sslogin;
					LoadFile("login.html", sslogin);
					ss << sslogin.str();
				}
		} else {
			if ( sock.auth & KRUNCH_PERMIT_IP )
			{
				string UserName;
				const string CookiePath( InformationSocket::CookiePathFromSock( Cookie, sock ) ) ;


				if ( request.ReadPost() )
					if ( request.IsPayloadText() )
					{

						stringmap& form( request.Form() );
						if ( form.size() > 1 )
						{
							const string checkname( form[ "uname" ] );
							const string psw( form[ "psw" ] );

							stringstream ss;
							ss << "|SECURE|ATTEMPT|" << checkname << fence << psw << fence << endl;
							Log (ss.str());

							if ( checkname == "jmt" )
								if ( psw == "pwd" )
									UserName=checkname;
						}
					}

				if ( !UserName.empty() )
				{
					const string UserPath( CookiePath + string( "/user/" ) + UserName );
					const string UserPropertiesFile( UserPath + string( "/properties" ) );
//					JTools::assertpath( UserPath );

					struct stat sb;
					if (stat(UserPropertiesFile.c_str(), &sb)==0)
					{
						stringstream ssout; ssout << fence << "[SECURE][COOKIE]" << fence << "UserPropertiesFileExists" << fence << UserPropertiesFile << fence; 
						Log (ssout.str());
						cerr << ssout.str() << endl;

						ifstream cf( UserPropertiesFile.c_str() );
						while ( ! cf.eof() )
						{
							string line;
							getline( cf, line );
							cerr << line << endl;
						}

					} else {
						JTools::FileLocker flock( UserPropertiesFile, false );
						if ( flock )
						{
							stringstream ssout; ssout << fence << "[SECURE][COOKIE]" << fence << "NewUserPropertiesFile" << fence << UserPropertiesFile << fence; 
							cerr << ssout.str() << endl;
							Log (ssout.str());
							ofstream cf( UserPropertiesFile.c_str() );
							cf << "|Cookie|" << Cookie << fence << endl; 
							cf << "|User|" << UserName << fence << endl; 
						} else {
							stringstream ssout; ssout << fence << "[SECURE][COOKIE]" << fence << "CannotLock" << fence << UserPropertiesFile << fence; 
							cerr << ssout.str() << endl;
							Log (ssout.str());
							stringstream sslogin;
							LoadFile("login.html", sslogin);
							ss << sslogin.str();
						}
					}
				} else {
					stringstream ssout; ssout << fence << "[SECURE][POST]" << fence << "CannotReadPost" << fence; 
					cerr << ssout.str() << endl;
					Log (ssout.str());
					stringstream sslogin;
					LoadFile("login.html", sslogin);
					ss << sslogin.str();
				}
			}

		}

		stringstream response;
	
		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(status) << endl;
		response << "Content-Type: " << contenttype << endl;
		if ( USE_SSL == 1 ) response << "Strict-Transport-Security: max-age=15768000" << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << ss.str().size() << endl;
		response << endl;
		response << ss.str();
		sock.write(response.str().c_str(), response.str().size());
		sock.flush();
		stringstream ssout; ssout << fence << "[SECURE]" << fence << Cookie << file << fence << contenttype << fence; Log(ssout.str());
	}
	protected:
	const string tbd;
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[home]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};

struct Response_Home : Response
{
	Response_Home(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;

		string srequest(request.c_str());

		string file(tbd.c_str());
		const string contenttype(ContentType(srequest));

		const string Cookie(request.Cookie());
		cerr << fence << "HOME" << fence << Cookie << fence << endl;

		status=200;
		if ( USE_SSL == 1 )
		{
			if ( Cookie.empty() ) 
			{
				if ( sock.auth & KRUNCH_PERMIT_IP )
				{
					LoadFile("mustcookie.html", ss );
				} else {
					ss << "Unauthorized access";
					status=403;
				}
			}  else {

				LoadFile(file.c_str(), ss);

				if ( ! Cookie.empty() ) 
					if ( sock.auth & KRUNCH_PERMIT_IP )
						if ( USE_SSL == 1 )
						{
							stringstream sslogin;
							LoadFile("login.html", sslogin);
							ss << sslogin.str();
						}
			}

		} else {
			LoadFile(file.c_str(), ss);
		}



		stringstream response;
		//response << ( ( USE_SSL == 1 ) ? "HTTPS" : "HTTP" ) << "/1.1 ";
		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(status) << endl;
		response << "Content-Type: " << contenttype << endl;
		if ( USE_SSL == 1 ) response << "Strict-Transport-Security: max-age=15768000" << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << ss.str().size() << endl;



		if ( Cookie.empty() )  
		{
			if ( sock.auth & KRUNCH_PERMIT_IP )
			{
				//cout << request.Headers() << endl;


				const string when( request.ExpiryTime() );
				const string ucook( Hyper::UniqCookie() );
				const string CookiePath( InformationSocket::CookiePathFromSock( ucook, sock ) ) ;
				const string CookieFile( CookiePath + string( "/cookie" ) );
				//cerr << "CookiePath: " << CookiePath << endl;	
//				JTools::assertpath( CookiePath );
				//cerr << "CookieFile: " << CookieFile << endl;	

				struct stat sb;
				if (stat(CookieFile.c_str(), &sb)==0)
				{
					stringstream ssout; ssout << fence << "[HOME][COOKIE]" << fence << "CookieFileExists" << fence << CookieFile << fence; 
					Log (ssout.str());
				} else {
					stringstream ssout; ssout << fence << "[HOME][COOKIE]" << fence << "NewCookieFile" << fence << CookieFile << fence; 
					Log (ssout.str());
					ofstream cf( CookieFile.c_str() );
					cf << ucook;
				}


				stringstream ssc;
				ssc<< "Set-Cookie: __Secure-ID=" << ucook << "; Expires=" << when << "; Secure; HttpOnly; Domain=jackmthompson.ninja; Path=/; ";
				response << ssc.str() << endl;
			}
		}
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

struct Response_JMeter : Response
{
	Response_JMeter(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}


#if 0
               ct = "text/html"
                if s.path=='/jmt': ct="text/xml"
                if s.path=='/': ct="text/xml"
                if s.path=='/Assertions': ct="text/xml"
                if s.path=='/xslt': ct="text/xslt"
                if s.path=='/js': ct="text/js"
                if s.path=='/css': ct="text/css"

                if s.path=='/favicon.ico':
                        sys.stderr.write( "Doesnt exist" + s.path + "\n")
                        s.send_response(400)
                        s.send_header("Content-type", ct )
                        s.end_headers()
                        s.wfile.write( "<html>not found</html>" )
                        return

                s.send_response(200)
                s.send_header("Content-type", ct )
                s.end_headers()

                FileName=''

                if s.path=='/jmt': FileName="jmeter/Recording.jmt.test.xml"

                if s.path=='/': FileName="jmeter/Recording.xml"
                if s.path=='/Assertions': FileName="jmeter/Assertions.xml"


                if s.path=='/js': FileName="jmeterreport.js"
                if s.path=='/xslt': FileName="jmeterreport.xslt"
                if s.path=='/css': FileName="jmeterreport.css"
                if s.path=='/htmlreport': FileName="jmeterreport.html"


                if FileName.endswith('.xml' ):
                        LineOne="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                        LineTwo="<?xml-stylesheet type=\"text/xsl\" href=\"/xslt\"?>\n"
                        s.wfile.write( LineOne )
                        s.wfile.write( LineTwo )
#endif

	


	struct Stuff
	{
		stringstream ss;
		string contenttype;
		long status;

		void operator()( icstring req )
		{
			if ( req.find( "get /jmeter/xml " ) == 0 )
			{
				contenttype="text/xml";
				status=200;
				ifstream in( "text/swift/jmeter/Recording.xml" );
				while ( ! in.eof() )
				{
					string line;
					getline( in, line );
					if ( ss.str().empty() )
					{
						ss << line << endl;
						ss << "<?xml-stylesheet type=\"text/xsl\" href=\"/jmeter/xslt\"?>" << endl;
					} else {
						ss << line << endl;
					}
				}

				return;
			}

			if ( req.find( "get /jmeter/xslt " ) == 0  ) 
			{
				contenttype="text/xslt";
				status=200;
				ifstream in( "text/swift/jmeterreport.xslt" );
				while ( ! in.eof() )
				{
					string line;
					getline( in, line );
					ss << line << endl;
				}

				return;
			}
		}
	} stuff;


	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;

		string srequest(request.c_str());
		icstring icsrequest(request.c_str());

		stuff( icsrequest );

		stringstream response;
		//response << ( ( USE_SSL == 1 ) ? "HTTPS" : "HTTP" ) << "/1.1 ";
		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(stuff.status) << endl;
		response << "Content-Type: " << stuff.contenttype << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << stuff.ss.str().size() << endl;
		response << endl;
		response << stuff.ss.str();
		sock.write(response.str().c_str(), response.str().size());
		sock.flush();
		stringstream ssout; ssout << fence << "[HOME]" << fence  ; Log(ssout.str());
	}
	protected:
	const string tbd;
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[home]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};

struct Response_Posted : Response
{
	Response_Posted(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;
		ss << tbd;

		string srequest(request.c_str());

		const string contenttype("text/html");

		status=200;

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
		stringstream ssout; ssout << fence << "[HOME]" << fence ; Log(ssout.str());
	}
	protected:
	const string tbd;
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[home]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};

struct Response_Ping : Response
{
	Response_Ping(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ss;

		string srequest(request.c_str());

		string contenttype("text/xml");

		ss<<"<Ping><pong></pong></Ping>";
		status=200;

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
	}
	protected:
	const string tbd;
	Request& request;
	int& status;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[home]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
	string PngTxt()
	{
		stringstream ss;
		string cmd="top | grep infokruncher | tr -s ' '  | cut -d ' ' -f 1,5,6,10";
		ss << runpipe(cmd);

		return ss.str();
	}
};



struct Response_Binary : Response
{
	Response_Binary(Request& _request, const string& _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();


		stringstream ss;

		string srequest(request.c_str());

		const string contenttype(ContentType(srequest));
		status=200;
		const size_t fsize(FileSize(tbd));

		stringstream response;
		//response << ( ( USE_SSL == 1 ) ? "HTTPS" : "HTTP" ) << "/1.1 ";
		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(status) << endl;
		response << "Content-Type: " << contenttype << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << fsize << endl;
		response << endl;

		response << ss.str();
		sock.write(response.str().c_str(), response.str().size());
		sock.flush();

		WriteBinary(tbd, fsize, sock.GetSock(), sock);
	}
	protected:
	Request& request;
	int& status;
	const string tbd;
	private:


	void WriteBinary(const string fname, const size_t fsize, int sock, Socket& sockt)
	{
		//cout << "Writing binary file " << fname << " " << fsize << " bytes, to " << sock << endl;
		string pathname("text/");
		pathname+=fname;
		//stringstream ssout; ssout << fence << "[loadblob]" << pathname << fence << (int) fsize << fence; Log(ssout.str());
		ifstream in(pathname.c_str(), ios::binary);
		string line;
		Characters chars(1<<14);
		const size_t Sz(chars);

		unsigned char* buffer(chars);

		size_t togo(fsize);
		size_t bwrit(0);
		size_t unsent(0);
		double pcnt;
		while (!in.fail()) 
		{
			if (togo < 1) return;
			memset(buffer, 0, Sz);
			if (in.eof()) return;
			const size_t much((togo>Sz) ? Sz : togo);
			in.read((char*)buffer, much);
			if (!in) continue;
			const int sent(
				( USE_SSL==1 ) ?  
					SSL_write(sockt.GetSsl(), buffer, much )
						: 
					send(sock, buffer, much, 0)
			);
			if (sent!=-1) 
			{
				togo-=sent;
				bwrit+=sent;
				unsent=much-sent;
				pcnt=(((double)sent/(double)much)*100);
			}
			//if (pcnt != 100.00) cerr << "wrote " << setw(8) << sent << " bytes, leaving " << unsent << " bytes, which is " << pcnt << "% of " << much << " requested.   ttl sent: " << setw(8) << bwrit << cleol << crgret << flsh;
		}
	}
	virtual ostream& operator<<(ostream& o) const { o << fence << "[binary]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};

struct Response_Page : Response
{
	Response_Page(Request& _request, const string _tbd, int& _status) : request(_request), tbd(_tbd), status(_status) {}
	virtual void operator ()()
	{
		Socket& sock(request);
		sock.flush();
		stringstream ssplaintext, sssecure;

		string srequest(request.c_str());
		string file(tbd.c_str());
		string contenttype(ContentType(srequest));
		{stringstream ssout; ssout << fence << "[PAGESTART]" << "cookie" << fence << contenttype << fence << tbd << fence; Log(ssout.str());}

		if (FileExists(file.c_str())) 
		{
			if ( 
				( contenttype == "text/plain" )
					&&
				( file.find("ajax/Everything.js") != string::npos )
			)
			{
				{stringstream ssout; ssout << fence << "[AUTHED]" << fence << contenttype << fence << tbd << fence << ssplaintext.str() << fence ; Log(ssout.str());}
			}
			//cerr << "Loading: " << file << endl;
			LoadFile(file.c_str(), ssplaintext);
		} else {LoadFile("notfound.html",ssplaintext); contenttype="text/html";}

		const string Cookie(request.Cookie());
		//cerr << fence << "PAGE" << fence << Cookie << fence << endl;
			

		const string CookiePath( InformationSocket::CookiePathFromSock( Cookie, sock ) ) ;
		const string CookieFile( CookiePath + string( "/cookie" ) );
		//JTools::assertpath( CookiePath );

		struct stat sb;
#if 0
		if (stat(CookieFile.c_str(), &sb)==0)
		{
			stringstream ssout; ssout << fence << "[PAGE][COOKIE]" << fence << "CookieFileExists" << fence << CookieFile << fence; 
			//cerr << ssout.str() << endl;
			Log (ssout.str());
			//cerr << ssout.str() << endl;
			ifstream cf( CookieFile.c_str() );
			while ( ! cf.eof() )
			{
				string line;
				getline( cin, line );
				//cerr << line << endl;
			}
		}
#endif

		if ( Cookie.empty() ) 
		{
			if ( sock.auth & KRUNCH_PERMIT_IP )
				if ( USE_SSL == 1 )
				{
					stringstream sslogin;
					LoadFile("mustcookie.html", sslogin);
					sssecure << sslogin.str();
				}
		} else {
			if ( sock.auth & KRUNCH_PERMIT_IP )
				if ( USE_SSL == 1 )
					if ( file == "WebKruncher.text/Left.xml" )
					{
						string placer( ssplaintext.str() );
						size_t where( placer.find("<!-- login  -->") );
						stringstream sslogin;
						if ( sock.auth & KRUNCH_PERMIT_LOGGEDIN )
						{
							LoadFile("WebKruncher.text/UserPrefMenu.xml", sslogin);
							//cerr << "Loaded Left For Logged In connection" << endl << placer << endl;
						} else {
							LoadFile("WebKruncher.text/LoginMenu.xml", sslogin);
							//cerr << "Loaded Left For Login for permissioned ssh connection" << endl << placer << endl;
						}
						placer.insert( where, sslogin.str() );
						sssecure << placer; 
					}
		}




		status=200;



		stringstream ss;
		if ( sssecure.str().size() )
			ss << sssecure.str();
		else 
			ss << ssplaintext.str();

			

		stringstream response;

		response <<  "HTTP" << "/1.1 ";
		response << status << " " << statusText(status) << endl;
		response << "Content-Type: " << contenttype << endl;
		if ( USE_SSL == 1 ) response << "Strict-Transport-Security: max-age=15768000" << endl;
		response << "Server: WebKruncher" << endl;
		response << "Connection: close" << endl;
		response << "Content-Length:" << ss.str().size() << endl;
		response << endl;
		response << ss.str();
//if ( tbd == "WebKruncher.text/test.json" )
//	cerr << response.str() << endl;
		sock.write(response.str().c_str(), response.str().size());
		sock.flush();
		stringstream ssout2; ssout2 << fence << "[PAGEEND]" << fence << Cookie << file << fence << contenttype << fence; Log(ssout2.str());
	}
	protected:
	Request& request;
	int& status;
	const string tbd;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[page]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};


void trigger()
{
	ofstream oo( "/home/fabric/info.cron", ios::app );
	time_t now( time( NULL ) );
	oo << "Triggered by the kruncher @ " << asctime( localtime( &now ) );
}

struct RequestManager : Request
{
	RequestManager(const icstring& _request, const icstringvector& _headers, Socket& _sock) :
		Request(_request, _headers, _sock) {}
	operator Response* ()
	{
		Response* ret(NULL);
		{
			if (request.find("GET /trigger") == 0 )
				trigger();

			if (request.find("POST /contract") == 0 )
			{
				string slen( value("content-length") );
				long len( atol( slen.c_str() ) );
				
				unsigned char* dest( ( unsigned char * ) malloc( len )  );
				if ( ! dest ) return new Response_Posted(*this, "Cannot allocate memory for post data", status);
				sock.read( (char*) dest, len );

				basic_string<unsigned char> buffer;
				buffer.assign( dest, len );
				free( dest );

				const size_t eoch( buffer.find_first_of( ( unsigned char * ) "\r\n" ) );
				if ( eoch == string::npos ) return new Response_Posted(*this, "Cannot find first post data tag", status);

				string tag;
				tag.assign( ( char * ) buffer.data(), eoch ); 
				if ( tag.size() < 3 ) return new Response_Posted(*this, "invalid content tag", status);

				basic_string<unsigned char> content;

				const size_t etag( buffer.find( ( unsigned char * ) tag.data(), eoch ) );
				if ( etag == string::npos ) return new Response_Posted(*this, "Cannot find second post data tag", status);

				content.assign( (unsigned char*) buffer.data()+eoch, etag-eoch );

				size_t eolen( 4 );
				size_t eoh( content.find( ( unsigned char* ) "\r\n\r\n" ) );
				if ( eoh==string::npos) { eolen=2; eoh=( content.find( ( unsigned char* ) "\r\r" ) ); }
				if ( eoh==string::npos) { eolen=2; eoh=( content.find( ( unsigned char* ) "\n\n" ) ); }
				if ( eoh==string::npos) return new Response_Posted(*this, "Cannot find end of content mime", status);


				eoh+=eolen;
				basic_string<unsigned char> payload;
				payload.assign( content.data()+eoh, content.size()-eoh-2 );
				ofstream oo( "posted" );
				oo.write( ( char* ) payload.data(), payload.size() );



				stringstream sstbd;
				sstbd << "Request processing" << endl;
				
				stringstream ssout; ssout << fence << "[ContractPost]" << fence ; Log(ssout.str());
				return new Response_Posted(*this, sstbd.str(), status);
			}
		}
		const string tbd(Tbd(request, *this));
		ret=ifBinary(tbd);
		if (ret) return ret;
		if (request.find("/?ping ")!=string::npos) return new Response_Ping(*this, tbd, status);
		if (request.find("GET / ")==0) return new Response_Home(*this, tbd, status);
		if (request.find("POST / ")==0) return new Response_Secure(*this, tbd, status);
		if (tbd.empty()) return new Response_NotFound(*this, status);
		else 
		{
			//cout << "Using ResponsePage"<<endl;
			return new Response_Page(*this, tbd, status);
		}
		return new Response_NotFound(*this, status);
	}
	private:
	Response* ifBinary(const string tbd)
	{
		if (request.find("GET /")==0) 
		{
			if (request.find(".ico ")!=string::npos) return new Response_Binary(*this, tbd, status);
			if (request.find(".png ")!=string::npos) return new Response_Binary(*this, tbd, status);
			if (request.find(".jpg ")!=string::npos) return new Response_Binary(*this, tbd, status);
			if (request.find(".gif ")!=string::npos) return new Response_Binary(*this, tbd, status);
		}
		return NULL;
	}

	virtual ostream& operator<<(ostream& o) const
	{
		if (status==0) {o << fence << "[nostatus]" << fence ; Request::operator<<(o); o<<fence; return o;}
		if (status==200) {o << fence << "[ok]" << fence ; Request::operator<<(o); o<<fence; return o;}
		o << "["<<status<< "]"<< fence << "?" << fence  << RequestUrl(request.c_str()) << fence; Request::operator<<(o); 
		o << fence;
		return o;
	}
	int status;
};


struct SockQ
{
	SockQ() { lock.Init(); }
	void operator+=(int j){socks.push_back(j);}
	operator int ()
	{ 
		if (socks.empty()) return 0;
		const int j(socks.front());
		socks.pop_front();
		return j;
	}
	Locker lock;
	deque<int> socks;
};



void* service(void* lk)
{
        UserRegistry UserPermissions;


	{const int T((rand()%80)+300); usleep(T);}
	SockQ* qp((SockQ*)lk);
	SockQ& Q(*qp);
	Locker& lock(Q.lock);
	stringstream ssexcept;
	try
	{
		
		while (!KILL)
		{

			int sock(0);
			{
				ThreadWidget locker(lock);
				sock=Q;
			}

			if (!sock) 
			{
				const int T((rand()%40)+10); 
				usleep(T); 
				continue;
			}


			const bool UsingSsl( USE_SSL==1 );
			//cerr << "Serving: " << ( UsingSsl ? "SSL" : "PLAINTEXT" ) << endl; cerr.flush();

			SSL_CTX* ctx( create_context( UsingSsl ) );

			if ( ( UsingSsl ) && ( ! ctx ) ) continue;

			STACK_OF(X509_NAME) * x509names( configure_context( ctx ) ); 
			if ( ( UsingSsl ) && ( x509names == NULL ) ) throw string( "Cannot get x509 names" );

			SSL* ssl( ( UsingSsl ) ? SSL_new(ctx) : NULL );

			if ( UsingSsl )
			{
				SSL_set_fd(ssl, sock);
				if (SSL_accept( ssl ) <= 0) 
				{
					ERR_print_errors_fp(stderr);
					continue;
				}
			}


			Socket ss( sock, ssl );

			SslContext sslcontext( ctx, ssl, ss );
                        const string sslmsg( ( UsingSsl )  ? "USING SSL" : "USING PLAIN TEXT" );
                        Log( string("From ") + ss.dotted()  +  sslmsg );


			ss.timeout(8, 0);
			ss.blocking(true);

			icstringvector headers;
			icstring requestline;


			while (true)
			{
				string line;
				ss.getline(line);
				if (line.empty()) break;
				if (requestline.empty()) requestline=line;
				else headers.push_back(line.c_str());
			}



                        Log( string("Splitting ") + ss.dotted() );
                        stringvector ipaddr;
                        ipaddr.split( ss.dotted(), ":" );
                        Log( string("Checking ") + ss.dotted() );
                        if ( ipaddr.size() < 1 )
                        {
                                Log( string("\033[31m") + ss.dotted() + string(" has no : \033[0m") );
                                continue;
                        }
                        const string addr( ipaddr[ 0 ] );

                        ss.auth=UserPermissions( addr );
			if ( ss.auth == KRUNCH_IP_BLOCKED )
			{
				cerr << "THIS IP IS BLOCKED" << endl;
				ss.close();
				{const int T((rand()%50)+10); usleep(T); }
				continue;
			}



			{
				RequestManager request(requestline, headers, ss);
				if (!request.resolve()) 
				{
					ss.close();
					return NULL;
				}

				Response* rq(request);
				if (!rq) 
				{
					ss.close();
					return NULL;
				}
				auto_ptr<Response> respond(rq);
				Response& response(*respond.get());
				response();
			}

			ss.close();
			{const int T((rand()%100)+1000); usleep(T); }
		}
	}
	catch (char* e) {ssexcept<<e;}
	catch (string& e) {ssexcept<<e;}
	catch (exception& e) {ssexcept<<e.what();}
	catch (...) {ssexcept<<"unknown exception";}
	if (!ssexcept.str().empty()) { cerr << red << "dumbservice: " << Abrieviate(ssexcept.str()) << normal << endl; cerr.flush(); }
	if (!ssexcept.str().empty()) //Log(ssexcept.str());
	{
		cout << red << ssexcept.str()<<endl;
		stringstream ssout; ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); Log(ssout.str());
	}
	return NULL;
}

int main(const int argc, const char** argv)
{
	cout << "Starting infokruncher." << SERVICE_PORT << endl;
	CmdLine cmdline(argc, argv);
	if (!cmdline.exists("-d")) cerr << "Daemonizing infokruncher" << endl;
	else cerr << "Loading infokruncher in test mode" << endl;
	if (!cmdline.exists("-d")) SetSignals();
	if (!cmdline.exists("-d")) Daemonizer daemon(argv[0]);
	if (cmdline.exists("-sc")) 
		return SecurityCheck::main( cmdline );
	srand(time(0));
	stringstream ssexcept;
	try
	{
		init_openssl();
		SockQ Q;
		Locker& lock(Q.lock);

		streamingsocket ssrvc("", SERVICE_PORT );
		if (!ssrvc.open()) throw "Cannot open";
		ssrvc.blocking(true);
		ssrvc.timeout(2,0);

		if (!ssrvc.listen()) throw "Cannot listen";

		vector<pthread_t> threads;
		pthread_t t(NULL);
		for (int j=0;j<8;j++)
		{
			pthread_create(&t,0,service,&Q);
			if (!t) throw "Can't thread";
			threads.push_back(t);
		}

		while (!KILL)
		{
			const int incoming(ssrvc.accept());
			ThreadWidget locker(lock);
			Q+=incoming;
			{const int T((rand()%10)+20); usleep(T); }
		}

		Log("joining");

		for (vector<pthread_t>::iterator it=threads.begin();it!=threads.end();it++)
		{
			pthread_t t(*it);
			pthread_join(t, 0);
		}
		Log("exiting");
		cleanup_openssl();
	}
	catch (const char* e) {ssexcept<<e;}
	catch (char* e) {ssexcept<<e;}
	catch (string& e) {ssexcept<<e;}
	catch (exception& e) {ssexcept<<e.what();}
	catch (...) {ssexcept<<"unknown exception";}
	if (!ssexcept.str().empty()) cerr << red << Abrieviate(ssexcept.str()) << normal << endl;
	return 0;
}

// wip

