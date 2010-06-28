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

function playBeerCutScene(elapsedTime)
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
		--Camera.flyTo(1417,10,55); -- The beer
		CutScene.nextStep(); -- Simply go to next step	
	elseif step == 3 then
		if Camera.hasArrived() then
			Gui.setForeground("beer_2");
			Gui.showForeground();
			CutScene.nextStep(); -- Simply go to next step
		end		
	elseif step == 4 then
		-- Wait 2 seconds
		if CutScene.wait(2) then
			Gui.hideForeground();
			PostProcess.enableCompositor("RadialBlur");
			Camera.rumble(2,5);
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 5 then
		-- Wait 2 seconds
		if CutScene.wait(2) then
			Gui.hideForeground();
			PostProcess.disableCompositor("RadialBlur");
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 6 then
		-- Wait 2 seconds
		--Camera.flyTo(); -- The wyverns
		CutScene.nextStep(); -- Simply go to next step	
	elseif step == 7 then
		if Camera.hasArrived() then
			Gui.setForeground("beer_3");
			Gui.showForeground();
			CutScene.nextStep(); -- Simply go to next step
		end	
	elseif step == 8 then
		-- Wait 1 seconds
		if CutScene.wait(1) then
			Gui.setForeground("beer_4");
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 9 then
		-- Wait 5 seconds
		if CutScene.wait(5) then
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