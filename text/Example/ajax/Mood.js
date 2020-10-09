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


