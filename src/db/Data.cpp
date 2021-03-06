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

#define SERVICE_PORT 99

#include <deque>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
using namespace std;
#include "../infosock.h"
using namespace InformationSocket;
#include "../tools.h"
using namespace JTools;

#include "../oformat.h"
using namespace OFormat;

#include <sys/ioctl.h>

#include "../spinlock.h"

volatile bool KILL(false);

#include <signal.h>
#include "dataset.h"
using namespace DataSet;

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <deque>
using namespace std;
typedef string stringtype;
typedef char chartype;
typedef stringstream stringstreamtype;
#include <exexml>

namespace XmlPayload
{
	using namespace XmlFamily;
	struct Item : XmlNode
	{
		friend struct Payload;
		virtual XmlNodeBase* NewNode(Xml& _doc,XmlNodeBase* parent,stringtype name) 
		{ 
			XmlNodeBase* ret(NULL);
			ret=new Item(_doc,parent,name); 
			return ret;
		}
		virtual ostream& operator<<(ostream& o) 
		{ 
			Log ( name );
			if ( name == "get" )
			{
				o << "<got>dbvalue</got>";
			} else {
				XmlNode::operator<<(o); 
			}
			return o;
		}
		virtual bool operator()(ostream& o) { return XmlNode::operator()(o); }
		Item(Xml& _doc,const XmlNodeBase* _parent,stringtype _name) : XmlNode(_doc,_parent,_name) {}
	};
	inline ostream& operator<<(ostream& o,Item& xmlnode){return xmlnode.operator<<(o);}

	struct Payload : Xml
	{
		Payload(){} 
		virtual XmlNode* NewNode(Xml& _doc,stringtype name) { return new Item(_doc,NULL,name); }
		ostream& operator<<(ostream& o) { Xml::operator<<(o); return o;}
		operator Item& () { if (!Root) throw string("No root node"); return static_cast<Item&>(*Root); }
	};
	inline ostream& operator<<(ostream& o,Payload& xml){return xml.operator<<(o);}

} // XmlPayload
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

        string contenttype("text/xml");

	string sLen( request.sValue( "content-length" ) );
	const int Len( atoi( sLen.c_str() ) );
	if ( ( sLen.empty() ) || ( Len>1024 ) )
	{ 
		stringstream ssout; ssout << fence << "[DATAREQUEST-INVALIDSIZE]" << fence << request.c_str() << fence << request.Headers() << fence << sLen << fence;  
		Log(ssout.str());
		sock.flush(); 
		return ;
	} 
	char buf[ Len +1 ];
	memset( buf, 0, Len+ 1 );
	sock.read( buf, Len );
	stringtype S( buf );

	XmlPayload::Payload config;
	config.Load(S, NULL);
	config.TabLevel(0);
	ss<<config;


        status=200;

        stringstream response;
        response << "HTTP/1.1 ";
        response << status << " " << statusText(status) << endl;
        response << "Content-Type: " << contenttype << endl;
        response << "Server: WebKruncher" << endl;
        response << "Connection: close" << endl;
        response << "Content-Length:" << ss.str().size() << endl;
        response << endl;

        response << ss.str();

        sock.write(response.str().c_str(), response.str().size());
        sock.flush();
        {stringstream ssout; ssout << fence << "[DATAREQUEST]" << fence << request.c_str() << fence << request.Headers() << fence << S << fence << sLen << fence;  Log(ssout.str());}
        {stringstream ssout; ssout << fence << "[DATARESPONDED]" << fence << NoBreaks(response.str()) << fence;  Log(ssout.str());}
    }
    protected:
    Request& request;
    int& status;
    private:
    virtual ostream& operator<<(ostream& o) const { o << fence << "[DATAREQUEST]" << fence << request.Host() << fence << RequestUrl(request.c_str()) << fence << request.Headers(); return o; }
};

struct RequestManager : Request
{
    RequestManager(const icstring& _request, const icstringvector& _headers, Socket& _sock ) :
        Request(_request, _headers, _sock ) {}

    operator Response& ()
    {
        if (  response.get() ) return *response.get(); 

        response=unique_ptr<Response>( new Response_NotFound(*this, status) );
        if ( response.get() ) { Log("Not Found Page 2"); return *response.get(); }

        if ( ! response.get() ) throw string( "Can't get response" );
        return *response.get(); 
    }

