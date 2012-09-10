#include "SpawnWaves.h"

#include "StateLevel.h"
#include "SpawnBase.h"

#include "UnitPlayer.h"
#include "UnitSpike.h"

#include "gameDefines.h"

WaveBase::WaveBase() :
	probability( 0 )
{
}

// ----- WaveBomb --------------------------------------------------------------

#define WAVE_BOMB_RADIUS_SQR 22500
#define WAVE_BOMB_AMOUNT 12

int WaveBomb::checkConditions( StateLevel const *const level, SpawnBase const *const spawn, const int &delta )
{
	Vector2d<float> temp( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
	if ( (level->player->shape.pos - temp).lengthSquared() < WAVE_BOMB_RADIUS_SQR )
	{
		++probability;
	}
	else
		probability = 0;

	return probability;
}

void WaveBomb::spawn( StateLevel *level, SpawnBase *spawn )
{
	for ( int I = 0; I < WAVE_BOMB_AMOUNT; ++I )
	{
		level->addUnit( spawn->spawnUnit( UnitBase::utBomb ) );
	}
}

// ----- WaveSpikeStar ---------------------------------------------------------

#define WAVE_SPIKESTAR_BORDER 50
#define WAVE_SPIKESTAR_RADIUS 100
#define WAVE_SPIKESTAR_AMOUNT 8

int WaveSpikeStar::checkConditions( StateLevel const *const level, SpawnBase const *const spawn, const int &delta )
{
	if ( !level->player->shape.pos.isInRect( WAVE_SPIKESTAR_BORDER, WAVE_SPIKESTAR_BORDER,
				APP_SCREEN_WIDTH - WAVE_SPIKESTAR_BORDER, APP_SCREEN_HEIGHT - WAVE_SPIKESTAR_BORDER ) )
	{
		++probability;
	}
	else
		probability = 0;

	return 0;
}

void WaveSpikeStar::spawn( StateLevel *level, SpawnBase *spawn )
{
	float angle = 0;
	UnitSpike* temp = NULL;
	for ( int I = 0; I < WAVE_SPIKESTAR_AMOUNT; ++I )
	{
		temp = new UnitSpike( level );
		temp->shape.pos = Vector2d<float>( cos( angle ) * WAVE_SPIKESTAR_RADIUS,
											sin( angle ) * WAVE_SPIKESTAR_RADIUS );
		temp->shape.pos += Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
		temp->vel = Vector2d<float>( cos( angle ), sin( angle ) ) * 0.1f;
		temp->accel = Vector2d<float>( cos( angle ), sin( angle ) ) * 0.005f;
		level->addUnit( temp );

		angle += 2 * M_PI / WAVE_SPIKESTAR_AMOUNT;
	}
}
