-- Module : ItemLogic
-------------------------------------
-- Runs item logic
-- @param itemId item id
-- @param state current item state
-------------------------------------

-- ItemStates
Initial = 0
Catch = 1
Caught = 2
Removed = 3

-- Distances
CatchDistance = 10;

function runItemLogic(itemId, state)
	
	local newState = state
		
	local player = Physics.getNearestPlayer(itemId);
	local distance = Physics.getDistance(itemId,player);
	
	-- How much time has passed since we entered last state?
	local timeout = Item.getStateTimeout(itemId);
	
	if state == Initial then
		if distance<CatchDistance then
			newState = Catch -- Over item 
		end
	elseif state == Catch then
		newState = Caught
	elseif state == Caught then
		if timeout > 2 then
			newState = Removed -- Item animation/sound done, remove it
		end
	else
		newState = state
	end
  
	return newState 
end
