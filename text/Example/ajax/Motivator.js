
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


