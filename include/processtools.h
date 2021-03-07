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
#ifndef KRUNCHPROCESSTOOLS
#define KRUNCHPROCESSTOOLS

namespace InfoTools
{

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


	inline void KillSignalHandler (int signum)
	{ 
		Log("Got a Kill signal");
		KILL=true;
	}


	inline void SignalHandler (int signum)
	{ 
		Log("signal handled");
		sleep( 1 );
	}

	inline int SetSignals()
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
        return 0;
	}



} // JTools
#endif // KRUNCHPROCESSTOOLS
