#ifndef UPWINDHIGHLIGHTER_H
#define UPWINDHIGHLIGHTER_H

#include <QSyntaxHighlighter>

/**
  * UpwindHighlighter is used to highlight NMEA strings in a QTextEdit widget.
  * It loads the words to be highlighted and their highlighted color from
  * file nmeawords.txt located in the directory defined by
  * Environment::getDataDirectory()
  */
class UpwindHighlighter: public QSyntaxHighlighter {

    Q_OBJECT

public:

    UpwindHighlighter(QTextDocument *parent = 0);

protected:
    /**
      * Applies the loaded higlighting rules to a string.
      * @param text The string to be highlighted
      */
    void highlightBlock(const QString &text);

private:

    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;

};

#endif // UPWINDHIGHLIGHTER_H
