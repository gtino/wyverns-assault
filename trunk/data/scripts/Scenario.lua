-- Module : LightsLogic
-------------------------------------
-- Runs lights logic
-- @param id Session identification.
-------------------------------------

function runLightsLogic()
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	local r,g,b = Lights.getLightDiffuseColor("Point1")
	
	local dayTime = 20
	local dayOrNight = totalSeconds - math.floor(totalSeconds/dayTime)*dayTime
	--dayOrNight = totalSeconds % dayTime
	
	if dayOrNight > 10 then
		r = 0.1
		g = 0.1
		b = 0.2
	else
		r = 0.5
		g = 0.5
		b = 0.5
	end
	
	r = r+elapsedSeconds/10.0
	g = g+elapsedSeconds/10.0
	b = b+elapsedSeconds/10.0
	
	if r > 1.0 then
		r = 0.0
	end
  
	if g > 1.0 then
		g = 0.0
	end
  
	if b > 1.0 then
		b = 0.0
	end
  
	Lights.setLightDiffuseColor("Point1",r,g,b)
  
	local x,y,z = Lights.getLightPosition("Point1")
  
	x = x + elapsedSeconds*50
  
	if x > 1000.0 then
		x = 0.0
	end
  
	Lights.setLightPosition("Point1",x,y,z)
  
	return true 
end