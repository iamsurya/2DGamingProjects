#include "collisionStrategy.h"
class CollStrat
{
    public:
        static CollStrat & getInstance() {static CollStrat CS;
                                        return CS;}
        
        bool execute(const Drawable& obj1, const Drawable& obj2)        {
            return coll->execute(obj1, obj2);
        }
    private:
    CollisionStrategy * coll;
    CollStrat() : coll(new PerPixelCollisionStrategy) {};
    CollStrat operator=(CollStrat &);

};