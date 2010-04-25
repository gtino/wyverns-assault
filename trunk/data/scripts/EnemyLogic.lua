-- Module : EnemyLogic
-------------------------------------
-- Runs enemy logic
-- @param enemyId enemy id
-- @param state current enemy state
-------------------------------------

-- EnemyStates
Initial = 0
Idle = 1
Sleeping = 2
What = 3
Alert = 4
Rage = 5
Love = 6
Fear = 7
Magic = 8
Patrol = 9

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
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Initial then 
		newState = Idle
	elseif state == Idle then -- Enemy is patrolling...
		if distance<SoundDistance then -- And suddenly hears a noise..
			newState = What -- What happened? 
		end
	elseif state == What then -- Something is happening here...
		if distance<SightDistance then -- The enemy can see the player!
			newState = Alert -- ...and chase him!
		elseif (distance>SoundDistance and timeout>5) then -- Nothing heard for a while
			newState = Idle -- ...patrol again
		end
	elseif state == Alert then -- ...we see the player and we are chasing him
		if distance<FightingDistance then -- ...we reached the player
			newState = Love -- ...so let's attack him!
		elseif (distance>SightDistance and timeout>10) then -- We cannot see the enemy anymore!
			newState = What -- But still we are waiting to se what's going on
		end
	elseif state == Love then -- A naked man is usually in love!
		if Enemy.isHurt(enemyId) then -- But we are now seriously hurt!
			newState = Love -- ...the more you hit me, the more I love you!
		elseif distance>FightingDistance then -- The player is too far
			newState = Alert -- ...chase him
		elseif distance>SightDistance then -- ...well the player really ran away
			newState = What -- Caution for a little bit more...
		end
	else
		newState = state -- same as before
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
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Initial then 
		newState = Idle
	elseif state == Love then -- A wizard is never in love!
		newState = Idle -- ..he just reads his books!
	elseif state == Patrol then -- A wizard never patrols!
		newState = Idle -- ...he just reads his books!
	elseif state == Idle then -- Wizard is idle...
		if distance<SoundDistance then -- And suddenly hears a noise..
			newState = What -- What happened? 
		end
	elseif state == What then -- Something is happening here...
		if distance<SightDistance then -- The enemy can see the player!
			newState = Magic -- ...fireballs!
		elseif (distance>SoundDistance and timeout>15) then -- Nothing heard for a while
			newState = Idle -- ...idle again
		end
	elseif state == Magic then -- ...we see the player and we are throwing fireballs at him!
		if distance<FightingDistance then -- ...we reached the player
			newState = Rage -- ...so let's attack him!
		elseif (distance>SightDistance and timeout>15) then -- We cannot see the enemy anymore!
			newState = What -- But still we are waiting to se what's going on
		end
	elseif state == Rage then -- ...we are attacking the player!
		if distance>FightingDistance then -- The player is too far
			newState = Magic -- ...fireballs
		elseif distance>SightDistance then -- ...well the player really ran away
			newState = What -- Caution for a little bit more...
		end
	else
		newState = state -- same as before
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
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Initial then 
		newState = Idle
	elseif state == Love then -- A peasant is never in love!
		newState = Idle -- ..he just works!
	elseif state == Idle then -- Peasant working...
		if distance<SoundDistance then -- And suddenly hears a noise..
			newState = What -- What happened? 
		end
	elseif state == What then -- Something is happening here...
		if distance<SightDistance then -- The enemy can see the player!
			newState = Fear -- ... and gets the fuck outta here
		elseif (distance>SoundDistance and timeout>15) then -- Nothing heard for a (long) while
			newState = Idle -- ...work again
		end
	elseif state == Fear then
		if (distance<FightingDistance and not Enemy.isHurt(enemyId)) then -- Try to fight
			newState = Rage
		else
			newState = Fear -- Fear is just fear
		end
	elseif state == Rage then -- ...we are attacking the player!
		if Enemy.isHurt(enemyId) then -- But we are now seriously hurt!
			newState = Fear -- Get the fuck outta here!
		else
			newState = Rage -- Kepp fighting your way otta here!
		end
	else
		newState = state -- same as before
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
	
	if state == Initial then 
		newState = Idle
	elseif state == Love then -- A soldier is never in love!
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