#include "cgameboard.h"
#include <QPainter>
#include <QDebug>

// Размер доски
#define BOARD_SIZE 1024
#define LINE_SIZE 1
#define CELL_COUNT 128
#define CELL_SIZE (BOARD_SIZE / CELL_COUNT)

#define STATE_ALIVE 1
#define STATE_DEATH 0

/**
 * @brief Конструктор
 * @param parent - родитель
 */
CGameBoard::CGameBoard(QWidget *parent): QWidget(parent)
{
    parent->resize(BOARD_SIZE, BOARD_SIZE);

    initMatrix(m_Data);
    randomMatrix();
}

/**
 * @brief Инициализация матрицы
 * @param in_Data
 */
void CGameBoard::initMatrix(CellMatrix& in_Data)
{
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        CellVector line;
        line.reserve(CELL_COUNT);
        in_Data.push_back(line);
        for(size_t j = 0; j < CELL_COUNT; j++)
            in_Data[i].push_back(STATE_DEATH);
    }
}

/**
 * @brief Установка новой матрицы
 * @param in_Data
 */
void CGameBoard::setMatrix(CellMatrix &in_Data)
{
    // Копирую данные
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        for(size_t j = 0; j < CELL_COUNT; j++)
            m_Data[i][j] = in_Data[i][j];
    }
}


/**
 * @brief Печать матрицы
 */
void CGameBoard::printMatrix()
{
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        for(size_t j = 0; j < CELL_COUNT; j++)
            qDebug() << m_Data[i][j];
    }
}


/**
 * @brief Случайная матрица
 */
void CGameBoard::randomMatrix()
{
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        for(size_t j = 0; j < CELL_COUNT; j++)
            m_Data[i][j] = rand() % 2;
    }
    repaint();
}

/**
 * @brief Анализ клетки
 * @param x
 * @param y
 */
size_t CGameBoard::analizeCell(size_t x, size_t y)
{
    size_t result = STATE_DEATH;
    size_t state = m_Data[x][y];

    // получаю смежные клеткм
    CellVector data;
    getAdjacents(x, y, data);

    // Считаю кол-во живых
    size_t aliveCount = 0;
    for(size_t i = 0; i < data.size(); i++)
    {
        if(data[i] == STATE_ALIVE)
            aliveCount++;
    }

    // Правила мира:
    switch(state)
    {
        case STATE_DEATH:
        {
            // 1. в пустой (мёртвой) клетке, рядом с которой ровно три живые клетки, зарождается жизнь;
            if(aliveCount == 3)
                result = STATE_ALIVE;

            break;
        }

        case STATE_ALIVE:
        {
            // если у живой клетки есть две или три живые соседки,
            // то эта клетка продолжает жить;

            // в противном случае, если соседей меньше двух или больше трёх,
            // клетка умирает («от одиночества» или «от перенаселённости»)
            if(aliveCount == 2 || aliveCount == 3)
                result = STATE_ALIVE;

            break;
        }
    }

    return result;
}

/**
 * @brief Следующий шаг
 */
void CGameBoard::next()
{
    // Создаю следующую матрицу
    CellMatrix next;
    initMatrix(next);

    // Заполняю ее
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        for(size_t j = 0; j < CELL_COUNT; j++)
            next[i][j] = analizeCell(i, j);
    }

    setMatrix(next);
    repaint();
}

/**
 * @brief Нарисовать матрицу
 */
void CGameBoard::drawMatrix()
{
    QPainter painter;
    painter.begin(this);

    // Рисование живых клеток
    for(size_t i = 0; i < CELL_COUNT; i++)
    {
        for(size_t j = 0; j < CELL_COUNT; j++)
        {
            size_t value = m_Data[i][j];
            if(value == STATE_ALIVE)
                painter.fillRect(QRect(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE), QBrush(Qt::green));
        }
    }

    // Рисование сетки
    for(size_t i = 0; i <= CELL_COUNT; i++)
    {
        // (int x1, int y1, int x2, int y2)
        painter.drawLine(QLine(i * CELL_SIZE, 0, i * CELL_SIZE, BOARD_SIZE));
        painter.drawLine(QLine(0, i * CELL_SIZE, BOARD_SIZE, i * CELL_SIZE));
    }

    painter.end();
}

/**
 * @brief Получить смежные клетки
 * @param in_Data
 */
void CGameBoard::getAdjacents(size_t in_X, size_t in_Y, CellVector &in_Data)
{
    // попадание в диапазон
    if(in_X < CELL_COUNT && in_Y < CELL_COUNT)
    {
        // справа
        if(in_X < CELL_COUNT - 1)
            in_Data.push_back(m_Data[in_X + 1][in_Y]);

        // снизу
        if(in_Y < CELL_COUNT - 1)
            in_Data.push_back(m_Data[in_X][in_Y + 1]);

        // справа снизу
        if(in_X < CELL_COUNT - 1 && in_Y < CELL_COUNT - 1)
            in_Data.push_back(m_Data[in_X + 1][in_Y + 1]);

        // сверху
        if(in_X >= 1)
        {
            if(in_Y < CELL_COUNT - 1)
                in_Data.push_back(m_Data[in_X - 1][in_Y + 1]);

            in_Data.push_back(m_Data[in_X - 1][in_Y]);
        }

        // слева
        if(in_Y >= 1)
        {
           if(in_X < CELL_COUNT - 1)
             in_Data.push_back(m_Data[in_X + 1][in_Y - 1]);

           in_Data.push_back(m_Data[in_X][in_Y - 1]);
        }

        // сверху и слева
        if(in_X >= 1 && in_Y >= 1)
            in_Data.push_back(m_Data[in_X - 1][in_Y - 1]);
    }
}

/**
 * @brief Событие рисования
 * @param event - событие
 */
void CGameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    drawMatrix();
}
