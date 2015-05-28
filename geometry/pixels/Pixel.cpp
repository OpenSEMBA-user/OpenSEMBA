/*
 * Pixel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#include "Pixel.h"

void Pixel::printInfo() const {
    cout << " -- Pixel info -- " << endl;
    cout << "Pos: " << CVecI3::toStr() << endl;
}

string Pixel::toStr(const CartesianDirection& dir) {
    switch (dir) {
    case CartesianDirection::dirX:
        return "X";
    case CartesianDirection::dirY:
        return "Y";
    case CartesianDirection::dirZ:
        return "Z";
    default:
        return "Node";
    }
}

bool Pixel::operator<(const Pixel& rhs) const {
    for (UInt i = 0; i < 3; i++) {
        if (val[i] < rhs.val[i]) {
            return true;
        }
        if (val[i] > rhs.val[i]) {
            return false;
        }
    }
    return false;
}
