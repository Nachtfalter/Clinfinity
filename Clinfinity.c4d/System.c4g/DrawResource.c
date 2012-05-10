/*	Script: DrawResource.c
	Draws a resource vein.
	The algorithm draws a vein in consecutive triangles, clockwise, around a central point. */

#strict 2

/*	Function: DrawResource
	Draws a resource vein around a central point.

	Parameters:
	materialIndex		- Material the vein will be made of.
	materialTexture		- The material's texture.
	tunnelBackground	- If true, the vein will be drawn as 'underground'.
	x					- Horizontal coordinate of the centre.
	y					- Vertical coordinate of the centre. */
global func DrawResource(int materialIndex, string materialTexture, bool tunnelBackground, int x, int y) {
	AddEffect("DrawResource", 0, 1, 1, 0, 0, [materialIndex, materialTexture, tunnelBackground], x, y);
}

/* Indices */
static const DrawResourceCentreX = 0;
static const DrawResourceCentreY = 1;
static const DrawResourceMaterialIndex = 2;
static const DrawResourceMaterialTexture = 3;
static const DrawResourceTunnelBackground = 4;
static const DrawResourcePreviousEdgeX = 5;
static const DrawResourcePreviousEdgeY = 6;
static const DrawResourceSize = 7;
static const DrawResourceStartEdgeX = 8;
static const DrawResourceStartEdgeY = 9;

static const MaterialDescriptionIndex = 0;
static const MaterialDescriptionTexture = 1;
static const MaterialDescriptionTunnelBackground = 2;

/* Standard and constant values */
static const DrawResourceStandardMinSize = 10; // 1
static const DrawResourceStandardMaxSize = 65; // 1
static const DrawResourceStandardMinInitialSize = 20; // 1
static const DrawResourceStandardMaxInitialSize = 25; // 1
static const DrawResourceSizeChange = 6;
static const DrawResourceAngularRate = 5;
static const DrawResourceStandardDrawingCycles = 2; // 3

global func FxDrawResourceStart(object target, int effectNumber, int temporary, array materialDescription, x, y) {
	if(temporary == 0) {
		EffectVar(DrawResourceCentreX, target, effectNumber) = x;
		EffectVar(DrawResourceCentreY, target, effectNumber) = y;
		EffectVar(DrawResourceMaterialIndex, target, effectNumber) = materialDescription[MaterialDescriptionIndex];
		EffectVar(DrawResourceMaterialTexture, target, effectNumber) = materialDescription[MaterialDescriptionTexture];
		EffectVar(DrawResourceTunnelBackground, target, effectNumber) = materialDescription[MaterialDescriptionTunnelBackground];

		var size = RandomX(DrawResourceStandardMinInitialSize, DrawResourceStandardMaxInitialSize);
		EffectVar(DrawResourceSize, target, effectNumber) = size;
		EffectVar(DrawResourceStartEdgeX, target, effectNumber) = size;
		EffectVar(DrawResourceStartEdgeY, target, effectNumber) = 0;
		EffectVar(DrawResourcePreviousEdgeX, target, effectNumber) = size;
		EffectVar(DrawResourcePreviousEdgeY, target, effectNumber) = 0;
	}
}

global func FxDrawResourceTimer(object target, int effectNumber, int effectTime) {
	var centreX = EffectVar(DrawResourceCentreX, target, effectNumber);
	var centreY = EffectVar(DrawResourceCentreY, target, effectNumber);
	var materialIndex = EffectVar(DrawResourceMaterialIndex, target, effectNumber);
	var materialTexture = EffectVar(DrawResourceMaterialTexture, target, effectNumber);
	var material = Format("%s-%s", MaterialName(materialIndex), materialTexture);
	var tunnelBackground = EffectVar(DrawResourceTunnelBackground, target, effectNumber);
	var previousX = EffectVar(DrawResourcePreviousEdgeX, target, effectNumber);
	var previousY = EffectVar(DrawResourcePreviousEdgeY, target, effectNumber);
	var result = FX_OK;

	var size, currentX, currentY;
	if(effectTime < DrawResourceStandardDrawingCycles * 360 / DrawResourceAngularRate) {
		size = EffectVar(DrawResourceSize, target, effectNumber);
		size = BoundBy(size + RandomX(-DrawResourceSizeChange, DrawResourceSizeChange), DrawResourceStandardMinSize, DrawResourceStandardMaxSize);

		currentX = Cos(effectTime * DrawResourceAngularRate, size);
		currentY = Sin(effectTime * DrawResourceAngularRate, size) / 2;
	} else {
		// Make start and end fit together
		size = EffectVar(DrawResourceStartEdgeX, target, effectNumber);
		currentX = EffectVar(DrawResourceStartEdgeX, target, effectNumber);
		currentY = EffectVar(DrawResourceStartEdgeY, target, effectNumber);
		result = FX_Execute_Kill;
	}

	EffectVar(DrawResourcePreviousEdgeX, target, effectNumber) = currentX;
	EffectVar(DrawResourcePreviousEdgeY, target, effectNumber) = currentY;
	EffectVar(DrawResourceSize, target, effectNumber) = size;

	DrawMaterialQuad(material, centreX, centreY, centreX + previousX, centreY + previousY, centreX + currentX, centreY + currentY, centreX, centreY, tunnelBackground);

	var particleMinSize = 300;
	var particleMaxSize = 2000;
	var particleMinSpeed = 30;
	var particleMaxSpeed = 50;

	var particleSize = RandomX(particleMinSize, particleMaxSize);
	var speedFactor = (particleMaxSize - particleSize) / (particleMaxSize - particleMinSize);
	var particleSpeed = particleMinSpeed + (particleMaxSize - particleSize) * (particleMaxSpeed - particleMinSpeed) / (particleMaxSize - particleMinSize);
	var particleMoveAngle = Random(360);
	var particleSpeedX = Cos(particleMoveAngle, particleSpeed);
	var particleSpeedY = Sin(particleMoveAngle, particleSpeed);

	//CreateParticle("PSpark", centreX + currentX / 2, centreY + currentY / 2, 0, 0, size * 25, RGBa(GetMaterialColor(materialIndex, 0, 0), GetMaterialColor(materialIndex, 0, 1), GetMaterialColor(materialIndex, 0, 2), 100));
	CreateParticle("PSpark", centreX, centreY, particleSpeedX, particleSpeedY, particleSize, RGBa(GetMaterialColor(materialIndex, 0, 0), GetMaterialColor(materialIndex, 0, 1), GetMaterialColor(materialIndex, 0, 2), 100));

	return result;
}