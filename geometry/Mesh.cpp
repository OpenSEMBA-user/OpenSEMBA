#include "Mesh.h"

Mesh::Mesh(const LayerGroup<>& layers)
:   LayerGroup<>(layers.newGroupOf<Layer>()) {

}

Mesh::Mesh(const Mesh& rhs)
:   LayerGroup<>(rhs.layers().newGroupOf<Layer>()) {

}

Mesh::~Mesh() {

}


Mesh& Mesh::operator=(const Mesh& rhs) {
    if(this == &rhs) {
        return *this;
    }
    LayerGroup<>::operator=(rhs.layers().newGroupOf<Layer>());

    return *this;
}

void Mesh::printInfo() const {
    LayerGroup<>::printInfo();
}
