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


var ActiveMenuLink=null
var MenuMessages=null

function NodePosition(x,y,w,h)
{
	this.x=x
	this.y=y
	this.w=w
	this.h=h
	this.MX=0
	this.MY=0
	this.Inflate = function(byx,byy)
	{
		this.x-=(byx)
		this.y-=(byy)
		this.w+=(2*byx)
		this.h+=(2*byy)
	}
}

function DivObject(doMenuRoot,Parking,root,name,id,parent,where,callback)
{
	this.name=name
	this.doMenuRoot=doMenuRoot
	this.root=root
	this.where=where
	this.parent=parent
	this.UiObj=null
	this.Parking=Parking

	this.Clone = root.cloneNode(false)
	this.callback=callback
	this.Clone.id=id
	this.RootChild=null

	this.Move = function()
	{
		n=this.Clone
		style="position:absolute; width:"+(this.where.w)+"px; height:"+(this.where.h)+"px; top:"+this.where.y+"px; left:"+this.where.x+"px; "
		style+="display:inline;"
		n.setAttribute('style',style)
	}

	this.remove = function()
	{
		n=this.Clone
		style="display:none;"
		n.setAttribute('style',style)
		this.Clone=null
	}

	this.Finishing = function()
	{
		this.RootChild=GetObject(this.Parking).lastChild
		o=this.RootChild.lastChild
		for (xnf=0; xnf < 10; xnf++)
		{
			fn=o.getAttribute("frame")
			if (fn)
			{
				if (fn.length)
				{
					//Whatever(fn)
					xnf=10
				} else {
					//Whatever("?"+o.id)
					o=o.lastChild
				}
			} else {
				//Whatever("$"+o.id)
				o=o.lastChild
			}
		}
		this.UiObj=o
		if (this.callback) this.callback(this)
		this.Move()
		return
	}


	this.Positioning = function(xfrmation) { }


	this.ajax = function()
	{
		Myself=this
		n=Myself.Clone
		style="display:none;"
		n.setAttribute('style',style)
if (buggness) UserMessages.Write("Testing ajax");
		new ajax(this.doMenuRoot+this.name+".xml",this.Clone,
			function() { Myself.Finishing() }
			,null
		)
	};
	this.Move()
	nn=GetObject(this.Parking)
	nn.appendChild(this.Clone)
	this.ajax()
}


function MenuLinkObject(o,wherex,wherey)
{
	TO=o
	this.DivObj=o
	this.wherex=wherex
	this.wherey=wherey
	if (o.mymenuobject)
		o.mymenuobject.MousedAt(wherex,wherey)
	this.Detect = function(d)
	{
		if (TO.DetectCollision)
			return TO.DetectCollision(wherex,wherey)
		collisions=this.DivObj.getAttribute('collisions')
		if (collisions)
		{
			f=collisions+'('+d+')'
			return eval(f)
		}

		if (d < 50)
			return false
		else return true
	}

	this.Assess = function(mlox,mloy)
	{
		disty = mlox - posX
		distx = mloy - posY 
		//this.direction = Math.atan2(disty, distx)
		this.distance=Math.sqrt( (distx * distx) + (disty * disty) )
		return this.distance 
	}
}

function SubKruchMenu(thiskm)
{
	this.kmname=thiskm
}

function SubKrunchMenus(parent)
{
	this.parent = parent
	this.existing=null
	this.added=null

	this.Add = function (mnamen,nkm)
	{
		if (this.existing)
			try{
				this.existing.Dvo.remove()
			}catch(e) {}
		this.existing=nkm
		this.added=mnamen
	}

	this.Exists = function(who)
	{
		return false
	}
}

function KrunchMenu(Parking,root,where,name,parent,kmcallback)
{
	this.SubKms=new SubKrunchMenus(this)
	this.root=root
	this.where=where
	this.parent=parent
	this.name=name
	this.actions = null
	this.kmcallback=kmcallback
	this.Parking=Parking
	this.SubFrameOut = function()
	{
		try
		{
			this.Dvo.remove()
			if (this.parent) this.parent.SubFrameOut()
		} catch (e) { }
	}

	this.FrameOut = function()
	{
		try
		{
			FO=this.Dvo.UiObj.parentNode.parentNode.parentNode
			ww=FO.clientWidth
			hh=FO.clientHeight
			tt=FO.offsetTop
			ll=FO.offsetLeft
			tt=this.Dvo.where.y
			ll=this.Dvo.where.x
			if (!ww) return false
			if (!hh) return false
			if (!Within( posX,posY, ll, tt, ww,hh))
			{
				try
				{
					this.Dvo.remove()
					if (this.parent) this.parent.SubFrameOut()
				} catch (e) { }
				return true
			}
		} catch (e) {
			Whatever(e)
			return false
		}
		return false
	}
	
	this.Write = function(msg)
	{
		if (InitDiagnostics)
			InitDiagnostics.Write(msg)
		if (MenuMessages)
			MenuMessages.Write(msg)
		else
			if (UserMessages)
				UserMessages.Write(msg)
			else 
			{
				Now=new Date()
				sinceopened=Now.getTime()-StartedApplicationAt.getTime()
				if (sinceopened > 4000)
					alert(msg)
			}
	}

	r=GetObject("WholePage")
	TKM=this
	this.Dvo = new DivObject (MenuRoot,this.Parking,r, this.name,this.name+uniqueid(),this, where, 
		function(dvo)
		{
				MenuLoaded=1
			//TKM.Write(".Loaded "+TKM.name+"-"+dvo.UiObj.id)
			TKM.actions = new MenuActions(this.Parking,dvo.UiObj,null,TKM)
			dvo.UiObj.childNodes[0].style.display="block"
			if (TKM.kmcallback)
				TKM.kmcallback(dvo)
		}
	)

	this.GetTopMenu = function()
	{
		return this
		TN=this
		for (mpnti=0;mpnti<10;mpnti++)
		{
			if (TN.parent)
				TN=TN.parent
			else mpnti=10
		}
		return TN
	}

}

function UpdatePopupFrame(nnam)
{
	if (!nnam.MenuActionsObject.km.FrameOut())
	{
		//Whatever("removing:"+nnam.id)
	} else {
		f="UpdatePopupFrame("+nnam.id+")"
		setTimeout(f,200)
	}
}

function MenuFrameMouseOut(o)
{
	t=o.childNodes[1].firstChild.firstChild
	name=t.id
	f="UpdatePopupFrame("+name+")"
	setTimeout(f,200)
}

