-- Module : animalLogic
-------------------------------------
-- Runs animal logic
-- @param animalId animal id
-- @param state current animal state
-------------------------------------

-- AnimalStates
Initial_animal = 0
Idle_animal = 1
Alert_animal = 2

-- Distances
AlertDistance = 100
SoundDistance = 200

function runChickenLogic(animalId, state)
	
	local newState = state
		
	local player = Physics.getNearestPlayer(animalId);
	local distance = Physics.getDistance(animalId,player);
	
	if state == Initial_animal then 
		newState = Idle
	elseif state == Idle_animal then -- Animal is patrolling...
		--if distance<SoundDistance then 
		--end
	elseif state == Idle_animal then -- Something is happening here...
		if distance<AlertDistance then -- The Animal can see the player!
			newState = Alert_animal -- ...and chase him!
		end
	elseif state == Alert_animal then -- ...we see the player and we are chasing him
		if distance>AlertDistance then -- ...we reached the player
			newState = Idle_animal 
		end
	else
		newState = state -- same as before
	end
  
	return newState 
end
