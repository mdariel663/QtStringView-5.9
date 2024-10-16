#include <QString>
#include <QChar>
#include <QDebug>
#include <algorithm>

//!
//! \brief QStringView
//! Reinterpreting of class QStringView for Qt5.9
//! @class QStringView
//! @description Is a module <QStringView> in Qt5.9
class QStringView
{
public:
    enum SplitBehavior { KeepEmptyParts, SkipEmptyParts };

    QStringView() : m_data(nullptr), m_size(0) {}
    QStringView(const QString& string)
        : m_data(string.constData()), m_size(string.size()) {}

    QStringView(const QChar* data, int size)
        : m_data(data), m_size(size) {}

    QStringView(const char16_t* data, int size)
        : m_data(reinterpret_cast<const QChar*>(data)), m_size(size) {}

    QStringView(char16_t* data, int size);

    int size() const { return m_size; }
    bool isEmpty() const { return m_size == 0; }

    const QChar* data_ptr() const { return m_data; }
    int toInt(bool *ok = Q_NULLPTR, int base=10){ return QString(data_ptr()).toInt(ok, base); }



    QList<QStringView> split(const char16_t* delimiter, SplitBehavior splitBehavior = SplitBehavior::KeepEmptyParts) const {
        QList<QStringView> result;

        int start = 0;
        for (int i = 0; i < m_size; ++i) {
            if (m_data[i] == QChar(*delimiter)) { // Convertir char16_t a QChar
                QStringView part = mid(start, i - start);
                if (SplitBehavior::SkipEmptyParts == splitBehavior && !part.isEmpty()){
                        result.append(part);
                }
                start = i + 1;
            }
        }

        if (start < m_size) {
            QStringView part = mid(start, m_size - start);
            if (!part.isEmpty()) {
                result.append(part);
            }
        }

        return result;
    }

    QList<QStringView> split(QChar delimiter, SplitBehavior splitBehavior = SplitBehavior::KeepEmptyParts) const {
        QList<QStringView> result;

        int start = 0;
        for (int i = 0; i < m_size; ++i) {
            if (m_data[i] == delimiter) {
                QStringView part = mid(start, i - start);
                if (SplitBehavior::SkipEmptyParts == splitBehavior && !part.isEmpty()) {
                    result.append(part);
                }
                start = i + 1;
            }
        }

        if (start < m_size) {
            QStringView part = mid(start, m_size - start);
            if (!part.isEmpty()) {
                result.append(part);
            }
        }

        return result;
    }

    QChar operator[](int index) const {
        Q_ASSERT(index >= 0 && index < m_size);
        return m_data[index];
    }

    QString toString() const {
        return QString(m_data, m_size);
    }

    bool operator==(const QStringView& other) const {
        return m_size == other.m_size && std::equal(m_data, m_data + m_size, other.m_data);
    }

    bool operator!=(const QStringView& other) const {
        return !(*this == other);
    }

    bool operator==(const QString& other) const {
        return m_size == other.size() && std::equal(m_data, m_data + m_size, other.constData());
    }

    bool operator==(const QString other) const {
        return m_size == other.size() && std::equal(m_data, m_data + m_size, other.constData());
    }

    bool compare(const QString& other) const {
        return m_size == other.size() && std::equal(m_data, m_data + m_size, other.constData());
    }

    //bref Método mid
    QStringView mid(int pos, int n = -1) const {
        if (pos < 0 || pos >= m_size) {
            return QStringView();
        }
        if (n == -1 || pos + n > m_size) {
            n = m_size - pos;
        }
        return QStringView(m_data + pos, n);
    }

private:
    const QChar* m_data;
    int m_size;
};
