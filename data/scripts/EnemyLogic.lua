-- Module : Scenario
-------------------------------------
-- Runs light changing during the day
-- @param id Session identification.
-------------------------------------

function runNakedLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = 6;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local distance = Physics.getDistance(enemyId,"Player1");
	
	if distance<100 then
		newState = 5 -- Love
	elseif distance<250 then
		newState = 3 -- Alert
	elseif distance<500 then
		newState = 2 -- What
	elseif newState ~= 1 then -- not in sleeping
		newState = 0 -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == 0) then
		newState = 1 -- in sleeping
	end
  
	return newState 
end

function runWizardLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = 6;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local distance = Physics.getDistance(enemyId,"Player1");
	
	if distance<200 then
		newState = 7 -- Rage
	elseif distance<250 then
		newState = 3 -- Alert
	elseif distance<500 then
		newState = 2 -- What
	elseif newState ~= 1 then -- not sleeping
		newState = 0 -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == 0) then
		newState = 1 -- Sleeping
	end
  
	return newState  
end

function runPeasantLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = 6;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local distance = Physics.getDistance(enemyId,"Player1");
	
	if distance<100 then
		newState = 6 -- Rage
	elseif distance<250 then
		newState = 6 -- Alert
	elseif distance<500 then
		newState = 2 -- What
	elseif newState ~= 1 then -- not sleeping
		newState = 0 -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == 0) then
		newState = 1 -- Sleeping
	end
  
	return newState  
end

function runSoldierLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = 6;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local distance = Physics.getDistance(enemyId,"Player1");
	
	if distance<100 then
		newState = 4 -- Rage
	elseif distance<250 then
		newState = 3 -- Alert
	elseif distance<500 then
		newState = 2 -- What
	elseif newState ~= 1 then -- not sleeping
		newState = 0 -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == 0) then
		newState = 1 -- Sleeping
	end
  
	return newState 
end

function forEachEnemy()
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	local n = Enemy.getCount()
	
	 for i = 0, n, 1 do 
		local enemyId = Enemy.getName(i) 
		
		local nStates = 5
		
		local state = totalSeconds - math.floor(totalSeconds/nStates)*nStates
		
		Enemy.setState(enemyId,state)
	 end
  
	return true 
end