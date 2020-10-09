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


pbias = 0
function ParticleMotion()	// makeshift physics object
{
	this.d=0;
	this.decay=0;
	this.PX=Random(400);
	this.PY=Random(400);
	this.PX+=200;
	this.PY+=200;
	this.rf=10
	this.o=Random(this.rf)+1

	this.direction=-1;
	this.distance=0;
	this.force=0
	this.turning=0;

	this.Turn=function(t){this.turning=t;}

	this.Dead = function(ps)
	{
		if (!this.force)  return;
		if (Math.abs(this.force) > 3) return;
		if (!ps.owner) return;
		if (!ps.owner.owner) return;
		if (ps.l) return;
		if (ps.r) return;
		ps.Die();
	}

	this.FollowOwner=function(ps)
	{
		if (ps)
		{
			this.wave+=0.10
			if (this.wave > 1) this.wave=-1
			disty = ps.Motivation.GetY() - this.GetY() 
			distx = ps.Motivation.GetX() - this.GetX()
			this.direction = Math.atan2(disty, distx)
			this.distance=Math.sqrt( (distx * distx) + (disty * disty) )
			a = this.wave + Math.PI
			j=(ps.maxdepth-ps.d+1)*2
			this.force= a*(this.distance/j)
			this.force/=this.o
			dx = this.force * Math.cos(this.direction);
			dy = this.force * Math.sin(this.direction);
			xx=this.GetX()+dx;
			yy=this.GetY()+dy;
			this.PX=xx
			this.PY=yy
		}
	}

	this.KeepGoing=function(ps)
	{
		if (ps)
		{
			dx = this.force * Math.cos(this.direction);
			dy = this.force * Math.sin(this.direction);
			xx=this.GetX()+dx;
			yy=this.GetY()+dy;
			this.Dead(ps)
			this.PX=xx
			this.PY=yy
			this.direction+=this.turning;
		}
	}


	this.Think = this.FollowOwner;

	this.wave = ( Random(Math.PI*100)/100 ) - Math.PI

	this.GoTo = function (ps,xx,yy)
	{
		pbias+=0.01
		if (pbias > 1) pbias=-1
		this.PX=xx;
		this.PY=yy;
		//Diagnostics.Write('>' + xx + 'x' + yy)
	}

	this.Bump = function (d)
	{
		if (d > 0) 
			this.decay=d;
		else this.decay=0;
	}

	this.Decay = function(ps)
	{
		if (this.decay) this.decay=this.decay-1;
		if (this.Think) this.Think(ps);
		return this.decay;
	}

	this.ContinueForward=function()
	{
		this.Think=this.KeepGoing;
	}

	this.GetX = function(ps) { return this.PX; }
	this.GetY = function(ps) { return this.PY; }
}


