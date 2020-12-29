#include <gtest/gtest.h>

#include <numberedit.h>

#include "keyboard.h"

void PrintTo(const QString& s, std::ostream* os)
{
    *os << "\"" << qPrintable(s) << "\"";
}

TEST(NumberEdit, withSpaces)
{
    EXPECT_EQ("", NumberEdit::separated("", 3));
    EXPECT_EQ("1", NumberEdit::separated("1", 3));
    EXPECT_EQ("12", NumberEdit::separated("12", 3));
    EXPECT_EQ("123", NumberEdit::separated("123", 3));
    EXPECT_EQ("1 234", NumberEdit::separated("1234", 3));
}

TEST(NumberEdit, append)
{
    NumberEdit e;

    EXPECT_EQ("", e.text());
    EXPECT_EQ(0, e.cursorPosition());

    keyboard(&e, Qt::Key_1, "1");

    EXPECT_EQ("1", e.text());
    EXPECT_EQ(1, e.cursorPosition());

    keyboard(&e, Qt::Key_2, "2");

    EXPECT_EQ("12", e.text());
    EXPECT_EQ(2, e.cursorPosition());

    keyboard(&e, Qt::Key_3, "3");

    EXPECT_EQ("123", e.text());
    EXPECT_EQ(3, e.cursorPosition());

    keyboard(&e, Qt::Key_4, "4");

    EXPECT_EQ("1 234", e.text());
    EXPECT_EQ(5, e.cursorPosition());

    keyboard(&e, Qt::Key_5, "5");

    EXPECT_EQ("12 345", e.text());
    EXPECT_EQ(6, e.cursorPosition());
}

TEST(NumberEdit, backspace)
{
    NumberEdit e;
    e.setText("123456");

    keyboard(&e, Qt::Key_End);

    EXPECT_EQ("123 456", e.text());
    EXPECT_EQ(7, e.cursorPosition());

    keyboard(&e, Qt::Key_Backspace);

    EXPECT_EQ("12 345", e.text());
    EXPECT_EQ(6, e.cursorPosition());

    keyboard(&e, Qt::Key_Left);

    EXPECT_EQ(5, e.cursorPosition());

    keyboard(&e, Qt::Key_Backspace);

    EXPECT_EQ("1 235", e.text());
    EXPECT_EQ(4, e.cursorPosition());

    keyboard(&e, Qt::Key_Left);

    EXPECT_EQ(3, e.cursorPosition());

    keyboard(&e, Qt::Key_Delete);

    EXPECT_EQ("125", e.text());
    EXPECT_EQ(2, e.cursorPosition());
}

TEST(NumberEdit, delete_begin)
{
    NumberEdit e;
    e.setText("1234");
    keyboard(&e, Qt::Key_Home);

    EXPECT_EQ("1 234", e.text());
    EXPECT_EQ(0, e.cursorPosition());

    keyboard(&e, Qt::Key_Delete);

    EXPECT_EQ("234", e.text());
    EXPECT_EQ(0, e.cursorPosition());
}

TEST(NumberEdit, backspace_end)
{
    NumberEdit e;
    e.setText("1234");
    keyboard(&e, Qt::Key_End);

    EXPECT_EQ("1 234", e.text());
    EXPECT_EQ(5, e.cursorPosition());

    keyboard(&e, Qt::Key_Backspace);

    EXPECT_EQ("123", e.text());
    EXPECT_EQ(3, e.cursorPosition());
}

TEST(NumberEdit, selection_1)
{
    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 0, 0, NumberEdit::mcBase2);
        EXPECT_EQ(0, selection);
        EXPECT_EQ(0, from);
        EXPECT_EQ(0, to);
    }
}

TEST(NumberEdit, selection_2)
{
    {
        auto [selection, from, to] = NumberEdit::extractSelection("1000 0000", 0, 1, NumberEdit::mcBase2);
        EXPECT_EQ(1, selection);
        EXPECT_EQ(7, from);
        EXPECT_EQ(7, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("1100 0000", 0, 2, NumberEdit::mcBase2);
        EXPECT_EQ(3, selection);
        EXPECT_EQ(7, from);
        EXPECT_EQ(6, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("0000 0001", 8, 9, NumberEdit::mcBase2);
        EXPECT_EQ(1, selection);
        EXPECT_EQ(0, from);
        EXPECT_EQ(0, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("0000 0011", 7, 9, NumberEdit::mcBase2);
        EXPECT_EQ(3, selection);
        EXPECT_EQ(1, from);
        EXPECT_EQ(0, to);
    }
}

TEST(NumberEdit, selection_3)
{
    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 0, 1, NumberEdit::mcBase16);
        EXPECT_EQ(0x01, selection);
        EXPECT_EQ(31, from);
        EXPECT_EQ(28, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 0, 2, NumberEdit::mcBase16);
        EXPECT_EQ(0x12, selection);
        EXPECT_EQ(31, from);
        EXPECT_EQ(24, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 8, 9, NumberEdit::mcBase16);
        EXPECT_EQ(0x08, selection);
        EXPECT_EQ(3, from);
        EXPECT_EQ(0, to);
    }

    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 7, 9, NumberEdit::mcBase16);
        EXPECT_EQ(0x78, selection);
        EXPECT_EQ(7, from);
        EXPECT_EQ(0, to);
    }
}

TEST(NumberEdit, selection_4)
{
    {
        auto [selection, from, to] = NumberEdit::extractSelection("1234 5678", 3, 6, NumberEdit::mcBase16);
        EXPECT_EQ(0x45, selection);
        EXPECT_EQ(19, from);
        EXPECT_EQ(12, to);
    }
}

TEST(NumberEdit, bigdata)
{
    NumberEdit source(16, 4);
    NumberEdit target(2, 4);

    QObject::connect(&source, &NumberEdit::valueChanged, &target, &NumberEdit::setValue);

    keyboard(&source, Qt::Key_1, "1");

    EXPECT_EQ("1", source.text());
    EXPECT_EQ("1", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("10", source.text());
    EXPECT_EQ("1 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("100", source.text());
    EXPECT_EQ("1 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("1000", source.text());
    EXPECT_EQ("1 0000 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("1 0000", source.text());
    EXPECT_EQ("1 0000 0000 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("10 0000", source.text());
    EXPECT_EQ("1 0000 0000 0000 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("100 0000", source.text());
    EXPECT_EQ("1 0000 0000 0000 0000 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("1000 0000", source.text());
    EXPECT_EQ("1 0000 0000 0000 0000 0000 0000 0000", target.text());

    keyboard(&source, Qt::Key_0, "0");

    EXPECT_EQ("1 0000 0000", source.text());
    EXPECT_EQ("1 0000 0000 0000 0000 0000 0000 0000 0000", target.text());
}
