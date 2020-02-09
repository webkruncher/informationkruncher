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
#ifndef JTOOLS_H
#define JTOOLS_H
#include <sys/stat.h>
#include <syslog.h>
//#include <oformat.h>
#include <map>
#include <sys/signal.h>
#include <memory>

typedef std::string stringtype;
typedef char chartype;
typedef std::stringstream stringstreamtype;
extern volatile bool KILL;


namespace JTools
{
	//using namespace OFormat;
	void Log(const string& what) { if (1) syslog(1,what.c_str()); }
	//void Log(const string& what) { cerr << what.c_str() << endl; }


	struct stringvector : vector<string> 
	{

                virtual void split( string what ) { return split( what, "|" ); }
                virtual void split( string what, string _how )
                {
                        while ( true )
                        {
                                const size_t where( what.find( _how ) );
                                if ( where  == string::npos ) 
				{
					if ( ! what.empty() ) push_back( what );
					return;
				}
                                push_back( what.substr(0, where ) );
                                what.erase( 0, where+ 1 );
                        }
                }
 

		friend ostream& operator<<(ostream&,const stringvector&);
		ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) o << it->c_str() << endl;
			return o;
		}
	}; 

	inline ostream& operator<<(ostream& o,const stringvector& m)
		{ return m.operator<<(o); }




	struct ignorecase_traits : public std::char_traits<char>
	{
		static bool eq(const char& c1,const char& c2) {return toupper(c1)==toupper(c2);}
		static bool lt(const char& c1,const char& c2) {return toupper(c1)<toupper(c2);}
		static int compare(const char* s1,const char* s2,std::size_t n)
		{
			for (std::size_t i=0;i<n;i++) 
				if (!eq(s1[i],s2[i])) 
					return lt(s1[i],s2[i])?-1:1;
			return 0;
		}
		static const char* find(const char* s,std::size_t n,const char& c)
		{
			for(std::size_t i=0;i<n;i++)
				if (eq(s[i],c))
					return &(s[i]);
			return 0;
		}
	};

	struct icstring : public std::basic_string<char,ignorecase_traits>
	{
		icstring (){}
		icstring (const icstring& c) :
			std::basic_string<char,ignorecase_traits>(c.c_str()) { }
		icstring (const string& c) :
			std::basic_string<char,ignorecase_traits>(c.c_str()) { }
		icstring (const char* c) :
			std::basic_string<char,ignorecase_traits>(c) { }
		icstring& operator=(const char* c)
		{
			std::basic_string<char,ignorecase_traits>::operator=(c);
			return *this;
		}
		icstring& operator=(const icstring& c)
		{
			std::basic_string<char,ignorecase_traits>::operator=(c.c_str());
			return *this;
		}
		friend ostream& operator<<(ostream&,const icstring&);
		ostream& operator<<(ostream& o) const
		{
			o << c_str() ;
			return o;
		}
	}; 
	inline ostream& operator<<(ostream& o,const icstring& m) {return m.operator<<(o);}

	struct icstringvector : vector<icstring> 
	{
		friend ostream& operator<<(ostream&,const icstringvector&);
		ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) o << it->c_str() << endl;
			return o;
		}
	}; 

	inline ostream& operator<<(ostream& o,const icstringvector& m)
		{ return m.operator<<(o); }

	bool FileExists(const string& filename)
	{
		string pathname("text/");
		pathname+=filename;
		struct stat sb;
		if (stat(pathname.c_str(), &sb)==0)
		{
			//stringstream ssout; ssout << fence << "[exists]" << fence << pathname << fence << boolalpha << true << fence; Log(ssout.str());
			return true;
		} else {
			//stringstream ssout; ssout << fence << "[exists]" << fence << pathname << fence << boolalpha << false << fence; Log(ssout.str());
			return false;
		}
	}

	size_t FileSize(const string& filename)
	{
		string pathname("text/");
		pathname+=filename;
		struct stat sb;
		memset(&sb, 0, sizeof(sb));
		//cout << pathname << "^" << endl;
		if (stat(pathname.c_str(), &sb)==0) 
		{
			//stringstream ssout; ssout << fence << "[fsize]" << fence << pathname << fence << sb.st_size << fence; Log(ssout.str());
			return sb.st_size;
		} else {
			//stringstream ssout; ssout << fence << logerror << "[fsize]" << fence << pathname << fence << sb.st_size << fence; Log(ssout.str());
			return 0;
		}
	}

	void LoadFile(const string& filename, stringstream& ss)
	{
		string pathname("text/");
		pathname+=filename;
		//stringstream ssout; ssout << fence << "[load]" << pathname << fence; Log(ssout.str());
		ifstream in(pathname.c_str());
		string line;
		if (!in.fail())
		while (!in.eof()) { getline(in, line); ss << line << endl;}
		//cout << "\033[32m" << ">" << "\033[0m"; cout.flush();
	}

	void LoadFile(const string& filename, unsigned char* dest, size_t sz)
	{
		string pathname("text/");
		pathname+=filename;
		//stringstream ssout; ssout << fence << "[loadbinary]" << pathname << fence; Log(ssout.str());
		ifstream in(pathname.c_str(), ios::binary);
		string line;
		if (!in.fail()) 
		{
			in.read((char*)dest, sz);
			//cout << "read " << pathname << endl;
		}
		//cout << pathname << endl;
		//cout << "\033[32m" << ">" << "\033[0m"; cout.flush();
	}

	inline string runpipe(string what)
	{
		string ret; char * line = NULL;	size_t len = 0;	ssize_t bread;
		FILE* fp = popen((char*)what.c_str(),"r");
		if (fp == NULL) {return "";}
		while (1)
		{
			char buf[514];
			memset(buf,0,514);
			int l = fread(buf,1,512,fp);
			if (l > 0) ret+=buf;
			else return ret;
		}
		return ret;
	}

	inline string NoBreaks(const string line)
	{
		string out(line.c_str()); 
		while (true)
		{
			size_t n(out.find_first_of("\t\n\r\f"));
			if (n==string::npos) return out;
			out.erase(n, 1);
		}
	}

	struct Characters
	{
		Characters(const size_t sz) : Sz(sz) 
		{
			chars=new unsigned char[Sz+1];
		}
		~Characters()
		{
			delete[] chars;
		}
		operator unsigned char* () { return chars; }
		operator const size_t () const { return Sz; }
		private:
		const size_t Sz;
		unsigned char *chars;
	};

	string statusText(const int status)
	{
		if (status == 200) return "OK";
		if (status == 404) return "Not Found";
		return "ERROR";
	}

	string RequestUrl(const char* _line)
	{
		if (!_line) return "";
		string line(_line);
		const size_t sp1(line.find(" "));
		if (sp1==string::npos) return "";
		const size_t sp2(line.find(" ", sp1+1));
		if (sp2==string::npos) return "";
		return line.substr(sp1,sp2-sp1);
	}



	string ContentType(const string& _what)
	{
		icstring what(_what.c_str());

		if (what.find(".png ")!=string::npos) 
			return "image/png";

		if (what.find(".jpeg ")!=string::npos) 
			return "image/jpeg";

		if (what.find(".gif ")!=string::npos) 
			return "image/gif";

		if (what.find(".jpg ")!=string::npos) 
			return "image/jpeg";

		if (what.find(".ico ")!=string::npos) 
			return "image/x-icon";

		if (what.find(".xslt ")!=string::npos) 
			return "application/xhtml+xml";

		if (what.find(".css ")!=string::npos) 
			return "text/css";

		if (what.find(".js ")!=string::npos) 
			return "text/plain";

		if (what.find(".xml ")!=string::npos) 
			return "text/xml";

		if (what.find(".json ")!=string::npos) 
			return "application/json";

		return "text/html";
	}

	string Abrieviate(const string instr)
	{
		size_t m((instr.size()>128)?128:instr.size());
		return instr.substr(0,m);
	}

	struct stringmap : public map<stringtype,stringtype> { };

	class CmdLine : public stringmap
	{
	public:
		int Argc()
		{
			if (!argc) Build();
			return argc;
		}
	
		const char** Argv()
		{
			if (!argv) Build();
			return argv;
		}

		CmdLine() : programname("utility"),argv(NULL),argc(0),ownerofargs(false) {}
		CmdLine(int _argc,const char** _argv,string _progname="utility") 
			: programname(_progname), argv(_argv),argc(_argc),ownerofargs(false)
		{
			for (int i=1;i<argc;i++)
			{
				string arg=argv[i];
				bool trip=false;
				int nexti = i+1;
				if (arg.find("-")==0)
				{
					if (nexti<argc)
					{
						string next = argv[nexti];
						if (next.find("-")!=0)
						{
							(*this)[arg]=next;
							trip=true;
						}
					}
				}
				if (trip) i=nexti;
				else (*this)[arg]="";
			}
		}

		virtual ~CmdLine() { if (ownerofargs) if (argv) free(argv); }
		ostream& operator<<(ostream& o) const
		{
			for(const_iterator it=begin();it!=end();it++)
				o<<it->first<<":"<<it->second<<endl;
			return o;
		}
		const bool exists(string name) const
		{
			for(const_iterator it=begin();it!=end();it++) if (it->first==name) return true;
			return false;
		}

		string programname; // user settable
		private: int argc; const char **argv; string argbuffer; bool ownerofargs;
		void Build()
		{
			if (argc) return; if (argv) return;
			ownerofargs=true;
			size_t bytes(0);
			int items(0);
			for (iterator it=begin();it!=end();it++) 
			{
				bytes+=it->first.size(); 
				items++; 
				if (it->second.size()) {bytes+=it->second.size()+1; items++;} 
			}
			argbuffer.resize(bytes);
			argv=(const char**)malloc(sizeof(char*)*items);
			if (!argv) throw string("Cannot allocate argv double pointer");
			int i(0);
			for (iterator it=begin();it!=end();it++)
			{
				argv[i++]=&argbuffer[argbuffer.size()];
				if (it->first.size()) argbuffer+=it->first;
				if (it->second.size()) 
				{
					argbuffer+="\0"; 
					argv[i++]=&argbuffer[argbuffer.size()];
					argbuffer+=it->second;
				}
			}
			argc=i;
		}
	};

	inline ostream& operator<<(ostream& o,const CmdLine& l){return l.operator<<(o);}

	inline u_long cmdlinevalue(stringmap& _cmdline,string name,u_long def)
	{
		CmdLine& cl=static_cast<CmdLine&> (_cmdline);
		if (!cl.exists(name)) return def;
		return atol(&cl[name][0]);
	}



	struct Daemonizer
	{
		Daemonizer(const char* argv0)
		{
			pid_t pid(fork()); 
			if (pid < 0)  throw string("Cannot daemonize");
			if (pid > 0) exit(0);
			umask(0);
			openlog(argv0,LOG_NOWAIT|LOG_PID,LOG_USER); 
			syslog(LOG_NOTICE, "Successfully started daemon\n"); 
			pid_t sid(setsid());
			if (sid < 0) throw string("Cannot create process group");
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
		}
	};


	void KillSignalHandler (int signum)
	{ 
		Log("Got a Kill signal");
		KILL=true;
	}


	void SignalHandler (int signum)
	{ 
		Log("signal handled");
	}

	int SetSignals()
	{
		int Sigs[]=
		{
			SIGHUP,     /* hangup */
			SIGINT,     /* interrupt */
			SIGQUIT,    /* quit */
			SIGILL,     /* illegal instruction (not reset when caught) */
			SIGTRAP,    /* trace trap (not reset when caught) */
			SIGABRT,    /* abort() */

			SIGIO,   	/* input/output possible signal */
			SIGWINCH, 	/* window size changes */

			SIGFPE,  	/* floating point exception */
			SIGKILL, 	/* kill (cannot be caught or ignored) */
			SIGBUS,    	/* bus error */
			SIGSEGV,   	/* segmentation violation */
			SIGSYS,    	/* bad argument to system call */
			SIGPIPE,   	/* write on a pipe with no one to read it */
			SIGALRM,   	/* alarm clock */
			SIGURG,    	/* urgent condition on IO channel */
			SIGSTOP,   	/* sendable stop signal not from tty */
			SIGTSTP,   	/* stop signal from tty */
			SIGCONT,   	/* continue a stopped process */
			SIGCHLD,   	/* to parent on child stop or exit */
			SIGTTIN,   	/* to readers pgrp upon background tty read */
			SIGTTOU,   	/* like TTIN for output if (tp->t_local&LTOSTOP) */

			SIGXCPU,   	/* exceeded CPU time limit */
			SIGXFSZ,   	/* exceeded file size limit */
			SIGVTALRM, 	/* virtual time alarm */
			SIGPROF, 	/* profiling time alarm */

			SIGUSR1, 	/* user defined signal 1 */
			SIGUSR2 	/* user defined signal 2 */
		};

		for (int j=0;j<sizeof(Sigs)/sizeof(int);j++)
		{
			const int sig(Sigs[j]);
			if (signal (sig, SignalHandler) == SIG_IGN)
				signal (sig, SIG_IGN);
		}

		if (signal (SIGTERM, KillSignalHandler) == SIG_IGN)
			signal (SIGTERM, SIG_IGN);
	}


	struct FileLocker
	{
		FileLocker( const string _fname, bool _mode ) 
			: fname( _fname ), mode( _mode ), 
			lockname( _fname + string( ".lock" )   )
		{}
		~FileLocker()
		{
			int times( 8 ); // 2 seconds
			while ( times-- )
			{
				unlink( lockname.c_str() );
				struct stat sb;
				if (stat(lockname.c_str(), &sb)==0)
				{
					cerr << lockname << " still exists, trying to delete again" <<endl;
					usleep( 250 );
				}  else 
					return;
			}
		}
		operator bool ()
		{
			int times( 8 ); // 2 seconds
			if ( ! mode )
			{
				// Wait until no lock fie
				// Return false after a while
				bool ok( false );
				while ( ! ok )
				{
					struct stat sb;
					if (stat(lockname.c_str(), &sb)==0)
					{
						cerr << lockname << " exists, waiting" <<endl;
						usleep( 250 );
						if ( ! ( times-- ) ) return false;
					} else {
						ofstream lockit( lockname.c_str() );
						time_t now( time( 0 ) );
						lockit << now;
						return true; 
					}
				}
				return false;
			} else {
			}
		}
		private:
		const string fname;
		const bool mode;
		const string lockname;
	};

	struct RaiiMem
	{
		RaiiMem( unsigned long len ) : dest( NULL )
		{
			dest=  ( unsigned char * ) malloc( len )  ;
		}
		operator unsigned char* () { return dest; }
		~RaiiMem()
		{
			if ( dest ) free( dest );
		}
		private:
		unsigned char* dest;
	};

} // JTools
#endif // JTOOLS_H

