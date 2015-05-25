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
