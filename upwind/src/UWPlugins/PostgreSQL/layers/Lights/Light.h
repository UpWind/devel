
#ifndef LIGHT_H_
#define LIGHT_H_

#include "Utilies.h"

class QString;
class Point;
class QGraphicsItemGroup;

class Light {
public:
	virtual ~Light();

	virtual void render(QGraphicsItemGroup *layer) = 0;
	virtual QString getName() const = 0;
	virtual LightType getLightType() const = 0;
};

inline Light::~Light() {
}

#endif /* LIGHT_H_ */
