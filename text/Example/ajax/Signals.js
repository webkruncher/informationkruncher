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

function AddThisSignal(form,signalnode)
{
	this.xml=signalnode
	if (this.xml.nodeType==4) 
		if (this.xml.nodeName=='#cdata-section')
		{
			if(this.xml.parentNode.nodeName=="error")
				UserMessages.Write(this.xml.nodeValue)
			if(this.xml.parentNode.nodeName=="trace")
				form.trace(this.xml.nodeValue)
			if(this.xml.parentNode.nodeName=="toctic")
				form.toctic(this.xml.nodeValue)
			if(this.xml.parentNode.nodeName=="submit")
				form.nextSibling.nextSibling.load()
			else
				form.nextSibling.nextSibling.values[this.xml.parentNode.nodeName]=this.xml.nodeValue
		} 
	for (this.i=0;this.i<this.xml.childNodes.length;this.i++)
		new AddThisSignal(form,this.xml.childNodes[this.i])
}

function tester(where)
{
	//this.style="color:blue;background:gray;display:block;height:"+(this.h*2)+"px";
	//where.setAttribute("style",this.style)
	for (this.p=0;this.p<750;this.p++)
	{
		sp=GetObject('spot')
		point=sp.cloneNode(true)
		id="sp_"+uniqueid()
		point.setAttribute('id',id)
		where.appendChild(point)
		this.l=Math.floor(this.p)
		this.t+=where.offsetTop
		this.l+=where.offsetLeft;
		this.style="left:"+this.l+"px;"
		this.style+="top:"+this.t+"px;"
		sp.setAttribute("style","font-family:times new roman;position:absolute;display:inline;color:red;"+this.style)
		sp.innerHTML="&#46"
	}

}

function Mark(where,T,H,txt)
{
	sp=GetObject('spot')
	this.point=sp.cloneNode(true)
	id="m_"+uniqueid()
	this.point.setAttribute('id',id)
	where.appendChild(this.point)
	this.l=T
	this.t=H
	this.t+=where.offsetTop
	this.l+=where.offsetLeft;
	this.style="left:"+this.l+"px;"
	this.style+="top:"+this.t+"px;"
	sp.setAttribute("style","font-family:times new roman;position:absolute;display:inline;color:blue;"+this.style)
	sp.innerHTML=txt
}

function InnerPlotSignal(id)
{
	try
	{
		this.where=GetObject(id)
		if (this.where.p==0)
		{
			signal=this.where.signal
			this.T=(1000/2);	// 1000/2 px = 1 sec
			this.inc=this.T/this.where.rate;
			this.where.points=this.where.signal.split(",");
			this.h=100;//this.max-this.min
			this.style="width:500px;color:blue;background:gray;display:block;height:100px";
			this.where.setAttribute("style",this.style)
			Mark(this.where,this.T,0,"1s");
			Mark(this.where,-30,0,"10A");
			Mark(this.where,-30,90,"-10A");
			this.hr=document.createElement("div")
			this.middle=this.where.offsetTop+(this.h/2)
			this.midstyle="display:block;background:blue;width:500px;color:blue;height:1px;position:absolute;top:"+this.middle+"px;";
			this.hr.setAttribute("style",this.midstyle)
			this.where.appendChild(this.hr)
		}
		this.where.ender=this.where.points.length-this.where.p
		if (this.where.ender>100) this.where.ender=10

		for (this.where.subp=0;this.where.subp<this.where.ender;this.where.subp++)
		{
			sp=GetObject('spot')
			this.point=sp.cloneNode(true)
			id="sp_"+uniqueid()
			this.point.setAttribute('id',id)
			this.where.appendChild(this.point)
			this.l=Math.floor(this.where.p*this.inc)
			this.t=this.where.points[this.where.p]*(this.where.amps/2)
			this.t+=((this.h/2))
			this.t+=parseFloat(this.where.offsetTop)
			this.t-=4
			this.l+=parseFloat(this.where.offsetLeft);
			this.style="left:"+this.l+"px;"
			this.style+="top:"+this.t+"px;"
			sp.setAttribute("style","font-size:0.8em;font-family:times new roman;position:absolute;display:inline;color:red;"+this.style)
			sp.innerHTML="&#8729;";
			this.where.p++;
		}
		if (this.where.p<this.where.points.length)
		{
			this.where.timeout=setTimeout("InnerPlotSignal('"+this.where.id+"')",1)
		} else this.where.timeout=undefined
	} catch (e) {
		UserMessages.Write(e)
	}
}

function PlotSignal(where,signal,rate,amps)
{
	this.where=where
	this.signal=signal
	this.rate=parseFloat(rate)
	this.amps=parseFloat(amps)
	this.go=function()
	{
		if (this.where.id=="trace") this.where.id="sig_"+uniqueid()
		while (this.where.childNodes.length)
			this.where.removeChild(this.where.lastChild)
		this.where.signal=this.signal
		this.where.rate=this.rate
		this.where.amps=this.amps
		this.where.p=0;
		this.where.subp=0
		if (this.where.timeout!=undefined)
			killTimeout(this.where.timeout)
		this.where.timeout=setTimeout("InnerPlotSignal('"+this.where.id+"')",10)
	}
}



function AddCombined(form,signalnode)
{
	this.xml=signalnode
	if (this.xml.nodeType==4) 
		if (this.xml.nodeName=='#cdata-section')
		{
			if(this.xml.parentNode.nodeName=="trace")
				form.trace(this.xml.nodeValue)
		} 
	for (this.i=0;this.i<this.xml.childNodes.length;this.i++)
		new AddCombined(form,this.xml.childNodes[this.i])
}

function CombineSignalsList(forms)
{
	//alert('under construction')
	ShowObject("Combine","block")
	this.combine=GetObject("Combine")
	this.values={}
	this.combine.response=function(xml)
	{
		new AddCombined(this,xml)
	}
	this.combine.trace=function(v)
	{
		if (v=="0") return;
		this.forms.v=v
		this.plotter=new PlotSignal(this,v,512,10)
		this.plotter.go()
	}
	this.combine.forms=forms
	this.post="<form>\n<signals>\n";
	for (this.j=0;this.j<forms.childNodes.length;this.j++)
	{
		this.o=forms.childNodes[this.j]
		this.post+="<signal>"
		this.post+=this.o.childNodes[1].signal
		this.post+="</signal>\n"
	}
	this.post+="</signals>\n</form>\n";
	new actionpost("/demos/fft/aggregate",this.post,this.combine)
}

function ShowFftResults(textarea,signalnode)
{
	this.xml=signalnode
	if (this.xml.nodeType==3) 
		if (this.xml.nodeName=='#text')
		{
				textarea.value=(this.xml.nodeValue)
		} 
	for (this.i=0;this.i<this.xml.childNodes.length;this.i++)
		new ShowFftResults(textarea,this.xml.childNodes[this.i])
}

function FastFourieTransform(v)
{
	ShowObject("ResultFft","block")
	ShowObject("TransformedHeader","block")

	this.fft=GetObject("ResultFft")
	this.values={}
	this.fft.response=function(xml)
	{
		new AddCombined(this,xml)
	}
	this.fft.trace=function(v)
	{
		if (v=="0") return;
		this.plotter=new PlotSignal(this,v,512,10)
		this.plotter.go()
	}
	this.post="<form>\n<signals>\n";
	{
		this.post+="<signal>"
		this.post+=v
		this.post+="</signal>\n"
	}
	this.post+="</signals>\n</form>\n";
	new actionpost("/demos/fft/fourie",this.post,this.fft)
}

