


<script>
	function mouser(e) 
	{
		GetViewport();
		getMousePosition(e);
		mousetrails.Chase(posX,0,posY,0,1);
	}

	window.onload = function () 
	{
		mousetrails = new ParticleSystem('mousetrails',1,4, 1,25,"25","25","./particles/",".png",4,3,3);
		document.onmousemove = mouser;
		Initialize();
	}

	window.onscroll = function()
	{
		getScrollingPosition();
	};

	window.onresize = function()
	{
		getScrollingPosition();
	};
</script>
