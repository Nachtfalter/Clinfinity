#strict 2#appendto TREElocal maxTreeRotation;protected func Construction() {	maxTreeRotation = 5;	ScheduleCall(this, "RotateTree", 1);	return _inherited();}protected func RotateTree() {	RelSetR(RandomX(-maxTreeRotation, maxTreeRotation), 0, GetObjHeight() / 2);}