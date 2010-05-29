-- Module : ItemLogic
-------------------------------------
-- Runs item logic
-- @param itemId item id
-- @param state current item state
-------------------------------------

-- ItemStates
Modest = 0
Catch = 1

-- Distances
CatchDistance = 10;

function runItemLogic(itemId, state)
	
	local newState = state
		
	local player = Physics.getNearestPlayer(itemId);
	local distance = Physics.getDistance(itemId,player);
	
	if state == Modest then
		if distance<CatchDistance then
			newState = Catch -- Item catched 
		end
	else
		newState = state
	end
  
	return newState 
end
