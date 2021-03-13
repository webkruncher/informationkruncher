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


#include <deque>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
using namespace std;
#include <infosock.h>
using namespace InformationSocket;
#include <infotools.h>
using namespace InfoTools;
#include <processtools.h>

#include <oformat.h>
using namespace OFormat;

#include <sys/ioctl.h>

#include <spinlock.h>

#include <hyper.h>
using namespace Hyper;
volatile bool KILL(false);

#include <signal.h>
#include <DataBase.h>

#include <uuid.h>

struct RequestManager : Request, KrunchData::DataBase
{
    RequestManager(const icstring& _request, const icstringvector& _headers, Socket& _sock ) :
        HyperBase(_request, _headers, _sock ),
        Request(_request, _headers, _sock ),
        KrunchData::DataBase(_request, _headers, _sock )
    {}

    operator Response& ()
    {
        if (  response.get() ) return *response.get(); 

        KrunchData::DataBase& db(*this);
	string data( db );
	{stringstream ssout; ssout << fence << "[DBRESULT]" << fence << data << fence; Log (ssout.str());}
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




void* service(void* lk)
{
    {const int T((rand()%80)+300); usleep(T);}
    stringstream ssexcept;
    try
    {
        while (!KILL)
        {
		{const int T((rand()%1000)+10000); usleep(T); }
		Socket sock( (char*) "localhost", 80 );
		sock.connect();
		icstringvector headers;
		icstring requestline("HTTP/1.1 GET / ");
		//{sleep( 1 ); stringstream ss; ss<<"Unit tester"; Log( ss.str() );}
		RequestManager request(requestline, headers, sock );
		Response& rq(request);
		sock.close();
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
    cerr << "Datakruncher unit tester." << endl;
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
        Log("Initializing webkruncher");
        vector<pid_t> children;
        while ( children.size() < 16 )
        {
            //Log( "Spawn" );
            const pid_t newfork( fork() );
            if ( newfork == 0 )
            {
                vector<pthread_t> threads;
                pthread_t t(NULL);
                for (int j=0;j<8;j++)
                {
                    pthread_create(&t,0,service,NULL);
                    if (!t) throw "Can't thread";
                    threads.push_back(t);
                }

                while (!KILL)
                {
                    {const int T((rand()%10)+20); usleep(T); }
                }

                //Log("joining");

                for (vector<pthread_t>::iterator it=threads.begin();it!=threads.end();it++)
                {
                    pthread_t t(*it);
                    pthread_join(t, 0);
                }
                //Log("exiting");
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
            //Log( "Parent killing child" );
            kill(*pit, SIGKILL);
        }
    }
    catch (const char* e) {ssexcept<<e;}
    catch (string& e) {ssexcept<<e;}
    catch (exception& e) {ssexcept<<e.what();}
    catch (...) {ssexcept<<"unknown exception";}
    if (!ssexcept.str().empty()) cerr << red << Abrieviate(ssexcept.str()) << normal << endl;
    return 0;
}


