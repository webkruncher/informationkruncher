

function Goallie()
{
	this.force=10;
	this.direction=00;
	this.distance=0
	this.way=1

	this.Turning = 0
	this.txt="?";

	this.d=0;
	this.approaching=false
	this.approach=0


	this.Seek = function (pforce,pdirection,psx,psy)
	{
		if (this.distance > 100) return
		this.way*=-1
		this.approach=3
	}

	this.LoopCheck = function ()
	{
		if (this.approach)
		{
			if (this.distance<d)
			{
				if (this.flip)
				{
					this.flip=false
					this.approach-=1
					if (!this.approach)
					{
						this.way*=-1
					}
				}
			} else {
				this.flip=true
			}
		}
	}

	this.Update = function (ps)
	{
		disty = ps.my - ps.ps.Motivation.GetY()
		distx = ps.mx - ps.ps.Motivation.GetX()
		d = Math.sqrt( (distx * distx) + (disty * disty) )
		this.LoopCheck()
		this.distance=d
		if (d) d=Math.sqrt(d*4)		
		this.Turning=d*this.way
		this.d+=this.Turning
		this.direction=this.d.Normalize(-360,360,-Math.PI,Math.PI);
	}

}

function Lemming(myname,ps)
{
     this.myname=myname;
     this.ps=ps;
     this.mx=0;
     this.my=0;
     this.Wander=0;
     this.direction=0;//Random(360);
     this.lastmx=0;
     this.lastmy=0;

     this.Ticks = function () {return new Date();}
     this.Delta = function (a,b) {return b - a;}
     this.Now = this.Ticks().getTime();
     this.Ellapsed = function () {return this.Delta ( this.Now, this.Ticks().getTime() );}
     this.Set = function () { this.Now = this.Ticks().getTime(); }

     this.TurnTicker = new Ticker();
     this.Goal = new Goallie();

     this.Tick = function ()
     {
          if (this.Ellapsed() > 100)
          {
               this.Wander=1;
          } else {
               this.Wander=0;
          }
     }

     this.Wanderer = function ()
     {
          this.Tick();
          if (this.Wander)
          {
               if (this.TurnTicker.Ellapsed() > 100)
               {
                    this.Goal.Seek(this.ps.Motivation.force,this.ps.Motivation.direction,this.ps.Motivation.GetX(),this.ps.Motivation.GetY());
                    this.TurnTicker.Set();
			seeking.Update(this.mx,this.my," !  "+this.mx+"x"+this.my)

               } else {
                    this.Goal.Update(this)
               }


               dx = this.Goal.force * Math.cos(this.Goal.direction);
               dy = this.Goal.force * Math.sin(this.Goal.direction);

               xx=this.ps.Motivation.GetX()+dx;
               yy=this.ps.Motivation.GetY()+dy;
		

		disty = this.my - this.ps.Motivation.GetY()
		distx = this.mx - this.ps.Motivation.GetX()
		  d = Math.sqrt( (distx * distx) + (disty * disty) )
			current.Update(xx,yy," ? " + this.mx+"x"+this.my+"<br />"+d.pretty())

               this.ps.Chase(xx,0,yy,0,1);
               f=this.myname+'.Wanderer()';
               setTimeout(f,10);
          } else {
               f=this.myname+'.Wanderer()';
               setTimeout(f,10);
          }
     }

     this.MouseMoved = function (mmmx,mmmy) 
     {
          this.Set();
          this.mx=mmmx;
          this.my=mmmy;
		seeking.Update(this.mx,this.my," -> "+this.mx+"x"+this.my)
          dy=mmmy-this.lastmy
          dx=mmmx-this.lastmx
          d = Math.atan2(dy, dx)
	  this.Goal.d=d.Normalize(-Math.PI,Math.PI,-360,360);
          this.lastmx=mmmx;
          this.lastmy=mmmy;
     }
     this.TurnTicker.Set();
     this.Wanderer();

}

