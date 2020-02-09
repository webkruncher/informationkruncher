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

#ifndef INFOSSOCKS_H
#define INFOSSOCKS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sstream>
#include <memory>
#include <set>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define KRUNCH_PERMIT_IP 0X10
#define KRUNCH_PERMIT_LOGGEDIN 0X20
#define KRUNCH_IP_BLOCKED 0X8000

namespace InformationSocket
{
	#define thing (*this)
	#define BufferSize 256

	inline bool Serve(int,sockaddr_in&,int);
	inline bool Blocking(int,bool);
	inline int  Accept(int,sockaddr_in&);
	inline int  Timeout(u_long,int,int);

	template <class SocketType>
		struct SocketBuffer : protected streambuf
	{
		SocketBuffer(SocketType& st, SSL* _ssl=NULL) : D(st),buffersize(BufferSize), outb(NULL),inb(NULL), streambuf(),sock(0), much(0), ssl( _ssl )
		{
			inb=(char*)malloc(buffersize+6);setg(inb+4,inb+4,inb+4);
			outb=(char*)malloc(buffersize+6);setp(outb,outb+(buffersize-1));
		}
		SocketBuffer(SocketType& st,int _bsize) : D(st), buffersize(_bsize), outb(NULL),inb(NULL), streambuf(),sock(0), much(0)
		{
			inb=(char*)malloc(buffersize+6);setg(inb+4,inb+4,inb+4);
			outb=(char*)malloc(buffersize+6);setp(outb,outb+(buffersize-1));
		}
		virtual ~SocketBuffer() {sync(); if (outb) free(outb); if (inb) free(inb);}
		void operator =(int s){sock=s;}
	protected:
		void blocking(bool b=false){Blocking=b;}
		unsigned long much;
		virtual int underflow()
		{
			if (gptr() < egptr()) 
				return *gptr();
			int putback( gptr()-eback() );
			if (putback>4) putback=4;
			memcpy(inb+(buffersize-putback),gptr()-putback,putback);
			int num(read((char*)inb+4,(size_t)buffersize-4));
			if (num<=0) return EOF;
			setg(inb+(4-putback),inb+4,inb+4+num);
			return *gptr();
		}
		virtual int overflow(int c)
		{
			if (c!=EOF) {*pptr()=c; pbump(1);}
			if (Flush()==EOF) {return EOF;}
			return c;
		}
		virtual int sync(){if (Flush()==EOF) return -1;			return 0;}
		virtual int read(char* destination,size_t byts)
		{
			if ( ssl )
			{
				const int ret(SSL_read(ssl, destination, byts )); 
				return ret;
			} else {	
				const int ret(recv(sock, destination, byts, 0));
				return ret;
			}
		}
		virtual SSL* GetSsl(){return ssl;}

	private:
		virtual void close() = 0;
		virtual int Flush()
		{
			if (!sock) return 0;
			const int num( pptr()-pbase() );
			if (!num) return 0;
			if (write(outb,num)!=num) return EOF;
			pbump(-num);
			return num;
		}		
		SocketType& D;
	protected:
		virtual int write(const char* source,size_t byts)
		{ 
			if ( ssl )
			{
				const int ret(SSL_write(ssl, source, byts )); 
				return ret;
			} else {	
				const int ret(send(sock, source, byts, 0)); 
				return ret;
			}
		}
		virtual void throttle () = 0;
		int sock;
		int buffersize;
		char* inb,*outb;
		bool Blocking;
		SSL* ssl;
	};

	class streamingsocket
	{
		public:
			streamingsocket(const char* _host,int _port) : host(_host), port(_port),sock(0),
			_fail(false),_open(false) {}		
		public:

			virtual bool blocking(bool b=false) { return InformationSocket::Blocking(sock,b); }
			virtual bool listen(int n) { return InformationSocket::Serve(n,client,10); }
			virtual bool listen() { return InformationSocket::Serve(sock,client,100); }
			virtual int accept() { return InformationSocket::Accept(sock,client); }
			virtual bool is_open(){return _open;}
			virtual int timeout(int s,int u) { return InformationSocket::Timeout(sock,s,u); }

			virtual bool open(int socktype=SOCK_STREAM,int addressfamily = AF_INET,int proto=IPPROTO_TCP) 
			{
				if (_open) return true;
				if (sock) return true;
				sock=socket(addressfamily,socktype,proto); 
				if (sock==-1) 
				{
					cerr<<"Can't get a socket"<<endl;
					_fail=true; 
					return false;
				}
				memset(&client, 0, sizeof(struct sockaddr_in));	

				if (!GetHost().size())
				{
					client.sin_family 	= AF_INET;
					client.sin_port=htons(port);
				} else {
					client.sin_family = AF_INET;
					client.sin_addr.s_addr=inet_addr(GetHost().c_str());
					client.sin_port=htons(port);
					he = ::gethostbyname(GetHost().c_str());
					if (!he) 
					{
						_fail=true;
						close();
						return false;
					}	
					memcpy((char *) &client.sin_addr,(char *) he->h_addr,he->h_length);	
				}

				_open=true;
				return true;
			}

