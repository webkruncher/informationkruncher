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


var MsgNdx = 0

function Heartbeat()
{
	try
	{
		Now=new Date()
		
		if (LastAjaxSuccess == null)
		{
			sinceopened=Now.getTime()-FirstLoadedAt.getTime()
			if (sinceopened > 2000)
			{
				SystemLocked(true,"Cant AJax "+sinceopened)
			}
		} else {
			delta=Now.getTime()-LastAjaxSuccess.getTime()
			//if (Diagnostics) Diagnostics.Write(delta+"> Diagnostics")
			if (delta < 0)
			{
				SystemLocked(true,"Unexpected error - system went back in time")
			} else {
				if (delta > 10000)
				{
					if (ConnectionStatus) 
						SystemLocked(true,"Connection interrupted")
					ConnectionStatus=false
				} else {
					if (!ConnectionStatus) 
						SystemLocked(false,"Connection established")
					ConnectionStatus=true
				}
			}

			pto=GetObject("pingtarget")
			pto.innerHTML=""
			if (pto)
				new ajax("ping.xml",pto,function(xfrmd)
				{
					gt=GetObject("pingtarget")
					if (gt) 
					{
						tgtx=gt.innerHTML
						if (tgtx.length > 0)
							UserMessages.Write(tgtx)
					}
					MsgNdx++
					return false
				},null)
		}
	} catch (e) {
	//	AjaxFailure(e,6000)
	}

}


function StartHeartbeat()
{
	HeartBeatInterval= setInterval("Heartbeat()",500)
}
//setTimeout("StartHeartbeat()",3000)


