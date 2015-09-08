#include "ProjectileLaser.h"

#include "sparrowPrimitives.h"

#define PROJECTILE_LASER_BLINK_TIME 75

ProjectileLaser::ProjectileLaser( StateLevel *newParent, const int &duration ) :
	UnitBase( newParent, &shape )
{
	x = &(shape.target.x);
	y = &(shape.target.y);
	flags.add(ufDeadlyOnTouch);
	flags.add(ufInvincible);
	life.start( duration );
	blink.start( PROJECTILE_LASER_BLINK_TIME );
	blinkStatus = 0;
	timers.push_back( &life );
	timers.push_back( &blink );
	next = NULL;
	reflector = NULL;
}

ProjectileLaser::~ProjectileLaser()
{
	if (next)
	{
		std::vector<ProjectileLaser*> temp;
		ProjectileLaser *iter = this;
		while (iter->next)
		{
			temp.push_back(iter->next);
			iter = iter->next;
		}
		for (std::vector<ProjectileLaser*>::iterator I = temp.begin(); I != temp.end(); ++I)
			delete *I;
	}
}


///--- PUBLIC ------------------------------------------------------------------

int ProjectileLaser::update( const Uint32 &delta )
{
	if ( life.stopped() )
		toBeRemoved = true;
	if ( blink.stopped() )
	{
		blinkStatus = !blinkStatus;
		blink.start( PROJECTILE_LASER_BLINK_TIME );
	}

	if (next)
	{
		if (next->toBeRemoved)
		{
			delete next;
			next = NULL;
			// Reset this laser
			Vector2d<float> direction = (shape.target - shape.pos).unit();
			shape.target.x = shape.pos.x + direction.x * 1000;
			shape.target.y = shape.pos.y + direction.y * 1000;
			next = NULL;
		}
		else
		{
			next->update(delta);
			// Mark for deletion (will be reset if same reflector is still hit)
			if (next->reflector)
				next->toBeRemoved = true;
		}
	}

	return UnitBase::update( delta );
}

void ProjectileLaser::render( SDL_Surface *const target )
{
	if ( blinkStatus )
	{
		spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
				spGetFastRGB( 255, 0, 0 ) );
	}
	else
	{
		spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
				-1 );
	}
	if (next)
		next->render(target);
}

bool ProjectileLaser::checkCollision(UnitBase const * const other)
{
	// TODO: Falsche Kollision wenn Ray direkt auf Circle zeigt. dann geht erster Abschnitt durch den Circle und zeigt auf die gegenüberliegende Seite.
	//       der zweite Abschnitt ist dann die korrekte Kollision, allerdings vom hinteren Rand des Kreises aus.
	//       Aber nur beim Spawn im StateCollision??? Nach Bewegung (auch 100% vertikal) verschwunden???
	CollisionResponse temp;
	if (other->shape && shape.checkCollision(other->shape, temp))
	{
		if (other->type == utSpike) // TODO: Proper check here for reflective type (new unit flag probably)
		{
			*x = temp.position.x;
			*y = temp.position.y;
			if (next && next->reflector != other)
			{
				delete next;
				next = NULL;
			}
			if (!next)
				next = new ProjectileLaser(parent, -1);
			next->shape.pos.x = temp.position.x;
			next->shape.pos.y = temp.position.y;
			float angle = (shape.pos - shape.target).angle(temp.direction);
			Vector2d<float> tempVec = temp.direction.rotate(angle * (temp.lhs ? 1 : -1));
			next->shape.target.x = temp.position.x + tempVec.x * 1000;
			next->shape.target.y = temp.position.y + tempVec.y * 1000;
			next->reflector = const_cast<UnitBase*>(other);
			next->toBeRemoved = false;
		}
		return true;
	}
	if (next)
	{
		return next->checkCollision(other);
	}
	return false;
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