			virtual bool connect()
			{
				if (!GetHost().size())  return false;
				int rval=::connect(sock,(struct sockaddr *)&client,sizeof(client));
				if (rval!=0) _fail=true; 
				return !_fail;
			}

			virtual void close()
			{
				if (sock) ::shutdown(sock,SHUT_RDWR);
				if (sock) ::close(sock);
				sock=0; 
			}

			virtual int GetSock(){return sock;}
			virtual int GetPort()
			{
				socklen_t saSize(sizeof(struct sockaddr_in));
				getsockname(sock,(struct sockaddr *)&client,&saSize);
				port=ntohs(client.sin_port);
				return port;
			}
			virtual void SetSock(int s){sock=s;}
			virtual string& GetHost(){return host;}
			virtual void SetHost(char* h){host = h;}
			struct sockaddr_in client,listenclient;
		private:
			string host;
			int sock,port;
			hostent* he;
			bool _fail;
			bool _open;
		protected:
			virtual string dotted()
			{
				stringstream ss;
				socklen_t saSize(sizeof(struct sockaddr_in));
				getpeername(sock,(struct sockaddr *)&client,&saSize);
				ss<<inet_ntoa(client.sin_addr)<<":"<<ntohs(client.sin_port);
				return ss.str();
			}
	};


	struct Socket : public streamingsocket, public iostream
	{
		unsigned long auth;
		Socket(int sock) : auth( 0 ), buffer(*this), streamingsocket((char*)"",0), 
			iostream((streambuf*)&buffer)
		{
			buffer=sock;SetSock(sock);
		}
		Socket(int sock, SSL* _ssl ) : auth( 0 ), buffer(*this, _ssl), streamingsocket((char*)"",0), 
			iostream((streambuf*)&buffer)
		{
			buffer=sock;SetSock(sock);
		}
		Socket(int sock,int buffersize) : auth( 0 ), buffer(*this,buffersize), 
			streamingsocket((char*)"",0), iostream((streambuf*)&buffer)
		{
			buffer=sock;SetSock(sock);
		}
		Socket(char* host, int port) : auth( 0 ), buffer(*this), streamingsocket(host,port), 
			iostream((streambuf*)&buffer) { }

		Socket(char* host, int port,int socktype) : auth( 0 ), buffer(*this), 
			streamingsocket(host,port), iostream((streambuf*)&buffer) { }

		virtual bool open(int socktype=SOCK_STREAM,int addressfamily = AF_INET,int proto=IPPROTO_TCP) 
			{if (!streamingsocket::open(socktype,addressfamily,proto)) return false; buffer=GetSock(); return true;}
				
		virtual void close()
		{
			sync();
			streamingsocket::close();
			buffer=0;
			buffer.much=0; buffer.bread=0; 
		}
		virtual ~Socket() { }
		virtual SSL* GetSsl(){return buffer.GetSsl();}
		void getline(string& line);
		virtual int write(const char* source,long byts)
		{
			return buffer.write(&source[0],byts);
		}

		virtual int write(const unsigned char* source,long byts)
		{
			return buffer.write((char*)&source[0],byts);
		}
		virtual void throttle() {}

		virtual string dotted() // returns dotted ip address
		{
			return streamingsocket::dotted();
		}
		virtual bool blocking(bool b=false) 
		{ 
			streamingsocket::blocking(b); 
			buffer.blocking(b);
		}
		protected:
		struct  StreamBuffer : public SocketBuffer  <StreamBuffer>
		{
			friend class Socket;
			private:
			StreamBuffer(Socket& _linkage, SSL* _ssl=NULL) : linkage(_linkage) ,SocketBuffer<StreamBuffer>(thing, _ssl),much(0),bread(0) {}
			StreamBuffer(Socket& _linkage,int bsize) :much(0),bread(0),
			SocketBuffer<StreamBuffer>(thing,bsize),linkage(_linkage) {}
			virtual void close() {linkage.close();}
			virtual void throttle (){linkage.throttle();}
			virtual int write(const char* source,size_t byts)
				{ return SocketBuffer<StreamBuffer>::write(source,byts); }
			void operator=(int s){SocketBuffer<StreamBuffer>::operator=(s);}
			Socket& linkage;
			unsigned long long much,bread;
		} buffer;		
	};

	void Socket::getline(string& line)
	{
		stringstream ss;
		set<char> skips; 
		skips.insert('\r');
		while (true)
		{
//cerr << "G";
			int c(get());
			if (skips.find(c)!=skips.end())
				{ skips.erase(c); continue; }
			if (!isprint(c)) {line=ss.str();return;}
			ss << (char) c;
		}
	}


