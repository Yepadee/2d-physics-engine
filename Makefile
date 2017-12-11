.PHONY: ArrayList RigidBody
GCC = gcc -std=c99 -O3 -pedantic -Wall -o collision
collision:
	$(GCC) collision.c "Physics\RigidBodies.c" "Physics\Data Structures\QTree.c" "Physics\Data Structures\ArrayList.c" "Physics\Data Structures\Data\RigidBody.c" "Physics\Data Structures\Data\Geometry\Polygon.c" "Physics\Data Structures\Data\Geometry\Vector2D.c" "Input\MouseInput.c" "Input\KeyboardInput.c" -lSDL2
	./collision
QTree:
	$(GCC) -DqTreeMain=main "Physics\Data Structures\QTree.c" "Physics\Data Structures\ArrayList.c" "Physics\Data Structures\RigidBody.c" "Physics\Data Structures\Geometry\Polygon.c" "Physics\Data Structures\Geometry\Vector2D.c" -lSDL2
	./collision
ArrayList:
	$(GCC) -DarrayListMain=main "Physics\Data Structures\ArrayList.c" "Physics\Data Structures\RigidBody.c" "Physics\Data Structures\Geometry\Polygon.c" "Physics\Data Structures\Geometry\Vector2D.c" -lSDL2
	./collision
RigidBody:
	$(GCC) -DrigidBodyMain=main "Physics\Data Structures\RigidBody.c" "Physics\Data Structures\Geometry\Polygon.c" "Physics\Data Structures\Geometry\Vector2D.c" -lSDL2
	./collision
