#strict 2

#appendto CLNK

local maxGlideSpeedY, glideSpeedXFactor;

protected func Initialize() {
	maxGlideSpeedY = 30;
	glideSpeedXFactor = 2;
	return _inherited();
}

protected func ControlDownDouble() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			if(GetEffect("WingSuit", this) == 0) {
				AddEffect("WingSuit", this, 150, 1, this);
			} else {
				RemoveEffect("WingSuit", this);
			}
		}
	}
	return 0;
}

protected func FxWingSuitTimer(object target, int effectNumber, int effectTime) {
	if(target->GetAction() == "Jump") {
		// Maximale Sinkgeschwindigkeit
		if( target->GetYDir() > maxGlideSpeedY ) {
			target->SetYDir( target->GetYDir() - 2 );
		}
		// Horizontale Geschwindigkeit berechnen und anpassen
		var targetXDir = Abs(target->GetYDir()) * (-glideSpeedXFactor + 2 * glideSpeedXFactor * target->GetDir());
		var currentXDir = target->GetXDir();
		if(currentXDir < targetXDir) {
			target->SetXDir( target->GetXDir() + 1 );
		} else if(currentXDir > targetXDir) {
			target->SetXDir( target->GetXDir() - 1 );
		}
		// Rotiert die Jump-Animation, sodass es aussieht, als w�rde der Clonk einen Wingsuit tragen.
		SetDTRotation(90 - target->GetDir() * 180, 0, 0, target);
		return 0;
	} else {
		return -1;
	}
}

protected func FxWingSuitStop(object target, int effectNumber, int reason, bool temporary) {
	if(!temporary) {
		SetDTRotation(0, 0, 0, target);
	}
}

/* Aus der Doku kopiert */
global func SetDTRotation (int r, int xoff, int yoff, object obj) {
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
    obj
  );
}