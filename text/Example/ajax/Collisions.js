
function CrashTestDummy(ps,ItemBugRow)
{
	this.ps=ps;
	this.ItemBugRow=ItemBugRow
	this.Status=0
	this.LastStatus=0
	this.DistanceToImpact=0
	this.Detect = function()
	{
		if (ActiveMenuLink)
			return ActiveMenuLink.Detect(this.DistanceToImpact )
		return true
	}

	this.Update = function(X,Y)
	{
		if (ActiveMenuLink)
			this.DistanceToImpact=ActiveMenuLink.Assess(X,Y)
	}
}

