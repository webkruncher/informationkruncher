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


#ifndef KRUNCHERSHMEMTOOLS
#define KRUNCHERSHMEMTOOLS

#include <unistd.h>
#include <sys/mman.h>

namespace InfoTools
{

#if 0
	#define MAX_LEN 10000
	struct region {        /* Defines "structure" of shared memory */
	    int len;
	    char buf[MAX_LEN];
	};
	struct region *rptr;
	int fd;
	fd = shm_open("/myregion", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	    /* Handle error */;
	if (ftruncate(fd, sizeof(struct region)) == -1)
	    /* Handle error */;
	/* Map shared memory object */
	rptr = mmap(NULL, sizeof(struct region),
	       PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (rptr == MAP_FAILED)
	    /* Handle error */;
	   //rptr->len */
#endif



	template < typename T >
		struct ShMem 
	{
		ShMem( const string& _name ) 
			: name( _name ), rptr( nullptr ), fd( 0 ) {}
		~ShMem()
		{
			if ( fd ) close( fd );
			shm_unlink( name.c_str() );
		}
		operator bool ()
		{
			fd = shm_open( name.c_str() , O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
			if (fd == -1) return false;
			if (ftruncate(fd, sizeof( T )) == -1) return false;
			rptr = (T*) mmap(NULL, sizeof( T ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			if (rptr == MAP_FAILED) return false;
			return true;
		}
		operator T& ()
		{
			if ( ! rptr ) throw string("ShMem failed");
			return *rptr;
		}
		private:
		const string name;
		T *rptr;
		int fd;
	};


} // InfoTools
#endif // KRUNCHERSHMEMTOOLS

