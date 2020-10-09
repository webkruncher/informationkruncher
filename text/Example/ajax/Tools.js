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



Number.prototype.wave = function (rate,hz,angle)
{
   tr=this/rate
   return Math.sin((Math.PI*hz*tr)+angle)

}

Number.prototype.wavecycle=function(rate,hz,angle)
{
	this.n=this
	this.n=this.n.wave(rate,hz,angle)
	this.n+=1
	this.n*=128
	return Math.floor(this.n)
}

Number.prototype.zeroFormat = function(n, f, r){return n = new Array((++n, f ? (f = (this + "").length) < n ? n - f : 0 : n)).join(0), r ? this + n : n + this;};

Number.prototype.pretty = function ()
{
          return Math.round(this*100)/100;
}


Number.prototype.hexcolornumber = function(r,g,b)
{

   r=r.Clamp(0,255)
   g=g.Clamp(0,255)
   b=b.Clamp(0,255)
   r=r.toString(16)
   g=g.toString(16)
   b=b.toString(16)
   while(r.length<2) r="0"+r
   while(g.length<2) g="0"+g
   while(b.length<2) b="0"+b
   return r+g+b
}


Number.prototype.Clamp = function (lo,hi)
{
	if (this<lo) 
		return hi-(low-this)
	else
		if (this>hi) 
			return lo+(this-hi)
		else 
			return this
}

Number.prototype.Normalize = function(fromlow,fromhi,tolow,tohi)
{
	num=this;
	fromrange = fromhi - fromlow;
	distfromfromlow=num-fromlow;
	torange=tohi-tolow;
	
	sourcepercent=distfromfromlow/fromrange;
	fromtolow=sourcepercent*torange;
	answer=tolow+fromtolow;
	return answer;
}
    

function innerGetObject(name)
{
	if (document.layers){return document.layers[name];}
	else if (document.all){return document.all[name];}
	else if (document.getElementById){return document.getElementById(name);}
}

function GetObject(name)
{
	o=innerGetObject(name)
		
	return o
}


function setVisible(obj,sw)
{
	if (document.layers)
	{
		s = (sw) ? 'show' : 'hide'
		obj.visibility = s;
	}
	else if (document.all)
	{
		s = (sw) ? 'visible'	: 'hidden';
		obj.style.visibility = s;
	}
	else if (document.getElementById)
	{
		s= (sw) ? 'visible' : 'hidden';
		obj.style.visibility = s;
		s= (sw) ? 'inline' : 'none';
	}
}

function setVisibleStyle(obj,sw)
{
	if (document.layers)
	{
		s = (sw) ? 'show' : 'hide'
		obj.visibility = s;
	}
	else if (document.all)
	{
		s = (sw) ? 'visible'	: 'hidden';
		obj.style.visibility = s;
	}
	else if (document.getElementById)
	{
		this.tstyle=obj.getAttribute("style")
		this.tstyle=this.tstyle.ChangeStyle("display", s)
		obj.setAttribute("style", this.tstyle)
	}
}

function Random(m)
{
    var ranNum= Math.floor(Math.random()*m);
    return ranNum;
}

var isIE = document.all ? true : false;
var posX=-1;
var posY=-1;

function getMousePosition(e) 
{
	if (!isIE) {
		posX = e.pageX;
		posY = e.pageY;
	}
	if (isIE) {
		posX = event.clientX + document.body.scrollLeft;
		posY = event.clientY + document.body.scrollTop;
	}
	return true;
}


var viewportwidth=0;
var viewportheight=0;
 
function GetViewport () 
{
	if (typeof window.innerWidth != 'undefined')
	{
		viewportwidth = window.innerWidth,
		viewportheight = window.innerHeight
	}
	
	
	else if (typeof document.documentElement != 'undefined'
		&& typeof document.documentElement.clientWidth !=
		'undefined' && document.documentElement.clientWidth != 0)
		{
			viewportwidth = document.documentElement.clientWidth,
			viewportheight = document.documentElement.clientHeight
		}
	
		else
		{
			viewportwidth = document.getElementsByTagName('body')[0].clientWidth,
			viewportheight = document.getElementsByTagName('body')[0].clientHeight
		}
}


scrollposX=0;
scrollposY=0;

