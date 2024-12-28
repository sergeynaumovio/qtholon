// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowareastate.h"
#include <QMainWindow>

class HolonWindowAreaStateCopy
{
    QDataStream istream;
    QDataStream ostream;
    QByteArray out;

    bool valid{true};

    enum StateMarker : unsigned char
    {
        VersionMarker = 0xff,
        DockWidgetStateMarker = 0xfd,
        SequenceMarker = 0xfc,
        WidgetMarker = 0xfb,
        TabMarker = 0xfa,
        FloatingDockWidgetTabMarker = 0xf9
    };

    enum StateValue : unsigned char
    {
        Version,
        Count,
        Position,
        Size,
        Orientation,
        Flags,
        Dummy
    };

    template<typename T>
    bool copy(StateMarker type)
    {
        T marker;
        istream >> marker;

        if (valid)
            ostream << marker;

        return (marker == type);
    }

    template<typename T>
    int copy(StateValue)
    {
        T number;
        istream >> number;

        if (valid)
            ostream << number;

        return number;
    }

    template<typename T>
    void copy()
    {
        T value;
        istream >> value;

        if (valid)
            ostream << value;
    }

    void copyState(int level)
    {
        if (!copy<uchar>(SequenceMarker))
            return;

        copy<uchar>(Orientation);

        int count;
        istream >> count;

        if (level == 0)
            ostream << int(1);
        else if (valid)
            ostream << count;

        for (int i = 0; i < count; ++i)
        {
            if (valid && level == 0 && i > 0)
                valid = false;

            if (copy<uchar>(SequenceMarker))
            {
                copy<int>(Position);
                copy<int>(Size);
                copy<int>(Dummy);
                copy<int>(Dummy);

                copyState(level + 1);
            }
            else  // WidgetMarker
            {
                copy<QString>(); // name
                copy<uchar>(Flags);

                copy<int>(Position);
                copy<int>(Size);
                copy<int>(Dummy);
                copy<int>(Dummy);
            }
        }
        valid = true;
    }

public:
    HolonWindowAreaStateCopy(const QByteArray &in)
    :   istream(in),
        ostream(&out, QIODeviceBase::WriteOnly)
    {
        out.reserve(in.size());
        istream.setVersion(QDataStream::Qt_5_0);
        ostream.setVersion(istream.version ());

        if (!copy<int>(VersionMarker))
            return;

        copy<int>(Version);

        int level = 0;

        while (!istream.atEnd())
        {
            if (!copy<uchar>(DockWidgetStateMarker)) // else if (copy<uchar>(FloatingDockWidgetTabMarker))
                return;

            int count = copy<int>(Count);
            for (int i = 0; i < count; ++i)
            {
                copy<int>(Position);
                copy<QSize>();

                copyState(level);
            }

            copy<QSize>();

            for (int i = 0; i < 5; ++i)
                copy<int>();
        }
    }

    operator QByteArray() const { return out; }
};

HolonWindowAreaState::HolonWindowAreaState(QMainWindow *const *window)
:   mainWindow(*window)
{ }

QByteArray HolonWindowAreaState::save()
{
    return HolonWindowAreaStateCopy(mainWindow->saveState());
}
