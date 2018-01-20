

var Happy = 40;
var Sad = -40;

function OverallFeeling()
{
	this.Poll = 0;
	this.clock = new Date();
	this.Block=false;
	this.Reset = function () { this.Block=false; }
	this.Gate = function()
	{
		if (this.Block) return true;
		b=this.Block;
		this.Block=true;
		if (b) return true;
		if (!b) setTimeout("Everybody.Reset()",100);
		return b;
	}

	this.Feeling  = function(ps,mood)
	{
		if (this.Gate()) return 0;
		
		if (mood > 0)
		{
			if (this.Poll < Happy) this.Poll += mood;
		} else {
			if (this.Poll > Sad) this.Poll += mood;
		}
		results = this.Poll;
		if (this.Poll >= Happy) this.Poll -= Math.abs(mood);
		if (this.Poll <= Sad) this.Poll = Math.abs(mood);

		if ( (results > Happy) || (results < Sad) )
		{
			DiagBug.Row("DB_r_"+ps.myname);
			DiagBug.Column(ps.myname);
			DiagBug.Column(this.Poll);
			DiagBug.Column(results)
			if (results > Happy) DiagBug.Column("+");
			else
				if (results < Sad) DiagBug.Column("-");
				else
					DiagBug.Column("---")
			DiagBug.Commit();
		}
		return results;
	}
}

Everybody  = new OverallFeeling();

function MyMood()
{
	this.expect=0;
	this.When = Random(10)+20;
	this.tick=0;
	this.Feeling = function(ps)
	{
	return 0
		if (!this.expect) return 0;
		this.Clock = new Date();
		now = this.Clock.getTime();
		results = 0;
		results = (  this.expect - now );
		R= Everybody.Feeling(ps,results);
		return R;
	}

	this.Expect = function(ps)
	{
	return
		expectation=ps.interval;
		this.tick+=1;
		if (this.tick < this.When) {this.expect=0; return;}
		this.tick=0;
		this.clock = new Date();
		now = this.clock.getTime();
		this.expect=now+expectation;
	}
}


