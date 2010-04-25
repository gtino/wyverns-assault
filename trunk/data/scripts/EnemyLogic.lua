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

-- Distances
FightingDistance = 100
MagicDistance = 250
SightDistance = 250
SoundDistance = 500

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

function runWizardLogic(enemyId, state)
	
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

function runPeasantLogic(enemyId, state)
	
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

function runSoldierLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Game.getTotalSeconds()
	local elapsedSeconds = Game.getElapsedSeconds()
	
	--local nStates = Fear;
	--local nTimeInState = totalSeconds / 10;
	--local newState = nTimeInState - math.floor(nTimeInState/nStates)*nStates
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Love then -- A soldier is never in love!
		newState = Patrol -- ..he just patrols!
	elseif state == Idle then -- A soldier in never idle!
		newState = Patrol -- ...he always patrols!
	elseif state == Patrol then -- Enemy is patrolling...
		if distance<SoundDistance then -- And suddenly hears a noise..
			newState = What -- What happened? 
		end
	elseif state == What then -- Something is happening here...
		if distance<SightDistance then -- The enemy can see the player!
			newState = Alert -- ...and chase him!
		elseif (distance>SoundDistance and timeout>5) then -- Nothing heard for a while
			newState = Patrol -- ...patrol again
		end
	elseif state == Alert then -- ...we see the player and we are chasing him
		if distance<FightingDistance then -- ...we reached the player
			newState = Rage -- ...so let's attack him!
		elseif (distance>SightDistance and timeout>10) then -- We cannot see the enemy anymore!
			newState = What -- But still we are waiting to se what's going on
		end
	elseif state == Rage then -- ...we are attacking the player!
		if Enemy.isHurt(enemyId) then -- But we are now seriously hurt!
			newState = Fear -- Get the fuck outta here!
		elseif distance>FightingDistance then -- The player is too far
			newState = Alert -- ...chase him
		elseif distance>SightDistance then -- ...well the player really ran away
			newState = What -- Caution for a little bit more...
		end
	elseif state == Fear then
		newState = Fear -- Fear is just fear
	else
		newState = state -- same as before
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