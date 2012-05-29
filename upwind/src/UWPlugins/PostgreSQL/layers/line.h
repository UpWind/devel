
#ifndef LINE_H_
#define LINE_H_

#include <QLineF>
#include <QHash>
#include <QVariant>

class Line {
private:
	QLineF line;
	QHash<QString, QVariant> properties;

public:
	Line();
	virtual ~Line();

	QLineF getLine();
	void setLine(QLineF);

	void put(QString key, QVariant value);
	QVariant get(QString key);
};

#endif /* LINE_H_ */
