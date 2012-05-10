/*	Script: Prop
	Attached to the underside of platforms.
	Generates enough force to make platforms fly! */

#strict 2

local controlMediator;

/*	Constructor: CreateProp
	Factory method for propellers.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:*	You should always create a prop using this method.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	forPlatform	- The platform this prop attaches to.
	mediator	- The control mediator this prop sends its events to.

	Returns:
	The created prop. */
public func CreateProp(int x, int y, object forPlatform, object mediator) {
	var prop = CreateObject(PROP, x, y, forPlatform->GetOwner());
	prop->LocalN("controlMediator") = mediator;
	prop->AttachTo(forPlatform);
	return prop;
}

protected func CheckCollision() {
	if(Stuck()) {
		controlMediator->ControlEvent(COMD_Stop, this);
	}
}

/*	Function: MovementEvent
	Event handler for movement events.

	Parameters:
	direction	- Direction of movement, specified by one of the COMD_* constants.
	source		- Source of the event.

	See Also:
	<Control mediator> */
public func MovementEvent(int direction, object source) {
	if(direction == COMD_Up) {
		SetAction("AttachFast");
	} else if(direction == COMD_Down) {
		SetAction("AttachSlow");
	} else {
		SetAction("Attach");
	}
}