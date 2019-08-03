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


#define HOME "/home/jmt/csr/"

#define CERT_FILE  HOME "server.crt"
#define KEY_FILE  HOME  "server.key"

#define CA_FILE "/home/jmt/csr/RapidSSL.pem"
#define CA_DIR  NULL

#define KEY_PASSWD ""

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

#include <oformat.h>
using namespace OFormat;

#include <sys/ioctl.h>

#include <spinlock.h>

#include "hyper.h"
using namespace Hyper;
volatile bool KILL(false);




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



string Tbd(const icstring& request, const Request& req)
{
	const icstring host(req.Host().c_str());
	{stringstream ssout; ssout << fence << "[TBD]" << fence << host << fence << request.c_str() << fence ; Log(ssout.str());}

	const string target(Target("WebKruncher.text", RequestUrl(request.c_str())));
	return target;

	if 
	(
		(host.find("example.ninja")!=string::npos) 
	)
	{ 
		return Example(request, req);
	}
	if (host.find("localhost")!=string::npos) return Example(request, req);


        {
                if (request.find("get / ") == 0 )
                {
                        stringstream ssout; ssout << fence << "[SomethingElse]" << fence << request << fence ; Log(ssout.str());
                        return "WebKruncher.text/wip.html";
                }
                return Example(request, req);
        }


	string ret("index.html");
	return ret;
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
		response << ( ( USE_SSL == 1 ) ? "HTTP" : "HTTP" ) << "/1.1 ";
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

		LoadFile(file.c_str(), ss);
		status=200;

		stringstream response;
		response << ( ( USE_SSL == 1 ) ? "HTTP" : "HTTP" ) << "/1.1 ";
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
		response << ( ( USE_SSL == 1 ) ? "HTTP" : "HTTP" ) << "/1.1 ";
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
		response << ( ( USE_SSL == 1 ) ? "HTTP" : "HTTP" ) << "/1.1 ";
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
			if (pcnt != 100.00) cerr << "wrote " << setw(8) << sent << " bytes, leaving " << unsent << " bytes, which is " << pcnt << "% of " << much << " requested.   ttl sent: " << setw(8) << bwrit << cleol << crgret << flsh;
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
		stringstream ss;

		string srequest(request.c_str());
		string file(tbd.c_str());
		string contenttype(ContentType(srequest));
		{stringstream ssout; ssout << fence << "[PAGE]" << fence << contenttype << fence << tbd << fence; Log(ssout.str());}
		status=200;
		if (FileExists(file.c_str())) 
		{
			if ( 
				( contenttype == "text/plain" )
					&&
				( file.find("ajax/Everything.js") != string::npos )
			)
			{
				{stringstream ssout; ssout << fence << "[AUTHED]" << fence << contenttype << fence << tbd << fence << ss.str() << fence ; Log(ssout.str());}
			}
			LoadFile(file.c_str(), ss);
		} else {LoadFile("notfound.html",ss); contenttype="text/html";}



		stringstream response;
		response << ( ( USE_SSL == 1 ) ? "HTTP" : "HTTP" ) << "/1.1 ";
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
	Request& request;
	int& status;
	const string tbd;
	private:
	virtual ostream& operator<<(ostream& o) const { o << fence << "[page]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
};

struct RequestManager : Request
{
	RequestManager(const icstring& _request, const icstringvector& _headers, Socket& _sock) :
		Request(_request, _headers, _sock) {}
	operator Response* ()
	{
		Response* ret(NULL);
		const string tbd(Tbd(request, *this));
		ret=ifBinary(tbd);
		if (ret) return ret;
		//if (request.find("GET /src ")==0) return new Response_SrcSys(*this, tbd, status);
		if (request.find("/?ping ")!=string::npos) return new Response_Ping(*this, tbd, status);
		if (request.find("GET / ")==0) return new Response_Home(*this, tbd, status);
		if (tbd.empty()) return new Response_NotFound(*this, status);
		else return new Response_Page(*this, tbd, status);
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

struct IpRegistry : map<string, unsigned long>
{
        bool operator()(const string addr)
        {
                iterator it( find( addr ) );
                if ( it == end() ) return false;
                it->second++;
                return true;
        }
};



void* service(void* lk)
{

        IpRegistry PermissionedIps;
        //PermissionedIps["127.0.0.1"] = 0;
        //PermissionedIps["149.134.173.200"] = 0;
        //PermissionedIps["69.243.17.71"] = 0;
        //PermissionedIps["98.233.128.99"] = 0;


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

                        if ( PermissionedIps( addr ) )
                        {
                                stringstream ssout; ssout << fence << "[PERMISSIONEDIP]" << fence << addr << fence; Log (ssout.str());
				ss.auth|=KRUNCH_PERMIT_IP;
                        } else {
                                stringstream ssout; ssout << fence << "[OTHERIP]" << fence << addr << fence; Log (ssout.str()); 
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
		cerr << red << ssexcept.str()<<endl;
		stringstream ssout; ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); Log(ssout.str());
	}
	return NULL;
}

int main(const int argc, const char** argv)
{
	cerr << "Starting infokruncher." << SERVICE_PORT << endl;
	SetSignals();
	CmdLine cmdline(argc, argv);
	if (!cmdline.exists("-d")) cerr << "Daemonizing infokruncher" << endl;
	else cerr << "Loading infokruncher in test mode" << endl;
	if (!cmdline.exists("-d")) Daemonizer daemon(argv[0]);
	SetSignals();
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


