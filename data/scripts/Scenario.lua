-- Module : LightsLogic
-------------------------------------
-- Runs lights logic
-- @param id Session identification.
-------------------------------------

function runScenario()
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	local r,g,b = Lights.getLightDiffuseColor("Point1")
	local ar,ag,ab = Lights.getAmbientLight();
	
	-- dayTime is 24 seconds
	local dayDuration = 24 
	local halfDayDuration = dayDuration / 2
	local dayTime = totalSeconds - math.floor(totalSeconds/dayDuration)*dayDuration
	--dayTime = 6
	
	if dayTime > 12 then
		ar = 0.05
		ag = 0.05
		ab = 0.1
	else
		ar = 0.7
		ag = 0.7
		ab = 0.7
	end
	
	r = 0.5
	g = 0.5
	b = 0.5
	
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
	Lights.setAmbientLight(ar,ag,ab)
  
	local x,y,z = Lights.getLightPosition("Point1")
  
	local degrees = (360.0 * dayTime) / dayDuration
	local radians = math.rad(degrees)
	local radius = 2000.0
	
	x = 0.0
	y = radius * math.sin(radians)
	z = radius * math.cos(radians)
  
	Lights.setLightPosition("Point1",x,y,z)
  
	return true 
end