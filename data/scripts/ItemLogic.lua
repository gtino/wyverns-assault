-- Module : ItemLogic
-------------------------------------
-- Runs item logic
-- @param itemId item id
-- @param state current item state
-------------------------------------

-- ItemStates
Modest = 0
Attention = 1
Catch = 2

-- Distances
AlertDistance = 50
CatchDistance = 10;

function runItemLogic(itemId, state)
	
	local newState = state
		
	local player = Physics.getNearestPlayer(itemId);
	local distance = Physics.getDistance(itemId,player);
	
	if state == Modest then
		if distance<AlertDistance then
			newState = Attention -- Item show her effect 
		end
	elseif state == Attention then 
		if distance<CatchDistance then
			newState = Catch -- Item catched 
		elseif (distance>AlertDistance) then
			newState = Modest
		end
	else
		newState = state
	end
  
	return newState 
end
