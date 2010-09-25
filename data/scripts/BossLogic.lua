-- Module : BossLogic
-------------------------------------
-- Runs boss logic
-- @param enemyId boss id
-- @param state current boss state
-------------------------------------

-- BossStates
Initial = 0
Idle = 1
Alert = 2
Dying = 11
Dead = 12

-- BossTimes
IdleTime = 3;
AttackBasic = 2;
AttackSpecial1 = 10
AttackSpecial2 = 10
AttackSpecial3 = 10


function runBossLogic(enemyId, state)	
	
	local newState = state
	
	local totalSeconds = Logic.getTotalSeconds()
	local elapsedSeconds = Logic.getElapsedSeconds()
		
	local playerNumber = Player.getNumPlayers();
	
	if playerNumber == 0 then
		newState = Idle -- Party state! (no players alive)
	elseif playerNumber > 0 then
	
		local player = Physics.getNearestPlayer(enemyId);
		local distance = Physics.getDistance(enemyId,player);
		
		Enemy.setTarget(enemyId,player);
		
		-- How much time has passed since we entered last state?
		local timeout = Enemy.getStateTimeout(enemyId)
		
		if state == Initial then 
			newState = Idle
		elseif state == Idle then
			if timeout > IdleTime then
				newState = Rage
			end
		elseif state == Rage then
			if timeout > AttackBasic then
				newState = Idle
			end
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
	
end
