function runLightsLogic ()
	
	totalSeconds = getTotalSeconds()
	elapsedSeconds = getElapsedSeconds()
	
	r,g,b = getLightDiffuseColor("Point1")
	
	dayTime = 20
	dayOrNight = totalSeconds - math.floor(totalSeconds/dayTime)*dayTime
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
	
	--r = r+elapsedSeconds/10.0
	--g = g+elapsedSeconds/10.0
	--b = b+elapsedSeconds/10.0
	
	if r > 1.0 then
    r = 0.0
  end
  
  if g > 1.0 then
    g = 0.0
  end
  
  if b > 1.0 then
    b = 0.0
  end
  
  setLightDiffuseColor("Point1",r,g,b)
  
  x,y,z = getLightPosition("Point1")
  
  x = x + elapsedSeconds*50
  
  if x > 1000.0 then
    x = 0.0
  end
  
  setLightPosition("Point1",x,y,z)
  
  return true
  
end