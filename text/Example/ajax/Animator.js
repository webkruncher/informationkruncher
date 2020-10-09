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


function Animator(myname,DivObj,start,end,inc,interval,maxw,maxy,path,ext,leadingzeros,loop)
{
	this.DivObj=DivObj;
	this.myname=myname;
	this.inc=inc;
	this.start=start;
	this.end=end;
	this.primed=false;
	this.maxw=maxw
	this.maxy=maxy
	this.interval=interval;
	this.Path=path;
	this.Ext=ext;
	this.ndx=this.start;
	this.imgLeadingZeros=leadingzeros;
	this.loop=loop;
	this.OnDone=null;
	this.bkupOnDone=null;
	setVisible(this.DivObj,false);


	this.Clicked  = function (o)
	{
		if ( this.OnDone ) 
		{
			this.bkupOnDone=this.OnDone
			this.OnDone=null
		} else {
			if ( this.bkupOnDone ) 
			{
				this.OnDone=this.bkupOnDone
				this.OnDone()
			}
		}
		if (this.AnimComplete)
		{
			if (this.interval == 0) 
				this.ResetSpinner()
			f=this.myname+'.Spin()';
			setTimeout(f,this.interval); 
		}
	}

	this.ResetSpinner = function ()
	{
		this.interval='1'
		this.ndx=this.spinstart
	}

	this.Spin = function()
	{
		this.ndx++
		if (this.ndx > this.spinend)
		{
			this.interval=0
			return
		}
		this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
		this.DivObj.style.backgroundImage="url("+this.imgname+")";
		f=this.myname+'.Spin()';
		setTimeout(f,this.interval); 
	}

	this.Animate = function ()
	{
		if ( this.primed == false )
		{
			this.ndx=(this.ndx+this.inc);
			if (this.ndx == this.end) 
			{
				this.primed=true;
				this.ndx=this.start;
				this.DivObj.style.width=this.maxw;
				this.DivObj.style.height=this.maxy;
				style="position:absolute; width:"+this.maxw+"; height:"+this.maxy+"; top:20px; left:300px; "
				//this.DivObj.setAttribute('style',style)
				setVisible(this.DivObj,true);
			}
			this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
			this.DivObj.style.backgroundImage="url("+this.imgname+")";
			f=this.myname+'.Animate()';
			setTimeout(f,1); 
		} else {
			this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
			this.DivObj.style.backgroundImage="url("+this.imgname+")";
			f=this.myname+'.Animate()';
			if (this.loop)
			{
				if (this.ndx == this.end)
				{
					this.ndx=this.start;
				}
				this.ndx=(this.ndx+this.inc);
				setTimeout(f,this.interval); 
			} else {
				if (this.ndx == this.end)
				{
					this.interval=0
					this.AnimComplete=true
					if ( this.OnDone ) this.OnDone()

				} else {
					this.ndx=(this.ndx+this.inc);
					setTimeout(f,this.interval); 
				}
			}
		}
	}
}
