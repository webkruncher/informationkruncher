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

function MsgCache(txt)
{
	this.txt=txt
}

function MsgTable (roottr)
{
	this.textcolor="silver";
	this.backgroundcolor="";
	this.roottr=roottr;
	this.LengthLimit=-1;
	this.MaxLength = function(l) {this.LengthLimit=l;}	
	this.msgcacher  = new Array()
	this.Write = function (txt)
	{
		l=this.roottr.childNodes.length;
		if (this.LengthLimit > 0)
			if (l > this.LengthLimit)
			{
				for (i=1;i<this.roottr.childNodes.length;i++)
				{
					ta=this.roottr.childNodes[i-1].firstChild.firstChild.firstChild.firstChild;
					tb=this.roottr.childNodes[i].firstChild.firstChild.firstChild.firstChild;
					ta.nodeValue=tb.nodeValue
				}
				this.roottr.childNodes[l-1].firstChild.firstChild.firstChild.firstChild.nodeValue=txt;
				return
			}

		tb=document.createElement("tbody");
		tr=document.createElement("tr");
		tr.setAttribute("class","utr");
		td=document.createElement("td");
		td.setAttribute("class","utd");
		d=document.createElement("div");
		d.setAttribute("class","UserMessageText")
		style="";
		if (this.backgroundcolor!="") style+="background:"+this.backgroundcolor+";"
		if (this.backgroundcolor!="") style+="color:"+this.textcolor+";"
		d.setAttribute("style",style);
		m=document.createTextNode(txt);
		this.lasttextnode=m
		d.appendChild(m)
		td.appendChild(d)
		tr.appendChild(td)
		tb.appendChild(tr)
		this.roottr.appendChild(tb)
	}


	this.Color = function(c)
	{
		this.textcolor=c;
	}
	this.Background = function(c)
	{
		this.backgroundcolor=c;
	}

	this.Hide = function()
	{
		try
		{
			for (i=0; i < this.roottr.childNodes.length;i++)
			{
				ta=this.roottr.childNodes[i].firstChild.firstChild.firstChild.firstChild;
				ta.nodeValue=''
			} 
		}catch (e) {
			CriticalFailure("Hidder: Write-> "+ txt)
		//	OneMessage("Cant Hide "+roottr.id+" - "+e)	
		}
	}
}	


function MsgView (TableDna)
{
	this.msgmgr = new MsgTable (TableDna)
	this.status=false
	this.ndx=0
	this.Cacher = new Array()
	this.donthide=false
	this.MaxLength  = function (l)
	{
		this.msgmgr.MaxLength(l)
	}

	this.Clear = function ()
	{
		this.Cacher.remove(0,this.Cacher.length)
	}

	this.Write = function (txt)
	{
		try
		{
			this.Cacher[this.Cacher.length]=new MsgCache(txt)
			if (this.status)
				this.msgmgr.Write(txt)

			ll=this.Cacher.length
			llim=this.msgmgr.LengthLimit
			if (ll > llim)
			{
				nr=ll-llim
				this.Cacher.remove(0,nr)
			}
		} catch (e) {
			CriticalFailure("Bugger: Write-> "+ txt)
		}
	}

	this.CantHide = function ()
	{
		this.donthide=true
	}

	this.Hide = function ()
	{
		this.status=false
		if (!this.donthide) this.msgmgr.Hide()
	}

	this.Show = function ()
	{
		this.Hide()
		try
		{
			this.status=true
			this.dblcache = new Array()
			this.l=this.Cacher.length
			this.lim=this.msgmgr.LengthLimit
			this.start=0
			if (this.l > this.lim)
				this.start=this.l-this.lim
			this.end=this.l

			this.newndx = 0
			this.vsh=0
			for (this.vsh=this.start; this.vsh < this.end; this.vsh++)
			{
				this.newndx++
				this.msgmgr.Write(this.Cacher[this.vsh].txt)
			}
		} catch (e) {
			CriticalFailure("Bugger: Hide -> "+ txt)
		}
	}
	this.MouseEvent = function(o,txt)
	{
		clientpos="["+o.clientX+"x"+o.clientY+"]"
		style=o.relatedTarget.getAttribute("class")
		this.Write(txt+" "+o.relatedTarget.id+" - "+clientpos+" - "+style)
	}
}


function NoView (TableDna)
{
	this.msgmgr = new MsgTable (TableDna)
	this.status=false
	this.ndx=0
	this.Cacher = new Array()
	this.donthide=false
	this.MaxLength  = function (l) { }
	this.Clear = function () { }
	this.Write = function (txt) 
	{
	}
	this.CantHide = function () { }
	this.Hide = function () { } 
	this.Show = function () { }
	this.MouseEvent = function(o,txt) { }
}

