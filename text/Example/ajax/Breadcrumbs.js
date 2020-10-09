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



function Breadcrumbs (BreadLink)
{
	this.ndx=0
	this.BreadLink=BreadLink;
	this.History = new Array()
	this.BreadNav=false
	this.Navigate = function (page)
	{
		//UserMessages.Write(page + " " + this.ndx + " / " + this.History.length)
		if (this.BreadNav) 
		{
			this.BreadNav=false
			return
		}
		if (this.ndx)
		{
			if (page==this.History[this.ndx-1])
				return;
		}
		this.History[ this.ndx ] = page
		this.ndx+=1
	}

	this.Buttons = function(target)
	{
		BkDisplay="none"
		FwdDisplay="none"
		BkBtn=GetObject('Pages').childNodes[target].firstChild.firstChild
		FwdBtn=GetObject('Pages').childNodes[target].firstChild.lastChild

		if ( (this.History.length > 1) && (this.ndx > 1) )
		{
			BkBtn.setAttribute("style", "display:inline;")
			BkDisplay="inline"
		}

		if (this.ndx < this.History.length )
		{
			FwdBtn.setAttribute("style", "display:inline;")
			FwdDisplay="inline"
		}

		BkBtn.setAttribute("style", "display:"+BkDisplay+";")
		FwdBtn.setAttribute("style", "display:"+FwdDisplay+";")
	}

	this.Fwd=function()
	{
		this.BreadNav=true
		this.ndx+=1
		Navigate(this.History[ this.ndx-1 ])
	}

	this.Back=function()
	{
		this.BreadNav=true
		if (this.ndx < 1) return;
		this.ndx-=1
		Navigate(this.History[ this.ndx-1 ])
	}
}


