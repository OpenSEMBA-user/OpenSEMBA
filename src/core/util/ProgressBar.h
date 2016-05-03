// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_UTIL_PROGRESSBAR_H_
#define SEMBA_UTIL_PROGRESSBAR_H_

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

namespace SEMBA {
namespace Util {

class ProgressBar {
public:
    ProgressBar();
    virtual ~ProgressBar();

    bool isConsole() const;

    void init(const std::string& name,
              const std::size_t& size,
              const std::size_t& prev = 2);
    void advance(const std::size_t& = 1);
    void end();

private:
    typedef std::chrono::high_resolution_clock Clock;

    typedef std::chrono::duration<double> Duration;
    typedef std::chrono::milliseconds     Milliseconds;
    typedef std::chrono::seconds          Seconds;
    typedef std::chrono::minutes          Minutes;
    typedef std::chrono::hours            Hours;

    bool init_;

    std::size_t prev_;
    std::string name_;
    std::size_t size_;
    std::size_t step_;

    Clock::time_point timeStart_;
    Clock::time_point timeAct_;

    void printLine_() const;
    std::size_t getSize_() const;
    void printSpaces_(const std::size_t) const;
    void printSize_(const std::size_t) const;
};

}
}

#endif /* SEMBA_UTIL_PROGRESSBAR_H_ */
