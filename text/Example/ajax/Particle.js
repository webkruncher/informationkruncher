//MsgTable.prototype.

function ParticleSystem(pbugger,myname,depth,maxdepth,side,mode,todo)
{
	this.Mood = new MyMood();
	this.Todo=todo;
	if (myname=="nobody") return;
	this.mode=mode;
	this.side=side;
	this.maxdepth=maxdepth;
	this.depth=depth;
	this.d = this.depth-1;
	this.Motivation=new ParticleMotion();
	this.Motivation.d=this.maxdepth-this.d;
	this.myname=myname;
	this.Piece=new Thing(this);

	this.interval=Random(20)+10
	this.l=null;
	this.r=null;
	this.callback=null;
	this.displayed=false;
	this.display=true;
	this.X=this.Motivation.GetX();
	this.Y=this.Motivation.GetY();

	this.owner=null;

	this.Collide=new CrashTestDummy(this,this.ItemBugRow);

	this.PleaseDie=0;

	this.Spawn = function ()
	{
		this.leftname=this.myname + this.d + 'L';
		this.l=new ParticleSystem(0,this.leftname,  this.d,this.maxdepth,"L",this.mode,null);
		this.le=(this.leftname  + " = " + "this.l" );
		eval(this.le);
		this.l.owner=this;
		this.righttname=this.myname + this.d + 'R';
		this.r=new ParticleSystem(0,this.righttname, this.d,this.maxdepth,"R",this.mode,null);
		this.re=(this.righttname  + " = " + "this.r" );
		eval(this.re);
		this.r.owner=this;
	}

	if (this.depth > 0) this.Spawn();

	this.Go = function ()
	{
		if (this.PleaseDie)
		{
			this.Die();
			return;
		}
		this.LifeAndDeath();
		if (this.Piece) this.Piece.Update();
		if (this.Motivation.Decay(this.owner))
		{
			this.display=true;
			halfw=this.maxw>>1;
			halfh=this.maxy>>1;
			doubw=this.maxw<<1;
			doubh=this.maxy<<1;

			this.Collide.Update(  this.Motivation.GetX(this.owner), this.Motivation.GetY(this.owner) );

			if (this.owner)
			{
				X=this.Motivation.GetX(this.owner)-(halfw);
				Y=this.Motivation.GetY(this.owner)-(halfh);
			}

			WW = ( viewportwidth + scrollposX );
			HH = ( viewportheight + scrollposY );

			ok = true;
			if ( X < ( scrollposX + doubw ) ) ok = false;
			if ( X > ( WW - doubw ) ) ok = false;
			if ( Y < ( scrollposY + doubh ) ) ok = false;
			if ( Y > ( HH - doubh ) ) ok = false;

			if ( !ok ) 
			{
				this.display=false;
			} else {
				this.X=X;
				this.Y=Y;
				this.display=true;
			}
		} else {
			this.display=false;
		}


		if (this.displayed) this.Kickit();
	}

	this.Die = function()
	{
		clearTimeout(this.callback);
		this.Piece.Die()
	}

	this.Nuke = function()
	{
		this.Die()
			if (this.l) this.l.Nuke();
		if (this.r) this.r.Nuke();
	}

	this.ClearLeft = function () 
	{
		this.l.Die(); 
		this.l=null;
	}

	this.ClearRight = function () 
	{
		this.r.Die()
			this.r=null;
	}

	this.SetMode = function(m)
	{
		this.txt='';
		this.mode=m;
		if (this.l) this.l.SetMode(m);
		if (this.r) this.r.SetMode(m);
	}

	this.KickMe = function (decay) 
	{
		this.Motivation.Bump(decay);
		if (this.l) this.l.KickMe(decay>>1);
		if (this.r) this.r.KickMe(decay>>1);
		if (!this.displayed) {this.Kickit();this.displayed=true;} 
	}

	this.Chase = function (mx,ox,my,oy,decay)
	{
		this.Motivation.GoTo(this,mx,my);
		this.KickMe((1<<this.maxdepth)*5);
	}

	this.EveryoneGoTo=function(x,y)
	{
		this.Motivation.GoTo(this,x,y);
		this.X=x;
		this.Y=y;
		if (this.l) this.l.EveryoneGoTo(x,y);
		if (this.r) this.r.EveryoneGoTo(x,y);
	}

	this.Kickit = function()
	{
		if (this.PleaseDie)
		{
			this.Die();
		} else {
			f=this.myname+'.Go()';
			this.callback=setTimeout(f,this.interval);        

			this.Mood.Expect(this);
		}
	}

	this.Grow = function()
	{
		this.maxdepth=this.maxdepth+1;
		this.d=this.d+1;
		this.depth=this.depth+1;
		if (this.l) 
		{    
			this.l.Grow(); 
			if (this.r) this.r.Grow(); 
		} else {
			this.Spawn();
		}
	}

	this.Shrink=function()
	{
		this.maxdepth=this.maxdepth-1;
		this.d=this.d-1;
		this.depth=this.depth-1;
		if (this.l) {this.l.Shrink(); if (this.r) this.r.Shrink(); return;}
		if (!this.owner) return;
		if (this.side=="L") this.owner.ClearLeft();
		if (this.side=="R") this.owner.ClearRight();
	}

	this.Shutdown=function ()
	{
		this.l=null;
		this.r=null;
	}


	this.Kickit();
	this.StartToDo = function()
	{
		if (this.Todo)
		{
			ff=this.myname+".Todo.RunUpdate()";
			setTimeout(ff,700);
		}
	}

	this.DontReProduce = function()
	{
		this.Piece.fertile=false;
		this.Piece.ThingTicker=-1;
		if (this.l) this.l.DontReProduce();
		if (this.r) this.r.DontReProduce();
	}

	this.RootNode = function ()
	{
		if (this.owner) return this.owner.RootNode();
		else return this;
	}

	this.Decease = function()
	{
		this.PleaseDie=1;
		if (this.l) this.l.Decease();
		if (this.r) this.r.Decease();
	}

	this.LifeAndDeath = function()
	{
		return;
		if ( (!this.r) && (!this.l) )
		{
			Feeling = this.Mood.Feeling(this)
				if (Feeling >= Happy) 
				{
					{
						Consensus.Row("GR_"+this.myname);
						Consensus.Column(this.myname);
						Consensus.Column("*");
						Consensus.Commit();
						if (this.l) this.l.owner=this;
						if (this.r) this.r.owner=this;
						if (this.l) this.l.DontReProduce();
						if (this.r) this.r.DontReProduce();
					}
				}

			if (Feeling <= Sad) 
				{
					Consensus.Row("GR_"+this.myname);
					Consensus.Column(this.myname);
					Consensus.Column("-");
					Consensus.Commit();
					this.Decease();
				}
		}
	}
}



