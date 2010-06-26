-- Module : Scenario
-------------------------------------
-- Runs light changing during the day
-- @param id Session identification.
-------------------------------------

function runDayLight(p)
	
	local totalSeconds = Logic.getTotalSeconds()
	local elapsedSeconds =Logic.getElapsedSeconds()
	
	local r,g,b = Light.getLightDiffuseColor("Point1")
	local ar,ag,ab = Light.getAmbientLight();
	
	-- dayTime is 24 seconds
	local dayDuration = 48 
	local halfDayDuration = dayDuration / 2
	local dayTime = totalSeconds - math.floor(totalSeconds/dayDuration)*dayDuration
	--dayTime = 6
	
	if dayTime > (dayDuration/2) then
		ar = 0.7
		ag = 0.7
		ab = 0.7
	else
		ar = 0.9
		ag = 0.9
		ab = 0.7
	end
	
	r = 0.8
	g = 0.8
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
  
	Light.setLightDiffuseColor("Point1",r,g,b)
	Light.setAmbientLight(ar,ag,ab)
  
	local x,y,z = Light.getLightPosition("Point1")
  
	local degrees = (360.0 * dayTime) / dayDuration
	local radians = math.rad(degrees)
	local radius = 2000.0
	
	x = radius * math.sin(radians)
	y = 500
	z = radius * math.cos(radians)
  
	Light.setLightPosition("Point1",x,y,z)
  
	return true 
end