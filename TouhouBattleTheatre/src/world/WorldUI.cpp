#include "WorldUI.h"
#include "../TestApplication.h"

WorldUI::WorldUI() : _cameraController(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{
}

void WorldUI::HandleClick(SDL_Event mouse_event)
{
	if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{


			// Handle left click
			// Example: Check if the click is within a specific UI element
			// if (isWithinUIElement(mouse_event.button.x, mouse_event.button.y))
			// {
			//     // Perform action for left click on UI element
			//  
			//     // Example: Open a menu, select an item, etc.
			//  
			//     // You can also change the state of the UI element here
			//     // Example: Set the state to active or hot
			//     uiElementState = UIState::active;
			//  
			//     // Update the UI to reflect the new state
			//     updateUI();
			//  
			//     // Optionally, you can also trigger any associated actions
			//     // Example: Perform an action related to the selected item
			//     performActionForSelectedItem();
			//  
			//     // You can also play a sound or provide visual feedback
			//     // Example: Play a sound effect for the click
			//     playClickSound();
			//     // Example: Change the color of the UI element to indicate selection
			//     changeUIElementColor(selectedColor);
			//  
			//     // If needed, you can also update the cursor position
			//     // Example: Set the cursor position to the center of the UI element
			//     setCursorPosition(uiElementCenterX, uiElementCenterY);
			//  
			//     // Finally, you can update the UI to reflect any changes
			//     // Example: Redraw the UI to show the updated state
			//     redrawUI();
			//  
			//     // You can also handle any additional logic specific to your UI
			//     // Example: Update the game state, trigger animations, etc.
			//     updateGameState();
			//     triggerUIAnimations();
			//  
			//     // Remember to handle any necessary cleanup or state management
			//     // Example: Reset the state of other UI elements if needed
			//     resetOtherUIElements();
			//     // Example: Clear any temporary variables or flags
			//     clearTemporaryVariables();
			//     // Example: Reset the state of the UI to its default state
			//     resetUIState();
			//     // Example: Reset the cursor position to its default position
			//     resetCursorPosition();
			//     // Example: Reset the game state to its initial state
			//     resetGameState();
			//     // Example: Reset any temporary variables or flags
			//     clearTemporaryVariables();



		}
		else if (mouse_event.button.button == SDL_BUTTON_RIGHT)
		{
			// Handle right click
		}
	}
	else if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{
			// Handle left click release
		}
		else if (mouse_event.button.button == SDL_BUTTON_RIGHT)
		{
			// Handle right click release
		}
	}
}
