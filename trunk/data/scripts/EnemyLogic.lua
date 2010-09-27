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
Fire = 9
Patrol = 10
Dying = 11
Dead = 12
IdleSpecial = 13
Special = 14

-- Common distances
FightingDistance = 30
MinimumFightingDistance = 20
LoveDistance = 40
SaveDistance = 70

-- Knight
ChaseDistance = 300

-- Wizard
MagicDistance = 100

-- Woman
ItemDistance = 75

-- Archer
FireDistance = 150

-- Battering Ram
BRAttackDistance = 35

-- Animals
ChickenAlertDistance = 50
CowAlertDistance = 70

function runNakedLogic(enemyId, state)	
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < LoveDistance then
				newState = Love
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Love then
			if timeout > 1 then
				newState = Fear			
			elseif distance > LoveDistance then
				newState = Alert
			end
		elseif state == Fear then
			if timeout > 3 then
				newState = Idle
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
	
end

function runWomanLogic(enemyId, state)	
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < ItemDistance then
				newState = Love
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Love then
			if timeout > 1 then
				newState = Fear
			end
		elseif state == Fear then
			if timeout > 3 then
				newState = Patrol
			end
		elseif state == Patrol then
			newState = Patrol
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end	
	end 
	
	return newState	
	
end

function runPeasantLogic(enemyId, state)
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < LoveDistance then
				newState = Love
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Love then
			if timeout > 1 then
				newState = Fear			
			elseif distance > LoveDistance then
				newState = Alert
			end
		elseif state == Fear then
			if timeout > 2 then
				newState = Idle
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
	
end

function runSoldierLogic(enemyId, state)
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < FightingDistance then
				newState = Rage
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Rage then
			if distance > FightingDistance then
				newState = Alert
			elseif distance < MinimumFightingDistance then
				newState = Alert
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
end

function runWizardLogic(enemyId, state)
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < MagicDistance then
				newState = Magic
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Magic then
			if timeout > 2 then
				newState = Alert
			elseif distance > MagicDistance then
				newState = Alert
			elseif distance < FightingDistance then
				newState = Fear			
			end
		elseif state == Fear then
			if distance > SaveDistance then
				newState = Alert
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end
		
	end
  
	return newState 
end

function runArcherLogic(enemyId, state)
	
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
			if distance < ChaseDistance then
				newState = Alert
			end
		elseif state == Alert then
			if distance < FireDistance then
				newState = Fire
			elseif distance > ChaseDistance then
				newState = Idle
			end
		elseif state == Fire then
			if timeout > 2 then
				newState = Alert
			elseif distance > FireDistance then
				newState = Alert
			elseif distance < FightingDistance then
				newState = Fear			
			end
		elseif state == Fear then
			if distance > SaveDistance then
				newState = Alert
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end
		
	end
  
	return newState 
end

function runChickenLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Logic.getTotalSeconds()
	local elapsedSeconds = Logic.getElapsedSeconds()
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Initial then 
		newState = Idle
	elseif state == Idle then
		if distance > ChickenAlertDistance then
			newState = Alert
		else
			newState = Fear
		end
	elseif state == Alert then
		if distance < ChickenAlertDistance then
			newState = Fear
		end
	elseif state == Fear then 
		if timeout > 3 then
			newState = Idle
		end
	elseif state == Dying then 
		newState = Dying
	else
		newState = state
	end
  
	return newState 
end

function runCowLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Logic.getTotalSeconds()
	local elapsedSeconds = Logic.getElapsedSeconds()
		
	local player = Physics.getNearestPlayer(enemyId);
	local distance = Physics.getDistance(enemyId,player);
	
	Enemy.setTarget(enemyId,player);
	
	-- How much time has passed since we entered last state?
	local timeout = Enemy.getStateTimeout(enemyId)
	
	if state == Initial then 
		newState = Idle
	elseif state == Idle then
		if distance < CowAlertDistance then
			newState = Fear
		end
	elseif state == Fear then 
		if timeout > 3 then
			newState = Idle
		end
	elseif state == Dying then 
		newState = Dying
	else
		newState = state
	end
  
	return newState 
end

function runBatteringRamLogic(enemyId, state)
	
	local newState = state
	
	local totalSeconds = Logic.getTotalSeconds()
	local elapsedSeconds = Logic.getElapsedSeconds()
		
	local playerNumber = Player.getNumPlayers();
	--local playerNumber = 0;
	
	if playerNumber == 0 then
		newState = Idle -- Party state! (no players alive)
	elseif playerNumber > 0 then
	
		local player = Physics.getNearestPlayer(enemyId);
		local distance = Physics.getDistance(enemyId,player);
		
		Enemy.setTarget(enemyId,player);
		
		-- How much time has passed since we entered last state?
		local timeout = Enemy.getStateTimeout(enemyId)
		
		if state == Initial then 
			newState = Alert
		elseif state == Alert then
			if distance < BRAttackDistance then
				newState = Rage
			end
		elseif state == Rage then
			if distance > BRAttackDistance then
				newState = Alert
			end
		elseif state == Dying then 
			newState = Dying
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
end

------ BOSS Logic ------

-- BossTimes
IdleTime = 5;
IdleSpecialTime = 8;
AttackBasicTime = 2;
AttackSpecialTime = 10;

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
			if timeout > AttackBasicTime then
				newState = IdleSpecial
			end
		elseif state == IdleSpecial then
			if timeout > IdleSpecialTime then
				newState = Special
			end
		elseif state == Special then
			if timeout > AttackSpecialTime then
				newState = Idle
			end
		else
			newState = state -- same as before
		end		
	end
  
	return newState 
end