function getScrollingPosition()
{
	if (typeof window.pageYOffset != 'undefined')
	{
		position = [window.pageXOffset,window.pageYOffset];
	}
	else if (typeof document.documentElement.scrollTop != 'undefined' && document.documentElement.scrollTop > 0)
	{
		position = [document.documentElement.scrollLeft,document.documentElement.scrollTop];
	}
	else if (typeof document.body.scrollTop != 'undefined')
	{
		position = [document.body.scrollLeft,document.body.scrollTop];
	}
	scrollposX=parseInt(position[0]);
	scrollposY=parseInt(position[1]);
	return position;
}

function Clamp(i,b,t)
{
	if (i < b) return b;
	if (i > t) return t;
	return i;
}

function Ticker()
{
	this.Ticks = function () {return new Date();}
	this.Delta = function (a,b) {return b - a;}
	this.Now = this.Ticks().getTime();
	this.Ellapsed = function () {return this.Delta ( this.Now, this.Ticks().getTime() );}
	this.Set = function () { this.Now = this.Ticks().getTime(); }
}
	var mousetrails = null;
	var traillimitter=2

function ThrottleManager(name)
{
	this.myname=name;
	this.UpdateCall = this.myname+'.Update()';
	this.interval = 100;
	this.mood=0;
	this.Benchmarks = new Ticker;
	this.accumulator=0;

	this.Delta = function () 
	{
		n = this.Benchmarks.Ellapsed();
		this.Benchmarks.Set();
		return n;
	}


	this.Update = function ()
	{
		this.accumulator=0;
		if (mousetrails) mousetrails.SetThrottle(this,null);
		traillimitter=0;
		if (this.accumulator < 20) traillimitter=1;
		if (this.accumulator < 10) traillimitter=3;

		this.delta=this.Delta();

		if (this.delta < this.interval)
		{
			this.mood=this.mood+1;
			this.interval=this.interval+1;
		}


		if (this.delta > this.interval)
		{
			this.mood=this.mood-1;
			this.interval=this.interval-1;
		}

		if (this.delta > (this.interval+100) )
		{
			this.mood=-10;
			this.interval=this.interval-1;
		}


		//Bugger.Placement([0,0],".");
		//Bugger.Placement([[0,0],[0,1]],'a');
		//Bugger.Placement([0],"mood:" + this.mood );
                //Bugger.Placement([1],"delta:" + this.delta);
                //Bugger.Placement([2],"interval:" + this.interval)
                //Bugger.Placement([3,0],"counter_1:" + this.accumulator)
                //Bugger.Placement([3,1],"counter_2:" + this.accumulator)


               

		this.interval = Clamp(this.interval,100,1000);
		this.mood = Clamp(this.mood,-10,10);
	}

	Interval = setInterval(this.UpdateCall,this.interval);  

	this.Rating = function ()
	{
		return this.interval;
	}

	this.Mood = function ()
	{
		return this.mood;
	}

	this.AlterMood = function (n)
	{
		this.mood = this.mood + n;
	}
}

//http://www.hunlock.com/blogs/Mastering_Javascript_Arrays
var isNumeric = function(x) 
{
   // returns true if x is numeric and false if it is not.
   var RegExp = /^(-)?(\d*)(\.?)(\d*)$/; 
   return String(x).match(RegExp);
}


//This prototype is provided by the Mozilla foundation and
//is distributed under the MIT license.
//http://www.ibiblio.org/pub/Linux/LICENSES/mit.license

if (!Array.prototype.every)
{
  Array.prototype.every = function(fun /*, thisp*/)
  {
    var len = this.length;
    if (typeof fun != "function")
      throw new TypeError();

    var thisp = arguments[1];
    for (var i = 0; i < len; i++)
    {
      if (i in this &&
          !fun.call(thisp, this[i], i, this))
        return false;
    }

    return true;
  };
}

//This prototype is provided by the Mozilla foundation and
//is distributed under the MIT license.
//http://www.ibiblio.org/pub/Linux/LICENSES/mit.license

if (!Array.prototype.forEach)
{
	Array.prototype.forEach = function(fun /*, thisp*/)
	{
		var len = this.length;
		if (typeof fun != "function") throw new TypeError();
		var thisp = arguments[1];
		for (var i = 0; i < len; i++)
		{
			if (i in this)
				fun.call(thisp, this[i], i, this);
		}
	};
}

Array.prototype.remove = function(from, to) 
{
	var rest = this.slice((to || from) + 1 || this.length);
	this.length = from < 0 ? this.length + from : from;
	return this.push.apply(this, rest);
};



function ShowNode(sw,r)
{
	try
	{
		if (sw)
		{
			r.setAttribute("style","display:inline;")
		} else {
			r.setAttribute("style","display:none;")
		}
	} catch (e) {
		if (sw)
		{
			r.style="display:inline;"
		} else {
			r.style="display:none;"
		}
	}
}
		
