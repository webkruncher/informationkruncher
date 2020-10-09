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




function WinMover(handlerwindow,targetmover)
{
	this.handler=handlerwindow
	this.target=targetmover
	this.name=handlerwindow.id
	MouserFunctions.Add(this)

	this.hww=handlerwindow.clientWidth
	this.hhw=handlerwindow.clientHeight
	this.hot=handlerwindow.offsetTop
	this.hol=handlerwindow.offsetLeft

	this.tww=targetmover.clientWidth
	this.thw=targetmover.clientHeight
	this.tot=targetmover.offsetTop
	this.tol=targetmover.offsetLeft

	this.deltatop=this.hot-this.tot
	this.deltaleft=this.hol-this.tol
	this.deltamy=this.hot-my
	this.deltamx=this.hol-mx
	this.lastmx=mx
	this.lastmy=my
	
	this.onmouseup=function()
	{
		MouserFunctions.Remove(this.name)
	}


	this.Update=function(eventitem)
	{
		this.style=this.target.getAttribute("style")
		this.dx=this.lastmx-mx
		this.dy=this.lastmy-my
		this.nowtop=this.target.offsetTop
		this.nowleft=this.target.offsetLeft
		this.nowleft-=this.dx
		this.nowtop-=this.dy
		this.mndx=this.style.indexOf("left:");
		if (this.mndx>0) this.style=this.style.substr(0,this.mndx);
		this.mndx=this.style.indexOf("top:");
		if (this.mndx>0) this.style=this.style.substr(0,this.mndx);
		this.style=this.style+"position:absolute;top:"+this.nowtop+"px;left:"+this.nowleft+"px;";
		this.lastmx=mx
		this.lastmy=my
		this.target.setAttribute("style",this.style)
	}
}

function NewWinMover(handler,target)
{
	if (handler.wmover) handler.wmover.onmouseup();
	handler.wmover=WinMover(handler,target)
}

