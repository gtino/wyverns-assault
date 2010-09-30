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

function playBossCutScene(elapsedTime)
	local finished = false;

	-- Internal step position and play time
	local step = CutScene.getCurrentStep();
	local elapsedSceneTime = CutScene.getElapsedSceneTime();
	
	if step == 0 then
		-- Initialize cut scene
		Player.disable(); -- Suspend player input until the end of the cut scene
		Logic.disable(); -- Suspend enemy logic
		Physics.disable(); -- Suspend gae physics
		Enemy.disable(); -- Suspend enemy animations
		Item.disable(); -- Suspend items animation
		GameArea.disable(); -- Suspend game area update
		
		Gui.hideUi();
		Gui.showForeground();
		Gui.setForeground("boss_1");
		Gui.showFrame();
		Gui.setFrame("Frame_Normal");
		
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
		Audio.playLoop("Boss1.mp3")
		Camera.setCurrent(CameraMode_Game); -- Sets the current camera
		
		Gui.hideFrame();
		Gui.hideForeground();
		Gui.showUi();
	
		-- scene
		Player.enable(); -- Resume player input
		Logic.enable(); -- Resume enemy logic
		Physics.enable(); -- Resume game physics
		Enemy.enable(); -- Resume enemy animations
		Item.enable(); -- Resume items animation
		GameArea.enable() -- Resume game area update
		
		CutScene.reset();		
		finished = true; -- Scene has ended
	end
		
	return finished;
end