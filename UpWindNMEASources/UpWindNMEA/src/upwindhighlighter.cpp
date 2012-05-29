#include "upwindhighlighter.h"

#include <QDebug>
#include <QFile>

#include "environment.h"

UpwindHighlighter::UpwindHighlighter(QTextDocument *parent):
    QSyntaxHighlighter(parent) {

    QFile file(Environment::getDataDirectory() + "nmeawords.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //Throw?
    }

    QTextStream in(&file);
    while(!in.atEnd()) {

        QStringList words = in.readLine().split(" ");

        //Discard comment lines
        if(words.at(0).at(0) != '#') {

            //Valid lines have 4 tokens
            if(words.size() == 4) {

                HighlightingRule rule;

                QTextCharFormat format;
                format.setForeground(QBrush(QColor(words.at(1).toInt(),
                                                   words.at(2).toInt(),
                                                   words.at(3).toInt())));
                format.setFontWeight(QFont::Bold);

                rule.pattern = QRegExp(words.at(0));
                rule.format = format;

                highlightingRules.append(rule);
            }
        }
    }
}

void UpwindHighlighter::highlightBlock(const QString &text) {

    foreach(const HighlightingRule &rule, highlightingRules) {

        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
