// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nu�ez (miguel@semba.guru)
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

#include "ProgressBar.h"

namespace SEMBA {
namespace Util {

ProgressBar::ProgressBar() {
    prev_ = 0;
    init_ = false;
    size_ = 0;
    step_ = 0;
}

ProgressBar::~ProgressBar() {

}

bool ProgressBar::isConsole() const {
#ifdef _WIN32
    if (!_isatty(_fileno(stdout))) {
        return false;
    }
#else
    if (!isatty(fileno(stdout))) {
        return false;
    }
#endif
    return true;
}

void ProgressBar::init(const std::string& name,
                       const std::size_t& size,
                       const std::size_t& prev) {
    if (size == 0) {
        return;
    }
    if (!isConsole()) {
        printSpaces_(prev);
        std::cout << name << std::endl;
        return;
    }
    init_ = true;
    prev_ = prev;
    name_ = name;
    size_ = size;
    step_ = 0;
    timeStart_ = Clock::now();
    timeAct_   = Clock::now();
    printLine_();
}

void ProgressBar::advance(const std::size_t& size) {
    if (!init_) {
        return;
    }
    step_ += size;
    if (step_ >= size_) {
        step_ = size_;
    }
    Clock::time_point timeAct = Clock::now();
    if (std::chrono::duration_cast<Milliseconds>(timeAct-timeAct_).count() >=
            1000) {
        timeAct_ = timeAct;
        printLine_();
    }
}

void ProgressBar::end() {
    if (!init_) {
        return;
    }
    step_ = size_;
    printLine_();
    std::cout << std::endl;
    init_ = false;
}

void ProgressBar::printLine_() const {
    std::size_t pos = 0;
    std::size_t sizeCon    = getSize_();
    std::size_t sizePrev   = prev_;
    std::size_t sizeName   = std::max(std::min(sizeCon/5,(size_t)20), (size_t)10);
    if (sizeName > sizePrev) {
        sizeName -= sizePrev;
    } else {
        sizeName = 0;
    }
    std::size_t sizeSpaces = std::max(std::min(sizeCon/20, (size_t) 5), (size_t) 1);
    std::size_t sizeStep   = 13;
    std::size_t sizeTime   = 8;
    std::size_t sizeBar    = 0;
    std::size_t sizePorc   = 4;
    std::size_t sizeOther  = sizePrev + sizeName + sizeSpaces + sizeStep +
                      1 + sizeTime + 1 + 1 + sizePorc + 1;
    if (sizeOther > sizeCon) {
        sizeBar = 0;
    } else {
        sizeBar = sizeCon - sizeOther;
    }
    if (pos + sizePrev >= sizeCon) {
        return;
    }
    std::cout << "\r";
    printSpaces_(sizePrev);
    pos += sizePrev;
    if (pos + sizeName > sizeCon) {
        sizeName = sizeCon - 1 - pos;
    }
    std::string name = name_;
    if (sizeName < 4) {
        return;
    }
    if (name.size() > sizeName) {
        name = name.substr(0, sizeName-3) + "...";
    }
    std::cout << std::left << std::setw(sizeName) << name;
    pos += sizeName;
    if (pos + sizeSpaces >= sizeCon) {
        return;
    }
    printSpaces_(sizeSpaces);
    pos += sizeSpaces;
    if (pos + sizeStep >= sizeCon) {
        return;
    }
    printSize_(step_);
    std::cout << "/";
    printSize_(size_);
    pos += sizeStep;
    if (pos + 1 + sizeTime >= sizeCon) {
        return;
    }
    std::cout << " ";
    pos++;
    if (step_ == 0) {
        if (size_ == 0) {
            std::cout << "00:00:00";
        } else {
            std::cout << "99:99:99";
        }
    } else {
        Duration diffStart = std::chrono::duration_cast<Duration>(timeAct_ -
                                                             timeStart_);
        Duration diffEnd;
        if (step_ < size_) {
            Duration diffTotal = diffStart*(((double)size_)/((double)step_));
            diffEnd = diffTotal - diffStart;
        } else {
            diffEnd = diffStart;
        }
        std::size_t seconds = std::chrono::duration_cast<Seconds>(diffEnd).count()%60;
        std::size_t minutes = std::chrono::duration_cast<Minutes>(diffEnd).count()%60;
        std::size_t hours   = std::chrono::duration_cast<Hours  >(diffEnd).count();
        if (hours > 99) {
            std::cout << "99:99:99";
        } else {
            std::cout << std::right << std::setw(2) << std::setfill('0')
                      << hours;
            std::cout << ":";
            std::cout << std::right << std::setw(2) << std::setfill('0')
                      << minutes;
            std::cout << ":";
            std::cout << std::right << std::setw(2) << std::setfill('0')
                      << seconds;
        }
    }
    pos += sizeTime;
    if (pos + 1 + sizeBar >= sizeCon) {
        return;
    }
    std::cout << " ";
    pos++;
    if (sizeBar < 3) {
        printSpaces_(3);
    } else {
        std::cout << "[";
        std::size_t sizeBarInt = sizeBar - 2;
        std::size_t toFill = sizeBarInt;
        if (size_ != 0) {
            toFill = (step_*sizeBarInt)/size_;
        }
        for (std::size_t i = 0; i < sizeBarInt; i++) {
            if ((i <= toFill) & ((step_ != 0) || (size_ == 0))) {
                std::cout << "#";
            } else {
                std::cout << "-";
            }
        }
        std::cout << "]";
    }
    pos += sizeBar;
    if (pos + 1 + sizePorc >= sizeCon) {
        return;
    }
    std::cout << " ";
    if (size_ == 0) {
        std::cout << std::right << std::setw(3) << std::setfill(' ') << 100;
    } else {
        std::cout << std::right << std::setw(3) << std::setfill(' ')
                  << (step_ * 100) / size_;
    }
    std::cout << "%";
}

std::size_t ProgressBar::getSize_() const {
    std::size_t size = 0;
#ifdef _WIN32
    if (!_isatty(_fileno(stdout))) {
        return size;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return size;
    }
    size = csbi.srWindow.Right-csbi.srWindow.Left+1;
#else
    if (!isatty(fileno(stdout))) {
        return size;
    }
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    size = w.ws_col;
#endif
    return size;
}

void ProgressBar::printSpaces_(const std::size_t spaces) const {
    for (std::size_t i = 0; i < spaces; i++) {
        std::cout << " ";
    }
}

void ProgressBar::printSize_(const std::size_t size) const {
    std::size_t print = 0;
    std::string unit;
    if (size < 100000UL) {
        print = size;
        unit  = " ";
    } else if (size < 100000000UL) {
        print = size/1000UL;
        unit  = "K";
    } else if (size < 100000000000UL) {
        print = size/1000000UL;
        unit  = "M";
    } else if (size < 100000000000000UL) {
        print = size/1000000000UL;
        unit  = "G";
    }
    std::cout << std::right << std::setw(5) << std::setfill(' ') << print;
    std::cout << unit;
}

}
}
