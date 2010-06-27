-- Module : Cutscene
-------------------------------------
-- Runs the a cut-scene
-- @return : true if the cut scene has ended, false otherwise
-------------------------------------

CameraMode_Game = 0
CameraMode_Fixed = 1
CameraMode_Free = 2
CameraMode_CutScene = 3

Game_Screen = 3

function playBridgeCutScene(elapsedTime)
	local finished = false;

	-- Internal step position and play time
	local step = CutScene.getCurrentStep();
	local elapsedSceneTime = CutScene.getElapsedSceneTime();
	
	if step == 0 then
		-- Initialize cut scene
		Input.disable(); -- Suspend player input until the end of the cut scene
		Logic.disable(); -- Suspend enemy logic
		Physics.disable(); -- Suspend gae physics
		
		Gui.hideUi();
		Gui.showForeground();
		Gui.setForeground("bridge_1");
		
		local inputEnabled = Input.isEnabled(); -- Checks if input is enabled
		local logicEnabled = Logic.isEnabled(); -- Checks if logic is enabled
		local physicsEnabled = Physics.isEnabled(); -- Checks if physics are enabled
		
		Camera.setCurrent(CameraMode_CutScene); -- Sets the current camera	

		CutScene.nextStep(); -- Simply go to next step
	elseif step == 1 then
		if Camera.hasArrived() then	
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 2 then
		-- Wait 3 seconds
		if CutScene.wait(3) then
			Gui.hideForeground();
			CutScene.nextStep(); -- Simply go to next step
		end
	else
		Camera.setCurrent(CameraMode_Game); -- Sets the current camera
		
		Gui.hideForeground();
		Gui.showUi();
	
		-- scene
		Input.enable(); -- Resume player input
		Logic.enable(); -- Resume enemy logic
		Physics.enable(); -- Resume game physics
		
		CutScene.reset();		
		finished = true; -- Scene has ended
	end
		
	return finished;
end