function ShowDiv(sw,id)
{
	o=GetObject(id)
	ShowNode(sw,o)
}

var luinumber=40
function uniqueid()
{
	luinumber++
	return luinumber
}

function FocusOn(name)
{
	try
	{
		o=GetObject(name)
		o.focus()
	} catch (e) {
		//alert("FocusOn:"+e)
	}
}

function CriticalFailure(msg)
{
	try
	{
		Whatever(msg)
	} catch (e) {}
	ldo=GetObject("LastDitch")
	if (ldo)
		ldo.innerHTML="Goodbye"
	else alert(msg)
}


function UpdateSignal(Point)
{
	id="sgp"+Point
	dv=GetObject(id)
	dv.update()
}



colorping=0
colorpong=1
angle=0.0
angler=null
colorcycle=null
function SampleSignal()
{
	amp=20
	Points=60
	rate=400
	hz=40

       pl=GetObject('plot')
       sp=GetObject('spot')
       pl.style.display='inline'
       sp.style.display='none'

   for (i=0;i<Points;i++)
   {
       id="sgp"+i
       point=sp.cloneNode(true)
       point.setAttribute('id',id)
       pl.appendChild(point)
       dv=GetObject(id)
       dv.innerHTML='*'
       dv.ndx=i
       dv.rate=rate
       dv.hz=hz
       dv.loff=0
       dv.amp=amp
	dv.max=Points
       dv.update=function()
       {
	   if (!angler)
		{
			if (this.interval) clearInterval(this.interval);
			this.style.display='none';
			return;
		}
           t=this.ndx.wave(this.rate,this.hz,angle)
           t*=this.amp
           t=t-100
           t=t.pretty()
           l=(this.ndx*5)+240
		c=0
           c=c.hexcolornumber(this.ndx,colorping,colorping)
		
           style="display:inline;position:absolute;top:"+t+"px;left:"+l+"px;color:#"+c
           //style="display:inline;position:absolute;top:"+t+"px;left:"+l+"px;"
           this.setAttribute('style',style)
		next=this.ndx+1
		if (next>=this.max) next=0
       }
	   dv.interval=setInterval('UpdateSignal('+i+')',1)

   }

   angler=setInterval('angle+=0.1',1)
   colorcycle=setInterval('wavecolor()',3)
}

function wavecolor()
{
	   colorping+=colorpong
	   if (colorping>254) colorpong*=-1
	   if (colorping<1) colorpong*=-1
}

function StopSampleSignal()
{
	if (colorcycle) clearInterval(colorcycle)
	if (angler) clearInterval(angler)
	colorcycle=null
	angler=null
}

function GetAncestor(what,far)
{
	this.what=what
	this.far=far
	this.go=function()
	{
		this.d=this.far-1
		if (!this.d) return what.parentNode
		this.a=new GetAncestor(what.parentNode,this.d)
		return this.a.go()
	}
}

function findNode(o,what)
{
	this.obj=o
	this.sc=null
	this.what=what
	this.go=function()
	{
		for (this.i=0;this.i<this.obj.childNodes.length;this.i++)
		{
			if(this.obj.childNodes[this.i].id)
				if (this.obj.childNodes[this.i].id.toLowerCase()==this.what)
					return this.obj.childNodes[this.i]
			this.sc=new findNode(this.obj.childNodes[this.i],this.what);
			this.ret=this.sc.go()
			if (this.ret) return this.ret
		}
		return null
	}
}

function InnerShowObject(id,how)
{
	this.o=GetObject(id)
	this.o.style.display=how
}

function ShowObject(id,how)
{
	if (GetObject(id).style.display==how) return;
	this.method="InnerShowObject('"+id+"','"+how+"')"
	GetObject(id).timer=setTimeout(this.method,100)
	return false;
}



String.prototype.ChangeStyle = function ( what, how )
{               
        this.c_str=this.substr(0, this.length)
        this.where=this.c_str.indexOf( what+":" )
	if ( this.where != -1 )
	{
		this.left=this.c_str.substr( 0, this.where )
		this.eofield=this.c_str.indexOf( ";", this.where+1 )
		this.right=this.c_str.substr( this.eofield+1, this.length-this.eofield-1 )
		return this.left+what+":"+how+";"+this.right;
	} else {
		return this.c_str+";"+what+":"+how+";"
	}
}