	inline bool Serve(int sock,sockaddr_in& client,int backlog)
	{
		struct linger lingerer = {1,1};
		int optval = 1;
		if (-1==::setsockopt(sock, SOL_SOCKET, SO_LINGER, &lingerer,sizeof(linger))) 
			throw string("Can't set socket options");
		if (-1==::setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) 
			throw string("Can't set socket options");
		if (-1==::bind(sock, (struct sockaddr *) &client, sizeof(sockaddr_in))) 
			throw string("bind error");
		if (-1==::listen(sock, backlog)) throw string("listen error");
		return true;
	}

	inline bool Blocking(int sock,bool b)
	{
		u_long argp=b?0:1;
		if (!b) fcntl(sock, F_SETFL, O_NONBLOCK);
		else fcntl(sock, F_SETFL, 0);
		return b;
	}

	inline int Accept(int sock,sockaddr_in& client)
	{
		int addrlen=sizeof(client);
		return ::accept(sock,(struct sockaddr *) &client,(socklen_t*)&addrlen);
	}

#if 0
	inline int Timeout(u_long sock,int s,int u)
	{
		int ret(1);
		struct timeval tv;
		tv.tv_sec = s;
		tv.tv_usec = u;
		if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) < 0) 
			{ret=0; throw string("Cannot set timeout");}
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) 
			{ret=0; throw string("Cannot set timeout");}
		return ret;
	}
#endif

    inline int Timeout(u_long sock,int s,int u)
    {
return 1;
        int ret(1);
        struct timeval tv;
        tv.tv_sec = s;
        tv.tv_usec = u;
        if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) < 0)
            {ret=0; cout<< ("Can't set send timeout") << endl;}
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0)
            {ret=0; cout<< ("Can't set recieve timeout") << endl;}
        return ret;
    }



	inline void init_openssl()
	{ 
		SSL_library_init();
		SSL_load_error_strings();	
		OpenSSL_add_ssl_algorithms();
	}

	inline void cleanup_openssl()
	{
		EVP_cleanup();
	}

	inline SSL_CTX *create_context( const bool UsingSsl = false )
	{
		if ( ! UsingSsl ) return NULL;
		const SSL_METHOD *method;
		SSL_CTX *ctx;

		//method = SSLv23_server_method();
		method = TLSv1_2_server_method();

		ctx = SSL_CTX_new( method );

		if (!ctx)
		{
			ERR_print_errors_fp(stderr);
			throw string( "Cannot create SSL context" );
		}






		return ctx;
	}

	int pem_passwd_cb(char *buf, int size, int rwflag, void *userdata)
	{
		//cout << "pem_passwd_cb" << endl;
		return 0;
	}
	




	inline STACK_OF(X509_NAME) * configure_context(SSL_CTX *ctx)
	{
		if ( ! ctx ) return NULL;
		SSL_CTX_set_ecdh_auto(ctx, 1);
		stringstream ssf;
		ssf<<CERT_FILE;
		//cout << "Cert:"<<CERT_FILE<<";" << endl;
		if (SSL_CTX_use_certificate_file(ctx, ssf.str().c_str(), SSL_FILETYPE_PEM) <= 0) {                                                            
			stringstream ssm; ssm << "cannot load cert:"<< ssf.str().c_str()<<";"<<endl;
			throw string( ssm.str().c_str());
		}


		SSL_CTX_set_default_passwd_cb( ctx, pem_passwd_cb );
		SSL_CTX_set_default_passwd_cb_userdata(ctx,( char* ) KEY_PASSWD);

		stringstream ssk;
		ssk<<KEY_FILE;

		//cout << "Key:"<<ssk.str()<<";" << endl;

		if (SSL_CTX_use_PrivateKey_file(ctx, ssk.str().c_str(), SSL_FILETYPE_PEM) <= 0 ) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}


		if (SSL_CTX_check_private_key(ctx) == 0) {
			printf("Private key does not match the certificate public key\n");
			exit(0);
		}

		stringstream sscaf;
		sscaf<<HOME << CA_DIR << CA_FILE ;
		//cout << "\033[32mSetup ca:" << sscaf.str() << ";\033[0m" << endl;

#if 1

		//SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,NULL);
		SSL_CTX_set_verify(ctx,SSL_VERIFY_NONE,NULL);
		//if (SSL_CTX_load_verify_locations(ctx,sscaf.str().c_str(),NULL)<1) {

		if (SSL_CTX_load_verify_locations(ctx,sscaf.str().c_str(),NULL)<1) {
			printf("Error setting the verify locations.\n");
			exit(0);
		}
#endif
		return SSL_load_client_CA_file(sscaf.str().c_str());
	}

	struct SslContext
	{
		SslContext( SSL_CTX* _ctx, SSL* _ssl, Socket& _sck ) :
			ctx( _ctx ), ssl( _ssl ), sck( _sck ) {}
		~SslContext()
		{
			//cerr << "~"; cerr.flush();
			if ( ! ctx ) return; 
			SSL_free(ssl);
			sck.close();
			SSL_CTX_free( ctx );
		}
		private:
		SSL_CTX* ctx;
		SSL* ssl;
		Socket& sck;
	};

} // InformationSocket

#endif //INFOSSOCKS_H
// WebKruncher.com





