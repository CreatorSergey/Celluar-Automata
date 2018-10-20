#ifndef CGAMEBOARD_H
#define CGAMEBOARD_H

#include <QWidget>

typedef std::vector<size_t> CellVector;
typedef std::vector<CellVector> CellMatrix;

/**
 * @brief Игровая доска для (игра жизнь)
 * https://ru.wikipedia.org/wiki/%D0%98%D0%B3%D1%80%D0%B0_%C2%AB%D0%96%D0%B8%D0%B7%D0%BD%D1%8C%C2%BB
 */
class CGameBoard: public QWidget
{
    Q_OBJECT

public:
    CGameBoard(QWidget* parent = nullptr);

    virtual void paintEvent(QPaintEvent *event) override;

protected:
    void printMatrix();
    void drawMatrix();
    void initMatrix(CellMatrix& in_Data);
    void setMatrix(CellMatrix& in_Data);

    void getAdjacents(size_t in_X, size_t in_Y, CellVector& in_Data);
    size_t analizeCell(size_t x, size_t y);
    void randomMatrix();

public slots:
   void next();

protected:
    CellMatrix m_Data;
};

#endif // CGAMEBOARD_H
