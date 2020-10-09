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



mouselemming=null;
mousedmovedonce=false
mousetrails=null


var afetimes = 0
function artificialentry()
{
	if (mousedmovedonce) return
	GetViewport();
	getMousePosition(e);
	afemx=(10*afetimes)+200
	afemy=(10*afetimes)+200
	mousetrails.Chase(afemx,0,afemy,0,1);
	if (mouselemming) 
		mouselemming.MouseMoved(afemx,afemy);
	afetimes++
	if (afetimes < 3)
		setTimeout('artificialentry()',100)
	//Diagnostics.Write(afetimes)
}

function InitMouseTrails(throttle,pb,mode)
{
	seeking=new Plot("seeking","blue","yellow");
	current=new Plot("current","green","black");
	mousetrails = new ParticleSystem(pb,'m',2,1,"T",mode,null);
	mouselemming = new Lemming("mouselemming",mousetrails);
	setTimeout('artificialentry()',100)
}

function FeedSparky() 
{
	if (mousetrails) mousetrails.Grow();
}

function StarveSparky() 
{
	if (mousetrails) mousetrails.Shrink();
}

function CallStackItem(name,fun,target)
{
	this.name = name
	this.fun = fun
	this.target=target
	this.Update = function(eventitem)
	{
		eval(this.fun(this.name,this.target,eventitem))
	}
}

function CallingStack()
{
	this.Stacker = new Array()
	this.Update = function(eventitem)
	{
		for (csi=0;csi < this.Stacker.length; csi++)
			this.Stacker[csi].Update(eventitem)
	}
	this.Add = function(item)
	{
		this.Stacker[this.Stacker.length]=item
	}
	this.Remove = function(name)
	{
		kill=-1
		for (csi=0;csi < this.Stacker.length; csi++)
			if (this.Stacker[csi].name==name)
				kill=csi
		if (kill!=-1)
			this.Stacker.remove(kill)
	}

}

var ScrollerFunctions = new CallingStack()
var SizerFunctions = new CallingStack()
var MouserFunctions = new CallingStack()

window.onscroll = function()
{
	getScrollingPosition();
	ScrollerFunctions.Update()
};

window.onresize = function()
{
	getScrollingPosition();
	SizerFunctions.Update()
};


function mouser(e) 
{
	mousedmovedonce=true
	GetViewport();
	getMousePosition(e);
	mx=posX;
	my=posY;
	MouserFunctions.Update(e)
if (0)	if (Diagnostics)
	{
		cx=viewportwidth/2
		cy=viewportheight/2
		force=200
		distx=cx-mx
		disty=cy-my
		direction = Math.atan2(disty, distx)
		distance=Math.sqrt( (distx * distx) + (disty * disty) )
		//Diagnostics.Write(direction)
		if (direction>0)
		{
			if (Math.abs(direction)>(Math.PI/2))
			{
				Diagnostics.Write("Upper Right")
			} else {
				Diagnostics.Write("Upper Left")
			}
		} else {
			if (Math.abs(direction)>(Math.PI/2))
			{
				Diagnostics.Write("Lower Right")
			} else {
				Diagnostics.Write("Lower Left")
			}
		}
	}
}

document.onmousemove = mouser;

csme = new CallStackItem("SparkyMouser",function()
	{
		if (mousetrails)
			mousetrails.Chase(mx,0,my,0,1);
		if (mouselemming) 
			mouselemming.MouseMoved(mx,my);
	}
)

MouserFunctions.Add(csme)