    private:
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
    {const int T((rand()%80)+300); usleep(T);}
    SockQ* qp((SockQ*)lk);
    SockQ& Q(*qp);
    Locker& lock(Q.lock);
    stringstream ssexcept;
    try
    {

        while (!KILL)
        {

            {const int T((rand()%1000)+100000); usleep(T); }
            int sock(0);
            {
                ThreadWidget locker(lock);
                sock=Q;
            }

            if (!sock) 
            {
                const int T((rand()%400000)+100000); 
                usleep(T); 
                //Log( "not sock error" );
                continue;
            }

            Socket ss( sock );

            {stringstream pidstr; pidstr<<"GET PID " << getpid() << " ";
                       Log( pidstr.str() + string("From ") + ss.dotted()  );}


            ss.timeout(3, 0);
            ss.blocking(true);

            icstringvector headers;
            icstring requestline;

            while (true)
            {
                {const int T((rand()%10)+10); usleep(T); }
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
                {const int T((rand()%10)+10); usleep(T); }
                Log( "ipaddr error" );
                continue;
            }
            const string addr( ipaddr[ 0 ] );


            if ( true )
            {
                RequestManager request(requestline, headers, ss );
                if (!request.resolve()) 
                {
                    ss.close();
                    return NULL;
                }

                Response& rq(request);
                rq();
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
    cerr << "Starting krunchdata." << SERVICE_PORT << endl;
    SetSignals();
    CmdLine cmdline(argc, argv);
    if (!cmdline.exists("-d")) cerr << "Daemonizing krunchdata" << endl;
    else cerr << "Loading krunchdata in test mode" << endl;
    if (!cmdline.exists("-d")) Daemonizer daemon(argv[0]);
    SetSignals();
    srand(time(0));
    stringstream ssexcept;
    try
    {
        Log("Initializing krunchdata");
        SockQ Q;
        Locker& lock(Q.lock);


        streamingsocket ssrvc("", SERVICE_PORT );
        if (!ssrvc.open()) throw "Cannot open";
        ssrvc.blocking(true);
        ssrvc.timeout(2,0);


        if (!ssrvc.listen()) throw "Cannot listen";
#if 1 // Forking

        vector<pid_t> children;

        while ( children.size() < 16 )
        {
            Log( "Spawn" );
            const pid_t newfork( fork() );
            if ( newfork == 0 )
            {
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
            }  else {
                children.push_back( newfork );
            }
        } 


        while ( !KILL )
        {
            sleep(2);
        }

        for ( vector<pid_t>::iterator pit=children.begin();pit!=children.end();pit++)
        {
            Log( "Parent killing child" );
            kill(*pit, SIGKILL);
        }
#else // Not forking

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

        while ( !KILL )
        {
            sleep(2);
        }

#endif
        Log( "Parent done" );
    }
    catch (const char* e) {ssexcept<<e;}
    catch (string& e) {ssexcept<<e;}
    catch (exception& e) {ssexcept<<e.what();}
    catch (...) {ssexcept<<"unknown exception";}
    if (!ssexcept.str().empty()) cerr << red << Abrieviate(ssexcept.str()) << normal << endl;
    return 0;
}



#if 0

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <errno.h>
#include <db_cxx.h>
using namespace std;
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "Database.h" 
#include "DbCursor.h" 
#include <sys/stat.h>


using namespace BdbSpace;


int IpRegistry( int argc, char** argv );
int testmain( int argc, char** argv )
{
	//return IpRegistry( argc, argv );
	stringstream sserr;
	try
	{

		mkdir( "data", 0777  );
		chdir( "data" );

		const u_int32_t envFlags =
			DB_CREATE     |  // Create the environment if it does not exist
			DB_RECOVER    |  // Run normal recovery.
			DB_INIT_LOCK  |  // Initialize the locking subsystem
			DB_INIT_LOG   |  // Initialize the logging subsystem
			DB_INIT_TXN   |  // Initialize the transactional subsystem. This
			// also turns on logging.
			DB_INIT_MPOOL |  // Initialize the memory pool (in-memory cache)
			DB_THREAD;       // Cause the environment to be free-threaded
		DataBaseEnvironment environment(  envFlags );
		if ( ! environment ) throw string("Can't create environment");

		const u_int32_t openFlags = DB_CREATE              | // Allow database creation
			    DB_READ_UNCOMMITTED    | // Allow uncommitted reads
			    DB_AUTO_COMMIT         | // Allow autocommit
			    DB_THREAD; // Cause the database to be free-threaded 

		{
			DataBase database( "db", environment, openFlags, DB_BTREE, DB_DUPSORT );
			if ( ! database ) throw string( "Error loading db" ); 

			{ pair< string,string > kv( "pumpkin", "pie" ); database+=kv; }
			{ pair< string,string > kv( "apple", "sauce" ); database+=kv; }
			{ pair< string,string > kv( "peach", "cobbler" ); database+=kv; }
			cerr << database << endl;

			vector<string> TestKeys;
			TestKeys.push_back( "pumpkin" );
			TestKeys.push_back( "pear" );
			TestKeys.push_back( "apple" );
			TestKeys.push_back( "peach" );

			DbEnv& env( environment );
			DbTxn *txn;
			env.txn_begin(NULL, &txn, 0);
			for ( vector<string>::const_iterator it=TestKeys.begin();it!=TestKeys.end();it++)
			{
				DbCursor cursor( database );
				const string key( *it );
				const int found( cursor( txn, key ) );
				if ( ! found ) cout << "Can't find " << key << endl;
				else cout << key << ":" << cursor << endl;
			}
			txn->commit(0);

		}
	}
	catch (DbDeadlockException  &dbe) { sserr << "Exception: " << dbe.what() << endl; }
	catch (DbException &dbe) { sserr << "Exception: " << dbe.what() << endl; }
	catch ( string& e ) { sserr << "Exception: " << e << endl; }
	catch ( exception& e ) { sserr << "Std Exception: " << e.what() << endl; }
	catch ( ... ) { sserr << "Unknown Exception: " << endl; }

	if ( ! sserr.str().empty() ) { cerr << sserr.str() ; return EXIT_FAILURE; }

	return (EXIT_SUCCESS);
}

#endif
