#include "highlighter.h"

#include <QStack>

typedef enum XmlState_en{
    STATE_IDLE,
    STATE_BEGIN_TAG,
    STATE_END_TAG,
    STATE_STRING,
    STATE_TAG_CONTENT,
    STATE_BEGIN_COMMENT,
    STATE_ATTRIBUTE,
    STATE_ATTRIBUTE_VALUE,
} XmlState_t;

Highlighter::Highlighter(QPlainTextEdit *parent): QSyntaxHighlighter(parent->document())
{
    HighlightingRule rule;

    m_type = HIGHLIGHT_GENERIC_INI;

    keywordFormat.setForeground(Qt::darkGreen);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "database " << "databasename " << "hostname " << "password " << "username "
                    << "plugins " << "list" << "port " << "terminal " << "path ";

    foreach (QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(Qt::darkRed);
    keywordFormat.setFontWeight(QFont::Bold);
    keywordPatterns.clear();
    keywordPatterns << "\\[banco\\]" << "\\[dsm-paf\\]" << "\\[dsm-sg\\]" << "\\[ecf\\]" << "\\[report\\]" << "\\[nfe\\]";

    foreach (QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void Highlighter::setType(HighlightType type)
{
    m_type = type;
}

void Highlighter::highlightBlock(const QString &text)
{
    switch (m_type)
    {
        case HIGHLIGHT_DSM_INI:
        {
            highlightBlockDsmIni(text);
        }
        break;

        case HIGHLIGHT_GENERIC_INI:
        {
            highlightBlockGenericIni(text);
        }
        break;

        case HIGHLIGHT_GENERIC_XML:
        {
            highlightBlockGenericXml(text);
        }
        break;

        default:
        {
            // no highlight
        }
    }// switch

}

void Highlighter::highlightBlockDsmIni(const QString &text)
{
    Q_UNUSED(text);

    foreach (HighlightingRule rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
}

void Highlighter::highlightBlockGenericIni(const QString &text)
{
    enum
    {
        STATE_IDLE,
        STATE_VARIABLE,
        STATE_VALUE,
        STATE_GROUP,
    } state = STATE_VARIABLE;

    int count = 0;

    foreach(QChar c, text)
    {
        if ((c.isLetterOrNumber()) && (STATE_VARIABLE == state))
        {
            setFormat(count, 1, QColor(0, 0, 120));
        }
        else if (c.isLetterOrNumber() && (STATE_VALUE == state))
        {
            setFormat(count, 1, QColor(0, 0, 0));
        }
        else if (c.isLetterOrNumber() && (STATE_GROUP == state))
        {
            setFormat(count, 1, QColor(120, 0, 0));
        }
        else if (c == '=')
        {
            state = STATE_VALUE;
            setFormat(count, 1, QColor(0, 120, 0));
        }
        else if (c == '[' )
        {
            state = STATE_GROUP;
            setFormat(count, 1, QColor(0, 120, 0));
        }
        else if (c == ']')
        {
            state = STATE_IDLE;
            setFormat(count, 1, QColor(0, 120, 0));
        }
        else if (c == '\n' || c == '\r')
        {
            state = STATE_VARIABLE;
        } count++;
    }
}

#define XML_COLOR_COMMENT   QColor(150, 150, 150)
#define XML_COLOR_STRING    QColor(0, 120, 0)
#define XML_COLOR_TAGNAME   QColor(0, 0, 120)
#define XML_COLOR_ATTR      QColor(120, 0, 0)
#define XML_COLOR_ATTRVALUE QColor(120, 0, 0)

void Highlighter::highlightBlockGenericXml(const QString &text)
{
    XmlState_t state = STATE_IDLE;

    QStack<XmlState_t> stStack;
    int count = 0;

    foreach (QChar c, text)
    {
        if ((STATE_BEGIN_COMMENT == state) && (c.isLetterOrNumber() || ('-' == c)))
        {
            setFormat(count, 1, XML_COLOR_COMMENT);
        }
        if (c == '\"')
        {
            if (STATE_STRING == state)
            {
                state = stStack.top();
            }
            else
            {
                stStack.push(state);
                state = STATE_STRING;
            }

            setFormat(count, 1, XML_COLOR_STRING);
        }
        else if (STATE_STRING == state)
        {
            setFormat(count, 1, XML_COLOR_STRING);
        }
        else if (c.isLetterOrNumber() && (STATE_BEGIN_TAG == state || STATE_END_TAG == state))
        {
            setFormat(count, 1, XML_COLOR_TAGNAME);
        }
        else if (c.isLetterOrNumber() && (STATE_TAG_CONTENT == state))
        {
            setFormat(count, 1, XML_COLOR_TAGNAME);
        }
        else if (c.isLetterOrNumber() && (STATE_ATTRIBUTE == state))
        {
            setFormat(count, 1, XML_COLOR_ATTR);
        }
        else if ('<' == c)
        {
            state = STATE_BEGIN_TAG;
            setFormat(count, 1, QColor(0, 0, 0));
        }
        else if ((STATE_BEGIN_TAG == state) && ('!' == c))
        {
            state = STATE_BEGIN_COMMENT;
            setFormat(count - 1, 1, XML_COLOR_COMMENT);
            setFormat(count, 1, XML_COLOR_COMMENT);
        }
        else if ('>' == c)
        {
            if (STATE_BEGIN_COMMENT == state)
            {
                setFormat(count, 1, XML_COLOR_COMMENT);
            }
            else
            {
                setFormat(count, 1, QColor(0, 0, 0));
            }

            state = STATE_IDLE;
        }
        else if (('/' == c) && (STATE_BEGIN_TAG == state))
        {
            state = STATE_END_TAG;
            setFormat(count, 1, QColor(0, 120, 0));
        }
        else if ((' ' == c || '\t' == c) && (STATE_BEGIN_TAG == state))
        {
            state = STATE_ATTRIBUTE;
        }
        else if (('=' == c) && (STATE_ATTRIBUTE == state))
        {
            state = STATE_ATTRIBUTE_VALUE;
            setFormat(count, 1, XML_COLOR_ATTRVALUE);
        }
        else if ((' ' == c) && (STATE_ATTRIBUTE_VALUE == state))
        {
            state = STATE_ATTRIBUTE;
        }

        count++;
    } // foreach
}

