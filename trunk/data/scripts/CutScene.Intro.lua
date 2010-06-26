-- Module : Cutscene
-------------------------------------
-- Runs the initial cut-scene
-- @return : true if the cut scene has ended, false otherwise
-------------------------------------

CameraMode_Game = 0
CameraMode_Fixed = 1
CameraMode_Free = 2
CameraMode_CutScene = 3

function playIntroCutScene(elapsedTime)
	local finished = false;

	-- Internal step position and play time
	local step = CutScene.getCurrentStep();
	local elapsedSceneTime = CutScene.getElapsedSceneTime();
	
	if step == 0 then
		-- Initialize cut scene
		Input.disable(); -- Suspend player input until the end of the cut scene
		Logic.disable(); -- Suspend enemy logic
		Physics.disable(); -- Suspend gae physics
		
		local inputEnabled = Input.isEnabled(); -- Checks if input is enabled
		local logicEnabled = Logic.isEnabled(); -- Checks if logic is enabled
		local physicsEnabled = Physics.isEnabled(); -- Checks if physics are enabled
		
		--PostProcess.disableAllCompositors(); -- Disable all compositos
		
		Camera.setCurrent(CameraMode_CutScene); -- Sets the current camera
		Camera.moveTo(2000, 1500, -2000, 80, 24, 870); -- Start from some point far away	
		
		Audio.playLoop("soft_track.mp3"); -- Play the soundtrack

		CutScene.nextStep(); -- Simply go to next step
	elseif step == 1 then
		if Camera.hasArrived() then
			-- First part of the scene: camera flyes from the sky to the wyverns cave	
			Camera.flyTo(80, 180, 1140, 76.8, -100, 630, 5);		
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 2 then
		if Camera.hasArrived() then
			-- Camera has arrived
			CutScene.nextStep(); -- Simply go to next step
		end		
	elseif step == 3 then
		-- Second part of the scene: camera arrives in front of the dragon
		-- We just turn on some effects
		PostProcess.enableCompositor("OldMovie"); -- Enable the given compositor
		CutScene.nextStep(); -- Simply go to next step
	elseif step == 4 then
		-- Wait 4 seconds
		if CutScene.wait(4) then
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 5 then 
		PostProcess.disableCompositor("OldMovie"); -- Disable compositor
		Audio.playLoop("main_track.mp3"); -- Play the soundtrack
		Camera.setCurrent(CameraMode_Game); -- Sets the current camera
		CutScene.nextStep(); -- Simply go to next step
	else
		-- scene
		Input.enable(); -- Resume player input
		Logic.enable(); -- Resume enemy logic
		Physics.enable(); -- Resume game physics
		
		CutScene.reset();		
		finished = true; -- Scene has ended
	end
		
	return finished;
end