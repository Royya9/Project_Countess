// Free to play. By SoNa Games.


#include "Camera/Countess_Fireball_CameraShake.h"

UCountess_Fireball_CameraShake::UCountess_Fireball_CameraShake()
{
	OscillationDuration = 0.25f;
	this->RotOscillation.Pitch.Amplitude = 5.f;
	this->RotOscillation.Pitch.Frequency = 50.f;

	this->RotOscillation.Yaw.Amplitude = 5.f;
	this->RotOscillation.Yaw.Frequency = 50.f;

	this->LocOscillation.X.Amplitude = 50.f;
	this->LocOscillation.X.Frequency = 1.f;

	this->LocOscillation.Z.Amplitude = 10.f;
	this->LocOscillation.Z.Frequency = 50.f;
}
