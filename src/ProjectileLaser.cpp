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
	delete next;
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
		// Reflected (child) laser parts are set to be removed on every frame
		// If they still collide with something the variable is reset (and they
		// are not removed on the subsequent frame).
		// Else the child is removed and the parent laser part is reset.
		if (next->toBeRemoved)
		{
			delete next;
			next = NULL;
			// Reset this laser part
			Vector2d<float> direction = (shape.target - shape.pos).unit();
			shape.target.x = shape.pos.x + direction.x * 1000;
			shape.target.y = shape.pos.y + direction.y * 1000;
			next = NULL;
		}
		else
		{
			next->update(delta);
			// Mark for removal (will be reset if same reflector is still hit)
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
	CollisionResponse temp;
	if (other->shape && shape.checkCollision(other->shape, temp))
	{
		if (other->flags.has(ufReflective))
		{
			// New target = Reflection point
			*x = temp.position.x;
			*y = temp.position.y;
			if (next && next->reflector != other)
			{
				delete next;
				next = NULL;
			}
			if (!next)
				next = new ProjectileLaser(parent, -1);
			// Adjust reflection
			next->shape.pos.x = temp.position.x;
			next->shape.pos.y = temp.position.y;
			// Calculate reflection angle
			float angle = (shape.pos - shape.target).angle(temp.direction);
			// Since the angle() function uses vector-dot-product and since that only
			// returns positive values we have to use lhs to determine in which direction
			// to reflect
			Vector2d<float> tempVec = temp.direction.rotate(angle * (temp.lhs ? 1.0f : -1.0f));
			// Make reflected part "long enough"(tm)
			next->shape.target.x = temp.position.x + tempVec.x * 1000.0f;
			next->shape.target.y = temp.position.y + tempVec.y * 1000.0f;
			next->reflector = const_cast<UnitBase*>(other);
			next->toBeRemoved = false;
		}
		// TODO: Multiple collisions with the same unit.
		//       The current approach only works because units units move very few
		//       pixels at a time. Part-updates are handled one at a time and dependent
		//       on the collision check of the original laser (position in units-vector).
		//       Because of this long reflection chains and multiple reflections
		//       lead to problems. (uncommenting the two lines below will show)
		//       Ideally upon adding a new part a full collision-check-pass
		//       should be done for the new part. Also any change to any part should
		//       invalidate all following parts. (This leads to many memory operations)
//		if (next && next->next)
//			next->next->checkCollision(other);
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
