
var MsgNdx = 0

function Heartbeat()
{
	try
	{
		Now=new Date()
		
		if (LastAjaxSuccess == null)
		{
			sinceopened=Now.getTime()-FirstLoadedAt.getTime()
			if (sinceopened > 2000)
			{
				SystemLocked(true,"Cant AJax "+sinceopened)
			}
		} else {
			delta=Now.getTime()-LastAjaxSuccess.getTime()
			//if (Diagnostics) Diagnostics.Write(delta+"> Diagnostics")
			if (delta < 0)
			{
				SystemLocked(true,"Unexpected error - system went back in time")
			} else {
				if (delta > 10000)
				{
					if (ConnectionStatus) 
						SystemLocked(true,"Connection interrupted")
					ConnectionStatus=false
				} else {
					if (!ConnectionStatus) 
						SystemLocked(false,"Connection established")
					ConnectionStatus=true
				}
			}

			pto=GetObject("pingtarget")
			pto.innerHTML=""
			if (pto)
				new ajax("ping.xml",pto,function(xfrmd)
				{
					gt=GetObject("pingtarget")
					if (gt) 
					{
						tgtx=gt.innerHTML
						if (tgtx.length > 0)
							UserMessages.Write(tgtx)
					}
					MsgNdx++
					return false
				},null)
		}
	} catch (e) {
	//	AjaxFailure(e,6000)
	}

}


function StartHeartbeat()
{
	HeartBeatInterval= setInterval("Heartbeat()",500)
}
//setTimeout("StartHeartbeat()",3000)


