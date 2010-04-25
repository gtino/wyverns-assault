-- Module : EnemyLogic
-------------------------------------
-- Runs enemy logic
-- @param enemyId enemy id
-- @param state current enemy state
-------------------------------------

-- EnemyStates
Idle = 0
Sleeping = 1
What = 2
Alert = 3
Rage = 4
Love = 5
Fear = 6
Magic = 7
Patrol = 8

function runNakedLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = Fear;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
	
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	if distance<100 then
		newState = Love -- Love
	elseif distance<250 then
		newState = Alert -- Alert
	elseif distance<500 then
		newState = What -- What
	elseif newState ~= Sleeping then -- not in sleeping
		newState = Idle -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == Idle) then
		newState = Sleeping -- in sleeping
	end
  
	return newState 
end

function runWizardLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = Fear;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	if distance<200 then
		newState = Magic -- Rage
	elseif distance<250 then
		newState = Alert -- Alert
	elseif distance<500 then
		newState = What -- What
	elseif newState ~= Sleeping then -- not sleeping
		newState = Idle -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == Idle) then
		newState = Sleeping -- Sleeping
	end
  
	return newState  
end

function runPeasantLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = Fear;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	if distance<100 then
		newState = Fear -- Rage
	elseif distance<250 then
		newState = Fear -- Alert
	elseif distance<500 then
		newState = What -- What
	elseif newState ~= Sleeping then -- not sleeping
		newState = Idle -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == Idle) then
		newState = Sleeping -- Sleeping
	end
  
	return newState  
end

function runSoldierLogic(enemyId)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = Fear;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	if distance<100 then
		newState = Rage -- Rage
	elseif distance<250 then
		newState = Alert -- Alert
	elseif distance<500 then
		newState = What -- What
	elseif newState ~= Sleeping then -- not sleeping
		newState = Idle -- Idle
	end
	
	local timeout = Enemy.getStateTimeout(enemyId)
	
	-- after 20 seconds of idle, just go to sleep
	if (timeout > 20 and newState == Idle) then
		newState = Sleeping -- Sleeping
	end
  
	return newState 
end

function forEachEnemy()
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	local n = Enemy.getCount()
	
	 for i = 0, n, 1 do 
		local enemyId = Enemy.getName(i) 
		
		local nStates = Love
		
		local state = totalSeconds - math.floor(totalSeconds/nStates)*nStates
		
		Enemy.setState(enemyId,state)
	 end
  
	return true 
end