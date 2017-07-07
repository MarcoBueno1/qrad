#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QPlainTextEdit>

typedef enum HighlightType_en
{
    HIGHLIGHT_DSM_INI,
    HIGHLIGHT_GENERIC_INI,
    HIGHLIGHT_GENERIC_XML
}
HighlightType;

class Highlighter : public QSyntaxHighlighter
{
public:
    static const HighlightType  HighlightDSMIni     = HIGHLIGHT_DSM_INI;
    static const HighlightType  HighlightGenericIni = HIGHLIGHT_GENERIC_INI;
    static const HighlightType  HighlightGenericXml = HIGHLIGHT_GENERIC_XML;
public:
    Highlighter(QPlainTextEdit *parent);

    void setDSMIni();
    void setGenericIni();
    void setType(HighlightType type);
protected:
     void highlightBlock(const QString &text);
     void highlightBlockDsmIni(const QString &text);
     void highlightBlockGenericIni(const QString &text);
     void highlightBlockGenericXml(const QString &text);

private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

     QTextCharFormat keywordFormat;
     HighlightType m_type;
};

#endif // HIGHLIGHTER_H
