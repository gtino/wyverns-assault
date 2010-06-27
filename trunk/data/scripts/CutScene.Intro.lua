-- Module : Cutscene
-------------------------------------
-- Runs the initial cut-scene
-- @return : true if the cut scene has ended, false otherwise
-------------------------------------

CameraMode_Game = 0
CameraMode_Fixed = 1
CameraMode_Free = 2
CameraMode_CutScene = 3

Game_Screen = 3

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
		
		Gui.hideUi();
		Gui.showForeground();
		
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
			Gui.setForeground("intro_1");
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
		Gui.setForeground("intro_2");
		-- Second part of the scene: camera arrives in front of the dragon
		-- We just turn on some effects
		PostProcess.enableCompositor("OldMovie"); -- Enable the given compositor
		CutScene.nextStep(); -- Simply go to next step
	elseif step == 4 then
		-- Wait 4 seconds
		if CutScene.wait(4) then
			Gui.hideForeground();
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 5 then 
		-- Zoom closer to the wyvern
		Camera.flyTo(93,79.5,956,92.5,55.5,913,1);
		CutScene.nextStep();
	elseif step == 6 then
		if Camera.hasArrived() then
			-- Camera has arrived
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 7 then
		Camera.flyTo(158,37.8,898,204,35.5,878,1);
		CutScene.nextStep();
	elseif step == 8 then
		if Camera.hasArrived() then
			Gui.setForeground("intro_3");
			Gui.showForeground();
			-- Camera has arrived
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 9 then
		-- Wait 4 seconds, so we can see the rock
		if CutScene.wait(4) then
			Gui.hideForeground();
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 10 then -- rotate to the player 1
		Camera.moveTo(159,45,902,108,31,891);
		CutScene.nextStep();
	elseif step == 11 then
		if Camera.hasArrived() then
			Gui.setForeground("intro_4");
			Gui.showForeground();
			-- Camera has arrived
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 12 then
		-- Wait 4 seconds, so we can see the wyvern
		if CutScene.wait(4) then
			Gui.hideForeground();
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 13 then
		-- First part of the scene: camera flyes from the sky to the wyverns cave	
		Camera.flyTo(80, 180, 1140, 76.8, -100, 630, 2);		
		CutScene.nextStep(); -- Simply go to next step
	elseif step == 14 then
		if Camera.hasArrived() then
			Gui.setForeground("intro_5");
			Gui.showForeground();
			--Audio.playOnce("");
			-- Camera has arrived
			CutScene.nextStep(); -- Simply go to next step
		end	
	elseif step == 15 then
		-- Wait 4 seconds, so we can see the wyvern
		if CutScene.wait(4) then
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 16 then 
		Gui.setForeground("intro_6");
		PostProcess.disableCompositor("OldMovie"); -- Disable compositor
		CutScene.nextStep(); -- Simply go to next step
	elseif step == 17 then
		if CutScene.wait(2) then
			CutScene.nextStep();
		end
	else
		Audio.playLoop("main_track.mp3"); -- Play the soundtrack
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